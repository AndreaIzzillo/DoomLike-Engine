#pragma once

#include "game/materials/animated_material.hpp"

namespace Engine
{
    class AnimationManager
    {
    public:
        AnimationManager() = default;

        void update(float dt);

        void addAnimatedMaterial(Game::AnimatedMaterial *animatedMaterial);

    private:
        std::vector<Game::AnimatedMaterial *> animatedMaterials;
    };
} // namespace Engine