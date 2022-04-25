//
// Created by shawn on 2022/4/24.
//
#pragma once

#include <Physics/Physics.h>
#include <ComponentComposition/BoxCollider.h>
#include <ComponentComposition/SphereCollider.h>
#include <ComponentComposition/CapsuleCollider.h>

namespace PlatinumEngine
{
	btStaticPlaneShape* shape;
	btCapsuleShape* capsuleShape;
}