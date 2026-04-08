#pragma once

#include <math.h>

#include "utils/singleton.hpp"

namespace Game
{
    /**
     * @brief A class representing the game settings.
     *
     * This class is a singleton that holds all the settings of the game, such
     * as window size, camera properties, etc.
     */
    class Settings : public Utils::Singleton<Settings>
    {
        friend class Utils::Singleton<Settings>;

    public:
        /* Window settings */
        unsigned windowWidth = 1600;
        unsigned windowHeight = 900;
        int targetFramerate = 60;

        /* Camera settings */
        float cameraFov = M_PI_2;
        float cameraFocalDistance = 1.0f;

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