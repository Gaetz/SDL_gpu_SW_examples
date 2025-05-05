cmake_minimum_required(VERSION 3.10)
project(SDL_gpu_examples)
set(CMAKE_CXX_STANDARD 20)
cmake_policy(SET CMP0135 NEW)

# Download SDL3 from github
set(SDL3_VERSION 3.2.10)
find_package(SDL3 ${SDL3_VERSION} QUIET)
include(FetchContent)

if (NOT SDL3_FOUND)
    if (UNIX)
        FetchContent_Declare(
                SDL3
                URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-${SDL3_VERSION}.tar.gz
        )
        FetchContent_MakeAvailable(SDL3)
    elseif (WIN32)
        FetchContent_Declare(
                SDL3
                URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-${SDL3_VERSION}.zip
        )
        FetchContent_MakeAvailable(SDL3)
    endif()
endif()

add_executable(SDL_gpu_examples
        Examples/Main.cpp
        Examples/Common.cpp
        Examples/Demo001_Basics_BasicTriangle.cpp
        Examples/Demo003_Basics_Pipelines.cpp
        Examples/Shader.cpp
        Examples/GraphicsPipeline.cpp
        Examples/Buffer.cpp
        Examples/TransferBuffer.cpp
        Examples/RenderPass.cpp
        Examples/RenderPass.hpp
)

target_include_directories(SDL_gpu_examples
    PRIVATE SDL3::SDL3
)

target_link_libraries(SDL_gpu_examples
    SDL3::SDL3
)

add_custom_command(TARGET SDL_gpu_examples POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Content $<TARGET_FILE_DIR:SDL_gpu_examples>/Content
)
