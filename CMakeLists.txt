cmake_minimum_required(VERSION 3.20)
project(FinalProject)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(OpenAL
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.25.0
        GIT_SHALLOW ON
)
set(ALSOFT_BUILD_ROUTER OFF CACHE BOOL "" FORCE)
set(AL_USE_MODULES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW ON)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")

FetchContent_Declare(FreeType
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG master)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG master)

FetchContent_MakeAvailable(assimp OpenAL glfw FreeType json)

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