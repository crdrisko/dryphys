// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: ctStringHashing.hpp
// Author: crdrisko
// Date: 06/18/2024-05:17:04
// Description: Compile-time string hashing

#ifndef DRYPHYS_INCLUDE_DRYPHYS_UTILITIES_CTSTRINGHASHING_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_UTILITIES_CTSTRINGHASHING_HPP

#include <cstddef>
#include <cstdint>

namespace DryPhys
{
    using StringID = std::uint32_t;

    /*! 
     * Compile-time hashing function for strings using the DJB2 algorithm. Code was adapted from
     *  <https://stackoverflow.com/a/2112111/11995808>.
     * 
     * \param str - C-Style character array to be hashed
     * 
     * \returns A \c StringID to be used for string comparison at compile-time.
     */
    constexpr StringID djb2Hash(const char* str)
    {
        return *str ? static_cast<StringID>(*str) + 33 * djb2Hash(str + 1) : 5381;
    }

    namespace Literals
    {
        //! User-Defined Literal for compile-time string hashing
        constexpr auto operator""_sID(const char* str, std::size_t) { return djb2Hash(str); }
    }   // namespace Literals
}   // namespace DryPhys

#endif
