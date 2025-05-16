// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: GettingStarted01_01.cpp
// Author: crdrisko
// Date: 04/21/2025-07:00:25
// Description:

#include <partitionEngine/partitionEngine.hpp>

using namespace PartitionEngine;

int main()
{
    Window window {800, 600, "Getting Started: Hello Window"};

    while (window.isOpen())
    {
        window.pollEvents();

        window.clear();

        window.display();
    }
}
