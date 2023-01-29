/**
 * @file Immovable.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

namespace Savanna
{
    class Immovable
    {
    protected:
        Immovable() = default;
        ~Immovable() = default;

    private:
        Immovable(Immovable&&) = delete;
        Immovable& operator=(Immovable&&) = delete;
    };
} // namespace Savanna
