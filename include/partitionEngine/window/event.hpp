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

namespace PartitionEngine
{
    class Event
    {
    public:
        struct Closed
        {
        };

        struct Scroll
        {
            double x, y;
        };

        struct Resize
        {
            int width, height;
        };

    private:
        std::variant<Closed, Scroll, Resize> event_;

    public:
        Event() = default;

        template<typename EventType>
        explicit Event(const EventType& event) : event_ {event}
        {
        }

        template<typename EventType>
        EventType getEvent()
        {
            return std::get<EventType>(event_);
        }
    };
}   // namespace PartitionEngine

#endif
