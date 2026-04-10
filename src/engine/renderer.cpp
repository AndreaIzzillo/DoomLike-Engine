#include "engine/renderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <stdexcept>

#include "game/settings.hpp"
#include "math/point2.hpp"

#define T_MIN FLT_EPSILON
#define T_MAX std::numeric_limits<float>::infinity()

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
        unsigned horizon = screenHeight / 2;
        float cameraHeight = player.getCamera().getCameraHeight()
            + player.getCamera().getOffsetHeight();

        /* RAYCASTING AND WALL PROJECTION */
        for (unsigned x = 0; x < screenWidth; x++)
        {
            auto ray = player.getCamera().getRay(x);
            auto record = rayCaster.castRay(ray, scene, T_MIN, T_MAX);

            if (record.isHit)
            {
                /* Naming variables */
                /* Both vectors already normalized */
                auto rayDirection = ray.direction;
                auto forward = player.getCamera().getForward();

                /* Calculate the corrected distance to avoid fish-eye effect */
                float distance = correctDist(record.t, rayDirection, forward);

                /* Apply the perspective projection formula */
                /* horizon - (z - cameraHeight) * scale / distance */
                int projectedCeiling = projectScreen(
                    horizon, cameraHeight, Game::Settings::get().worldCeiling,
                    scale, distance);
                int projectedFloor = projectScreen(
                    horizon, cameraHeight, Game::Settings::get().worldFloor,
                    scale, distance);
                unsigned screenCeiling = static_cast<unsigned>(std::clamp(
                    projectedCeiling, 0, static_cast<int>(screenHeight)));
                unsigned screenFloor = static_cast<unsigned>(std::clamp(
                    projectedFloor, 0, static_cast<int>(screenHeight)));

                /* ================= */
                /* CEILING RENDERING */
                /* ================= */
                for (unsigned y = 0; y < screenCeiling; y++)
                {
                    image(x, y) = Utils::Color(0.5f, 0.7f, 1.0f);
                }

                auto material = record.material;

                /* Texture mapping */
                auto texProperties = material->getDescriptor();
                auto texCoord = Math::Point2(0.f, 0.f);

                /* Calculate texture X (u) coordinate */
                float u = record.u;
                u = u * record.textureScaleX + record.textureOffsetX;
                u = std::fmod(u, 1.f);
                texCoord.x = u * texProperties.textureWidth;

                /* Calculate texture Y (v) coordinate */
                float lineHeight = projectedFloor - projectedCeiling;
                float step =
                    (texProperties.textureHeight * record.textureScaleY)
                    / lineHeight;
                float v = (screenCeiling - projectedCeiling) * step;
                v += record.textureOffsetY * texProperties.textureHeight;

                /* ============== */
                /* WALL RENDERING */
                /* ============== */
                for (unsigned y = screenCeiling; y < screenFloor; y++)
                {
                    float wrappedV = std::fmod(v, texProperties.textureHeight);
                    if (wrappedV < 0.f)
                        wrappedV += texProperties.textureHeight;
                    texCoord.y = wrappedV;
                    v += step;

                    image(x, y) = material->getSample(record, texCoord).color;
                }

                /* =============== */
                /* FLOOR RENDERING */
                /* =============== */
                for (unsigned y = screenFloor; y < screenHeight; y++)
                {
                    image(x, y) = Utils::Color(0.3f, 0.3f, 0.3f);
                }
            }
        }

#pragma omp parallel for collapse(2)
        for (unsigned y = 0; y < screenHeight; y++)
        {
            for (unsigned x = 0; x < screenWidth; x++)
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
} // namespace Engine
