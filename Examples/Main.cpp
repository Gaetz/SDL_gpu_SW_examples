//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"


int main(int argc, char **argv) {
    Context context {};

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    InitializeAssetLoader(&context);

    Scene* scene = new Demo001_Basics_BasicTriangle();
    scene->Init(&context);

    Uint64 previous_time = 0;
    Uint64 current_time = 0;

    bool running = true;
    while (running) {

        // Compute delta time
        previous_time = current_time;
        current_time = SDL_GetTicks();
        context.delta_time = (float)(current_time - previous_time) / 1000.0f;

        // Main loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        scene->Update(&context);
        scene->Draw(&context);

        // Cap frame rate
        if (context.delta_time < 0.016f) {
            SDL_Delay(static_cast<Uint32>((0.016f - context.delta_time) * 1000));
        }
    }
    scene->Quit(&context);

    return 0;
}