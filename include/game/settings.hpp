#pragma once

#include <math.h>

#include "utils/singleton.hpp"

namespace Game
{
    class Settings : public Utils::Singleton<Settings>
    {
        friend class Utils::Singleton<Settings>;

    public:
        /*
         * Renderer settings
         */
        unsigned windowWidth = 800;
        unsigned windowHeight = 600;
        int targetFramerate = 60;

        /*
         * Camera settings
         */
        float cameraFov = M_PI_2;
        float cameraFocalDistance = 1.0f;

        unsigned cameraResolution = windowWidth;

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