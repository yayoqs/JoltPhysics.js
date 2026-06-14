cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

FIND_PACKAGE(Python3)
set(PYTHON ${Python3_EXECUTABLE} CACHE STRING "Python path")
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten CACHE STRING "Emscripten path")
set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(JOLT_FRONT_MATTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/front-matter.js)
set(JOLT_HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/JoltJS.h)
set(JOLT_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/JoltJS.idl)
set(OUTPUT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/dist/)
set(JOLT_TYPING ${OUTPUT_FOLDER}/types.d.ts)

# Option to turn on memory profiling
option(ENABLE_MEMORY_PROFILER "Enable emscriptens memory profiler to help find leaks" OFF)

# Option to turn on double precision
option(DOUBLE_PRECISION "Compile the library in double precision mode" OFF)

# Option to turn multi threading on
option(ENABLE_MULTI_THREADING "Compile the library in multi threading mode (warning: doesn't work in Safari nor with any JS callbacks currently)'" OFF)

# Option to turn on SIMD
option(ENABLE_SIMD "Enable SIMD instructions (warning: last major browser to support this was Safari 16.4 in March 2023)" OFF)

# Option to speed up compilation by only compiling the WASM compat version
option(BUILD_WASM_COMPAT_ONLY "Compile online the WASM compat version of the library (speeds up compilation)" OFF)

# Enable for resizable heap, with some amount of slowness
option(ALLOW_MEMORY_GROWTH "Allow Memory Growth" OFF)

# Default initial memory size is 128*1024*1024 = 128MB
set(INITIAL_MEMORY 134217728 CACHE STRING "Initial Memory")

# Default maximum memory size is 2*1024*1024*1024 = 2GB (only used when ALLOW_MEMORY_GROWTH is true)
set(MAXIMUM_MEMORY 2147483648 CACHE STRING "Maximum Memory")

# The configurations we support
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")

# No floating point exception checking
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)

# Turn off LTO
set(INTERPROCEDURAL_OPTIMIZATION OFF)

# Override debug symbols
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(GENERATE_DEBUG_SYMBOLS ON)
	set(JPH_DEBUG_SYMBOL_FORMAT "source-map" CACHE STRING "")
else()
	set(GENERATE_DEBUG_SYMBOLS OFF)
endif()

# Turn off custom allocator
set(DISABLE_CUSTOM_ALLOCATOR ON)

# Turn off ObjectStream
set(ENABLE_OBJECT_STREAM OFF)

# Use 32-bit object layers to support more bits in ObjectLayerPairFilterMask
set(OBJECT_LAYER_BITS 32)

# Enable multi threading
if (ENABLE_MULTI_THREADING)
	set(MULTI_THREADED_FLAG
		-pthread
		--post-js ${CMAKE_CURRENT_SOURCE_DIR}/multi-threaded.js
		-s SHARED_MEMORY)
	set(ENVIRONMENT_FLAG
		-s ENVIRONMENT=web,node,worker
		-s PTHREAD_POOL_SIZE=16)
	set(CMAKE_CXX_FLAGS "-pthread -s SHARED_MEMORY")
	set(OUTPUT_BASE_NAME "${OUTPUT_FOLDER}jolt-physics.multithread")
	set(REMOVE_THREAD_LOCAL "")
else()
	set(MULTI_THREADED_FLAG "")
	set(ENVIRONMENT_FLAG -s ENVIRONMENT=web,node,worker)
	set(OUTPUT_BASE_NAME "${OUTPUT_FOLDER}jolt-physics")
	set(REMOVE_THREAD_LOCAL sed -i.bak -e "s:thread_local::g" glue.cpp)
endif()

# Replace 'replace_by_import' by 'import' in the generated JS code to workaround a bug in emscripten
set(REPLACE_IMPORT sed -i -e "s:replace_by_import:import:g")

# Enable SIMD flags
if (ENABLE_SIMD)
	set(ENABLE_SIMD_FLAG
		-msimd128
		-msse4.2)
	set(USE_WASM_SIMD ON)
else()
	set(ENABLE_SIMD_FLAG "")
	set(USE_WASM_SIMD OFF)
endif()

# Include Jolt
include(FetchContent)
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.5.0"
		SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

# Enable resizable heap, with some amount of slowness. Default is 128*1024*1024 = 128MB
if (ALLOW_MEMORY_GROWTH)
	set(MEMORY_GROWTH_FLAG
		-s ALLOW_MEMORY_GROWTH=1
		-s MAXIMUM_MEMORY=${MAXIMUM_MEMORY})
