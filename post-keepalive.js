// Forzar que el optimizador JS conserve estos wrappers
(function() {
  if (typeof Jolt !== 'undefined') {
    // NarrowPhaseQuery
    Jolt.NarrowPhaseQuery && (Jolt.NarrowPhaseQuery.prototype.CastShape,
                               Jolt.NarrowPhaseQuery.prototype.CollidePoint,
                               Jolt.NarrowPhaseQuery.prototype.CollectTransformedShapes);
    // BodyInterface SoftBody
    Jolt.BodyInterface && (Jolt.BodyInterface.prototype.CreateAndAddSoftBody,
                            Jolt.BodyInterface.prototype.CreateSoftBody);
  }
})();