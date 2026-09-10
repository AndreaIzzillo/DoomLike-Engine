#include "engine/renderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Cursor.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include "game/scene/scene.hpp"
#include "game/settings.hpp"
#include "game/world/sprite.hpp"
#include "game/world/wall.hpp"
#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"
#include "utils/image.hpp"

#define FLT(x) static_cast<float>(x)

#define EPS 1e-5f

#define T_MIN 0.f
#define T_MAX std::numeric_limits<float>::max()

namespace Engine
{
    Renderer::Renderer()
        : image(Game::Settings::get().windowWidth, Game::Settings::get().windowHeight)
        , sprite(texture)
        , zBuffer(image.getWidth() * image.getHeight())
    {
        const auto windowWidth = Game::Settings::get().windowWidth;
        const auto windowHeight = Game::Settings::get().windowHeight;

        sf::ContextSettings settings;

        window = sf::RenderWindow(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)),
                                  "Projet ISIM", sf::Style::Default, sf::State::Windowed, settings);

        view.setSize(sf::Vector2f(windowWidth, windowHeight));
        view.setCenter({ FLT(windowWidth) / 2.f, FLT(windowHeight) / 2.f });
        window.setView(view);

        window.setFramerateLimit(Game::Settings::get().targetFramerate);
        window.setVerticalSyncEnabled(false);

        window.setMouseCursorGrabbed(false);
        window.setMouseCursorVisible(true);

        if (!texture.resize(sf::Vector2u(windowWidth, windowHeight)))
            throw std::runtime_error("Failed to allocate renderer texture");

        sprite.setTexture(texture, true);
        pixelBuffer.resize(static_cast<std::size_t>(windowWidth)
                           * static_cast<std::size_t>(windowHeight) * 4);
    }

    sf::RenderWindow &Renderer::getWindow()
    {
        return window;
    }

    sf::View &Renderer::getView()
    {
        return view;
    }

    void Renderer::render(const Game::Scene &scene)
    {
        const auto &screenWidth = image.getWidth();
        const auto &screenHeight = image.getHeight();
        const auto &player = scene.getPlayer();
        const auto &cam = player.getCamera();
        const auto &camPos = cam.getPosition();
        const auto &forward = cam.getForward();

        float aspectRatio = FLT(screenHeight) / FLT(screenWidth);
        float fovH = player.getCamera().getFov();
        float fovV = getVerticalFov(fovH, aspectRatio);
        float scale = (screenHeight / 2.f) / std::tan(fovV / 2.f);
        float cameraHeight = cam.getCameraHeight() + cam.getOffsetHeight();
        int horizon = screenHeight / 2;
        std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::infinity());

        planeSegments.clear();
        planeSegments.resize(screenWidth);

        /* Raycasting and wall rendering */
