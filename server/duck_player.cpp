#include "duck_player.h"

const uint8_t JUMP_IT = 40;
const uint8_t FLAPPING_IT = 5;
const float DUCK_SPEED = 4;
const float FALL_SPEED = 4;
const float MAP_EDGE = 50;

DuckPlayer::DuckPlayer(MapCollisions& map_collisions): status(), ammo(), it_jumping(), x(), y(), map_collisions(map_collisions) { status.is_dead = true; }

void DuckPlayer::set_coordenades_and_id(int16_t x, int16_t y, uint8_t id) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    status.duck_hp = 100;
    status.is_dead = false;
    status.x = x;
    status.y = y;
    status.duck_id = id;
    hitbox.x = x;
    hitbox.y = y;
    hitbox.height = DUCK_HITBOX_HEIGHT;
    hitbox.width = DUCK_HITBOX_WIDTH;
}

void DuckPlayer::update_status(const Command& command) {
    if (status.is_jumping) {
        ++it_jumping;
    }
    switch (command) {
        case StartMovingRight:
            if (!status.is_laying) {
                status.facing_right = true;
                status.is_running = true;
            }
            break;
        case StartMovingLeft:
            if (!status.is_laying) {
                status.facing_right = false;
                status.is_running = true;
            }
            break;
        case StopMoving:
            status.is_running = false;
            break;
        case StartShooting:
            status.is_shooting = true;
            break;
        case StopShooting:
            status.is_shooting = false;
            break;
        case LayDown:
            status.is_laying = (status.is_falling || status.is_jumping) ? false : true;
            status.is_running = status.is_laying ? false : status.is_running;
            break;
        case StandUp:
            status.is_laying = false;
            break;
        case Jump:
            if (status.is_falling) {
                it_flapping = status.is_flapping ? it_flapping : 1;
                status.is_flapping = true;
                break;
            }
            it_jumping = status.is_jumping ? it_jumping : 1;
            status.is_jumping = true;
            break;
        case PickUp:
            status.gun = map_collisions.pickup(hitbox);
            break;
        case DropGun:
            status.gun = None;
            ammo = 0;
            break;
        case DropArmor:
            status.armor_equiped = false;
            break;
        case DropHelmet:
            status.helment_equiped = false;
            break;
        default:
            break;
    }
}

void DuckPlayer::status_after_move(struct Collision& collision) {
    if (collision.vertical_collision && status.is_falling) {
        status.is_falling = false;
    } else if (collision.vertical_collision && status.is_jumping) {
        status.is_falling = true;
        status.is_jumping = false;
        it_jumping = 0;
    } else if (!collision.vertical_collision && !status.is_falling) {
        status.is_falling = true;
    }
    if (it_jumping > JUMP_IT) {
        status.is_jumping = false;
        status.is_falling = true;
        it_jumping = 0;
    }
    if (it_flapping > FLAPPING_IT) {
        status.is_flapping = false;
        it_flapping = 0;
    }
    if (x < -MAP_EDGE || x > MAP_WIDTH_PIXELS + MAP_EDGE || y > MAP_HEIGHT_PIXELS + MAP_EDGE) {
        status.is_dead = true;
        status.is_laying = true;
    }
    status.x = collision.last_valid_position.x;
    status.y = collision.last_valid_position.y;
}

void DuckPlayer::move_duck() {
    // Actualizar la posicion de las balas y vida de los patos si les pegan
    if (status.is_dead) { return; }
    
    int16_t new_x = x;
    int16_t new_y = y;
    if (status.is_running) {
        float move_x = 0;
        if (status.is_jumping) {
            move_x = DUCK_SPEED * 0.7;
        } else if (status.is_falling) {
            move_x = status.is_flapping ? DUCK_SPEED * 0.5 : DUCK_SPEED * 0.7;
        } else if (!status.is_laying) {
            move_x = DUCK_SPEED;
        }
        new_x = (status.facing_right) ? new_x + move_x : new_x - move_x;
    }
    if (status.is_falling) {
        float move_y = FALL_SPEED;
        if (status.is_flapping) {
            move_y /= 4;
            ++it_flapping;
        }
        new_y += move_y;
    }
    if (status.is_jumping) {
        float move_y = FALL_SPEED * static_cast<float>((JUMP_IT - it_jumping) / 10);
        it_jumping += 4;
        new_y = y - move_y;
    }
    hitbox.x = static_cast<int16_t>(x);
    hitbox.y = static_cast<int16_t>(y);
    struct Collision collision = map_collisions.check_near_blocks_collision(hitbox, new_x, new_y);
    x = collision.last_valid_position.x;
    y = collision.last_valid_position.y;
    hitbox.x = static_cast<int16_t>(x);
    hitbox.y = static_cast<int16_t>(y);
    status_after_move(collision);
}

Duck DuckPlayer::get_status() { return status; }
