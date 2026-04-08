#include "io/mapfile.hpp"

#include <fstream>
#include <sstream>

#include "game/enemy/enemy.hpp"
#include "game/objects/wall.hpp"

namespace IO
{
    MapFile::MapFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            std::cerr << "Cannot open file: " << path << std::endl;
            throw std::runtime_error("Failed to open scene file");
        }

        // P x0,y0 x1,y1 fov focal_distance resolution
        // W x0,y0 x1,y1 "texture"
        // E x,y "sprite"

        auto parsePoint = [](const std::string &s) -> Math::Point2 {
            float x, y;
            sscanf(s.c_str(), "(%f,%f)", &x, &y);
            return { x, y };
        };

        auto parseQuoted = [](const std::string &s) -> std::string {
            size_t a = s.find('"'), b = s.rfind('"');
            return (a != std::string::npos && b != a)
                ? s.substr(a + 1, b - a - 1)
                : s;
        };

        std::string line;
        bool playerSet = false;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream iss(line);
            char type;
            iss >> type;

            if (type == 'P')
            {
                std::string posS, lookS;
                float fov, focal;
                unsigned res;
                iss >> posS >> lookS;
                playerPosition = parsePoint(posS);
                playerLookAt = parsePoint(lookS);
                playerSet = true;
            }
            else if (type == 'W')
            {
                std::string startS, endS, height, tex;
                iss >> startS >> endS >> height;
                std::getline(iss, tex);
                objects.push_back(std::make_unique<Wall>(
                    parsePoint(startS), parsePoint(endS), std::stof(height)));
            }
            else if (type == 'E')
            {
                std::string posS, sprite;
                iss >> posS;
                std::getline(iss, sprite);
                objects.push_back(std::make_unique<Enemy>(parsePoint(posS)));
            }
        }

        if (!playerSet)
            throw std::runtime_error(
                "Scene file missing player definition (P)");
    }

    Player MapFile::getPlayer() const
    {
        return Player(playerPosition, playerLookAt);
    }

    std::vector<std::unique_ptr<IObject>> MapFile::getObjects()
    {
        return std::move(objects);
    }
} // namespace IO