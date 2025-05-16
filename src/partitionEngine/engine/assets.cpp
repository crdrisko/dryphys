// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: assets.cpp
// Author: crdrisko
// Date: 01/13/2025-06:38:42
// Description:

#include "partitionEngine/engine/assets.hpp"

#include <cassert>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <drychem/drychem.hpp>
#include <dryphys/dryphys.hpp>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "partitionEngine/graphics/graphics.hpp"
#include "partitionEngine/graphics/textureSheet.hpp"

using namespace DryChem::Literals;

namespace PartitionEngine
{
    void Assets::loadFromFile(const std::string& path)
    {
        DryChem::FileParser parser {path};
        auto rows = parser.parseDataFile(DryChem::AsRows());

        for (const auto& row : rows)
        {
            DryChem::Tokenizer tok {row};
            std::vector<std::string> splitRow = tok.split();

            switch (DryChem::djb2Hash(splitRow[0].c_str()))
            {
            case "VertexShader"_sID:
                /*
                 * VertexShader N P
                 *  Name:           N       std::string
                 *  Path:           P       std::string
                 */
                addShaderPath(splitRow[1], splitRow[2], GL_VERTEX_SHADER);
                break;
            case "FragmentShader"_sID:
                /*
                 * FragmentShader N P
                 *  Name:           N       std::string
                 *  Path:           P       std::string
                 */
                addShaderPath(splitRow[1], splitRow[2], GL_FRAGMENT_SHADER);
                break;
            case "ShaderProgram"_sID:
                /*
                 * ShaderProgram N P1 P2
                 *  Name:           N       std::string
                 *  Shader Path 1:  P1      std::string
                 *  Shader Path 2:  P2      std::string
                 */
                addShaderProgram(splitRow[1], splitRow[2], splitRow[3]);
                break;
            case "TextureSheet"_sID:
                /*
                 * TextureSheet N P C
                 *  Name:           N       std::string (no spaces)
                 *  Path:           P       std::string (no spaces)
                 *  Color:          C       std::string (no spaces)
                 */
                addTextureSheet(splitRow[1], splitRow[2]);
                break;
            case "TextureSheetFlipped"_sID:
                /*
                 * TextureSheetFlipped N P C
                 *  Name:           N       std::string (no spaces)
                 *  Path:           P       std::string (no spaces)
                 *  Color:          C       std::string (no spaces)
                 */
                addFlippedTextureSheet(splitRow[1], splitRow[2]);
                break;
            case "Font"_sID:
                /*
                 * Font N F
                 *  Name:       N       std::string (no spaces)
                 *  File Path:  P       std::string (no spaces)
                 */
                addFont(splitRow[1], splitRow[2]);
                break;
            default:
                break;
            }
        }
    }

    void Assets::addShaderPath(const std::string& name, const std::string& path, int type)
    {
        shaderPathMap_[name] = std::make_pair(path, type);
    }

    void Assets::addShaderProgram(const std::string& name, const std::string& shader1Name, const std::string& shader2Name)
    {
        shaderProgramMap_[name]
            = new ShaderProgram {Shader {shaderPathMap_[shader1Name].first, shaderPathMap_[shader1Name].second},
                Shader {shaderPathMap_[shader2Name].first, shaderPathMap_[shader2Name].second}};
    }

    void Assets::addTextureSheet(const std::string& name, const std::string& path, std::size_t perRow, std::size_t perCol)
    {
        textureSheetMap_[name] = TextureSheet {name, path, perRow, perCol};
    }

    void Assets::addFlippedTextureSheet(const std::string& name, const std::string& path)
    {
        textureSheetMap_[name] = TextureSheet {name, path, true};
    }

    void Assets::addFont(const std::string& name, const std::string& path) { fontMap_[name].addFontFile(path); }

    ShaderProgram* Assets::getShaderProgram(const std::string& name)
    {
        assert(shaderProgramMap_.find(name) != shaderProgramMap_.end());
        return shaderProgramMap_[name];
    }

    TextureSheet& Assets::getTextureSheet(const std::string& name)
    {
        assert(textureSheetMap_.find(name) != textureSheetMap_.end());
        return textureSheetMap_[name];
    }

    Font& Assets::getFont(const std::string& name)
    {
        assert(fontMap_.find(name) != fontMap_.end());
        return fontMap_[name];
    }
}   // namespace PartitionEngine
