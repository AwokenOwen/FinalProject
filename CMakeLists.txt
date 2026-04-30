cmake_minimum_required(VERSION 3.20)
project(FinalProject)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

# ─── Force static libs so nothing needs manual installation ───────────────────
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# ─── Assimp ───────────────────────────────────────────────────────────────────
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        v5.3.1
)
set(ASSIMP_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL             OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS  OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB          ON  CACHE BOOL "" FORCE)  # bundle zlib too
FetchContent_MakeAvailable(assimp)

# ─── OpenAL Soft ──────────────────────────────────────────────────────────────
FetchContent_Declare(
        OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG        1.24.2          # pin a release, not master
)
set(ALSOFT_UTILS    OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(OpenAL)

# ─── GLFW ─────────────────────────────────────────────────────────────────────
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    ON
)
set(GLFW_BUILD_DOCS  OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL     OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ─── FreeType ─────────────────────────────────────────────────────────────────
FetchContent_Declare(
        FreeType
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG        VER-2-13-3      # pin a release, not master
)
FetchContent_MakeAvailable(FreeType)

# ─── GLM ──────────────────────────────────────────────────────────────────────
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# ─── nlohmann/json ────────────────────────────────────────────────────────────
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

# ─── Include dirs ─────────────────────────────────────────────────────────────
include_directories(
        glad/include
        ${CMAKE_BINARY_DIR}/_deps/freetype-src/include
        ${CMAKE_BINARY_DIR}/_deps/glfw-src/include
        ${CMAKE_BINARY_DIR}/_deps/glm-src/glm
        ${CMAKE_BINARY_DIR}/_deps/json-src/include
        ${CMAKE_BINARY_DIR}/_deps/openal-src/include
)

# ─── Executable ───────────────────────────────────────────────────────────────
add_executable(FinalProject
        main.cpp
        MovementComponent.cpp   MovementComponent.h
        Spline.cpp              Spline.h
        SpiderManager.cpp       SpiderManager.h
        SpiderMovement.cpp      SpiderMovement.h
        SlingShotComponent.cpp  SlingShotComponent.h
        BallLauncher.cpp        BallLauncher.h
        Camera.cpp
        Component.cpp
        DirectionalLight.cpp
        GameManager.cpp
        InputManager.cpp
        Material.cpp
        Mesh.cpp
        MeshRenderer.cpp
        Object.cpp
        PointLight.cpp
        ResourceManager.cpp
        Scene.cpp
        WindowManager.cpp
        TextRenderer.cpp
        AudioManager.cpp
        AudioSource.cpp         AudioSource.h
        glad.c
)

target_link_libraries(FinalProject PRIVATE
        assimp::assimp
        freetype
        glfw
        OpenAL::OpenAL
)