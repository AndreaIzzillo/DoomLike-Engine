#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/interfaces/imaterial.hpp"
#include "game/scene/scene.hpp"
#include "game/world/wall.hpp"

namespace Engine
{
    /**
     * @brief Enumerate the types of planes that can be rendered.
     */
    enum class PlaneType
    {
        Floor,
        Ceiling
    };

    /**
     * @brief Represents a segment of a plane in the rendered image.
     *
     * Used for floor and ceiling rendering, it stores the vertical span.
     */
    struct PlaneSegment
    {
        int yTop = 0;
        int yBottom = 0;

        int x = 0;

        PlaneType type = PlaneType::Floor;
        const Game::Sector *sector = nullptr;
    };

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

        /* Fog properties */
        static constexpr float fogStart = 15.f;
        static constexpr float fogEnd = 25.f;
        static constexpr float fogColor = 0.05f;

    private: /* Drawing methods */
        void drawWallVertical(int yTop, int yBottom, int drawTop, int drawBottom, int x,
                              const Game::HitRecord &record, const Game::IMaterial *material,
                              const Game::TextureTransform &textureTransform);

        void drawPlaneVertical(const PlaneSegment &segment, int x, int horizon, float scale,
                               float cameraHeight, const Math::Point2 &camPos,
                               const Math::Vector2 &rayDir, const Math::Vector2 &forward);

    private: /* Helper methods */
        float getVerticalFov(float horizontalFov, float aspectRatio) const;

        int projectScreen(float horizon, float cameraHeight, float z, float scale,
                          float distance) const;

        float getFogLevel(float distance) const;
    };
} // namespace Engine
