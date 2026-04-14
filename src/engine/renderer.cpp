#include "engine/renderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <stdexcept>

#include "game/settings.hpp"
#include "math/point2.hpp"

#define T_MIN 0.f
#define T_MAX std::numeric_limits<float>::infinity()

#define CEILING Utils::Color(0.5f, 0.5f, 0.5f)
#define FLOOR Utils::Color(0.3f, 0.3f, 0.3f)

namespace Engine
{
    Renderer::Renderer()
        : image(Game::Settings::get().windowWidth,
                Game::Settings::get().windowHeight)
        , sprite(texture)
    {
        const auto windowWidth = Game::Settings::get().windowWidth;
        const auto windowHeight = Game::Settings::get().windowHeight;

        sf::ContextSettings settings;

        window = sf::RenderWindow(
            sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)),
            "Projet ISIM", sf::Style::Default, sf::State::Windowed, settings);

        view.setSize(sf::Vector2f(windowWidth, windowHeight));
        view.setCenter(sf::Vector2f(static_cast<float>(windowWidth) / 2.0f,
                                    static_cast<float>(windowHeight) / 2.0f));
        window.setView(view);

        window.setFramerateLimit(Game::Settings::get().targetFramerate);
        window.setVerticalSyncEnabled(false);

        if (!texture.resize(sf::Vector2u(windowWidth, windowHeight)))
        {
            throw std::runtime_error("Failed to allocate renderer texture");
        }

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
        const auto screenWidth = image.getWidth();
        const auto screenHeight = image.getHeight();

        const auto &player = scene.getPlayer();

        /* Pre-calculations and naming variables for projection */
        double fovH = player.getCamera().getFov();
        double aspectRatio = static_cast<double>(screenHeight)
            / static_cast<double>(screenWidth);
        double fovV = getVerticalFov(fovH, aspectRatio);
        double scale = (screenHeight / 2.0) / std::tan(fovV / 2.0);
        int horizon = screenHeight / 2;
        float cameraHeight = player.getCamera().getCameraHeight()
            + player.getCamera().getOffsetHeight();

/* RAYCASTING AND WALL PROJECTION */
#pragma omp parallel for schedule(dynamic, 8)
        for (int x = 0; x < screenWidth; x++)
        {
            auto ray = player.getCamera().getRay(x);
            auto records = rayCaster.castRay(ray, scene, T_MIN, T_MAX);

            /* Vertical clipping variables */
            int top = 0;
            int bottom = screenHeight;

            auto currentSector = scene.getCurrentSector();

            for (const auto &record : records)
            {
                /* Maps are supposed to be closed, no hit = no rendering, every
                 * wall is supposed to be textured */
                if (!record.isHit || !currentSector)
                    continue;

                auto frontSector = currentSector;
                auto backSector = record.frontSector == frontSector
                    ? record.backSector
                    : record.frontSector;

                /* Naming variables */
                /* Both vectors already normalized */
                auto rayDirection = ray.direction;
                auto forward = player.getCamera().getForward();

                /* Corrected distance to avoid fish-eye effect */
                float distance = correctDist(record.t, rayDirection, forward);
                distance = std::max(distance, 1e-5f);

                /* Apply the perspective projection formula */
                /* horizon - (z - cameraHeight) * scale / distance */
                int yTop = projectScreen(horizon, cameraHeight,
                                         frontSector->getCeilingHeight(), scale,
                                         distance);
                int yBottom = projectScreen(horizon, cameraHeight,
                                            frontSector->getFloorHeight(),
                                            scale, distance);

                int drawTop = std::max(yTop, top);
                int drawBottom = std::min(yBottom, bottom);

                /* Plain Wall */
                if (record.backSector == nullptr)
                {
                    /* Ceiling rendering */
                    int ceilingBottom = std::clamp(yTop, top, bottom);
                    for (int y = top; y < ceilingBottom; y++)
                    {
                        image(x, y) = CEILING;
                    }

                    /* Wall rendering */
                    drawWallVertical(yTop, yBottom, drawTop, drawBottom, x,
                                     record, record.material,
                                     record.textureTransform.scaleX,
                                     record.textureTransform.offsetX,
                                     record.textureTransform.scaleY,
                                     record.textureTransform.offsetY);

                    /* Floor rendering */
                    int floorTop = std::clamp(yBottom, top, bottom);
                    for (int y = floorTop; y < bottom; y++)
                    {
                        image(x, y) = FLOOR;
                    }

                    /* No portal, no more rendering for this column */
                    break;
                }
                /* Portal */
                else
                {
                    /* Calculate projected coordinates for the next sector */
                    int nextTop = projectScreen(horizon, cameraHeight,
                                                backSector->getCeilingHeight(),
                                                scale, distance);
                    int nextBottom = projectScreen(horizon, cameraHeight,
                                                   backSector->getFloorHeight(),
                                                   scale, distance);

                    /* Calculate the opening between the current sector and the
                     * next one */
                    int openingTop = std::max({ top, yTop, nextTop });
                    int openingBottom =
                        std::min({ bottom, yBottom, nextBottom });

                    /* Ceiling rendering */
                    int ceilingBottom = std::clamp(yTop, top, bottom);
                    for (int y = top; y < ceilingBottom; y++)
                    {
                        image(x, y) = CEILING;
                    }

                    /* Upper wall rendering */
                    int upperWallTop = std::max(top, yTop);
                    int upperWallBottom = std::min(bottom, nextTop);
                    drawWallVertical(yTop, nextTop, upperWallTop,
                                     upperWallBottom, x, record,
                                     record.upperMaterial,
                                     record.upperTextureTransform.scaleX,
                                     record.upperTextureTransform.offsetX,
                                     record.upperTextureTransform.scaleY,
                                     record.upperTextureTransform.offsetY);

                    /* Lower wall rendering */
                    int lowerWallTop = std::max(top, nextBottom);
                    int lowerWallBottom = std::min(bottom, yBottom);
                    drawWallVertical(nextBottom, yBottom, lowerWallTop,
                                     lowerWallBottom, x, record,
                                     record.lowerMaterial,
                                     record.lowerTextureTransform.scaleX,
                                     record.lowerTextureTransform.offsetX,
                                     record.lowerTextureTransform.scaleY,
                                     record.lowerTextureTransform.offsetY);

                    /* Floor rendering */
                    int floorTop = std::clamp(yBottom, top, bottom);
                    for (int y = floorTop; y < bottom; y++)
                    {
                        image(x, y) = FLOOR;
                    }

                    /* Update clipping for the next sector */
                    top = openingTop;
                    bottom = openingBottom;
                    currentSector = backSector;
                    if (top >= bottom)
                        break;
                }
            }
        }

