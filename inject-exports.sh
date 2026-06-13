#!/bin/bash
# inject-exports.sh - Añade exportaciones faltantes al glue.js generado
# Se ejecuta después de generar glue.js y antes de compilar el wasm

GLUE_FILE="glue.js"

# Lista de funciones que queremos conservar (nombres C++ sin mangling)
FUNCS=(
  "_ZN3JPH17CharacterVirtual6UpdateEfRKNS_6RVec3ERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterERKNS_11ShapeFilterERNS_13TempAllocatorE"
  "_ZN3JPH17CharacterVirtual14ExtendedUpdateEfRKNS_6RVec3ERKNS_22ExtendedUpdateSettingsERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterERKNS_11ShapeFilterERNS_13TempAllocatorE"
  "_ZN3JPH17CharacterVirtual11SetPositionERKNS_6RVec3E"
  "_ZN3JPH17CharacterVirtual11SetRotationERKNS_4QuatE"
  "_ZN3JPH17CharacterVirtual14RefreshContactsERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterERKNS_11ShapeFilterERNS_13TempAllocatorE"
  "_ZN3JPH17CharacterVirtual15SetShapeOffsetERKNS_4Vec3E"
  "_ZN3JPH17CharacterVirtual12SetUserDataEm"
  "_ZN3JPH17CharacterVirtual16SetInnerBodyShapeEPKN3JPH5ShapeE"
  "_ZN3JPH17CharacterVirtual24GetCenterOfMassPositionEv"
  "_ZN3JPH17CharacterVirtual25GetCenterOfMassTransformEv"
  "_ZN3JPH17CharacterVirtual20GetTransformedShapeEv"
  "_ZN3JPH17CharacterVirtual9SaveStateERNS_13StateRecorderE"
  "_ZN3JPH17CharacterVirtual12RestoreStateERNS_13StateRecorderE"
  "_ZN3JPH17NarrowPhaseQuery7CastRayERKNS_8RRayCastERNS_13RayCastResultERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH17NarrowPhaseQuery9CastShapeERKNS_10RShapeCastERNS_14ShapeCastResultERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH17NarrowPhaseQuery12CollideShapeEPKNS_5ShapeENS_10RVec3ArgENS_4QuatERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH17NarrowPhaseQuery12CollidePointERKNS_6RVec3ERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH17NarrowPhaseQuery36CollideShapeWithInternalEdgeRemovalEPKNS_5ShapeENS_10RVec3ArgENS_4QuatERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH17NarrowPhaseQuery23CollectTransformedShapesERKNS_5AABoxERKNS_15BroadPhaseLayerERKNS_11ObjectLayerERKNS_10BodyFilterE"
  "_ZN3JPH7Ragdoll19AddToPhysicsSystemENS_10EActivationE"
  "_ZN3JPH7Ragdoll19AddToPhysicsSystemENS_10EActivationEb"
  "_ZN3JPH7Ragdoll22RemoveFromPhysicsSystemEv"
  "_ZN3JPH7Ragdoll22RemoveFromPhysicsSystemEb"
  "_ZN3JPH7Ragdoll8ActivateEb"
  "_ZN3JPH7Ragdoll8ActivateEv"
  "_ZN3JPH23PathConstraintSettings6CreateERNS_4BodyES3_RKNS_6RVec3ES6_"
  "_ZN3JPH13BodyInterface10GetPositionERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface11GetRotationERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface17GetLinearVelocityERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface18GetAngularVelocityERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface18GetWorldTransformERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface25GetCenterOfMassTransformERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface20GetTransformedShapeERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface8GetShapeERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface14GetObjectLayerERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface15GetRestitutionERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface11GetFrictionERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface17GetGravityFactorERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface24GetUseManifoldReductionERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface8IsSensorERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface8IsActiveERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface12GetMotionTypeERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface16GetMotionQualityERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface11GetUserDataERKNS_6BodyIDE"
  "_ZN3JPH13BodyInterface10GetMaterialERKNS_6BodyIDEPKNS_5ShapeE"
  "_ZN3JPH13BodyInterface20CreateAndAddSoftBodyERKNS_25SoftBodyCreationSettingsENS_10EActivationE"
  "_ZN3JPH13BodyInterface13CreateSoftBodyERKNS_25SoftBodyCreationSettingsE"
  "_ZN3JPH13BodyInterface15CreateSoftBodyERKNS_25SoftBodyCreationSettingsERKNS_6BodyIDE"
)

# Generar una línea que las exporte (añadir al final de glue.js)
cat <<EOF >> "$GLUE_FILE"

// Inyección automática de exportaciones requeridas
Module['wasmExports'] = Module['wasmExports'] || {};
var _exp = Module['wasmExports'];
EOF

for func in "${FUNCS[@]}"; do
    cat <<EOF >> "$GLUE_FILE"
if (typeof Module['_${func}'] !== 'undefined') {
    _exp['${func}'] = Module['_${func}'];
}
EOF
done

echo "Inyectadas ${#FUNCS[@]} referencias en $GLUE_FILE"