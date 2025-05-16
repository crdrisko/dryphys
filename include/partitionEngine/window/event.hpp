// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: event.hpp
// Author: crdrisko
// Date: 01/20/2025-04:55:50
// Description: An event wrapper to be used with the Window class

#ifndef DRYPHYS_INCLUDE_PARTITIONENGINE_WINDOW_EVENT_HPP
#define DRYPHYS_INCLUDE_PARTITIONENGINE_WINDOW_EVENT_HPP

#include <variant>

#include <dryphys/dryphys.hpp>

namespace PartitionEngine
{
    class Event
    {
    public:
        struct Closed
        {
        };

        struct Resize
        {
            int width, height;
        };

        struct Scroll
        {
            DryPhys::real x, y;
        };

        struct MouseMoved
        {
            DryPhys::real xpos, ypos;
        };

    private:
        std::variant<Closed, Resize, Scroll, MouseMoved> event_;

    public:
        Event() = default;

        template<typename EventType>
        explicit Event(const EventType& event) : event_ {event}
        {
        }

        template<typename EventType>
        EventType* getEvent()
        {
            return std::get_if<EventType>(&event_);
        }
    };
}   // namespace PartitionEngine

#endif
