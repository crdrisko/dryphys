// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: texture.hpp
// Author: crdrisko
// Date: 01/14/2025-13:43:34
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE3D_TEXTURE_HPP
#define DRYPHYS_INCLUDE_ENGINE3D_TEXTURE_HPP

#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace PartitionEngine
{
    template<typename CountedType>
    class ObjectCounter
    {
    private:
        inline static std::size_t count {};

    protected:
        constexpr ObjectCounter() noexcept { count++; }
        ~ObjectCounter() noexcept { count--; }

        constexpr ObjectCounter(const ObjectCounter<CountedType>&) noexcept { count++; }
        constexpr ObjectCounter(ObjectCounter<CountedType>&&) noexcept { count++; }

        constexpr ObjectCounter& operator=(const ObjectCounter<CountedType>&) noexcept
        {
            count++;
            return *this;
        }

        constexpr ObjectCounter&& operator=(ObjectCounter<CountedType>&&) noexcept
        {
            count++;
            return std::move(*this);
        }

    public:
        static constexpr std::size_t live() noexcept { return count; }
    };

    class Texture : public ObjectCounter<Texture>
    {
    public:
        struct Image
        {
            int width, height, nrChannels;
            unsigned char* data;

            explicit Image(const std::string& path, bool flip = false)
            {
                stbi_set_flip_vertically_on_load(flip);

                data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            }

            ~Image() { stbi_image_free(data); }
        };

    private:
        unsigned int index_;

    public:
        explicit Texture(unsigned int wrapBehaviorS = GL_REPEAT, unsigned int wrapBehaviorT = GL_REPEAT)
        {
            glActiveTexture(GL_TEXTURE0 + live() - 1);
            glGenTextures(1, &index_);
            glBindTexture(GL_TEXTURE_2D, index_);

            // set the texture wrapping/filtering options (on currently bound texture)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapBehaviorS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapBehaviorT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        ~Texture() { glBindTexture(GL_TEXTURE_2D, 0); }

        bool loadTexture(int width, int height, unsigned char* data, int color = GL_RED)
        {
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, color, width, height, 0, color, GL_UNSIGNED_BYTE, data);

                return true;
            }

            return false;
        }

        bool loadTextureFromImage(const Image& image)
        {
            int color = GL_RED + image.nrChannels + 1;

            bool result = loadTexture(image.width, image.height, image.data, color);
            glGenerateMipmap(GL_TEXTURE_2D);

            return result;
        }

        bool loadTextureFromFile(const std::string& path, bool flip = false)
        {
            Image image {path, flip};

            return loadTextureFromImage(image);
        }

        void setActive() { glActiveTexture(GL_TEXTURE0 + live() - 1); }

        unsigned int getID() const { return index_ - 1; }
    };
}   // namespace PartitionEngine

#endif
