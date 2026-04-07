#include <SFML/Graphics.hpp>

#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/scene.hpp"

namespace Engine
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() = default;

        sf::RenderWindow &getWindow();
        sf::View &getView();

        void render(const Game::Scene &scene);

    private:
        sf::RenderWindow window;
        sf::View view;

        RayCaster rayCaster;

        Utils::Image image;
        sf::Texture texture;
        sf::Sprite sprite;
        std::vector<std::uint8_t> pixelBuffer;
    };
} // namespace Engine
