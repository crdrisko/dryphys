// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entity.hpp
// Author: crdrisko
// Date: 06/28/2024-07:31:04
// Description:

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <tuple>

#include "engine2d/forwardDeclare.hpp"

namespace Engine2D
{
    template<typename... ComponentTypes>
    class Entity
    {
    private:
        friend class EntityManager<ComponentTypes...>;

        std::size_t id_ {};
        std::string tag_ {};
        bool active_ {true};
        std::tuple<ComponentTypes...> components_;

        Entity(const std::size_t id, const std::string& tag) : id_ {id}, tag_ {tag} {}

    public:
        void destroy() { active_ = false; }
        bool isActive() const { return active_; }
        const std::string& tag() const { return tag_; }

        std::size_t& id() { return id_; }
        const std::size_t& id() const { return id_; }

        template<typename T>
        bool hasComponent() const
        {
            return getComponent<T>().has;
        }

        template<typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            auto& component = getComponent<T>();
            component       = T(std::forward<TArgs>(mArgs)...);
            component.has   = true;
            return component;
        }

        template<typename T>
        T& getComponent()
        {
            return std::get<T>(components_);
        }

        template<typename T>
        const T& getComponent() const
        {
            return std::get<T>(components_);
        }

        template<typename T>
        void removeComponent()
        {
            getComponent<T>() = T();
        }
    };
}   // namespace Engine2D

#endif
