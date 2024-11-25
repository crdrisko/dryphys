// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: testActionFunctions.hpp
// Author: crdrisko
// Date: 08/04/2024-07:34:54
// Description: Provides ~100% unit test coverage over all Engine2D::Action functions

#ifndef TESTACTIONFUNCTIONS_HPP
#define TESTACTIONFUNCTIONS_HPP

#include <engine2d/action.hpp>

GTEST_TEST(testActionFunctions, differentConstructorsInitializeObjectsAsExpected)
{
    DryPhys::Vector3D mpos {32.0f, 15.0f, 0.0f};

    Engine2D::Action defaultInitialized {};
    Engine2D::Action withoutMousePos {"UP", Engine2D::Action::END};
    Engine2D::Action withMousePos {"LEFT_CLICK", Engine2D::Action::START, mpos};

    ASSERT_TRUE(defaultInitialized.name().empty());
    ASSERT_FALSE(withoutMousePos.name().empty());
    ASSERT_FALSE(withMousePos.name().empty());
}

#endif
