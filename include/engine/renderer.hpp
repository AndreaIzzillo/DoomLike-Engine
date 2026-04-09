#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/scene/scene.hpp"
#include "math/vector2.hpp"

namespace Engine
{
    /**
     * @brief Owns the window and renders the scene with a column raycaster.
     *
     * It casts camera rays through RayCaster, projects wall slices with
     * floor/ceiling filling into a software buffer, then uploads the buffer to
     * an SFML texture for display.
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
        float correctDist(float distance, const Math::Vector2 &rayDirection,
                          const Math::Vector2 &cameraForward) const;

        float getVerticalFov(float horizontalFov, float aspectRatio) const;

        int projectScreen(float horizon, float cameraHeight, float z,
                          float scale, float distance) const;
    };
} // namespace Engine
