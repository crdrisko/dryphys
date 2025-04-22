// Copyright (c) 2024 C.R. Drisko. All rights reserved.
// Licensed under the MIT License. See the LICENSE file in the project root for more information.
//
// Name: dryphys.hpp
// Author: crdrisko
// Date: 12/15/2024-07:02:28
// Description: The public API for the DryPhys library. This header file should be included in all
//   user programs using these utilities.
//
// Note: The internal implementation details associated with this API should not be used in any
//   user program as they are subject to change at any time without warning.

#ifndef DRYPHYS_INCLUDE_DRYPHYS_DRYPHYS_HPP
#define DRYPHYS_INCLUDE_DRYPHYS_DRYPHYS_HPP

#include "dryphys/math/matrix.hpp"
#include "dryphys/math/quaternion.hpp"
#include "dryphys/math/vector.hpp"
#include "dryphys/particleSystems/collisionGenerators/groundCollisions.hpp"
#include "dryphys/particleSystems/collisionGenerators/links.hpp"
#include "dryphys/particleSystems/collisions.hpp"
#include "dryphys/particleSystems/forceGenerator.hpp"
#include "dryphys/particleSystems/forceGenerators/buoyancy.hpp"
#include "dryphys/particleSystems/forceGenerators/drag.hpp"
#include "dryphys/particleSystems/forceGenerators/gravity.hpp"
#include "dryphys/particleSystems/forceGenerators/springs.hpp"
#include "dryphys/particleSystems/world.hpp"
#include "dryphys/rigidBodySystems/collisions/broadCollision.hpp"
#include "dryphys/rigidBodySystems/forceGenerator.hpp"
#include "dryphys/rigidBodySystems/forceGenerators/aero.hpp"
#include "dryphys/rigidBodySystems/forceGenerators/buoyancy.hpp"
#include "dryphys/rigidBodySystems/forceGenerators/gravity.hpp"
#include "dryphys/rigidBodySystems/forceGenerators/springs.hpp"
#include "dryphys/rigidBodySystems/world.hpp"
#include "dryphys/types/particle.hpp"
#include "dryphys/types/rigidBody.hpp"
#include "dryphys/utilities/config.hpp"
#include "dryphys/utilities/ctStringHashing.hpp"

#endif
