#include "io/mapfile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "game/materials/color_material.hpp"
#include "game/materials/texture_material.hpp"
#include "game/world/wall.hpp"

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
                int res;
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
            else if (type == 'S')
            {
                std::string id, floorHeight, ceilingHeight, floorMat,
                    ceilingMat;
                iss >> id >> floorHeight >> ceilingHeight >> floorMat
                    >> ceilingMat;
                int sectorId = std::stoi(id);
                float floorH = std::stof(floorHeight);
                float ceilingH = std::stof(ceilingHeight);
                floorMat = parseQuoted(floorMat);
                ceilingMat = parseQuoted(ceilingMat);

                if (sectors.find(sectorId) != sectors.end())
                    throw std::runtime_error("Duplicate sector ID: " + id);
                if (materials.find(floorMat) == materials.end())
                    throw std::runtime_error("Undefined material: " + floorMat);
                if (materials.find(ceilingMat) == materials.end())
                    throw std::runtime_error("Undefined material: "
                                             + ceilingMat);

                sectors[sectorId] = std::make_unique<Sector>(
                    floorH, ceilingH, materials[floorMat],
                    materials[ceilingMat]);
            }
            else if (type == 'W')
            {
                std::string startS, endS, mat;
                iss >> startS >> endS >> mat;

                mat = parseQuoted(mat);
                if (materials.find(mat) == materials.end())
                    throw std::runtime_error("Undefined material: " + mat);

                std::string frontS, backS;
                iss >> frontS >> backS;
                int front = std::stoi(frontS);
                int back = std::stoi(backS);
                if (sectors.find(front) == sectors.end())
                    throw std::runtime_error("Undefined sector ID: " + frontS);
                if (back != -1 && sectors.find(back) == sectors.end())
                    throw std::runtime_error("Undefined sector ID: " + backS);

                if (back == -1)
                {
                    walls.push_back(std::make_unique<Wall>(
                        parsePoint(startS), parsePoint(endS),
                        sectors[front].get(), nullptr, materials[mat]));
                }
                else
                {
                    walls.push_back(std::make_unique<Wall>(
                        parsePoint(startS), parsePoint(endS),
                        sectors[front].get(), sectors[back].get(),
                        materials[mat]));
                }
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

    std::vector<std::unique_ptr<Wall>> MapFile::getWalls()
    {
        return std::move(walls);
    }

    std::vector<std::unique_ptr<Sector>> MapFile::getSectors()
    {
        std::vector<std::unique_ptr<Sector>> result;
        for (auto &entry : sectors)
            result.push_back(std::move(entry.second));
        return result;
    }

    const Sector *MapFile::getStartingSector() const
    {
        for (const auto &entry : sectors)
        {
            if (entry.first == 0)
                return entry.second.get();
        }
        return nullptr;
    }
} // namespace IO
