// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: camera.hpp
// Author: crdrisko
// Date: 05/03/2025-07:05:56
// Description:

#ifndef DRYPHYS_INCLUDE_PARTITIONENGINE_ENGINE_CAMERA_HPP
#define DRYPHYS_INCLUDE_PARTITIONENGINE_ENGINE_CAMERA_HPP

#include <algorithm>

#include <dryphys/dryphys.hpp>

namespace PartitionEngine
{
    class Camera
    {
    public:
        struct Config
        {
            DryPhys::real speed {2.5};
            DryPhys::real fov {45.0};
            DryPhys::real sensitivity {0.1};
            DryPhys::Vector3D up {0.0, 1.0, 0.0};
        };

        enum Movement
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };

    private:
        Config config_ {};
        DryPhys::Vector3D position_ {};
        DryPhys::Vector3D front_ {0, 0, -1};
        DryPhys::Vector3D up_ {};
        DryPhys::Vector3D right_ {};
        DryPhys::real yaw {-90.0};
        DryPhys::real pitch {};

        void updateVectors();

    public:
        explicit Camera(const DryPhys::Vector3D& position, const Camera::Config& config)
            : config_ {config}, position_ {position}
        {
            updateVectors();
        }

        explicit Camera(const DryPhys::Vector3D& position = DryPhys::Vector3D {},
            DryPhys::real speed                           = 2.5,
            DryPhys::real fov                             = 45.0,
            DryPhys::real sensitivity                     = 0.1,
            const DryPhys::Vector3D& up                   = DryPhys::Vector3D {0, 1, 0})
            : Camera {position, Config {speed, fov, sensitivity, up}}
        {
        }

        Config getConfig() const { return config_; }

        DryPhys::Transform4D getView() const;
        void processMouseScroll(DryPhys::real offset) { config_.fov = std::clamp(config_.fov - offset, 1.0, 45.0); }
        void processMouseMovement(DryPhys::real xoffset, DryPhys::real yoffset);
        void processMovement(Movement direction, DryPhys::real dt);
    };
}   // namespace PartitionEngine

#endif
