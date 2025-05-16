// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: learnOpenGL.hpp
// Author: crdrisko
// Date: 04/24/2025-12:09:25
// Description: Helper functions and classes for the Learn OpenGL examples

#ifndef DRYPHYS_SRC_DEMOS_LEARNOPENGL_LEARNOPENGL_HPP
#define DRYPHYS_SRC_DEMOS_LEARNOPENGL_LEARNOPENGL_HPP

#include <map>
#include <memory>
#include <string>

#include <dryphys/dryphys.hpp>
#include <partitionEngine/partitionEngine.hpp>

std::map<std::string, std::shared_ptr<PartitionEngine::MeshArray>> parseMeshFile(const std::string& filename);

DryPhys::Transform4D lookAt(const DryPhys::Vector3D& cameraPos,
    const DryPhys::Vector3D& cameraTarget = DryPhys::Vector3D {0, 0, 0},
    const DryPhys::Vector3D& up           = DryPhys::Vector3D {0, 1, 0});

DryPhys::Matrix4D makeProjection(
    DryPhys::real fovy, DryPhys::real aspectRatio, DryPhys::real nearPlane, DryPhys::real farPlane);

DryPhys::Transform4D rotate(DryPhys::real angle, const DryPhys::Vector3D& a);
DryPhys::Transform4D rotateAboutX(DryPhys::real angle);
DryPhys::Transform4D rotateAboutY(DryPhys::real angle);
DryPhys::Transform4D rotateAboutZ(DryPhys::real angle);

constexpr DryPhys::Transform4D translate(const DryPhys::Vector3D& vec)
{
    return DryPhys::Transform4D {1, 0, 0, vec.x, 0, 1, 0, vec.y, 0, 0, 1, vec.z};
}

constexpr DryPhys::Transform4D scale(DryPhys::real scale)
{
    return DryPhys::Transform4D {scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0};
}

#endif
