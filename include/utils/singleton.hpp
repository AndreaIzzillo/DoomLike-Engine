#pragma once

namespace Utils
{
    template <typename T>
    class Singleton
    {
    public:
        static T &instance();

    protected:
        Singleton();
        ~Singleton();

    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;
    };
} // namespace Utils
