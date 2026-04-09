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

    private: /* Helper functions for rendering */
        /** @brief Corrects the distance to avoid fish-eye effect.
         *  @param distance The original distance.
         *  @param rayDirection The direction of the ray.
         *  @param cameraForward The forward direction of the camera.
         *  @return The corrected distance.
         */
        float correctDist(float distance, const Math::Vector2 &rayDirection,
                          const Math::Vector2 &cameraForward) const;

        /** @brief Calculates the vertical field of view.
         *  @param horizontalFov The horizontal field of view.
         *  @param aspectRatio The aspect ratio.
         *  @return The vertical field of view.
         */
        float getVerticalFov(float horizontalFov, float aspectRatio) const;

        /** @brief Projects a point onto the screen.
         *  Very important: projected value is not clamped.
         *  @param horizon The horizon position.
         *  @param cameraHeight The height of the camera.
         *  @param z The z-coordinate of the point.
         *  @param scale The scale factor for the perspective projection.
         *  @param distance The distance to the point.
         *  @return The projected position on the screen.
         */
        int projectScreen(float horizon, float cameraHeight, float z,
                          float scale, float distance) const;
    };
} // namespace Engine
