#include "game/scene.hpp"

#include <fstream>

#include "game/enemy.hpp"
#include "game/segment.hpp"

namespace Game
{
    Scene::Scene(std::unique_ptr<Player> player)
        : player_(std::move(player))
    {}

    Scene::Scene(const std::string &path)
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
                iss >> posS >> lookS >> fov >> focal >> res;
                player_ = std::make_unique<Player>(
                    parsePoint(posS), parsePoint(lookS), fov, focal, res);
                playerSet = true;
            }
            else if (type == 'W')
            {
                std::string startS, endS, tex;
                iss >> startS >> endS;
                std::getline(iss, tex);
                objects_.push_back(std::make_unique<Segment>(parsePoint(startS),
                                                             parsePoint(endS)));
            }
            else if (type == 'E')
            {
                std::string posS, sprite;
                iss >> posS;
                std::getline(iss, sprite);
                objects_.push_back(std::make_unique<Enemy>(parsePoint(posS)));
            }
        }

        if (!playerSet)
            throw std::runtime_error(
                "Scene file missing player definition (P)");
    }

    const Player &Scene::get_player() const
    {
        return *player_;
    }

    const std::vector<std::unique_ptr<IObject>> &Scene::get_objects() const
    {
        return objects_;
    }

    void Scene::update(float dt)
    {
        player_->update(dt);
        for (const auto &object : objects_)
            object->update(dt);
    }

    void Scene::add_object(std::unique_ptr<IObject> object)
    {
        objects_.push_back(std::move(object));
    }
} // namespace Game