#ifndef TOOLS_HPP
#define TOOLS_HPP

namespace as
{
    template <typename T, typename F, typename... Args>
    inline void sset(T target, F first, Args... rest)
    {
        static_cast<F>(target) = first;
        sset(target, rest...);
    }

    template <typename T, typename F>
    inline void sset(T target, F first)
    {
        static_cast<F>(target) = first;
    }
}; // namespace as

#endif // TOOLS_HPP
