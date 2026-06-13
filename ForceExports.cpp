#include <emscripten.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/Physics/SoftBody/SoftBodyShape.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>

EMSCRIPTEN_KEEPALIVE void ForceExports_Dummy() {
    volatile const void* p = nullptr;

    // CharacterVirtual
    {
        // GetCenterOfMassTransform (sobrecarga sin argumentos)
        using FnType = JPH::RMat44 (JPH::CharacterVirtual::*)() const;
        volatile FnType f = &JPH::CharacterVirtual::GetCenterOfMassTransform;
        p = (void*)(f); // evitar warning
    }
    volatile auto cv0 = &JPH::CharacterVirtual::SetPosition;
    volatile auto cv1 = &JPH::CharacterVirtual::SetRotation;
    volatile auto cv2 = &JPH::CharacterVirtual::GetCenterOfMassPosition;
    volatile auto cv3 = &JPH::CharacterVirtual::GetTransformedShape;
    volatile auto cv4 = &JPH::CharacterVirtual::RefreshContacts;
    volatile auto cv5 = &JPH::CharacterVirtual::SaveState;
    volatile auto cv6 = &JPH::CharacterVirtual::RestoreState;

    // NarrowPhaseQuery
    {
        using FnType = bool (JPH::NarrowPhaseQuery::*)(const JPH::RRayCast &, JPH::RayCastResult &, const JPH::BroadPhaseLayerFilter &, const JPH::ObjectLayerFilter &, const JPH::BodyFilter &) const;
        volatile FnType f = &JPH::NarrowPhaseQuery::CastRay;
        p = (void*)(f);
    }
    volatile auto npq0 = &JPH::NarrowPhaseQuery::CollidePoint;
    volatile auto npq1 = &JPH::NarrowPhaseQuery::CollideShape;
    volatile auto npq2 = &JPH::NarrowPhaseQuery::CastShape;
    volatile auto npq3 = &JPH::NarrowPhaseQuery::CollectTransformedShapes;

    // Ragdoll
    volatile auto r0 = &JPH::Ragdoll::AddToPhysicsSystem;
    volatile auto r1 = &JPH::Ragdoll::IsActive;
    volatile auto r2 = &JPH::Ragdoll::SetGroupID;
    volatile auto r3 = &JPH::Ragdoll::GetRootTransform;
    volatile auto r4 = &JPH::Ragdoll::GetWorldSpaceBounds;

    // SoftBodyShape
    {
        using FnType = JPH::AABox (JPH::SoftBodyShape::*)(JPH::Mat44Arg, JPH::Vec3Arg) const;
        volatile FnType f = &JPH::SoftBodyShape::GetWorldSpaceBounds;
        p = (void*)(f);
    }

    // VehicleConstraint
    volatile auto vc = &JPH::VehicleConstraint::ResetWarmStart;

    // Uso de p para evitar warnings
    (void)p;
    (void)cv0; (void)cv1; (void)cv2; (void)cv3; (void)cv4; (void)cv5; (void)cv6;
    (void)npq0; (void)npq1; (void)npq2; (void)npq3;
    (void)r0; (void)r1; (void)r2; (void)r3; (void)r4;
    (void)vc;
}