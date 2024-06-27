// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: action.hpp
// Author: crdrisko
// Date: 06/17/2024-08:22:06
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ACTION_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ACTION_HPP

#include <config.h>

#include <sstream>
#include <string>

#include "dryphys/vector3d.hpp"

namespace Engine2D
{
    class Action
    {
    private:
        std::string name_ = "NONE";
        std::string type_ = "NONE";
        DryPhys::Vector3D mpos_ {};

    public:
        Action() = default;
        Action(const std::string& name, const std::string& type) : name_ {name}, type_ {type} {}
        Action(const std::string& name, const std::string& type, const DryPhys::Vector3D& mousePos)
            : name_ {name}, type_ {type}, mpos_ {mousePos}
        {
        }

        const std::string& name() const { return name_; }
        const std::string& type() const { return type_; }
        const DryPhys::Vector3D& pos() const { return mpos_; }

        std::string toString() const
        {
            std::stringstream ss;
            ss << name_ << " " << type_ << " " << static_cast<int>(mpos_[0]) << " " << static_cast<int>(mpos_[1]);
            return ss.str();
        }
    };
}   // namespace Engine2D

#endif
