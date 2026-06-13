#include <emscripten.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Ragdoll/Ragdoll.h>
#include <Jolt/Physics/SoftBody/SoftBodyCreationSettings.h>
#include <Jolt/Physics/Vehicle/VehicleConstraint.h>
#include <Jolt/Physics/Constraints/PathConstraint.h>

EMSCRIPTEN_KEEPALIVE
void KeepAlive_AllFunctions() {
    // ── CharacterVirtual ──────────────────────────
    volatile auto cv0  = &JPH::CharacterVirtual::Update;
    volatile auto cv1  = &JPH::CharacterVirtual::ExtendedUpdate;
    volatile auto cv2  = &JPH::CharacterVirtual::SetPosition;
    volatile auto cv3  = &JPH::CharacterVirtual::SetRotation;
    volatile auto cv4  = &JPH::CharacterVirtual::RefreshContacts;
    volatile auto cv5  = &JPH::CharacterVirtual::SetShapeOffset;
    volatile auto cv6  = &JPH::CharacterVirtual::SetUserData;
    volatile auto cv7  = &JPH::CharacterVirtual::SetInnerBodyShape;
    volatile auto cv8  = &JPH::CharacterVirtual::GetCenterOfMassPosition;
    // GetCenterOfMassTransform (sobrecarga sin argumentos)
    {
        using FnType = JPH::RMat44 (JPH::CharacterVirtual::*)() const;
        volatile FnType cv9 = &JPH::CharacterVirtual::GetCenterOfMassTransform;
        (void)cv9;
    }
    volatile auto cv10 = &JPH::CharacterVirtual::GetTransformedShape;
    volatile auto cv11 = &JPH::CharacterVirtual::SaveState;
    volatile auto cv12 = &JPH::CharacterVirtual::RestoreState;

    // ── NarrowPhaseQuery ───────────────────────────
    // CastRay (sobrecarga con 4 argumentos: RRayCast, RayCastResult, BroadPhaseLayerFilter, ObjectLayerFilter, BodyFilter)
    {
        using FnType = bool (JPH::NarrowPhaseQuery::*)(const JPH::RRayCast &, JPH::RayCastResult &, const JPH::BroadPhaseLayerFilter &, const JPH::ObjectLayerFilter &, const JPH::BodyFilter &) const;
        volatile FnType npq0 = &JPH::NarrowPhaseQuery::CastRay;
        (void)npq0;
    }
    volatile auto npq1 = &JPH::NarrowPhaseQuery::CastShape;
    volatile auto npq2 = &JPH::NarrowPhaseQuery::CollideShape;
    volatile auto npq3 = &JPH::NarrowPhaseQuery::CollidePoint;
    volatile auto npq4 = &JPH::NarrowPhaseQuery::CollideShapeWithInternalEdgeRemoval;
    volatile auto npq5 = &JPH::NarrowPhaseQuery::CollectTransformedShapes;

    // ── Ragdoll ────────────────────────────────────
    volatile auto r0 = &JPH::Ragdoll::AddToPhysicsSystem; // sobrecarga de 1 arg (EActivation)
    volatile auto r1 = &JPH::Ragdoll::RemoveFromPhysicsSystem; // sobrecarga de 0 args
    volatile auto r2 = &JPH::Ragdoll::Activate; // sobrecarga de 0 args

    // ── PathConstraintSettings ─────────────────────
    volatile auto pc = &JPH::PathConstraintSettings::Create;

    // ── BodyInterface getters ──────────────────────
    volatile auto bi0  = &JPH::BodyInterface::GetPosition;
    volatile auto bi1  = &JPH::BodyInterface::GetRotation;
    volatile auto bi2  = &JPH::BodyInterface::GetLinearVelocity;
    volatile auto bi3  = &JPH::BodyInterface::GetAngularVelocity;
    volatile auto bi4  = &JPH::BodyInterface::GetWorldTransform;
    volatile auto bi5  = &JPH::BodyInterface::GetCenterOfMassTransform;
    volatile auto bi6  = &JPH::BodyInterface::GetTransformedShape;
    volatile auto bi7  = &JPH::BodyInterface::GetShape;
    volatile auto bi8  = &JPH::BodyInterface::GetObjectLayer;
    volatile auto bi9  = &JPH::BodyInterface::GetRestitution;
    volatile auto bi10 = &JPH::BodyInterface::GetFriction;
    volatile auto bi11 = &JPH::BodyInterface::GetGravityFactor;
    volatile auto bi12 = &JPH::BodyInterface::GetUseManifoldReduction;
    volatile auto bi13 = &JPH::BodyInterface::IsSensor;
    volatile auto bi14 = &JPH::BodyInterface::IsActive;
    volatile auto bi15 = &JPH::BodyInterface::GetMotionType;
    volatile auto bi16 = &JPH::BodyInterface::GetMotionQuality;
    volatile auto bi17 = &JPH::BodyInterface::GetUserData;
    volatile auto bi18 = &JPH::BodyInterface::GetMaterial;

    // ── SoftBody ───────────────────────────────────
    volatile auto sb0 = &JPH::BodyInterface::CreateAndAddSoftBody;
    volatile auto sb1 = &JPH::BodyInterface::CreateSoftBody;

    // ── Evitar warnings ────────────────────────────
    (void)cv0; (void)cv1; (void)cv2; (void)cv3; (void)cv4; (void)cv5; (void)cv6; (void)cv7;
    (void)cv8; (void)cv10; (void)cv11; (void)cv12;
    (void)npq1; (void)npq2; (void)npq3; (void)npq4; (void)npq5;
    (void)r0; (void)r1; (void)r2;
    (void)pc;
    (void)bi0; (void)bi1; (void)bi2; (void)bi3; (void)bi4; (void)bi5; (void)bi6; (void)bi7;
    (void)bi8; (void)bi9; (void)bi10; (void)bi11; (void)bi12; (void)bi13; (void)bi14;
    (void)bi15; (void)bi16; (void)bi17; (void)bi18;
    (void)sb0; (void)sb1;
}