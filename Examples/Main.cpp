//
// Created by Gaetz on 16/04/2025.
//

#include <memory>

#include "Demo001_Basics_BasicTriangle.hpp"

int main(int argc, char** argv)
{
    Context context{};

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    InitializeAssetLoader(context);

    // Init SDL, window and device
    const int common_init = CommonInit(context, 0);
    if (common_init < 0) { return common_init; }

    // Init scene
    const auto scene = std::make_unique<Demo001_Basics_BasicTriangle>();
    const int scene_init = scene->Init(context);
    bool running = scene_init >= 0;

    Uint64 previous_time = 0;
    Uint64 current_time = 0;

    // Main loop
    while (running)
    {
        // Compute delta time
        previous_time = current_time;
        current_time = SDL_GetTicks();
        context.delta_time_ = static_cast<float>(current_time - previous_time) / 1000.0f;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Update and draw the scene
        scene->Update(context);
        scene->Draw(context);

        // Cap frame rate
        if (context.delta_time_ < 0.016f)
        {
            SDL_Delay(static_cast<Uint32>((0.016f - context.delta_time_) * 1000));
        }
    }
    scene->Quit(context);
    CommonQuit(context);

    return 0;
}
