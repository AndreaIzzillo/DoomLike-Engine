#include "io/mapfile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "game/materials/color_material.hpp"
#include "game/materials/texture_material.hpp"
#include "game/walls/plain_wall.hpp"

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

        auto parsePoint = [](const std::string &s) -> Math::Point2 {
            float x, y;
            sscanf(s.c_str(), "(%f,%f)", &x, &y);
            return { x, y };
        };

        auto parseColor = [](const std::string &s) -> Utils::Color {
            float r, g, b;
            sscanf(s.c_str(), "(%f,%f,%f)", &r, &g, &b);
            return { r, g, b };
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
            else if (type == 'M')
            {
                std::string type, name;
                iss >> type >> name;
                name = parseQuoted(name);
                if (type == "Color")
                {
                    std::string colorS;
                    iss >> colorS;
                    materials[name] =
                        std::make_shared<ColorMaterial>(parseColor(colorS));
                }
                else if (type == "Texture")
                {
                    std::string pathS;
                    iss >> pathS;
                    pathS = parseQuoted(pathS);
                    materials[name] = std::make_shared<TextureMaterial>(pathS);
                }
                else
                {
                    std::runtime_error("Unsupported material type: " + type);
                }
            }
            else if (type == 'W')
            {
                std::string startS, endS, mat;
                iss >> startS >> endS;

                std::getline(iss, mat);
                mat = parseQuoted(mat);
                if (materials.find(mat) == materials.end())
                    throw std::runtime_error("Undefined material: " + mat);

                auto wall = std::make_unique<Wall>(parsePoint(startS),
                                                   parsePoint(endS));
                wall->setMaterial(materials[mat]);

                objects.push_back(std::move(wall));
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

    std::vector<std::unique_ptr<IWall>> MapFile::getObjects()
    {
        return std::move(objects);
    }
} // namespace IO
