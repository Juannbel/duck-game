#include "constant_looper.h"
#include "common/snapshot.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <iostream>

#define SPRITE_SIZE 32

#define FPS 30
#define RATE 1000/FPS

using namespace SDL2pp;

ConstantLooper::ConstantLooper(uint8_t duck_id, Snapshot& snapshot): duck_id(duck_id), last_snapshot(snapshot) {}

void ConstantLooper::run() try {
	// Initialize SDL library
	SDL sdl(SDL_INIT_VIDEO);

	// Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
	Window window("Duck game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_RESIZABLE);

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	// cargo una imagen png como textura
	Texture duck_sprite(renderer, DATA_PATH "/duck/duck_sprite.png");


    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i] = new RenderableDuck(&duck_sprite, DATA_PATH "/duck/frames_" + std::to_string(i) + ".yaml");

        ducks_renderables[i]->update_from_snapshot(duck);
    }


    bool keep_running = true;
	unsigned int t1 = SDL_GetTicks();

	while (keep_running) {
		// Event processing:
        keep_running = process_events();

        // aca habria que recibir el ultimo snapshot
        // last_snapshot = snapshots.try_pop();

        // Actualizar el estado de todo lo que se renderiza
        process_snapshot();

		// Clear screen
		renderer.Clear();

        // Update de los renderizables
        for (auto& duck : ducks_renderables) {
            duck.second->update();
        }

        // Render de los renderizables
        for (auto& duck : ducks_renderables) {
            duck.second->render(renderer);
        }

		renderer.Present();

		unsigned int t2 = SDL_GetTicks();
		int rest = RATE - (t2 - t1);
		if (rest < 0) {
			int behind = -rest;
			int lost = behind - behind % int(RATE);

            // recuperamos los frames perdidos
            uint8_t frames_to_skip = int(lost / RATE);

            for (auto& duck : ducks_renderables) {
                duck.second->skip_frames(frames_to_skip);
            }

			t1 += lost;
		} else {
			SDL_Delay(rest);
		}

		t1 += RATE;
	}

} catch (std::exception& e) {
    std::cerr << "Error catched on constant_looper.cpp" << std::endl;
    std::cerr << e.what() << std::endl;
}

bool ConstantLooper::process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return false;
            case SDLK_d:
                // si el pato no estaba corriendo, enviamos el comando de correr
                if (!last_snapshot.ducks[duck_id].is_running || !last_snapshot.ducks[duck_id].facing_right) {
                    // commands.push(StartMovingRight)
                }
                break;

            case SDLK_a:
                if (!last_snapshot.ducks[duck_id].is_running || last_snapshot.ducks[duck_id].facing_right) {
                    // commands.push(StartMovingLeft)
                }
                break;

            case SDLK_w:
                // siempre lo enviamos porque este o no saltando, indica tambien que quiere seguir aleteando
                // commands.push(Jump)
                break;
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {

            case SDLK_d:
                // si el pato no estaba corriendo, enviamos el comando de correr
                if (last_snapshot.ducks[duck_id].is_running) {
                    // commands.push(StopMovingRight)
                }
                break;

            case SDLK_a:
                if (last_snapshot.ducks[duck_id].is_running) {
                    // commands.push(StopMovingLeft)
                }
                break;
            }
        }
    }

    return true;
}

void ConstantLooper::process_snapshot() {
    // actualizar el estado de todos los renderizables
    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i]->update_from_snapshot(duck);
    }
}

Rect ConstantLooper::get_minimum_bounding_box() {
    int left = std::numeric_limits<int>::max();
    int right = std::numeric_limits<int>::min();
    int up = std::numeric_limits<int>::max();
    int down = std::numeric_limits<int>::min();

    // Recorrer todos los patos y encontrar los bordes mínimos y máximos
    for (auto& duck : ducks_renderables) {
        if (duck.second->is_dead()) {
            continue;
        }

        Rect bounding_box = duck.second->get_bounding_box();

        left = std::min(left, bounding_box.x);
        right = std::max(right, bounding_box.x + bounding_box.w);
        up = std::min(up, bounding_box.y);
        down = std::max(down, bounding_box.y + bounding_box.h);
    }

    return Rect(left, up, right - left, down - up);
}