else()
	set(MEMORY_GROWTH_FLAG "")
endif()

# Enable memory profiler
if (ENABLE_MEMORY_PROFILER)
	set(MEMORY_PROFILER_FLAG "--memoryprofiler")
	set(INCLUDE_DEMANGLE_ALL_FLAG "-s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE='$demangleAll'")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MEMORY_PROFILER_FLAG}")
else()
	set(MEMORY_PROFILER_FLAG "")
	set(INCLUDE_DEMANGLE_ALL_FLAG "")
endif()

set(EMCC_ARGS
  -fno-rtti
  -fno-exceptions
  --extern-pre-js ${JOLT_FRONT_MATTER_FILE}
  --post-js glue.js
  --closure=0
  --closure-args="--dynamic_import_alias=replace_by_import"
  --closure-args="--externs"
  --closure-args="${CMAKE_CURRENT_SOURCE_DIR}/extern-import.js"
  ${MULTI_THREADED_FLAG}
  --post-js ${CMAKE_CURRENT_SOURCE_DIR}/helpers.js
  ${MEMORY_GROWTH_FLAG}
  -s ALLOW_TABLE_GROWTH=1
  -s WASM_BIGINT=0
  ${ENVIRONMENT_FLAG}
  -s EXPORT_ES6=1
  -s EXPORT_NAME="Jolt"
  -s MODULARIZE=1
  -s NO_EXIT_RUNTIME=1
  -s NO_FILESYSTEM=1
  -s STACK_SIZE=1048576
  -s INITIAL_MEMORY=${INITIAL_MEMORY}
  -s EXPORTED_RUNTIME_METHODS=HEAP8,HEAPU8,HEAP16,HEAPU16,HEAP32,HEAPU32,HEAPF32,HEAPF64,UTF8ToString
  ${MEMORY_PROFILER_FLAG}
  ${INCLUDE_DEMANGLE_ALL_FLAG}
  ${ENABLE_SIMD_FLAG})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(EMCC_ARGS
		${EMCC_ARGS}
		-g${JPH_DEBUG_SYMBOL_FORMAT}
		-s ASSERTIONS)
else()
	set(EMCC_ARGS
		${EMCC_ARGS}
		-O3)
endif()

set(EMCC_JS_ARGS ${EMCC_ARGS}
  -s AGGRESSIVE_VARIABLE_ELIMINATION=1
  -s ELIMINATE_DUPLICATE_FUNCTIONS=1
  -s SINGLE_FILE=1
  -s LEGACY_VM_SUPPORT=1
  -s WASM=0)

set(EMCC_WASM_ARGS ${EMCC_ARGS}
  -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1
  -s WASM=1)

