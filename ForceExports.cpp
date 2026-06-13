// ForceExports.cpp — obliga al linker a conservar TODAS las funciones eliminadas
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/Physics/SoftBody/SoftBodyShape.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>

void ForceExports_Dummy() {
    // CharacterVirtual
    {
        // GetCenterOfMassTransform (sobrecarga sin argumentos)
        using FnType = JPH::RMat44 (JPH::CharacterVirtual::*)() const;
        FnType f = &JPH::CharacterVirtual::GetCenterOfMassTransform;
        (void)f;
    }
    // Otras funciones sin sobrecarga o con firma única
    (void)&JPH::CharacterVirtual::SetPosition;
    (void)&JPH::CharacterVirtual::SetRotation;
    (void)&JPH::CharacterVirtual::GetCenterOfMassPosition;
    (void)&JPH::CharacterVirtual::GetTransformedShape;
    (void)&JPH::CharacterVirtual::RefreshContacts;
    (void)&JPH::CharacterVirtual::SaveState;
    (void)&JPH::CharacterVirtual::RestoreState;

    // NarrowPhaseQuery
    {
        // CastRay (sobrecarga con 4 argumentos: RRayCast, RayCastResult, BroadPhaseLayerFilter, ObjectLayerFilter, BodyFilter)
        using FnType = bool (JPH::NarrowPhaseQuery::*)(const JPH::RRayCast &, JPH::RayCastResult &, const JPH::BroadPhaseLayerFilter &, const JPH::ObjectLayerFilter &, const JPH::BodyFilter &) const;
        FnType f = &JPH::NarrowPhaseQuery::CastRay;
        (void)f;
    }
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

    // SoftBodyShape::GetWorldSpaceBounds (sobrecarga con Mat44 y Vec3)
    {
        using FnType = JPH::AABox (JPH::SoftBodyShape::*)(JPH::Mat44Arg, JPH::Vec3Arg) const;
        FnType f = &JPH::SoftBodyShape::GetWorldSpaceBounds;
        (void)f;
    }

    // VehicleConstraint: forzar inclusión de la clase usando un método existente (GetWheelCount)
    (void)&JPH::VehicleConstraint::GetWheelCount;
}