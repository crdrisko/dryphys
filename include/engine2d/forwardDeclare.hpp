// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: forwardDeclare.hpp
// Author: crdrisko
// Date: 07/04/2024-10:44:15
// Description:

#ifndef FORWARDDECLARE_HPP
#define FORWARDDECLARE_HPP

namespace Engine2D
{
    class Action;
    class Animation;
    class Assets;
    class Component;
    class Engine;
    class Scene;
    class TextureSheet;

    template<typename... ComponentTypes>
    class Entity;

    template<typename... ComponentTypes>
    class EntityManager;
}   // namespace Engine2D

#endif
