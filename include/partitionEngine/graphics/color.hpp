// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: color.hpp
// Author: crdrisko
// Date: 01/12/2025-06:10:06
// Description:

#ifndef DRYPHYS_INCLUDE_PARTITIONENGINE_GRAPHICS_COLOR_HPP
#define DRYPHYS_INCLUDE_PARTITIONENGINE_GRAPHICS_COLOR_HPP

#include <algorithm>

namespace PartitionEngine
{
    /*!
     * @brief 
     */
    struct ColorGL
    {
        float r {}, g {}, b {}, a {1.0f};

        constexpr ColorGL() noexcept = default;
        constexpr ColorGL(float red, float green, float blue, float alpha)
        {
            r = std::clamp(red, 0.0f, 1.0f);
            g = std::clamp(green, 0.0f, 1.0f);
            b = std::clamp(blue, 0.0f, 1.0f);
            a = std::clamp(alpha, 0.0f, 1.0f);
        }

        constexpr ColorGL& operator*=(float s)
        {
            r *= s;
            g *= s;
            b *= s;
            a *= s;

            return *this;
        }

        constexpr ColorGL& operator/=(float s)
        {
            s = 1.0f / s;
            *this *= s;

            return *this;
        }
    };

    /*!
     * 
     */
    struct ColorRGBA
    {
        unsigned char r {}, g {}, b {}, a {255};

        constexpr ColorRGBA() noexcept = default;
        constexpr ColorRGBA(int red, int green, int blue, int alpha) noexcept
            : r {static_cast<unsigned char>(red)}, g {static_cast<unsigned char>(green)},
              b {static_cast<unsigned char>(blue)}, a {static_cast<unsigned char>(alpha)}
        {
        }
        constexpr ColorRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) noexcept
            : r {red}, g {green}, b {blue}, a {alpha}
        {
        }

        constexpr ColorRGBA& operator*=(float s)
        {
            r *= s;
            g *= s;
            b *= s;
            a *= s;

            return *this;
        }

        constexpr ColorRGBA& operator/=(float s)
        {
            s = 1.0f / s;
            *this *= s;

            return *this;
        }

        constexpr ColorGL convertToColorGL() const
        {
            ColorGL colorGL {};

            colorGL.r = static_cast<float>(r) / 255.0f;
            colorGL.g = static_cast<float>(g) / 255.0f;
            colorGL.b = static_cast<float>(b) / 255.0f;
            colorGL.a = static_cast<float>(a) / 255.0f;

            return colorGL;
        }
    };

    namespace Colors
    {
        static inline constexpr ColorRGBA Black {0, 0, 0, 255};
        static inline constexpr ColorRGBA White {255, 255, 255, 255};
        static inline constexpr ColorRGBA Red {255, 0, 0, 255};
        static inline constexpr ColorRGBA Green {0, 255, 0, 255};
        static inline constexpr ColorRGBA Blue {0, 0, 255, 255};
    }   // namespace Colors
}   // namespace PartitionEngine

#endif