# Lista explícita de las 42 funciones rescatadas (nombres de los wrappers en glue.cpp)
set(EXPORTED_FUNCTIONS_LIST
  # CharacterVirtual (13 funciones)
  "_emscripten_bind_CharacterVirtual_Update_7"
  "_emscripten_bind_CharacterVirtual_ExtendedUpdate_8"
  "_emscripten_bind_CharacterVirtual_SetPosition_1"
  "_emscripten_bind_CharacterVirtual_SetRotation_1"
  "_emscripten_bind_CharacterVirtual_RefreshContacts_5"
  "_emscripten_bind_CharacterVirtual_SetShapeOffset_1"
  "_emscripten_bind_CharacterVirtual_SetUserData_1"
  "_emscripten_bind_CharacterVirtual_SetInnerBodyShape_1"
  "_emscripten_bind_CharacterVirtual_GetCenterOfMassPosition_0"
  "_emscripten_bind_CharacterVirtual_GetCenterOfMassTransform_0"
  "_emscripten_bind_CharacterVirtual_GetTransformedShape_0"
  "_emscripten_bind_CharacterVirtual_SaveState_1"
  "_emscripten_bind_CharacterVirtual_RestoreState_1"
  # NarrowPhaseQuery (6 funciones)
  "_emscripten_bind_NarrowPhaseQuery_CastRay_7"
  "_emscripten_bind_NarrowPhaseQuery_CollideShape_10"
  "_emscripten_bind_NarrowPhaseQuery_CollideShapeWithInternalEdgeRemoval_10"
  "_emscripten_bind_NarrowPhaseQuery_CastShape_8"
  "_emscripten_bind_NarrowPhaseQuery_CollidePoint_6"
  "_emscripten_bind_NarrowPhaseQuery_CollectTransformedShapes_6"
  # Ragdoll (5 funciones)
  "_emscripten_bind_Ragdoll_AddToPhysicsSystem_1"
  "_emscripten_bind_Ragdoll_AddToPhysicsSystem_2"
  "_emscripten_bind_Ragdoll_RemoveFromPhysicsSystem_0"
  "_emscripten_bind_Ragdoll_RemoveFromPhysicsSystem_1"
  "_emscripten_bind_Ragdoll_Activate_0"
  "_emscripten_bind_Ragdoll_Activate_1"
  # PathConstraintSettings (1 función)
  "_emscripten_bind_PathConstraintSettings_Create_2"
  # BodyInterface getters (18 funciones)
  "_emscripten_bind_BodyInterface_GetPosition_1"
  "_emscripten_bind_BodyInterface_GetRotation_1"
  "_emscripten_bind_BodyInterface_GetLinearVelocity_1"
  "_emscripten_bind_BodyInterface_GetAngularVelocity_1"
  "_emscripten_bind_BodyInterface_GetWorldTransform_1"
  "_emscripten_bind_BodyInterface_GetCenterOfMassTransform_1"
  "_emscripten_bind_BodyInterface_GetTransformedShape_1"
  "_emscripten_bind_BodyInterface_GetShape_1"
  "_emscripten_bind_BodyInterface_GetObjectLayer_1"
  "_emscripten_bind_BodyInterface_GetRestitution_1"
  "_emscripten_bind_BodyInterface_GetFriction_1"
  "_emscripten_bind_BodyInterface_GetGravityFactor_1"
  "_emscripten_bind_BodyInterface_GetUseManifoldReduction_1"
  "_emscripten_bind_BodyInterface_IsSensor_1"
  "_emscripten_bind_BodyInterface_IsActive_1"
  "_emscripten_bind_BodyInterface_GetMotionType_1"
  "_emscripten_bind_BodyInterface_GetMotionQuality_1"
  "_emscripten_bind_BodyInterface_GetUserData_1"
  # SoftBody (2 funciones)
  "_emscripten_bind_BodyInterface_CreateAndAddSoftBody_2"
  "_emscripten_bind_BodyInterface_CreateSoftBody_1"
)

list(JOIN EXPORTED_FUNCTIONS_LIST "', '" EXPORTED_FUNCTIONS_STR)
set(EXPORTED_FUNCTIONS_STR "['${EXPORTED_FUNCTIONS_STR}']")

set(EMCC_WASM_COMPAT_ARGS ${EMCC_WASM_ARGS}
  -s SINGLE_FILE=1
  -s EXPORTED_FUNCTIONS=${EXPORTED_FUNCTIONS_STR})

set(EMCC_GLUE_ARGS
  -c
  -I${JoltPhysics_SOURCE_DIR}/..
  -Wall
  -std=c++17
  -fno-rtti
  -fno-exceptions
  ${MULTI_THREADED_FLAG}
  -DJPH_OBJECT_LAYER_BITS=32
  -DJPH_DISABLE_CUSTOM_ALLOCATOR
  -include ${JOLT_HEADER_FILE}
  ${MEMORY_PROFILER_FLAG}
  ${ENABLE_SIMD_FLAG})

