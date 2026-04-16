#pragma once

#include "utils/singleton.hpp"

namespace Game
{
    /**
     * @brief Global runtime configuration shared by engine and gameplay code.
     *
     * Exposes window, camera, and world constants through a singleton accessed
     * by renderer, player camera setup, and hit defaults.
     */
    class Settings : public Utils::Singleton<Settings>
    {
        friend class Utils::Singleton<Settings>;

    public:
        /* Window settings */
        int windowWidth = 1400;
        int windowHeight = 875;
        int targetFramerate = 60;

        /* Camera settings */
        float cameraFov = 1.57079632679f;
        float cameraFocalDistance = 1.f;

        /* World settings */
        float worldFloor = 0.f;
        float worldCeiling = 1.f;

    public:
        static Settings &get()
        {
            return Settings::instance();
        }

    private:
        Settings() = default;
        ~Settings() = default;
    };
} // namespace Game
