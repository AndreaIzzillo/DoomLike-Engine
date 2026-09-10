#include "engine/animation_manager.hpp"

namespace Engine
{
    void AnimationManager::update(float dt)
    {
        for (Game::AnimatedMaterial *animatedMaterial : animatedMaterials)
        {
            animatedMaterial->update(dt);
        }
    }

    void AnimationManager::addAnimatedMaterial(Game::AnimatedMaterial *animatedMaterial)
    {
        animatedMaterials.push_back(animatedMaterial);
    }
} // namespace Engine
