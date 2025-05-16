// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: graphics.hpp
// Author: crdrisko
// Date: 01/07/2025-06:45:37
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE3D_GRAPHICS_HPP
#define DRYPHYS_INCLUDE_ENGINE3D_GRAPHICS_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <dryphys/dryphys.hpp>
#include <glad/glad.h>

#include "partitionEngine/graphics/shaders.hpp"

namespace PartitionEngine
{
    struct Vertex
    {
        DryPhys::Vector3D position_;
        DryPhys::Vector3D normal_;
        DryPhys::Vector2D texCoords_;
        DryPhys::Vector3D color_;
        // DryPhys::Vector3D tangent_;
        // DryPhys::Vector3D biTangent_;
    };

    class MeshArray
    {
    protected:
        std::vector<Vertex> vertices_;
        unsigned int VAO, VBO;

    private:
        void init()
        {
            // Create the vertex arrays and buffers
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            // Bind and load data into the buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

            // Load data into the vertex array according to their location in the Vertex struct
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords_));

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color_));

            // glEnableVertexAttribArray(3);
            // glVertexAttribPointer(3, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent_));

            // glEnableVertexAttribArray(4);
            // glVertexAttribPointer(4, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent_));

            // Unbind the current vertex array
            glBindVertexArray(0);
        }

    public:
        MeshArray()          = default;
        virtual ~MeshArray() = default;

        explicit MeshArray(const std::vector<Vertex>& vertices, bool doInit = true) : vertices_ {vertices}
        {
            if (doInit)
                init();
        }

        virtual void draw(ShaderProgram* shaderProgram = nullptr)
        {
            if (shaderProgram)
                shaderProgram->use();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices_.size()));
            glBindVertexArray(0);
        }
    };

    class Mesh : public MeshArray
    {
    private:
        std::vector<unsigned int> indices_;
        unsigned int EBO;

        void init()
        {
            // Create the vertex arrays and buffers
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            // Bind and load data into the buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

            // Load data into the vertex array according to their location in the Vertex struct
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords_));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(3, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color_));

            // glEnableVertexAttribArray(3);
            // glVertexAttribPointer(3, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent_));

            // glEnableVertexAttribArray(4);
            // glVertexAttribPointer(4, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent_));

            // Unbind the current vertex array
            glBindVertexArray(0);
        }

    public:
        Mesh() = default;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : MeshArray {vertices, false}, indices_ {indices}
        {
            init();
        }

        void draw(ShaderProgram* shaderProgram = nullptr) override
        {
            if (shaderProgram)
                shaderProgram->use();

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    };
}   // namespace PartitionEngine

#endif
