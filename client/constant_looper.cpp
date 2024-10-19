#include "constant_looper.h"
#include "client/camera.h"
#include "common/snapshot.h"
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <iostream>

#define FPS 30
#define RATE 1000/FPS

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define FLOOR_HEIGHT 100

#define USE_CAMERA true

#define DUCK_VELOCITY 10

using namespace SDL2pp;

ConstantLooper::ConstantLooper(uint8_t duck_id, Snapshot& snapshot): duck_id(duck_id), last_snapshot(snapshot) {}

void ConstantLooper::run() try {
	// Initialize SDL library
	SDL sdl(SDL_INIT_VIDEO);

	// Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
	Window window("Duck game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_RESIZABLE);

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	Texture gradient(renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_STATIC, 1, 256);
	{
		// Array holding RGB values for 256 pixels
		unsigned char grad[256 * 3];
		int n = 0; // counter

        for (int i = 0; i < 256; i++) {
            grad[n++] = 255;
            grad[n++] = 255;
            grad[n++] = 255;
        }


		// Update texture with our raw color data
		gradient.Update(NullOpt, grad, 3);
	}

	// cargo una imagen png como textura
	Texture duck_sprite(renderer, DATA_PATH "/sprites/duck/duck_sprite.png");
    Texture background(renderer, DATA_PATH "/backgrounds/forest.png");


    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i] = new RenderableDuck(&duck_sprite, DATA_PATH "/sprites/duck/frames_" + std::to_string(i) + ".yaml");

        ducks_renderables[i]->update_from_snapshot(duck);
    }

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    bool keep_running = true;
	unsigned int t1 = SDL_GetTicks();

	while (keep_running) {
		// Event processing:
        keep_running = process_events();

        // aca habria que recibir el ultimo snapshot
        // last_snapshot = snapshots.try_pop();
        // lo actualizamos manual por ahora como que fueramos el server, si hay un pato corriendo, lo movemos
        for (int i = 0; i < last_snapshot.players_quantity; i++) {
            Duck duck = last_snapshot.ducks[i];
            if (duck.is_jumping) {
                last_snapshot.ducks[i].y -= DUCK_VELOCITY;
                if (duck.y < 0) {
                    last_snapshot.ducks[i].y = 0;
                }
            } else {
                int floor_y = renderer.GetOutputHeight() - FLOOR_HEIGHT;
                if (duck.y < floor_y - SPRITE_SIZE) {
                    last_snapshot.ducks[i].y += DUCK_VELOCITY;
                } else {
                    last_snapshot.ducks[i].y = floor_y - SPRITE_SIZE;
                }
            }
            if (duck.is_running) {
                if (duck.facing_right) {
                    last_snapshot.ducks[i].x += DUCK_VELOCITY;
                    if (last_snapshot.ducks[i].x > renderer.GetOutputWidth() - SPRITE_SIZE) {
                        last_snapshot.ducks[i].x = renderer.GetOutputWidth() - SPRITE_SIZE;
                    }
                } else {
                    last_snapshot.ducks[i].x -= DUCK_VELOCITY;
                    if (last_snapshot.ducks[i].x < 0) {
                        last_snapshot.ducks[i].x = 0;
                    }
                }
            }
        }
        if (t1 > 10000) {
            last_snapshot.ducks[1].duck_hp = 0;
        }

        // Actualizar el estado de todo lo que se renderiza
        process_snapshot();

        if (USE_CAMERA) {
            Rect target_area = get_minimum_bounding_box();
            camera.setTarget(target_area);
        }

        camera.update();
        Rect camera_rect = camera.getCurrentRect();

        float scaleX = static_cast<float>(WINDOW_WIDTH) / camera_rect.w;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / camera_rect.h;
        float scale = std::min(scaleX, scaleY);

		// Clear screen
		renderer.Clear();

        // dibujar el fondo teniendo en cuenta la camara
        Rect background_rect(0, 0, renderer.GetOutputWidth(), renderer.GetOutputHeight());
        background_rect.x = -camera_rect.x * scale;
        background_rect.y = -camera_rect.y * scale;
        background_rect.w = renderer.GetOutputWidth() * scale;
        background_rect.h = renderer.GetOutputHeight() * scale;

        renderer.Copy(background, NullOpt, background_rect);

        // Dibujar el piso
        renderer.SetDrawColor(200,200,200, 255);
        Rect floor(0, (renderer.GetOutputHeight() - FLOOR_HEIGHT - camera_rect.y)*scale, renderer.GetOutputWidth()*scale, FLOOR_HEIGHT*scale);
        renderer.FillRect(floor);

        renderer.SetDrawColor(0,0,0, 255);


        // Update de los renderizables
        for (auto& duck : ducks_renderables) {
            duck.second->update();
        }

        // Render de los renderizables
        for (auto& duck : ducks_renderables) {
            duck.second->render(renderer, camera_rect, scale);
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
    std::cerr << e.what() << std::endl;
}

bool ConstantLooper::process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            // cambiar de pato (por ahora)
            duck_id = (duck_id + 1) % last_snapshot.players_quantity;
            return true;
        }

        if (last_snapshot.ducks[duck_id].duck_hp == 0) {
            return true;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            
            case SDLK_d:
                // si el pato no estaba corriendo, enviamos el comando de correr
                if (!last_snapshot.ducks[duck_id].is_running || !last_snapshot.ducks[duck_id].facing_right) {
                    last_snapshot.ducks[duck_id].facing_right = true;
                    last_snapshot.ducks[duck_id].is_running = true;
                    // commands.push(StartMovingRight)
                }
                break;

            case SDLK_a:
                if (!last_snapshot.ducks[duck_id].is_running || last_snapshot.ducks[duck_id].facing_right) {
                    last_snapshot.ducks[duck_id].facing_right = false;
                    last_snapshot.ducks[duck_id].is_running = true;
                    // commands.push(StartMovingLeft)
                }
                break;

            case SDLK_w:
                if (!last_snapshot.ducks[duck_id].is_jumping) {
                    last_snapshot.ducks[duck_id].is_jumping = true;
                }
                // siempre lo enviamos porque este o no saltando, indica tambien que quiere seguir aleteando
                // commands.push(Jump)
                break;

            case SDLK_s:
                if (!last_snapshot.ducks[duck_id].is_lying) {
                    last_snapshot.ducks[duck_id].is_lying = true;
                }
                // commands.push(StopJumping)
                break;
            }

        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {

            case SDLK_d:
                // si el pato no estaba corriendo, enviamos el comando de correr
                if (last_snapshot.ducks[duck_id].is_running && last_snapshot.ducks[duck_id].facing_right) {
                    last_snapshot.ducks[duck_id].is_running = false;
                    // commands.push(StopMovingRight)
                }
                break;

            case SDLK_a:
                if (last_snapshot.ducks[duck_id].is_running && !last_snapshot.ducks[duck_id].facing_right) {
                    last_snapshot.ducks[duck_id].is_running = false;
                    // commands.push(StopMovingLeft)
                }
                break;
            
            case SDLK_w:
                if (last_snapshot.ducks[duck_id].is_jumping) {
                    last_snapshot.ducks[duck_id].is_jumping = false;
                    // commands.push(StopJumping)
                }
                break;
            
            case SDLK_s:
                if (last_snapshot.ducks[duck_id].is_lying) {
                    last_snapshot.ducks[duck_id].is_lying = false;
                    // commands.push(StopLying)
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
    int top = std::numeric_limits<int>::max();
    int bottom = std::numeric_limits<int>::min();

    // Recorrer todos los patos y encontrar los bordes mínimos y máximos
    for (auto& duck : ducks_renderables) {
        if (duck.second->is_dead()) {
            continue;
        }

        Rect bounding_box = duck.second->get_bounding_box();

        left = std::min(left, bounding_box.x);
        right = std::max(right, bounding_box.x + bounding_box.w);
        top = std::min(top, bounding_box.y);
        bottom = std::max(bottom, bounding_box.y + bounding_box.h);
    }

    // que no se vaya de los bordes del mapa

    int padding = 50;
    left -= padding;
    right += padding;
    top -= padding;
    bottom += padding;

    left = std::max(left, 0);
    right = std::min(right, WINDOW_WIDTH);
    top = std::max(top, 0);
    bottom = std::min(bottom, WINDOW_HEIGHT);

    int width = right - left;
    int height = bottom - top;

    // Ajustar para mantener el aspect ratio de la ventana
    float targetAspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    float currentAspectRatio = static_cast<float>(width) / height;

    if (currentAspectRatio > targetAspectRatio) {
        // Demasiado ancho, aumentar altura
        height = static_cast<int>(width / targetAspectRatio);
    } else {
        // Demasiado alto, aumentar ancho
        width = static_cast<int>(height * targetAspectRatio);
    }

    // Centrar la cámara
    int centerX = (left + right) / 2;
    int centerY = (top + bottom) / 2;

    return Rect(centerX - width / 2, centerY - height / 2, width, height);

}