// keep-alive.js — Fuerza la inclusión de funciones eliminadas por el linker
(function() {
  // CharacterVirtual
  if (Jolt.CharacterVirtual) {
    Jolt.CharacterVirtual.prototype.Update;
    Jolt.CharacterVirtual.prototype.ExtendedUpdate;
    Jolt.CharacterVirtual.prototype.RefreshContacts;
  }

  // PathConstraintPathHermite
  if (Jolt.PathConstraintPathHermite) {
    Jolt.PathConstraintPathHermite.prototype;
  }

  // Otras funciones que necesites conservar en el futuro pueden añadirse aquí
})();