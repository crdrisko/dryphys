// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: entity.hpp
// Author: crdrisko
// Date: 06/30/2024-06:47:00
// Description:

#ifndef DRYPHYS_SRC_DEMOS_SHAPEWATCHER_ENTITY_HPP
#define DRYPHYS_SRC_DEMOS_SHAPEWATCHER_ENTITY_HPP

#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "shapeWatcher/components.hpp"

namespace ShapeWatcher
{
    class Entity
    {
    private:
        friend class EntityManager;

        std::size_t id_  = 0;
        std::string tag_ = "default";
        bool active_     = true;
        std::tuple<CTransform, CShape> components_;

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
}   // namespace ShapeWatcher

#endif