#pragma omp parallel for collapse(2)
        for (int y = 0; y < screenHeight; y++)
        {
            for (int x = 0; x < screenWidth; x++)
            {
                const Utils::Color color = image(x, y).clamp();

                const std::size_t index =
                    (static_cast<std::size_t>(y) * screenWidth + x) * 4;

                pixelBuffer[index] =
                    static_cast<std::uint8_t>(color.r * 255.0f);
                pixelBuffer[index + 1] =
                    static_cast<std::uint8_t>(color.g * 255.0f);
                pixelBuffer[index + 2] =
                    static_cast<std::uint8_t>(color.b * 255.0f);
                pixelBuffer[index + 3] = 255;
            }
        }

        texture.update(pixelBuffer.data());
        image.clear();

        window.draw(sprite);
        window.display();
    }

    float Renderer::correctDist(float distance,
                                const Math::Vector2 &rayDirection,
                                const Math::Vector2 &cameraForward) const
    {
        float cosAngle = rayDirection * cameraForward;
        return distance * cosAngle;
    }

    float Renderer::getVerticalFov(float horizontalFov, float aspectRatio) const
    {
        return 2.f * std::atan(std::tan(horizontalFov / 2.f) * aspectRatio);
    }

    int Renderer::projectScreen(float horizon, float cameraHeight, float z,
                                float scale, float distance) const
    {
        float p = horizon - (z - cameraHeight) * scale / distance;
        return static_cast<int>(p);
    }

    void Renderer::drawWallVertical(int yTop, int yBottom, int top, int bottom,
                                    int x, const Game::HitRecord &record,
                                    const Game::IMaterial *material,
                                    float scaleX, float offsetX, float scaleY,
                                    float offsetY)
    {
        /* Texture mapping */
        if (material == nullptr)
            material = record.material;
        auto texProperties = material->getDescriptor();
        auto texCoord = Math::Point2(0.f, 0.f);

        /* Calculate texture X (u) coordinate */
        float u = record.u;
        u = u * scaleX + offsetX;
        u = std::fmod(u, 1.f);
        texCoord.x = u * texProperties.textureWidth;

        /* Calculate texture Y (v) coordinate */
        float lineHeight = yBottom - yTop;
        if (lineHeight <= 0)
            return;

        float step = (texProperties.textureHeight * scaleY) / lineHeight;
        float v = (top - yTop) * step;
        v += offsetY * texProperties.textureHeight;

        for (int y = top; y < bottom; y++)
        {
            float wrappedV = std::fmod(v, texProperties.textureHeight);
            if (wrappedV < 0.f)
                wrappedV += texProperties.textureHeight;
            texCoord.y = wrappedV;
            v += step;

            image(x, y) = material->getSample(record, texCoord).color;
        }
    }
} // namespace Engine
