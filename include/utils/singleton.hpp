#pragma once

namespace Utils
{
    /**
     * @brief CRTP singleton helper providing a lazy static instance.
     *
     * Used by Settings to expose shared runtime configuration without global
     * mutable objects spread across the codebase.
     */
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
