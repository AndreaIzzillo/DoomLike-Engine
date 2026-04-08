#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/scene/scene.hpp"
#include "math/vector2.hpp"

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
     * - Renderer generates the current frame using RayCaster (based on the
     * updated scene)
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

    private:
        /* Helper functions for rendering */
        float correctDist(float distance, const Math::Vector2 &rayDirection,
                          const Math::Vector2 &cameraForward) const;
        float getVerticalFov(float horizontalFov, float aspectRatio) const;
        unsigned project(float horizon, float cameraHeight, float z,
                         float scale, float distance) const;
    };
} // namespace Engine
