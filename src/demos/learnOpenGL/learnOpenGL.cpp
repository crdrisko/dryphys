// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: learnOpenGL.cpp
// Author: crdrisko
// Date: 04/24/2025-12:10:15
// Description: Helper functions for the Learn OpenGL examples

#include "learnOpenGL/learnOpenGL.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <drychem/drychem.hpp>
#include <dryphys/dryphys.hpp>
#include <partitionEngine/partitionEngine.hpp>

using namespace DryChem::Literals;
using namespace DryPhys;
using namespace PartitionEngine;

std::map<std::string, std::shared_ptr<MeshArray>> parseMeshFile(const std::string& filename)
{
    DryChem::FileParser parser {"dryphys/src/demos/learnOpenGL/assets/meshes/" + filename};
    auto rows = parser.parseDataFile(DryChem::AsRows());

    std::vector<std::string> meshNames;
    std::map<std::string, std::vector<Vertex>> vertices;
    std::map<std::string, std::vector<unsigned int>> indices;

    for (const auto& row : rows)
    {
        DryChem::Tokenizer tok {row};
        std::vector<std::string> splitRow = tok.split();

        switch (DryChem::djb2Hash(splitRow[0].c_str()))
        {
        case "Vertex"_sID:
        {
            Vertex vertex {};

            switch (splitRow.size())
            {
            case 13:
                vertex.color_ = Vector3D {std::stof(splitRow[10]), std::stof(splitRow[11]), std::stof(splitRow[12])};
            case 10:
                vertex.texCoords_ = Vector2D {std::stof(splitRow[8]), std::stof(splitRow[9])};
            case 8:
                vertex.normal_ = Vector3D {std::stof(splitRow[5]), std::stof(splitRow[6]), std::stof(splitRow[7])};
            case 5:
                vertex.position_ = Vector3D {std::stof(splitRow[2]), std::stof(splitRow[3]), std::stof(splitRow[4])};
            default:
                break;
            }

            vertices[splitRow[1]].push_back(vertex);
            break;
        }
        case "Index"_sID:
        {
            indices[splitRow[1]].push_back(std::stoi(splitRow[2]));
            indices[splitRow[1]].push_back(std::stoi(splitRow[3]));
            indices[splitRow[1]].push_back(std::stoi(splitRow[4]));
            break;
        }
        case "Mesh"_sID:
        {
            meshNames.push_back(splitRow[1]);
            break;
        }
        }
    }

    std::map<std::string, std::shared_ptr<MeshArray>> meshes;

    for (const auto& name : meshNames)
    {
        if (indices.empty())
            meshes[name] = std::make_shared<MeshArray>(vertices[name]);
        else
            meshes[name] = std::make_shared<Mesh>(vertices[name], indices[name]);
    }

    return meshes;
}

Transform4D lookAt(const Vector3D& cameraPos, const Vector3D& cameraTarget, const Vector3D& up)
{
    Vector3D direction = cameraPos - cameraTarget;
    direction.normalize();

    Vector3D right = up.cross(direction);
    right.normalize();

    Vector3D cameraUp = direction.cross(right);

    Transform4D pos {1, 0, 0, -cameraPos.x, 0, 1, 0, -cameraPos.y, 0, 0, 1, -cameraPos.z};
    Transform4D cam {
        right.x, right.y, right.z, 0, cameraUp.x, cameraUp.y, cameraUp.z, 0, direction.x, direction.y, direction.z, 0};

    return cam * pos;
}

Matrix4D makeProjection(real fovy, real aspectRatio, real nearPlane, real farPlane)
{
    real g = 1 / std::tan(fovy * 0.5);

    return Matrix4D {g / aspectRatio,
        0,
        0,
        0,
        0,
        g,
        0,
        0,
        0,
        0,
        -(farPlane + nearPlane) / (farPlane - nearPlane),
        -(2 * farPlane * nearPlane) / (farPlane - nearPlane),
        0,
        0,
        -1,
        0};
}

Transform4D rotate(real angle, const Vector3D& vec)
{
    Vector3D axis = vec;
    axis.normalize();

    real s = std::sin(angle);
    real c = std::cos(angle);
    real d = static_cast<real>(1) - c;

    real x = axis.x * d;
    real y = axis.y * d;
    real z = axis.z * d;

    real axay = x * axis.y;
    real axaz = x * axis.z;
    real ayaz = y * axis.z;

    return Transform4D {c + x * axis.x,
        axay - s * axis.z,
        axaz + s * axis.y,
        0,
        axay + s * axis.z,
        c + y * axis.y,
        ayaz - s * axis.x,
        0,
        axaz - s * axis.y,
        ayaz + s * axis.x,
        c + z * axis.z,
        0};
}

Transform4D rotateAboutX(real angle)
{
    real s = std::sin(angle);
    real c = std::cos(angle);

    return {1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0};
}

Transform4D rotateAboutY(real angle)
{
    real s = std::sin(angle);
    real c = std::cos(angle);

    return {c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0};
}

Transform4D rotateAboutZ(real angle)
{
    real s = std::sin(angle);
    real c = std::cos(angle);

    return {c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0};
}
