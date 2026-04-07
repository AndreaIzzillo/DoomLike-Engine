#pragma once

namespace Utils
{
    template <typename T>
    class Singleton
    {
    public:
        static T &instance()
        {
            static T instance;
            return instance;
        }

    protected:
        Singleton() = default;
        ~Singleton() = default;

    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;
    };
} // namespace Utils