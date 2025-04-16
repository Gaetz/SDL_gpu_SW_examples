//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"

int main()
{
    Context context;
    Scene* scene = new Demo001_Basics_BasicTriangle();
    scene->Init(&context);

    Uint64 previousTime = 0;

    bool running = true;
    while (running) {

        // Compute delta time
        int currentTime = SDL_GetTicks();
        context.deltaTime = (currentTime - previousTime) / 1000.0f;

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
        if (context.deltaTime < 0.016f) {
            SDL_Delay((Uint32)((0.016f - context.deltaTime) * 1000));
        }
    }
    scene->Quit(&context);


    return 0;
}