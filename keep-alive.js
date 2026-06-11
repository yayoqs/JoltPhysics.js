// keep-alive.js — Fuerza la inclusión de funciones eliminadas por el linker
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
    rp.AddToPhysicsSystem;    // tanto la versión con 1 arg como con 2
    rp.IsActive;
    rp.SetGroupID;
    rp.GetRootTransform;
    rp.GetWorldSpaceBounds;
  }

  // ── PathConstraintSettings ────────────────────────
  if (Jolt.PathConstraintSettings) {
    Jolt.PathConstraintSettings.prototype.Create;
  }

  // ── PathConstraintPathHermite ─────────────────────
  if (Jolt.PathConstraintPathHermite) {
    Jolt.PathConstraintPathHermite.prototype;
  }

  // ── Otras clases que queramos conservar en el futuro ──
})();