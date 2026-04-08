#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/scene/scene.hpp"

namespace Engine
{
    /**
     * @brief A class for rendering the game scene.
     *
     * The Renderer class is responsible for taking the generated frame from the
     * RayCaster and displaying it on the screen.
     *
     * Engine classes execution flow:
     * - Runner updates its InputManager (updates the scene components fields)
     * - Runner updates its Scene (calculates physics, logic, etc.)
     * - RayCaster generates the current frame (based on the updated scene)
     * - Renderer takes the generated frame and updates the display
     */
    class Renderer
    {
    public:
        Renderer();
        ~Renderer() = default;

        sf::RenderWindow &getWindow();
        sf::View &getView();

        void render(const Game::Scene &scene);

    private:
        /* SFML Display */
        sf::RenderWindow window;
        sf::View view;

        /* RayCaster for generating the frame */
        RayCaster rayCaster;

        /* Frame buffer management */
        Utils::Image image;
        sf::Texture texture;
        sf::Sprite sprite;
        std::vector<std::uint8_t> pixelBuffer;
    };
} // namespace Engine
