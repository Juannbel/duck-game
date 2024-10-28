#include "duck_player.h"
#include <functional>
#include <memory>
#include "ticks.h"

const uint8_t JUMP_IT = TICKS * 1.5;
const uint8_t INC_JUMP_IT = TICKS/20;
const uint8_t DECREACENT_JUMP_SPEED = TICKS / 3;
const uint8_t FLAPPING_TIME = TICKS / 3;
const float DUCK_SPEED = 120.0f/TICKS;
const float FALL_SPEED = 120.0f/TICKS;

DuckPlayer::DuckPlayer(CollectablesManager& collectables, CollisionChecks &collisions): 
        status(), 
        it_jumping(), 
        it_flapping(), 
        ready_to_jump(),
        hitbox(), 
        collisions(collisions), 
        collectables(collectables), 
        equipped_gun(nullptr) 
        { status.is_dead = true; }

void DuckPlayer::set_coordenades_and_id(int16_t x, int16_t y, uint8_t id) {
    status.duck_hp = 100;
    status.is_dead = false;
    hitbox.coords.x = static_cast<float>(x);
    hitbox.coords.y = static_cast<float>(y);
    hitbox.height = DUCK_HITBOX_HEIGHT;
    hitbox.width = DUCK_HITBOX_WIDTH;
    status.x = x;
    status.y = y;
    status.duck_id = id;
    ready_to_jump = true;
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
    if (it_flapping > FLAPPING_TIME) {
        status.is_flapping = false;
        it_flapping = 0;
    } 
    if (collisions.out_of_map(hitbox.coords.x, hitbox.coords.y)) {
        status.is_dead = true;
        status.duck_hp = 0;
    }
    status.x = static_cast<int16_t>(collision.last_valid_position.x);
    status.y = static_cast<int16_t>(collision.last_valid_position.y);
}

void DuckPlayer::move_duck() {
    if (status.is_dead) { return; }
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
        float move_y = FALL_SPEED * static_cast<float>((JUMP_IT - it_jumping) / DECREACENT_JUMP_SPEED);
        it_jumping += INC_JUMP_IT;
        new_y -= move_y;
    }
    struct Collision collision = collisions.check_near_blocks_collision(hitbox, new_x, new_y);
    hitbox.coords.x = collision.last_valid_position.x;
    hitbox.coords.y = collision.last_valid_position.y;
    status_after_move(collision);
}

void DuckPlayer::run(bool right) {
    if (status.is_laying) { return; }
    status.facing_right = right;
    status.is_running = true;
}

void DuckPlayer::stop_running() {
    status.is_running = false;
}

void DuckPlayer::shoot() {  
    if (!equipped_gun) return;   
    equipped_gun->start_shooting();
    status.is_shooting = true;
}

void DuckPlayer::stop_shooting() {
    if (!equipped_gun) return;   
    equipped_gun->stop_shooting();
    status.is_shooting = false;
}

void DuckPlayer::update_gun_status() { 
    if (!equipped_gun) return;   
    equipped_gun->update_bullets(*this); 
}

void DuckPlayer::lay_down() {
    if (status.is_laying) return;
    status.is_laying = (status.is_falling || status.is_jumping) ? false : true;
    status.is_running = status.is_laying ? false : status.is_running;
    if (status.is_laying) {
        hitbox.coords.y += DUCK_LAYED_HITBOX_HEIGHT;
        hitbox.height = DUCK_LAYED_HITBOX_HEIGHT;
    }
}

void DuckPlayer::stand_up() {
    if (!status.is_laying) { return; }
    status.is_laying = false;
    hitbox.coords.y -= (DUCK_LAYED_HITBOX_HEIGHT+1);
    hitbox.height = DUCK_HITBOX_HEIGHT;
}

void DuckPlayer::jump() {
    if (!ready_to_jump || status.is_jumping) { return; }
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

uint32_t DuckPlayer::drop_and_pickup(){
    std::shared_ptr<GunEntity> new_gun = collectables.pickup(hitbox);
    collectables.drop_gun(equipped_gun, hitbox);
    equipped_gun = new_gun;
    Gun gun_info = {};
    if(new_gun) {
        gun_info = new_gun->get_gun_info();
        status.gun = gun_info.type;
    } 
    status.gun = gun_info.type;
    return gun_info.gun_id;
}

Duck DuckPlayer::get_status() { return status; }