# Can't find a way to automatically inherit settings set by target_compile_definitions, so hardcoding defines here
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(JOLT_IDL_FILE ${JOLT_IDL_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/JoltJS-DebugRenderer.idl )
	set(EMCC_GLUE_ARGS
		${EMCC_GLUE_ARGS}
		-DJPH_DEBUG_RENDERER -DJPH_PROFILE_ENABLED -D_DEBUG)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(JOLT_IDL_FILE ${JOLT_IDL_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/JoltJS-DebugRenderer.idl )
	set(EMCC_GLUE_ARGS
		${EMCC_GLUE_ARGS}
		-DJPH_DEBUG_RENDERER -DJPH_PROFILE_ENABLED -DNDEBUG)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Distribution")
	set(EMCC_GLUE_ARGS
		${EMCC_GLUE_ARGS}
		-DNDEBUG)
else()
	message("Unknown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# Add double precision define
if (DOUBLE_PRECISION)
	set(EMCC_GLUE_ARGS ${EMCC_GLUE_ARGS} -DJPH_DOUBLE_PRECISION)
endif()

project("JoltPhysics.js")

add_custom_command(
	OUTPUT ${JOLT_TYPING}
	COMMAND npx webidl-dts-gen -e -d -i jolt.idl -o ${JOLT_TYPING} -n Jolt
	DEPENDS jolt.idl
	COMMENT "Generating JoltPhysics.js typing"
	VERBATIM)

add_custom_command(
	OUTPUT glue.cpp glue.js jolt.idl
	BYPRODUCTS parser.out WebIDLGrammar.pkl
	COMMAND cat ${JOLT_IDL_FILE} > jolt.idl
	COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} jolt.idl glue
	COMMAND ${REMOVE_THREAD_LOCAL}
	# CORRECCIÓN: usar TempAllocatorMalloc en lugar de puntero nulo
	COMMAND sed -i.bak -e "s/return new TempAllocator();/return new TempAllocatorMalloc();/g" glue.cpp
	# Verificación (solo advertencia, no detiene la build)
	COMMAND sh -c "if ! grep -q 'TempAllocatorMalloc' glue.cpp; then echo '⚠️  ADVERTENCIA: El parche de TempAllocator no se aplicó. Revisa glue.cpp manualmente.'; fi"
	DEPENDS ${JOLT_IDL_FILE}
	COMMENT "Generating JoltPhysics.js bindings"
	VERBATIM)

add_custom_command(
    OUTPUT glue.o
    COMMAND emcc glue.cpp ${EMCC_GLUE_ARGS} -o glue.o
    DEPENDS glue.cpp ${JOLT_HEADER_FILE}
    COMMENT "Building JoltPhysics.js bindings (glue)"
    VERBATIM)

add_custom_target(jolt-bindings ALL DEPENDS glue.js glue.o ${JOLT_TYPING})

if (NOT BUILD_WASM_COMPAT_ONLY)
	if (NOT ENABLE_MULTI_THREADING
		AND NOT ENABLE_SIMD)
		add_custom_command(
			OUTPUT ${OUTPUT_BASE_NAME}.js
			COMMAND emcc glue.o $<TARGET_FILE:Jolt> ${EMCC_JS_ARGS} -o ${OUTPUT_BASE_NAME}.js
			DEPENDS jolt-bindings ${JOLT_FRONT_MATTER_FILE} ${JOLT_TARGETS} ${JOLT_IDL_FILE} ${JOLT_HEADER_FILE}
			COMMENT "Building JoltPhysics.js javascript"
			VERBATIM)
		add_custom_target(jolt-javascript ALL DEPENDS ${OUTPUT_BASE_NAME}.js)
	endif()

	add_custom_command(
		OUTPUT ${OUTPUT_BASE_NAME}.wasm.js ${OUTPUT_BASE_NAME}.wasm.wasm
		COMMAND emcc glue.o $<TARGET_FILE:Jolt> ${EMCC_WASM_ARGS} -o ${OUTPUT_BASE_NAME}.wasm.js
		COMMAND ${REPLACE_IMPORT} ${OUTPUT_BASE_NAME}.wasm.js
		DEPENDS jolt-bindings ${JOLT_FRONT_MATTER_FILE} ${JOLT_TARGETS} ${JOLT_IDL_FILE} ${JOLT_HEADER_FILE}
		COMMENT "Building JoltPhysics.js webassembly"
		VERBATIM)
	add_custom_target(jolt-wasm ALL DEPENDS ${OUTPUT_BASE_NAME}.wasm.js ${OUTPUT_BASE_NAME}.wasm.wasm)
endif()

add_custom_command(
	OUTPUT ${OUTPUT_BASE_NAME}.wasm-compat.js
	COMMAND emcc glue.o $<TARGET_FILE:Jolt> ${EMCC_WASM_COMPAT_ARGS} -o ${OUTPUT_BASE_NAME}.wasm-compat.js
	COMMAND ${REPLACE_IMPORT} ${OUTPUT_BASE_NAME}.wasm-compat.js
	DEPENDS jolt-bindings ${JOLT_FRONT_MATTER_FILE} ${JOLT_TARGETS} ${JOLT_IDL_FILE} ${JOLT_HEADER_FILE}
	COMMENT "Building JoltPhysics.js compat webassembly"
	VERBATIM)
add_custom_target(jolt-wasm-compat ALL DEPENDS ${OUTPUT_BASE_NAME}.wasm-compat.js)