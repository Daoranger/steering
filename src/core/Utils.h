//
// Created by hoang on 6/14/2026.
//

#ifndef STEERINGBEHAVIORS_UTILS_H
#define STEERINGBEHAVIORS_UTILS_H

#include <random>

template <typename T>
T randomInRange(T min, T max)
{
    static std::mt19937 rng(std::random_device{}());
    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(rng);
    }
    else
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(rng);
    }
}

// util for wander target's position
float randomClamped()
{
    return randomInRange(-1.f, 1.f);
}

#endif //STEERINGBEHAVIORS_UTILS_H
