#pragma once

#include <math.h>

#include "utils/singleton.hpp"

namespace Game
{
    class Settings : public Utils::Singleton<Settings>
    {
        friend class Utils::Singleton<Settings>;

    public:
        /* Camera Settings */
        float cameraFov = M_PI_2;
        float cameraFocalDistance = 1.0;

        unsigned cameraResolution = 800;

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