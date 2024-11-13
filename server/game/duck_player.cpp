#include "duck_player.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>

#include <sys/types.h>

#include "common/snapshot.h"
#include "server/game/collisions.h"

#include "ticks.h"

const uint8_t JUMP_IT = TICKS;
const uint8_t DECREACENT_JUMP_SPEED = TICKS / 3;
const uint8_t FLAPPING_TIME = TICKS / 3;
const uint8_t IT_TO_GET_HIT_AGAIN = TICKS / 10;
const uint8_t SLIDING_ITS = TICKS / 2;
const float DUCK_SPEED = 150.0f / TICKS;
const float FALL_SPEED = 120.0f / TICKS;
const float JUMP_SPEED = FALL_SPEED * 4;
const float KNOCKBACK_MOVE = 4;

DuckPlayer::DuckPlayer(CollectablesManager& collectables, CollisionChecks& collisions, int16_t x,
                       int16_t y, uint8_t id, const std::string& name):
        status(),
        it_jumping(),
        it_flapping(),
        it_sliding(),
        ready_to_jump(true),
        hitbox(),
        collisions(collisions),
        collectables(collectables),
        equipped_gun(nullptr) {
    status.duck_hp = 100;
    status.is_dead = false;
    hitbox.coords.x = static_cast<float>(x);
    hitbox.coords.y = static_cast<float>(y);
    hitbox.height = DUCK_HITBOX_HEIGHT;
    hitbox.width = DUCK_HITBOX_WIDTH;
    status.x = x;
    status.y = y;
    status.duck_id = id;
    std::strncpy(status.player_name, name.c_str(), MAX_PLAYER_NAME);
    status.player_name[MAX_PLAYER_NAME - 1] = '\0';
}

void DuckPlayer::die() {
    status.is_dead = true;
    status.duck_hp = 0;
    status.is_laying = true;
    drop_collectable();
}

