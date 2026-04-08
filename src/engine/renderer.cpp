#include "engine/renderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <stdexcept>

#include "game/settings.hpp"

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
        rayCaster.renderFrame(scene, image);

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
} // namespace Engine
