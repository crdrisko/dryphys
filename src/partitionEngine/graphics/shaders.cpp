// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: shaders.cpp
// Author: crdrisko
// Date: 04/18/2025-05:30:47
// Description: Implementation details for the shader wrappers for OpenGL

#include "partitionEngine/graphics/shaders.hpp"

#include <fstream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <common-utils/errors.hpp>
#include <glad/glad.h>

namespace PartitionEngine
{
    Shader::Shader(const std::string& path, int type)
    {
        readShaderFromFile(path);

        index_ = glCreateShader(type);

        compileShader();
    }

    void Shader::readShaderFromFile(const std::string& shaderPath)
    {
        std::ifstream inputFile {shaderPath};
        inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            source_.assign(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());
        }
        catch (const std::ifstream::failure& e)
        {
            DryChem::printErrorMessage(DryChem::ErrorMessage {"Shader file parsing failure", e.what(), __FILE__, __LINE__});
        }
    }

    void Shader::compileShader()
    {
        const char* c_str = source_.c_str();

        glShaderSource(index_, 1, &c_str, nullptr);
        glCompileShader(index_);

        int success;
        char infoLog[512];
        glGetShaderiv(index_, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(index_, 512, NULL, infoLog);
            DryChem::printErrorMessage(DryChem::ErrorMessage {"Shader compilation failure", infoLog, __FILE__, __LINE__});
        }
    }

    void ShaderProgram::link()
    {
        glLinkProgram(index_);

        int success;
        char infoLog[512];
        glGetProgramiv(index_, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(index_, 512, NULL, infoLog);
            DryChem::printErrorMessage(DryChem::ErrorMessage {"ShaderProgram linkage failure", infoLog, __FILE__, __LINE__});
        }
    }
}   // namespace PartitionEngine
