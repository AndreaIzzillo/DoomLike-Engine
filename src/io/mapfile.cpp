#include "io/mapfile.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "game/materials/color_material.hpp"
#include "game/materials/texture_material.hpp"
#include "game/sprite/sprite.hpp"
#include "game/world/wall.hpp"
#include "math/point2.hpp"
#include "utils/image.hpp"

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
            return (a != std::string::npos && b != a) ? s.substr(a + 1, b - a - 1) : s;
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

            /* Player start position */
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
            /* Material definition */
            else if (type == 'M')
            {
                std::string type, name;
                iss >> type >> name;
                name = parseQuoted(name);
                if (type == "Color")
                {
                    std::string colorS;
                    iss >> colorS;
                    materials[name] = std::make_shared<ColorMaterial>(parseColor(colorS));
                }
                else if (type == "Texture")
                {
                    std::string pathS;
                    iss >> pathS;
                    pathS = parseQuoted(pathS);
                    materials[name] = std::make_shared<TextureMaterial>(pathS);
                }
                else if (type == "Animated")
                {
                    std::string imageFrame, durationMs;
                    iss >> imageFrame >> durationMs;
                    imageFrame = parseQuoted(imageFrame);
                    float duration = std::stof(durationMs);

                    auto image = Utils::Image(imageFrame);

                    if (animations.find(name) == animations.end())
                    {
                        animations[name] =
                            std::make_shared<AnimatedMaterial>(image.getWidth(), image.getHeight());
                        animatedMaterials.push_back(animations[name].get());
                        materials[name] = animations[name];
                    }

                    animations[name]->addImageFrame(image, duration);
                }
                else
                {
                    throw std::runtime_error("Unsupported material type: " + type);
                }
            }
            /* Sector definition */
            else if (type == 'S')
            {
                std::string id, floorHeight, ceilingHeight, floorMat, ceilingMat;
                iss >> id >> floorHeight >> ceilingHeight >> floorMat >> ceilingMat;
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
                    throw std::runtime_error("Undefined material: " + ceilingMat);

                sectors[sectorId] = std::make_unique<Sector>(floorH, ceilingH, materials[floorMat],
                                                             materials[ceilingMat]);
            }
            /* Wall definition */
            else if (type == 'W')
            {
                std::string startS, endS, mat;
                iss >> startS >> endS >> mat;

                mat = parseQuoted(mat);
                if (!mat.empty() && materials.find(mat) == materials.end())
                    throw std::runtime_error("Undefined material: " + mat);

                std::string frontS, backS;
                iss >> frontS >> backS;
                int front = std::stoi(frontS);
                int back = std::stoi(backS);
                if (sectors.find(front) == sectors.end())
                    throw std::runtime_error("Undefined sector ID: " + frontS);
                if (back != -1 && sectors.find(back) == sectors.end())
                    throw std::runtime_error("Undefined sector ID: " + backS);

                std::string upperMatS, lowerMatS;
                iss >> upperMatS >> lowerMatS;
                upperMatS = parseQuoted(upperMatS);
                lowerMatS = parseQuoted(lowerMatS);
                if (!upperMatS.empty() && materials.find(upperMatS) == materials.end())
                    throw std::runtime_error("Undefined material: " + upperMatS);
                if (!lowerMatS.empty() && materials.find(lowerMatS) == materials.end())
                    throw std::runtime_error("Undefined material: " + lowerMatS);

                Wall *wall = nullptr;

                /* Plain wall*/
                if (back == -1)
                {
                    if (mat.empty())
                        throw std::runtime_error("One-sided walls must have a material");

                    walls.push_back(Wall::createPlain(parsePoint(startS), parsePoint(endS),
                                                      sectors[front].get(), materials[mat]));

                    wall = walls.back().get();
                    sectors[front]->addWall(wall);
                }
                /* Portal */
                else
                {
                    if (upperMatS.empty() || lowerMatS.empty())
                        throw std::runtime_error(
                            "Portal walls must have upper and lower materials");

                    walls.push_back(Wall::createPortal(parsePoint(startS), parsePoint(endS),
                                                       sectors[front].get(), sectors[back].get(),
                                                       materials[upperMatS], materials[lowerMatS]));

                    wall = walls.back().get();
                    sectors[front]->addWall(wall);
                    sectors[back]->addWall(wall);
                }
            }
            else if (type == 'T')
            {
                std::string posS, materialS;
                float mulHeight, mulSize, vPos;
                std::string sectorS;

                iss >> posS >> materialS >> mulHeight >> mulSize >> vPos >> sectorS;

                materialS = parseQuoted(materialS);
                int sectorId = std::stoi(sectorS);

                if (sectors.find(sectorId) == sectors.end())
                    throw std::runtime_error("Undefined sector ID for sprite: " + sectorS);

                if (materials.find(materialS) == materials.end())
                    throw std::runtime_error("Undefined material for sprite: " + materialS);

                sprites.push_back(std::make_unique<Sprite>(parsePoint(posS), materials[materialS],
                                                           mulHeight, mulSize, vPos));

                sectors[sectorId]->addSprite(sprites.back().get());
            }
        }

        if (!playerSet)
            throw std::runtime_error("Scene file missing player definition (P)");
        if (materials.empty())
            throw std::runtime_error("Scene file must define at least one material");
        if (sectors.empty())
            throw std::runtime_error("Scene file must define at least one sector");
        if (sectors.find(0) == sectors.end())
            throw std::runtime_error("Scene file must define sector with ID 0 as starting sector");
    }

    Player MapFile::getPlayer() const
    {
        return Player(playerPosition, playerLookAt);
    }

    std::vector<std::unique_ptr<Wall>> MapFile::getWalls()
    {
        return std::move(walls);
    }

    std::vector<std::unique_ptr<Sprite>> MapFile::getSprites()
    {
        return std::move(sprites);
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

    std::vector<AnimatedMaterial *> &MapFile::getAnimatedMaterials()
    {
        return animatedMaterials;
    }
} // namespace IO
