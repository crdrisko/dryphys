// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: shaders.hpp
// Author: crdrisko
// Date: 04/18/2025-05:23:16
// Description: Shader wrappers for OpenGL

#ifndef DRYPHYS_INCLUDE_PARTITIONENGINE_GRAPHICS_SHADERS_HPP
#define DRYPHYS_INCLUDE_PARTITIONENGINE_GRAPHICS_SHADERS_HPP

#include <string>
#include <type_traits>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <dryphys/dryphys.hpp>
#include <glad/glad.h>

namespace PartitionEngine
{
    /*!
     * @brief 
     */
    class Shader
    {
    private:
        std::string source_ {};     //!< The source code of the given shader
        unsigned int index_ {};     //!< The shader index of the given type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)

        void readShaderFromFile(const std::string& shaderPath);
        void compileShader();

    public:
        Shader(const std::string& path, int type);
        ~Shader() { glDeleteShader(index_); }

        unsigned int getIndex() const { return index_; }
    };

    /*!
     * @brief 
     */
    class ShaderProgram
    {
    private:
        unsigned int index_ {};

        void attach(const Shader& shader) { glAttachShader(index_, shader.getIndex()); }
        void link();

    public:
        template<typename... TArgs, typename = std::enable_if_t<(std::is_same_v<Shader, std::decay_t<TArgs>> && ...)>>
        ShaderProgram(TArgs&&... args)
        {
            index_ = glCreateProgram();

            (attach(std::forward<TArgs>(args)), ...);

            link();
        }

        ~ShaderProgram() { glDeleteProgram(index_); }

        void use() { glUseProgram(index_); }
        unsigned int getIndex() const { return index_; }

        /*!
         * Creates a shader uniform from the expanded parameter pack. Provides an easy and convient way of specifing a uniform 
         *  attached to the current ShaderProgram. All passed types must be the same and be one of float, int, or bool.
         * 
         * \code
         * float timeValue  = glfwGetTime();
         * float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
         * 
         * shaderProgram.use();
         * shaderProgram.setUniform("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
         * \endcode
         * 
         * @tparam ...TArgs - A list of types for parameter pack expansion.
         * 
         * @param    name - The name of the uniform in the shader file.
         * @param ...args - The data that makes up the uniform.
         */
        template<typename... TArgs>
        constexpr void setUniform(const std::string& name, TArgs... args) const
        {
            void (*uniform)(int location, TArgs... args);

            if constexpr ((std::is_same_v<float, TArgs> && ...))
            {
                if constexpr (sizeof...(TArgs) == 1)
                    uniform = glUniform1f;

                else if constexpr (sizeof...(TArgs) == 2)
                    uniform = glUniform2f;

                else if constexpr (sizeof...(TArgs) == 3)
                    uniform = glUniform3f;

                else if constexpr (sizeof...(TArgs) == 4)
                    uniform = glUniform4f;
            }
            else if constexpr ((std::is_same_v<int, TArgs> && ...) || (std::is_same_v<bool, TArgs> && ...))
            {
                if constexpr (sizeof...(TArgs) == 1)
                    uniform = glUniform1i;

                else if constexpr (sizeof...(TArgs) == 2)
                    uniform = glUniform2i;

                else if constexpr (sizeof...(TArgs) == 3)
                    uniform = glUniform3i;

                else if constexpr (sizeof...(TArgs) == 4)
                    uniform = glUniform4i;
            }

            uniform(glGetUniformLocation(index_, name.c_str()), args...);
        }

        /*!
         * Creates a shader uniform matrix from the different DryPhys matrix types. Provides an easy and convient way of specifing 
         *  a uniform matrix attached to the current ShaderProgram. All passed types must be one of Matrix3D, Matrix4D, 
         *  or Transform4D. 
         * 
         * \code
         * DryPhys::Matrix4D projection {0.00250, 0,    0, -1, 
         *                                  0, 0.00333, 0, -1, 
         *                                  0,    0,   -1,  0, 
         *                                  0,    0,    0,  1};
         *
         * shaderProgram.use();
         * shaderProgram.setUniformMatrix("projection", projection);
         * \endcode
         * 
         * @tparam MatrixType - The type of matrix used for the mat parameter.
         * 
         * @param name - The name of the uniform in the shader file.
         * @param  mat - The matrix to pass to the shader.
         * 
         * \note Internally the matrices are converted into their transpose for OpenGL.
         */
        template<typename MatrixType>
        constexpr void setUniformMatrix(const std::string& name, MatrixType mat) const
        {
            if constexpr (std::is_same_v<DryPhys::Matrix3D, MatrixType>)
            {
                float array[9] {};
                mat.fillGLArray(array);

                glUniformMatrix3fv(glGetUniformLocation(index_, name.c_str()), 1, GL_FALSE, array);
                return;
            }
            else if constexpr (std::is_same_v<DryPhys::Transform4D, MatrixType>)
            {
                float array[16] {};
                mat.fillGLArray(array);

                glUniformMatrix4fv(glGetUniformLocation(index_, name.c_str()), 1, GL_FALSE, array);
                return;
            }
            else if constexpr (std::is_same_v<DryPhys::Matrix4D, MatrixType>)
            {
                float array[16] {};
                mat.fillGLArray(array);

                glUniformMatrix4fv(glGetUniformLocation(index_, name.c_str()), 1, GL_FALSE, array);
                return;
            }
        }
    };
}   // namespace PartitionEngine

#endif
