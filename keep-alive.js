// keep-alive.js — Fuerza la inclusión de TODAS las funciones eliminadas por el linker
(function() {
  // ── CharacterVirtual ──────────────────────────────
  if (Jolt.CharacterVirtual) {
    var cvp = Jolt.CharacterVirtual.prototype;
    cvp.Update;
    cvp.ExtendedUpdate;
    cvp.RefreshContacts;
    cvp.SetPosition;
    cvp.SetRotation;
    cvp.GetCenterOfMassPosition;
    cvp.GetCenterOfMassTransform;
    cvp.GetTransformedShape;
    cvp.SaveState;
    cvp.RestoreState;
  }

  // ── NarrowPhaseQuery ──────────────────────────────
  if (Jolt.NarrowPhaseQuery) {
    var npqp = Jolt.NarrowPhaseQuery.prototype;
    npqp.CastRay;
    npqp.CollidePoint;
    npqp.CollideShape;
    npqp.CollideShapeWithInternalEdgeRemoval;
    npqp.CastShape;
    npqp.CollectTransformedShapes;
  }

  // ── Ragdoll ───────────────────────────────────────
  if (Jolt.Ragdoll) {
    var rp = Jolt.Ragdoll.prototype;
    rp.AddToPhysicsSystem;    // cubre tanto la versión con 1 arg como con 2
    rp.IsActive;
    rp.SetGroupID;
    rp.GetRootTransform;
    rp.GetWorldSpaceBounds;
  }

  // ── RagdollSettings ───────────────────────────────
  if (Jolt.RagdollSettings) {
    Jolt.RagdollSettings.prototype.CreateRagdoll;
    Jolt.RagdollSettings.prototype.Stabilize;
  }

  // ── PathConstraintSettings ────────────────────────
  if (Jolt.PathConstraintSettings) {
    Jolt.PathConstraintSettings.prototype.Create;
  }

  // ── PathConstraintPathHermite ─────────────────────
  if (Jolt.PathConstraintPathHermite) {
    Jolt.PathConstraintPathHermite.prototype;
  }

  // ── VehicleConstraint ─────────────────────────────
  if (Jolt.VehicleConstraint) {
    Jolt.VehicleConstraint.prototype;
  }

  // ── SoftBody ──────────────────────────────────────
  if (Jolt.BodyInterface) {
    Jolt.BodyInterface.prototype.CreateAndAddSoftBody;
    Jolt.BodyInterface.prototype.CreateSoftBody;
  }

  // ── SoftBodyShape (GetWorldSpaceBounds) ───────────
  if (Jolt.SoftBodyShape) {
    Jolt.SoftBodyShape.prototype.GetWorldSpaceBounds;
  }
})();