// post-keepalive.js – Fuerza la inclusión de las últimas 5 funciones
(function() {
  if (typeof Jolt !== 'undefined') {
    // NarrowPhaseQuery
    var npq = Jolt.NarrowPhaseQuery;
    if (npq) {
      npq.prototype.CastShape;
      npq.prototype.CollidePoint;
      npq.prototype.CollectTransformedShapes;
    }

    // BodyInterface (SoftBody)
    var bi = Jolt.BodyInterface;
    if (bi) {
      bi.prototype.CreateAndAddSoftBody;
      bi.prototype.CreateSoftBody;
    }
  }
})();