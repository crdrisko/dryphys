// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: utils.hpp
// Author: crdrisko
// Date: 06/18/2024-05:17:04
// Description:

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <cstdint>

namespace DryPhys
{
    using StringID = uint32_t;

    // Compile-time hashing function for strings
    constexpr StringID djb2Hash(const char* input)
    {
        return *input ? static_cast<StringID>(*input) + 33 * djb2Hash(input + 1) : 5381;
    }

    constexpr StringID operator""_sid(const char* str) { return djb2Hash(str); }
}   // namespace DryPhys

#endif