#pragma omp parallel for schedule(dynamic, 8)
        for (int x = 0; x < screenWidth; x++)
        {
            /* Raycasting */
            auto ray = player.getCamera().getRay(x);
            auto records = rayCaster.castRay(ray, scene, 0.f, T_MAX);

            /* Vertical clipping variables */
            int top = 0;
            int bottom = screenHeight;

            auto currentSector = scene.getCurrentSector();
            for (const auto &record : records)
            {
                if (!record.isHit)
                    continue;

                auto frontSector = currentSector;
                auto backSector =
                    record.frontSector == frontSector ? record.backSector : record.frontSector;

                /* Both vectors are already normalized */
                auto rayDirection = ray.direction;
                auto forward = player.getCamera().getForward();

                /* Corrected distance to avoid fish-eye effect */
                auto rayDir = ray.direction;
                float distance = record.t * (rayDir * forward);

                /* Apply the perspective projection formula */
                int yTop = projectScreen(horizon, cameraHeight, frontSector->getCeilingHeight(),
                                         scale, distance);
                int yBottom = projectScreen(horizon, cameraHeight, frontSector->getFloorHeight(),
                                            scale, distance);

                /* Plain wall */
                if (record.backSector == nullptr)
                {
                    /* Keep the ceiling */
                    int ceilingBottom = std::clamp(yTop, top, bottom);
                    planeSegments[x].push_back(
                        { top, ceilingBottom, x, PlaneType::Ceiling, frontSector });

                    /* Wall rendering */
                    int wallTop = std::clamp(yTop, top, bottom);
                    int wallBottom = std::clamp(yBottom, top, bottom);
                    drawWallVertical(yTop, yBottom, wallTop, wallBottom, x, distance, record,
                                     record.material, record.textureTransform, scene);

                    /* Keep the floor */
                    int floorTop = std::clamp(yBottom, top, bottom);
                    planeSegments[x].push_back(
                        { floorTop, bottom, x, PlaneType::Floor, frontSector });

                    break;
                }
                /* Portal */
                else
                {
                    /* Calculate projected coordinates of the back sector */
                    int nextTop = projectScreen(horizon, cameraHeight,
                                                backSector->getCeilingHeight(), scale, distance);
                    int nextBottom = projectScreen(horizon, cameraHeight,
                                                   backSector->getFloorHeight(), scale, distance);

                    /* Calculate the opening between the two sectors */
                    int openingTop = std::max({ top, yTop, nextTop });
                    int openingBottom = std::min({ bottom, yBottom, nextBottom });

                    /* Keep the ceiling */
                    int ceilingBottom = std::clamp(yTop, top, bottom);
                    planeSegments[x].push_back(
                        { top, ceilingBottom, x, PlaneType::Ceiling, frontSector });

                    /* Upper wall rendering */
                    int upperWallTop = std::max(top, yTop);
                    int upperWallBottom = std::min(bottom, nextTop);
                    drawWallVertical(yTop, nextTop, upperWallTop, upperWallBottom, x, distance,
                                     record, record.upperMaterial, record.upperTextureTransform,
                                     scene);

                    /* Lower wall rendering */
                    int lowerWallTop = std::max(top, nextBottom);
                    int lowerWallBottom = std::min(bottom, yBottom);
                    drawWallVertical(nextBottom, yBottom, lowerWallTop, lowerWallBottom, x,
                                     distance, record, record.lowerMaterial,
                                     record.lowerTextureTransform, scene);

                    /* Keep the floor */
                    int floorTop = std::clamp(yBottom, top, bottom);
                    planeSegments[x].push_back(
                        { floorTop, bottom, x, PlaneType::Floor, frontSector });

                    /* Update clipping for the next sector */
                    top = openingTop;
                    bottom = openingBottom;
                    currentSector = backSector;
                    if (top >= bottom)
                        break;
                }
            }
        }

        /* Floor and ceiling rendering */
#pragma omp parallel for schedule(dynamic, 8)
        for (int x = 0; x < screenWidth; x++)
        {
            auto ray = cam.getRay(x);
            auto rayDir = ray.direction;

            for (const auto &segment : planeSegments[x])
            {
                drawPlaneVertical(segment, x, horizon, scale, cameraHeight, camPos, rayDir, forward,
                                  scene);
            }
        }

        /* Sprite rendering */
        const auto &sectors = scene.getSectors();
        int bottom = screenHeight;
        int top = 0;

        std::vector<SpriteEntry> allSprites;

        for (const auto &sector : sectors)
        {
            const auto &sprites = sector->getSprites();
            for (const auto &sprite : sprites)
            {
                float dist = (camPos.x - sprite->getPos().x) * (camPos.x - sprite->getPos().x)
                    + (camPos.y - sprite->getPos().y) * (camPos.y - sprite->getPos().y);
                allSprites.push_back({ sprite, sector.get(), dist });
            }
        }

        std::sort(allSprites.begin(), allSprites.end(), SpriteEntry::compareSpriteEntry);

        for (const auto &entry : allSprites)
        {
            const auto sprite = entry.sprite;
            const auto sector = entry.sector;
            Math::Vector2 rel = sprite->getPos() - camPos;
            float depth = rel * cam.getForward();
            if (depth < 0.1f)
                continue;
            float side = rel * cam.getRight();
            float mulSize = sprite->getMulSize();
            float mulHeight = sprite->getMulHeight();
            float vPos = sprite->getVPos();

            int yTop = projectScreen(horizon, cameraHeight,
                                     sector->getFloorHeight() + vPos + mulHeight, scale, depth);
            int yBottom =
                projectScreen(horizon, cameraHeight, sector->getFloorHeight() + vPos, scale, depth);
            int SpriteTop = std::clamp(yTop, top, bottom);
            int SpriteBottom = std::clamp(yBottom, top, bottom);
            float screenX = (screenWidth / 2.f) * (1.f + side / depth);
            float size = (scale / depth) * mulSize;

            int startX = screenX - size / 2;
            int endX = screenX + size / 2;

            int drawStartX = std::clamp(startX, 0, screenWidth);
            int drawEndX = std::clamp(endX, 0, screenWidth);

            auto material = sprite->getMaterial().get();

            /* Compute lighting */
            Utils::Color lightContribution(0.f, 0.f, 0.f);
            if (enableLighting)
            {
                for (const auto &light : scene.getLights())
                {
                    auto p = sprite->getPos();
                    auto intensity = light->getIntensityAt(p);
                    lightContribution += light->getColor() * intensity;
                }
                lightContribution = lightContribution.clamp(0.25f, 3.f);
            }

#pragma omp parallel for schedule(static)
            for (int x = drawStartX; x < drawEndX; x++)
            {
                float u = (x - startX) / FLT(endX - startX);

                drawSpriteVertical(yTop, yBottom, SpriteTop, SpriteBottom, x, u, depth, material,
                                   lightContribution);
            }
        }

        /* Finalize image */
