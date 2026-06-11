<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Ragdoll corregido - Nueva compilación</title>
    <style>
        body { background: #1a1a1a; color: #0f0; font-family: monospace; padding: 10px; font-size: 12px; }
        #log { white-space: pre-wrap; max-height: 85vh; overflow-y: auto; }
        .ok { color: #4f4; } .err { color: #f44; } .warn { color: #ff0; } .info { color: #4af; } .titulo { color: #ffd700; }
    </style>
</head>
<body>
<div id="log"></div>
<script type="module">
import JoltInit from './libs/jolt-physics.wasm-compat.js';

const logDiv = document.getElementById('log');
function log(msg, cls = '') {
    const s = document.createElement('span');
    if (cls) s.className = cls;
    s.textContent = msg + '\n';
    logDiv.appendChild(s);
}
function titulo(msg) { log(`\n══ ${msg} ══`, 'titulo'); }
function ok(msg)   { log(`✅ ${msg}`, 'ok'); }
function err(msg)  { log(`❌ ${msg}`, 'err'); }
function info(msg) { log(`ℹ️ ${msg}`, 'info'); }

async function main() {
    log('🚀 Cargando Jolt...');
    const J = await JoltInit();
    ok('Jolt cargado\n');

    const V3 = (x,y,z) => new J.Vec3(x,y,z);
    const RV3 = (x,y,z) => new J.RVec3(x,y,z);
    const QID = () => J.Quat.prototype.sIdentity();

    // ── Mundo ─────────────────────────────────────────────────
    const objF = new J.ObjectLayerPairFilterTable(2); objF.EnableCollision(0,0);
    const bpI = new J.BroadPhaseLayerInterfaceTable(2,2);
    bpI.MapObjectToBroadPhaseLayer(0, new J.BroadPhaseLayer(0));
    bpI.MapObjectToBroadPhaseLayer(1, new J.BroadPhaseLayer(1));
    const bpF = new J.ObjectVsBroadPhaseLayerFilterTable(bpI,2,objF,2);
    const js = new J.JoltSettings();
    js.mObjectLayerPairFilter=objF; js.mBroadPhaseLayerInterface=bpI; js.mObjectVsBroadPhaseLayerFilter=bpF;
    js.mMaxBodies=16; js.mMaxBodyPairs=64; js.mMaxContactConstraints=64;
    const ji = new J.JoltInterface(js); J.destroy(js);
    const ps = ji.GetPhysicsSystem();
    const bi = ps.GetBodyInterface();
    ps.SetGravity(new J.Vec3(0,-9.81,0));

    // Suelo
    const sueloCS = new J.BodyCreationSettings(new J.BoxShape(V3(5,0.3,5)), RV3(0,-0.3,0), QID(), J.EMotionType_Static,0);
    bi.CreateAndAddBody(sueloCS, J.EActivation_DontActivate);
    J.destroy(sueloCS);

    // ── Skeleton ──────────────────────────────────────────────
    titulo('Creando Skeleton');
    const skeleton = new J.Skeleton();
    skeleton.AddJoint({
        GetParentJointIndex: () => -1,
        GetJointType: () => 0,
        GetCompliance: () => 0,
        GetInvMassScale: () => 1.0,
        GetInverseInertiaScale: () => 1.0,
        GetBodyIndex: () => 0,
    });
    skeleton.AddJoint({
        GetParentJointIndex: () => 0,
        GetJointType: () => 0,
        GetCompliance: () => 0,
        GetInvMassScale: () => 1.0,
        GetInverseInertiaScale: () => 1.0,
        GetBodyIndex: () => 1,
    });
    ok('Skeleton con 2 joints');

    // ── RagdollParts ─────────────────────────────────────────
    titulo('Creando RagdollParts');
    const parts = [];
    for (let i = 0; i < 2; i++) {
        const part = Object.create(J.RagdollPart.prototype);
        part.set_mPosition(RV3(0, 2 - i * 0.5, 0));
        part.set_mObjectLayer(0);
        part.SetShape(new J.CapsuleShape(0.15, 0.06));
        parts.push(part);
    }
    ok('2 RagdollParts creadas');

    // ── RagdollSettings ──────────────────────────────────────
    titulo('Configurando RagdollSettings');
    const ragSettings = new J.RagdollSettings();
    ragSettings.set_mSkeleton(skeleton);
    const arrParts = ragSettings.get_mParts();
    for (const part of parts) {
        arrParts.push_back(part);
    }
    info(`  Parts: ${arrParts.size()}`);

    // ── CreateRagdoll con 3 argumentos (FIRMA CORRECTA) ─────
    titulo('Creando Ragdoll (CreateRagdoll con 3 args)');
    let ragdoll;
    try {
        ragdoll = ragSettings.CreateRagdoll(0, 0, ps);
        ok('CreateRagdoll EXITOSO');
    } catch(e) {
        err(`CreateRagdoll falló: ${e.message}`);
    }

    // Probar también el constructor directo
    if (!ragdoll) {
        try {
            ragdoll = new J.Ragdoll(ps);
            ok('Ragdoll creado vía constructor new J.Ragdoll(ps)');
        } catch(e) {
            err(`new J.Ragdoll(ps) falló: ${e.message}`);
        }
    }

    if (!ragdoll) {
        err('No se pudo crear el Ragdoll');
    } else {
        const numBodies = ragdoll.GetBodyCount();
        info(`  Cuerpos: ${numBodies}`);

        // ── AddToPhysicsSystem ────────────────────────────────
        titulo('AddToPhysicsSystem');
        try {
            ragdoll.AddToPhysicsSystem(J.EActivation_Activate);
            ok('AddToPhysicsSystem EXITOSO');
        } catch(e) {
            err(`AddToPhysicsSystem: ${e.message}`);
        }

        // ── Simular ──────────────────────────────────────────
        titulo('Simulando 30 frames');
        for (let i = 0; i < 30; i++) {
            try {
                ji.Step(1/60, 1);
            } catch(e) {
                err(`Frame ${i}: ${e.message}`);
                break;
            }
            if (i % 15 === 0) {
                for (let b = 0; b < numBodies; b++) {
                    const bodyID = ragdoll.GetBodyID(b);
                    const pos = bi.GetPosition(bodyID);
                    info(`  Frame ${i}, Cuerpo ${b}: Y=${pos.GetY().toFixed(3)}`);
                }
            }
        }
        ok('Simulación completada');

        ragdoll.RemoveFromPhysicsSystem();
        J.destroy(ragdoll);
    }

    J.destroy(ragSettings);
    parts.forEach(p => J.destroy(p));
    J.destroy(skeleton);
    J.destroy(ji);
    log('\n🏁 Test completado.', 'ok');
}

main().catch(e => log(`💥 Error: ${e.message}`, 'err'));
</script>
</body>
</html>