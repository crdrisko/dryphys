// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: camera.cpp
// Author: crdrisko
// Date: 05/06/2025-06:45:08
// Description:

#include "partitionEngine/engine/camera.hpp"

#include <dryphys/dryphys.hpp>

namespace PartitionEngine
{
    DryPhys::Transform4D Camera::getView() const
    {
        DryPhys::Transform4D pos {1, 0, 0, -position_.x, 0, 1, 0, -position_.y, 0, 0, 1, -position_.z};
        DryPhys::Transform4D cam {
            right_.x, right_.y, right_.z, 0, up_.x, up_.y, up_.z, 0, -front_.x, -front_.y, -front_.z, 0};

        return cam * pos;
    }

    void Camera::updateVectors()
    {
        DryPhys::real yaw_rads   = DryPhys::radians(yaw);
        DryPhys::real pitch_rads = DryPhys::radians(pitch);

        front_.x = std::cos(yaw_rads) * std::cos(pitch_rads);
        front_.y = std::sin(pitch_rads);
        front_.z = std::sin(yaw_rads) * std::cos(pitch_rads);
        front_.normalize();

        right_ = front_.cross(config_.up);
        right_.normalize();

        up_ = right_.cross(front_);
        up_.normalize();
    }

    void Camera::processMouseMovement(DryPhys::real xoffset, DryPhys::real yoffset)
    {
        xoffset *= config_.sensitivity;
        yoffset *= config_.sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0)
            pitch = 89.0;
        else if (pitch < -89.0)
            pitch = -89.0;

        updateVectors();
    }

    void Camera::processMovement(Movement direction, DryPhys::real dt)
    {
        switch (direction)
        {
        case FORWARD:
            position_ += front_ * config_.speed * dt;
            break;
        case BACKWARD:
            position_ -= front_ * config_.speed * dt;
            break;
        case LEFT:
            position_ -= right_ * config_.speed * dt;
            break;
        case RIGHT:
            position_ += right_ * config_.speed * dt;
            break;
        default:
            break;
        }
    }
}   // namespace PartitionEngine