#pragma omp parallel for collapse(2)
        for (int y = 0; y < screenHeight; y++)
        {
            for (int x = 0; x < screenWidth; x++)
            {
                const Utils::Color color = image(x, y).clamp();

                const std::size_t index = (static_cast<std::size_t>(y) * screenWidth + x) * 4;

                pixelBuffer[index] = static_cast<std::uint8_t>(color.r * 255.f);
                pixelBuffer[index + 1] = static_cast<std::uint8_t>(color.g * 255.f);
                pixelBuffer[index + 2] = static_cast<std::uint8_t>(color.b * 255.f);
                pixelBuffer[index + 3] = 255;
            }
        }

        texture.update(pixelBuffer.data());

        window.draw(sprite);
        window.display();
    }

    float Renderer::getVerticalFov(float horizontalFov, float aspectRatio) const
    {
        return 2.f * std::atan(std::tan(horizontalFov / 2.f) * aspectRatio);
    }

    int Renderer::projectScreen(float horizon, float cameraHeight, float z, float scale,
                                float distance) const
    {
        float p = horizon - (z - cameraHeight) * scale / distance;
        return static_cast<int>(p);
    }

    void Renderer::drawSpriteVertical(int yTop, int yBottom, int drawTop, int drawBottom, int x,
                                      float u, float depth, const Game::IMaterial *material,
                                      const Utils::Color &lightContribution)
    {
        /* Texture mapping */
        auto texCoord = Math::Point2(0.f, 0.f);
        auto texProperties = material->getDescriptor();

        /* Calculate texture X (u) coordinate */
        if (texProperties.isTextured)
        {
            u -= std::floor(u);
            texCoord.x = u * texProperties.textureWidth;
            texCoord.x = std::clamp(texCoord.x, 0.f, texProperties.textureWidth - 1.f);
        }

        /* Compute fog */
        float fogOpposite = 1.f;
        Utils::Color fogColorIntensity(0.f, 0.f, 0.f);
        if (enableFog)
        {
            auto fog = getFogLevel(depth);
            fogOpposite = 1.f - fog;
            fogColorIntensity = fogColorVec * fog;
        }

        float lineHeight = yBottom - yTop;

        for (int y = drawTop; y < drawBottom; y++)
        {
            if (depth >= zBuffer[idx(x, y)])
            {
                continue;
            }

            if (texProperties.isTextured)
            {
                /* Calculate texture Y (v) coordinate */
                float v = FLT(y - yTop) / FLT(lineHeight);
                v -= std::floor(v);
                texCoord.y = v * texProperties.textureHeight;
                texCoord.y = std::clamp(texCoord.y, 0.f, texProperties.textureHeight - 1.f);
            }

            auto pixelSprite = material->getSample(texCoord).color;

            /* Check if the pixel is transparent */
            if (std::abs(pixelSprite.r) < EPS && std::abs(pixelSprite.g) < EPS
                && std::abs(pixelSprite.b) < EPS)
                continue;

            if (enableLighting)
            {
                pixelSprite = pixelSprite * lightContribution;
            }

            if (enableFog)
            {
                pixelSprite = pixelSprite * fogOpposite + fogColorIntensity;
            }

            zBuffer[idx(x, y)] = depth;
            image(x, y) = pixelSprite;
        }
    }

    float Renderer::getFogLevel(float distance) const
    {
        if (distance <= fogStart)
            return 0.f;
        else if (distance >= fogEnd)
            return 1.f;
        else
            return (distance - fogStart) / (fogEnd - fogStart);
    }

    void Renderer::drawWallVertical(int yTop, int yBottom, int drawTop, int drawBottom, int x,
                                    float distance, const Game::HitRecord &record,
                                    const Game::IMaterial *material,
                                    const Game::TextureTransform &textureTransform,
                                    const Game::Scene &scene)
    {
        /* Texture mapping */
        if (material == nullptr)
            material = record.material;
        auto texProperties = material->getDescriptor();
        auto texCoord = Math::Point2(0.f, 0.f);

        /* Calculate texture X (u) coordinate */
        if (texProperties.isTextured)
        {
            float u = record.u;
            u = u * textureTransform.scaleX + textureTransform.offsetX;
            u -= std::floor(u);
            texCoord.x = u * texProperties.textureWidth;
            texCoord.x = std::clamp(texCoord.x, 0.f, texProperties.textureWidth - 1.f);
        }

        /* Compute lighting */
        Utils::Color lightContribution(0.f, 0.f, 0.f);
        if (enableLighting)
        {
            for (const auto &light : scene.getLights())
            {
                auto p = record.point;
                auto intensity = light->getIntensityAt(p);
                lightContribution += light->getColor() * intensity;
            }
            lightContribution = lightContribution.clamp(0.25f, 3.f);
        }

        /* Compute fog */
        float fogOpposite = 1.f;
        Utils::Color fogColorIntensity(0.f, 0.f, 0.f);
        if (enableFog)
        {
            auto fog = getFogLevel(record.t);
            fogOpposite = 1.f - fog;
            fogColorIntensity = fogColorVec * fog;
        }

        float lineHeight = yBottom - yTop;
        for (int y = drawTop; y < drawBottom; y++)
        {
            if (distance >= zBuffer[idx(x, y)])
            {
                continue;
            }

            if (texProperties.isTextured)
            {
                /* Calculate texture Y (v) coordinate */
                float v = FLT(y - yTop) / FLT(lineHeight);
                v = v * textureTransform.scaleY + textureTransform.offsetY;
                v -= std::floor(v);
                texCoord.y = v * texProperties.textureHeight;
                texCoord.y = std::clamp(texCoord.y, 0.f, texProperties.textureHeight - 1.f);
            }

            auto color = material->getSample(texCoord).color;

            if (enableLighting)
            {
                color = color * lightContribution;
            }

            if (enableFog)
            {
                color = color * fogOpposite + fogColorIntensity;
            }

            zBuffer[idx(x, y)] = distance;
            image(x, y) = color;
        }
    }

    void Renderer::drawPlaneVertical(const PlaneSegment &segment, int x, int horizon, float scale,
                                     float cameraHeight, const Math::Point2 &camPos,
                                     const Math::Vector2 &rayDir, const Math::Vector2 &forward,
                                     const Game::Scene &scene)
    {
        auto zPlane = segment.sector->getFloorHeight();
        auto material = segment.sector->getFloorMaterial();
        auto texDesc = material->getDescriptor();
        auto texTransform = segment.sector->getFloorTextureTransform();

        if (segment.type == PlaneType::Ceiling)
        {
            zPlane = segment.sector->getCeilingHeight();
            material = segment.sector->getCeilingMaterial();
            texDesc = material->getDescriptor();
            texTransform = segment.sector->getCeilingTextureTransform();
        }

        for (int y = segment.yTop; y < segment.yBottom; y++)
        {
            if (y - horizon == 0)
                continue;

            float rowDistance = scale * (zPlane - cameraHeight) / FLT(horizon - y);
            if (rowDistance <= 0.f || rowDistance >= zBuffer[idx(x, y)])
                continue;

            float tRay = rowDistance / (rayDir * forward);
            auto world = camPos + rayDir * tRay;

            Math::Point2 texCoord(0.f, 0.f);

            if (texDesc.isTextured)
            {
                float u = world.x * texTransform.scaleX + texTransform.offsetX;
                u -= std::floor(u);
                float v = world.y * texTransform.scaleY + texTransform.offsetY;
                v -= std::floor(v);

                texCoord.x = u * texDesc.textureWidth;
                texCoord.y = v * texDesc.textureHeight;

                texCoord.x = std::clamp(texCoord.x, 0.f, texDesc.textureWidth - 1.f);
                texCoord.y = std::clamp(texCoord.y, 0.f, texDesc.textureHeight - 1.f);
            }

            auto color = material->getSample(texCoord).color;

            if (enableLighting)
            {
                Utils::Color lightContribution(0.f, 0.f, 0.f);
                for (const auto &light : scene.getLights())
                {
                    auto p = world;
                    auto intensity = light->getIntensityAt(p);
                    lightContribution += light->getColor() * intensity;
                }
                lightContribution = lightContribution.clamp(0.25f, 3.f);
                color = color * lightContribution;
            }

            if (enableFog)
            {
                auto fog = getFogLevel(rowDistance);
                color = color * (1.f - fog) + Utils::Color(fogColor, fogColor, fogColor) * fog;
            }

            zBuffer[idx(x, y)] = rowDistance;
            image(x, y) = color;
        }
    }

    inline int Renderer::idx(int x, int y)
    {
        return y * image.getWidth() + x;
    }
} // namespace Engine
