#include "engine/collision_manager.hpp"

#include <cfloat>
#include <tuple>

#include "math/point2.hpp"
#include "math/ray.hpp"

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    /* Double Bouble Dégezulasse pas trop opti mais bonne soluce pour les coins*/
    Math::Vector2
    CollisionManager::resolveVelocity(Math::Vector2 intent,
                                      const Game::Player &player,
                                      const Game::Scene &scene) const
    {
        const auto &camera = player.getCamera();
        const Math::Vector2 relativeIntent =
            camera.getRight() * intent.x + camera.getForward() * intent.y;
        const float norm = relativeIntent.norm();

        if (norm < FLT_EPSILON)
            return intent;

        for (auto &object : scene.getWalls())
        {
            Game::HitRecord hit =
                object->hit(Math::Ray(player.getPosition(), relativeIntent),
                            FLT_EPSILON, norm + hitboxSize);

            if (hit.isHit)
            {
                Math::Vector2 wallNormal(hit.normal.x, hit.normal.y);

                /* C NUL MAIS C POUR TESTER COMME SI LES MURS ETAIS DROITS*/
                if (wallNormal * relativeIntent < 0.f)
                    wallNormal = wallNormal * -1.f;

                const float penetrationDepth = relativeIntent * wallNormal;
                Math::Vector2 slide =
                    relativeIntent - wallNormal * penetrationDepth;

                slide = slide - wallNormal * (hitboxSize * 0.05f);

                // Clamp a la vitesse original pour éviter l'accélération sur
                // le côté quand on fonce dans un mur
                // resultat douteux mais qui marche bien pour les coins
                if (slide.norm() > norm)
                    slide = slide * (norm / slide.norm());

                if (slide.norm() < FLT_EPSILON)
                    return Math::Vector2(0.f, 0.f);

                for (auto &object2 : scene.getWalls())
                {
                    Game::HitRecord hit2 =
                        object2->hit(Math::Ray(player.getPosition(), slide),
                                     FLT_EPSILON, slide.norm() + hitboxSize);

                    if (hit2.isHit)
                    {
                        Math::Vector2 wallNormal2(hit2.normal.x, hit2.normal.y);

                        /* PAREIL */
                        if (wallNormal2 * slide < 0.f)
                            wallNormal2 = wallNormal2 * -1.f;

                        const float penetrationDepth2 = slide * wallNormal2;
                        slide = slide - wallNormal2 * penetrationDepth2;
                        slide = slide - wallNormal2 * (hitboxSize * 0.05f);

                        if (slide.norm() > norm)
                            slide = slide * (norm / slide.norm());

                        if (slide.norm() < FLT_EPSILON)
                            return Math::Vector2(0.f, 0.f);

                        break;
                    }
                }

                /* faut bien penser a faire ça pouir ce mettre dans le bon referenciel sinon passe a traver sles murs*/
                return Math::Vector2(slide * camera.getRight(),
                                     slide * camera.getForward());
            }
        }

        return intent;
    }
} // namespace Engine