void DuckPlayer::status_after_move(struct Collision& collision) {
    if (collision.vertical_collision && status.is_falling) {
        status.is_falling = false;
        status.is_flapping = false;
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
    if (it_flapping > FLAPPING_TIME) {
        status.is_flapping = false;
        it_flapping = 0;
    }
    if (collisions.out_of_map(hitbox.coords.x, hitbox.coords.y)) {
        die();
    }
    status.x = static_cast<int16_t>(collision.last_valid_position.x);
    status.y = static_cast<int16_t>(collision.last_valid_position.y);
}

Collision DuckPlayer::move_sliding() {
    float new_x = hitbox.coords.x;
    float new_y = hitbox.coords.y;
    new_x = status.facing_right ? new_x + DUCK_SPEED * 1.5 : new_x - DUCK_SPEED * 1.5;
    if (status.is_falling) {
        float move_y = FALL_SPEED;
        new_y += move_y;
    }
    if (status.is_jumping) {
        float move_y =
                JUMP_SPEED - (static_cast<float>(FALL_SPEED * it_jumping) / DECREACENT_JUMP_SPEED);
        it_jumping += 2;
        new_y -= move_y;
    }
    --it_sliding;
    if (!it_sliding)
        stand_up();
    return collisions.check_near_blocks_collision(hitbox, new_x, new_y);
}

Collision DuckPlayer::normal_duck_move() {
    float new_x = hitbox.coords.x;
    float new_y = hitbox.coords.y;
    if (status.is_running) {
        float move_x = 0;
        if (status.is_jumping) {
            move_x = DUCK_SPEED;
        } else if (status.is_falling) {
            move_x = status.is_flapping ? DUCK_SPEED * 0.7 : DUCK_SPEED;
        } else if (!status.is_laying) {
            move_x = DUCK_SPEED;
        }
        new_x = (status.facing_right) ? new_x + move_x : new_x - move_x;
    }
    if (status.is_falling) {
        float move_y = FALL_SPEED;
        if (status.is_flapping) {
            move_y /= 5;
            ++it_flapping;
        }
        new_y += move_y;
    }
    if (status.is_jumping) {
        float move_y =
                JUMP_SPEED - (static_cast<float>(FALL_SPEED * it_jumping) / DECREACENT_JUMP_SPEED);
        it_jumping += 2;
        new_y -= move_y;
    }
    return collisions.check_near_blocks_collision(hitbox, new_x, new_y);
}

void DuckPlayer::move_duck() {
    if (status.is_dead) {
        return;
    }
    Collision collision{};
    if (it_sliding) {
        collision = move_sliding();
    } else {
        collision = normal_duck_move();
    }

    hitbox.coords.x = collision.last_valid_position.x;
    hitbox.coords.y = collision.last_valid_position.y;
    status_after_move(collision);
}

void DuckPlayer::run(bool right) {
    if (status.is_laying) {
        return;
    }
    status.facing_right = right;
    status.is_running = true;
}

void DuckPlayer::stop_running() { status.is_running = false; }

void DuckPlayer::shoot() {
    if (!equipped_gun || status.is_laying)
        return;
    equipped_gun->start_shooting();
    status.is_shooting = true;
}

void DuckPlayer::stop_shooting() {
    if (!equipped_gun)
        return;
    equipped_gun->stop_shooting();
    status.is_shooting = false;
}

void DuckPlayer::update_gun_status() {
    if (!equipped_gun)
        return;
    if (equipped_gun->update_bullets(hitbox, status.facing_right, status.facing_up)) {
        GunType g_type = equipped_gun->get_gun_info().type;
        switch (g_type) {
            case Armor:
                equip_armor();
                break;
            case Helmet:
                equip_helmet();
                break;
            default:
                knockback(g_type);
                break;
        }
    }
    if(equipped_gun && equipped_gun->empty()) {
        drop_collectable();
    }
}

void DuckPlayer::equip_armor() {
    if (!status.armor_equiped)
        drop_collectable();
    status.armor_equiped = true;
}

void DuckPlayer::equip_helmet() {
    if (!status.helmet_equiped)
        drop_collectable();
    status.helmet_equiped = true;
}

void DuckPlayer::lay_down() {
    if (status.is_laying)
        return;
    status.is_laying = (status.is_falling || status.is_jumping) ? false : true;
    status.is_running = status.is_laying ? false : status.is_running;
    if (status.is_laying || it_sliding) {
        hitbox.coords.y += DUCK_LAYED_HITBOX_HEIGHT;
        hitbox.height = DUCK_LAYED_HITBOX_HEIGHT;
        stop_shooting();
    }
}

void DuckPlayer::stand_up() {
    if (!status.is_laying || it_sliding) {
        return;
    }
    status.is_laying = false;
    hitbox.coords.y -= (DUCK_LAYED_HITBOX_HEIGHT + 1);
    hitbox.height = DUCK_HITBOX_HEIGHT;
}

void DuckPlayer::face_up() {
    if (status.is_laying) {
        return;
    }
    status.facing_up = true;
}

void DuckPlayer::stop_face_up() { status.facing_up = false; }

void DuckPlayer::jump() {
    if (!ready_to_jump || status.is_jumping || status.is_laying) {
        return;
    }
    ready_to_jump = false;
    if (status.is_falling) {
        it_flapping = 1;
        status.is_flapping = true;
        return;
    }
    it_jumping = 1;
    status.is_jumping = true;
}

void DuckPlayer::stop_jump() { ready_to_jump = true; }

bool DuckPlayer::get_hit(const Rectangle& bullet, uint8_t damage) {
    if (status.is_dead)
        return false;
    if (collisions.rectangles_collision(hitbox, bullet).vertical_collision) {
        if (status.armor_equiped) {
            status.armor_equiped = false;
            return true;
        } else if (status.helmet_equiped) {
            status.helmet_equiped = false;
            return true;
        }
        uint8_t taken_dmg = damage;
        if (status.duck_hp < taken_dmg) {
            die();
        } else {
            status.duck_hp -= taken_dmg;
        }
        return true;
    }
    return false;
}

void DuckPlayer::slide() {
    stop_running();
    it_sliding = SLIDING_ITS;
    lay_down();
    status.is_laying = true;
    it_flapping = 0;
    status.is_flapping = false;
}

void DuckPlayer::knockback(GunType type) {
    float new_x = hitbox.coords.x;
    float move = type == Shootgun ? KNOCKBACK_MOVE * 3 : KNOCKBACK_MOVE;
    if (!status.facing_up)
        new_x = status.facing_right ? new_x - move : new_x + move;
    float new_y = status.facing_up ? hitbox.coords.y + move : hitbox.coords.y;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, new_x, new_y).last_valid_position;
}

uint32_t DuckPlayer::drop_and_pickup() {
    if (status.is_dead) return 0;
    
    stop_shooting();
    std::shared_ptr<GunEntity> old_gun = equipped_gun;
    equipped_gun = collectables.pickup(hitbox);
    if (old_gun != nullptr) {
        old_gun->throw_gun(status.facing_right);
        collectables.drop_gun(old_gun, hitbox);
    }

    Gun gun_info = {};
    if (equipped_gun) {
        gun_info = equipped_gun->get_gun_info();
    }
    return gun_info.gun_id;
}

void DuckPlayer::drop_collectable() {
    if (!equipped_gun)
        return;
    if (!status.is_dead)
        equipped_gun->destroy();
    stop_shooting();
    collectables.drop_gun(equipped_gun, hitbox);
    status.gun = None;
    equipped_gun = nullptr;
}

Duck DuckPlayer::get_status() {
    Duck status_copy = this->status;
    if (equipped_gun) 
        status.gun = equipped_gun->get_gun_info().type;
    else
        status.gun = None;

    return status_copy;
}

const Coordenades& DuckPlayer::get_coords() { return hitbox.coords; }
