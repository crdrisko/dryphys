// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: assets.hpp
// Author: crdrisko
// Date: 01/13/2025-06:38:16
// Description:

#ifndef DRYPHYS_INCLUDE_ENGINE3D_ASSETS_HPP
#define DRYPHYS_INCLUDE_ENGINE3D_ASSETS_HPP

#include <map>
#include <string>

#include "partitionEngine/graphics/graphics.hpp"
#include "partitionEngine/graphics/textureSheet.hpp"
#include "partitionEngine/graphics/text.hpp"

namespace PartitionEngine
{
    class Assets
    {
    private:
        std::map<std::string, std::pair<std::string, int>> shaderPathMap_;
        std::map<std::string, ShaderProgram*> shaderProgramMap_;
        std::map<std::string, TextureSheet> textureSheetMap_;
        std::map<std::string, Font> fontMap_;

        void addShaderPath(const std::string& name, const std::string& path, int type);
        void addShaderProgram(const std::string& name, const std::string& shader1Name, const std::string& shader2Name);
        void addTextureSheet(
            const std::string& name, const std::string& path, std::size_t perRow = 0, std::size_t perCol = 0);
        void addFlippedTextureSheet(const std::string& name, const std::string& path);
        void addFont(const std::string& name, const std::string& path);

    public:
        Assets() = default;
        ~Assets()
        {
            for (auto& [name, shaderProgram] : shaderProgramMap_)
            {
                if (shaderProgram)
                    delete shaderProgram;
            }
        };

        void loadFromFile(const std::string& path);

        ShaderProgram* getShaderProgram(const std::string& name);
        TextureSheet& getTextureSheet(const std::string& name);
        Font& getFont(const std::string& name);
    };
}   // namespace PartitionEngine

#endif
