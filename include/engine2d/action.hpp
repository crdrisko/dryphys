// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: action.hpp
// Author: crdrisko
// Date: 06/17/2024-08:22:06
// Description: A class encapsulating an interaction between the user and engine

#ifndef DRYPHYS_INCLUDE_ENGINE2D_ACTION_HPP
#define DRYPHYS_INCLUDE_ENGINE2D_ACTION_HPP

#include <config.h>

#include <sstream>
#include <string>

#include <dryphys/math.hpp>
#include <dryphys/utilities.hpp>

namespace Engine2D
{
    class Action
    {
    public:
        enum Types
        {
            START,
            END
        };

    private:
        std::string name_ {};         //!< Name of the current action
        DryPhys::StringID sid_ {};    //!< Hashed string id of the current action's name
        Types type_ {};               //!< The type of the current action, options are start or end
        DryPhys::Vector3D mpos_ {};   //!< Location of the mouse if needed by the current action

    public:
        //! Constructors
        Action() = default;
        Action(const std::string& name, Types type) : name_ {name}, sid_ {DryPhys::djb2Hash(name.c_str())}, type_ {type} {}
        Action(const std::string& name, Types type, const DryPhys::Vector3D& mousePos)
            : name_ {name}, sid_ {DryPhys::djb2Hash(name.c_str())}, type_ {type}, mpos_ {mousePos}
        {
        }

        //! Accessors
        std::string name() const { return name_; }
        DryPhys::StringID sid() const { return sid_; }
        Types type() const { return type_; }
        DryPhys::Vector3D pos() const { return mpos_; }

        /*!
         * Returns information about the current action as a string.
         */
        std::string toString() const
        {
            std::stringstream ss;
            ss << name_ << " " << ((type_ == START) ? "START" : "END") << " " << static_cast<int>(mpos_.x) << " "
               << static_cast<int>(mpos_.y);
            return ss.str();
        }
    };
}   // namespace Engine2D

#endif
