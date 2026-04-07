#include "utils/singleton.hpp"

#include "game/settings.hpp"

namespace Utils
{
    template <typename T>
    Singleton<T>::Singleton() = default;

    template <typename T>
    Singleton<T>::~Singleton() = default;

    template <typename T>
    T &Singleton<T>::instance()
    {
        static T instance;
        return instance;
    }

    template class Singleton<Game::Settings>;
} // namespace Utils
