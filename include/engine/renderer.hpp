#include <SFML/Graphics.hpp>
#include <SFML/Window/Cursor.hpp>
#include <cstdint>
#include <vector>

#include "engine/raycaster.hpp"
#include "game/interfaces/imaterial.hpp"
#include "game/scene/scene.hpp"
#include "game/settings.hpp"
#include "game/world/wall.hpp"

namespace Engine
{
    enum class PlaneType
    {
        Floor,
        Ceiling
    };

    struct PlaneSegment
    {
        int yTop = 0;
        int yBottom = 0;

        int x = 0;

        PlaneType type = PlaneType::Floor;
        const Game::Sector *sector = nullptr;
    };

    struct SpriteEntry
    {
        Game::Sprite *sprite = nullptr;
        Game::Sector *sector = nullptr;
        float distance = 0.f;

        static bool compareSpriteEntry(const SpriteEntry &a, const SpriteEntry &b)
        {
            return a.distance > b.distance;
        };
    };

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
        std::vector<float> zBuffer;

        /* Rendering management */
        std::vector<std::vector<PlaneSegment>> planeSegments;

        /* Fog properties */
        const bool enableFog = Game::Settings::get().enableFog;
        static constexpr float fogStart = 10.f;
        static constexpr float fogEnd = 15.f;
        static constexpr float fogColor = 0.05f;
        const Utils::Color fogColorVec = Utils::Color(fogColor, fogColor, fogColor);

        /* Lighting properties */
        const bool enableLighting = Game::Settings::get().enableLighting;

    private: /* Drawing methods */
        void drawWallVertical(int yTop, int yBottom, int drawTop, int drawBottom, int x,
                              float distance, const Game::HitRecord &record,
                              const Game::IMaterial *material,
                              const Game::TextureTransform &textureTransform,
                              const Game::Scene &scene);

        void drawPlaneVertical(const PlaneSegment &segment, int x, int horizon, float scale,
                               float cameraHeight, const Math::Point2 &camPos,
                               const Math::Vector2 &rayDir, const Math::Vector2 &forward,
                               const Game::Scene &scene);

        void drawSpriteVertical(int yTop, int yBottom, int drawTop, int drawBottom, int x, float u,
                                float depth, const Game::IMaterial *material,
                                const Utils::Color &lightContribution);

    private: /* Helper methods */
        float getVerticalFov(float horizontalFov, float aspectRatio) const;

        int projectScreen(float horizon, float cameraHeight, float z, float scale,
                          float distance) const;

        float getFogLevel(float distance) const;

        inline int idx(int x, int y);
    };
} // namespace Engine
