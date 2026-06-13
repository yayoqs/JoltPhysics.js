// ForceExports.cpp — obliga al linker a conservar TODAS las funciones eliminadas
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/Physics/SoftBody/SoftBodyShape.h>

void ForceExports_Dummy() {
    // CharacterVirtual
    (void)&JPH::CharacterVirtual::SetPosition;
    (void)&JPH::CharacterVirtual::SetRotation;
    (void)&JPH::CharacterVirtual::GetCenterOfMassPosition;
    (void)&JPH::CharacterVirtual::GetCenterOfMassTransform;
    (void)&JPH::CharacterVirtual::GetTransformedShape;
    (void)&JPH::CharacterVirtual::RefreshContacts;
    (void)&JPH::CharacterVirtual::SaveState;
    (void)&JPH::CharacterVirtual::RestoreState;

    // NarrowPhaseQuery
    (void)&JPH::NarrowPhaseQuery::CastRay;
    (void)&JPH::NarrowPhaseQuery::CollidePoint;
    (void)&JPH::NarrowPhaseQuery::CollideShape;
    (void)&JPH::NarrowPhaseQuery::CastShape;
    (void)&JPH::NarrowPhaseQuery::CollectTransformedShapes;

    // Ragdoll
    (void)&JPH::Ragdoll::AddToPhysicsSystem;
    (void)&JPH::Ragdoll::IsActive;
    (void)&JPH::Ragdoll::SetGroupID;
    (void)&JPH::Ragdoll::GetRootTransform;
    (void)&JPH::Ragdoll::GetWorldSpaceBounds;

    // SoftBodyShape::GetWorldSpaceBounds
    (void)&JPH::SoftBodyShape::GetWorldSpaceBounds;
}