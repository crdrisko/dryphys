// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/04/2024-10:44:15
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_FORWARDDECLARE_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_FORWARDDECLARE_HPP

#include <cstddef>

namespace Engine2D
{
    class Action;
    class Animation;
    class Assets;
    class Component;
    class Engine;
    class Scene;
    class TextureSheet;

    static const inline std::size_t MAX_ENTITIES {4096};

    template<typename... ComponentTypes>
    class EntityMemoryPool;

    template<typename... ComponentTypes>
    class EntityMP;

    template<typename... ComponentTypes>
    class EntityManagerMP;

    template<typename... ComponentTypes>
    class Entity;

    template<typename... ComponentTypes>
    class EntityManager;
}   // namespace Engine2D

#endif
