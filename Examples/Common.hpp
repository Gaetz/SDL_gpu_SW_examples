//
// Created by Gaetz on 16/04/2025.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL3/SDL.h>

enum class LoadOperation
{
    Load = SDL_GPU_LOADOP_LOAD,             /**< The previous contents of the texture will be preserved. */
    Clear = SDL_GPU_LOADOP_CLEAR,           /**< The contents of the texture will be cleared to a color. */
    DontCare = SDL_GPU_LOADOP_DONT_CARE     /**< The previous contents of the texture need not be preserved. The contents will be undefined. */
};

enum class StoreOperation
{
    Store = SDL_GPU_STOREOP_STORE,                      /**< The contents generated during the render pass will be written to memory. */
    DontCare = SDL_GPU_STOREOP_DONT_CARE,               /**< The contents generated during the render pass are not needed and may be discarded. The contents will be undefined. */
    Resolve = SDL_GPU_STOREOP_RESOLVE,                  /**< The multisample contents generated during the render pass will be resolved to a non-multisample texture. The contents in the multisample texture may then be discarded and will be undefined. */
    ResolveAndStore = SDL_GPU_STOREOP_RESOLVE_AND_STORE /**< The multisample contents generated during the render pass will be resolved to a non-multisample texture. The contents in the multisample texture will be written to memory. */
};

struct Context
{
    const char* example_name_{};
    const char* base_path_{};
    SDL_Window* window_ = nullptr;
    SDL_GPUDevice* device_ = nullptr;
    bool left_pressed_{};
    bool right_pressed_{};
    bool down_pressed_{};
    bool up_pressed_{};
    float delta_time_{};
};

int CommonInit(Context& context, SDL_WindowFlags window_flags);

void InitializeAssetLoader(Context& context);

void CommonQuit(const Context& context);

#endif //COMMON_HPP
