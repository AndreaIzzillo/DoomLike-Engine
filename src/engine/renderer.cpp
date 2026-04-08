#include "engine/renderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <stdexcept>

#include "game/settings.hpp"

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
        constexpr float worldFloor = 0.f;
        constexpr float worldCeiling = 2.f;

        const auto &player = scene.getPlayer();

        for (unsigned x = 0; x < image.getWidth(); x++)
        {
            auto ray = player.getCamera().getRay(x);
            auto record = rayCaster.castRay(ray, scene, T_MIN, T_MAX);

            if (record.isHit)
            {
                /* Naming variables */
                unsigned screenHeight = image.getHeight();
                unsigned screenWidth = image.getWidth();
                unsigned horizon = screenHeight / 2;
                float cameraHeight = player.getCamera().getCameraHeight();
                // Both vectors already normalized
                auto rayDirection = ray.direction;
                auto forward = player.getCamera().getForward();

                /* Calculate the corrected distance to avoid fish-eye effect */
                float distance = correctDist(record.t, rayDirection, forward);

                /* Calculate the scale factor for the perspective projection */
                float fovH = player.getCamera().getFov();
                float aspectRatio = static_cast<float>(screenHeight)
                    / static_cast<float>(screenWidth);
                float fovV = getVerticalFov(fovH, aspectRatio);
                float scale = (screenHeight / 2.f) / std::tan(fovV / 2.f);

                /* Apply the perspective projection formula */
                // horizon - (z - cameraHeight) * scale / distance
                auto screenCeiling = project(horizon, cameraHeight,
                                             record.ceiling, scale, distance);
                auto screenFloor = project(horizon, cameraHeight, record.floor,
                                           scale, distance);

                for (unsigned y = 0; y < screenCeiling; y++)
                {
                    image(x, y) = Utils::Color(0.5f, 0.7f, 1.0f);
                }
                for (unsigned y = screenCeiling; y < screenFloor; y++)
                {
                    image(x, y) = record.material->getProperties(record).color;
                }
                for (unsigned y = screenFloor; y < image.getHeight(); y++)
                {
                    image(x, y) = Utils::Color(0.3f, 0.3f, 0.3f);
                }
            }
        }

        const auto width = image.getWidth();
        const auto height = image.getHeight();

#pragma omp parallel for collapse(2)
        for (unsigned y = 0; y < height; y++)
        {
            for (unsigned x = 0; x < width; x++)
            {
                const Utils::Color color = image(x, y).clamp();

                const std::size_t index =
                    (static_cast<std::size_t>(y) * width + x) * 4;

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

        window.clear(sf::Color::Black);
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

    unsigned Renderer::project(float horizon, float cameraHeight, float z,
                               float scale, float distance) const
    {
        float p = horizon - (z - cameraHeight) * scale / distance;
        int screenProjection = static_cast<int>(p);
        return static_cast<unsigned>(std::clamp(
            screenProjection, 0, static_cast<int>(image.getHeight() - 1)));
    }
} // namespace Engine
