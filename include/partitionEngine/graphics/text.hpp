// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: text.hpp
// Author: crdrisko
// Date: 04/17/2025-10:20:32
// Description:

#ifndef TEXT_HPP
#define TEXT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <drychem/drychem.hpp>
#include <glad/glad.h>

#include "partitionEngine/graphics/color.hpp"
#include "partitionEngine/graphics/shaders.hpp"
#include "partitionEngine/graphics/texture.hpp"

namespace PartitionEngine
{
    struct CharacterGlyph
    {
        unsigned int textureID;
        unsigned int advance;
        unsigned int width, height;
        int bitmap_left, bitmap_top;
    };

    class Font
    {
    private:
        std::map<char, CharacterGlyph> fontMap_;

        void init(const std::string& path)
        {
            FT_Library library;
            FT_Face face;

            if (FT_Init_FreeType(&library))
            {
                DryChem::printErrorMessage(
                    DryChem::ErrorMessage {"ERROR::FREETYPE", "Could not init FreeType Library.", __FILE__, __LINE__});
                return;
            }

            if (FT_New_Face(library, path.c_str(), 0, &face))
            {
                DryChem::printErrorMessage(
                    DryChem::ErrorMessage {"ERROR::FREETYPE", "Failed to load font.", __FILE__, __LINE__});
                return;
            }
            else
            {
                FT_Set_Pixel_Sizes(face, 0, 48);

                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                for (unsigned char ch {}; ch < 128; ch++)
                {
                    if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
                    {
                        DryChem::printErrorMessage(
                            DryChem::ErrorMessage {"ERROR::FREETYPE", "Failed to load CharacterGlyph.", __FILE__, __LINE__});
                        continue;
                    }

                    // generate texture
                    Texture texture {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};

                    texture.loadTexture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

                    CharacterGlyph glyph = {texture.getID(),
                        static_cast<unsigned int>(face->glyph->advance.x),
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        face->glyph->bitmap_left,
                        face->glyph->bitmap_top};

                    fontMap_[ch] = glyph;
                }
            }

            FT_Done_Face(face);
            FT_Done_FreeType(library);
        }

    public:
        Font() = default;
        explicit Font(const std::string& path) { init(path); }

        void addFontFile(const std::string& path) { init(path); }

        std::map<char, CharacterGlyph> getFontMap() const { return fontMap_; }
        CharacterGlyph getCharacterGlyph(char ch) const { return fontMap_.at(ch); }
    };

    class Text
    {
    private:
        std::string text_ {};
        unsigned int characterSize_ {30};
        DryPhys::Vector2D pos_ {};
        DryPhys::real scale_ {1};
        const Font* font_;

        ColorRGBA fillColor_ {Colors::White};

        unsigned int VAO, VBO;

        void init()
        {
            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

    public:
        Text() { init(); };

        void setString(const std::string& str) { text_ = str; }
        void setSize(unsigned int size) { characterSize_ = size; };
        void setPos(const DryPhys::Vector2D& pos) { pos_ = pos; };
        void setScale(float scale) { scale_ = scale; };
        void setFillColor(const ColorRGBA& fillColor) { fillColor_ = fillColor; }
        void setFont(const Font& font) { font_ = &font; }

        void draw(ShaderProgram* shaderProgram = nullptr)
        {
            if (shaderProgram)
            {
                ColorGL color = fillColor_.convertToColorGL();

                shaderProgram->use();
                shaderProgram->setUniform("textColor", color.r, color.g, color.b);
            }

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);

            for (char ch : text_)
            {
                CharacterGlyph glyph = font_->getCharacterGlyph(ch);

                float xpos = pos_.x + glyph.bitmap_left * scale_;
                float ypos = pos_.y - (glyph.height - glyph.bitmap_top) * scale_;

                float w = glyph.width * scale_;
                float h = glyph.height * scale_;

                // update VBO for each character
                float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},
                    {xpos, ypos, 0.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},
                    {xpos, ypos + h, 0.0f, 0.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},
                    {xpos + w, ypos + h, 1.0f, 0.0f}};

                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, glyph.textureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(
                    GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);   // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                pos_.x += (glyph.advance >> 6) * scale_;
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    };
}   // namespace PartitionEngine

#endif
