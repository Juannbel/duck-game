#include "collectables_manager.h"

#include <unordered_map>
#include <vector>

#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"
#include "server/game/gun_entity.h"

#include "gun_types.h"
#include "ticks.h"

const float GUN_FALL_SPEED = 120.0f / TICKS;

CollectablesManager::CollectablesManager(CollisionChecks& collision,
                                         std::unordered_map<uint8_t, DuckPlayer>& ducks):
        collisions(collision), bullets(collision, ducks), collectable_id() {}

void CollectablesManager::reset_collectables() {
    collectable_id = 0;
    picked_up_guns.clear();
    for (auto& [id, gun]: guns) {
        gun.reset();
    }
    guns.clear();
    bullets.clear_bullets();
}


uint32_t CollectablesManager::get_and_inc_collectable_id() { return ++collectable_id; }

void CollectablesManager::new_gun(Gun& gun) {
    uint32_t id = gun.gun_id;
    switch (gun.type) {
        case Grenade:
            guns.emplace(id, new GrenadeG(gun, &bullets));
            break;
        case Banana:
            guns.emplace(id, new BananaG(gun, &bullets));
            break;
        case PewPewLaser:
            guns.emplace(id, new PewPewLaserG(gun, &bullets));
            break;
        case LaserRifle:
            guns.emplace(id, new LaserRifleG(gun, &bullets));
            break;
        case Ak47:
            guns.emplace(id, new Ak47G(gun, &bullets));
            break;
        case DuelingPistol:
            guns.emplace(id, new DuelingPistolG(gun, &bullets));
            break;
        case CowboyPistol:
            guns.emplace(id, new CowboyPistolG(gun, &bullets));
            break;
        case Magnum:
            guns.emplace(id, new MagnumG(gun, &bullets));
            break;
        case Shootgun:
            guns.emplace(id, new ShootgunG(gun, &bullets));
            break;
        case Sniper:
            guns.emplace(id, new SniperG(gun, &bullets));
            break;
        case Helmet:
            guns.emplace(id, new HelmetG(gun, &bullets));
            break;
        case Armor:
            guns.emplace(id, new ArmorG(gun, &bullets));
            break;
        default:
            break;
    }
}

void CollectablesManager::add_gun(Gun& gun) {
    if (gun.type == None) {
        return;
    }
    new_gun(gun);
}

std::shared_ptr<GunEntity> CollectablesManager::pickup(const Rectangle& duck) {
    Coordenades coords = {0, 0};
    Rectangle gun_r = {coords, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        Gun gun_info = gun->get_gun_info();
        gun_r.coords.x = gun_info.x;
        gun_r.coords.y = gun_info.y;
        Collision collision = collisions.rectangles_collision(duck, gun_r);
        if (collision.horizontal_collision && collision.vertical_collision) {
            picked_up_guns.insert(id);
            return gun;
        }
    }
    return nullptr;
}

void CollectablesManager::drop_gun(std::shared_ptr<GunEntity> gun, const Rectangle& duck_hitbox) {
    if (!gun)
        return;
    Gun gun_info = gun->get_gun_info();
    gun->set_new_coords(duck_hitbox.coords.x - (duck_hitbox.width / 2),
                        duck_hitbox.coords.y + duck_hitbox.height - COLLECTABLE_HITBOX_HEIGHT);
    picked_up_guns.erase(gun_info.gun_id);
    if (gun_info.type == None) {
        guns[gun_info.gun_id].reset();
        guns.erase(gun_info.gun_id);
    }
}

void CollectablesManager::move_guns_falling() {
    bullets.update_bullets();
    Coordenades coords = {0, 0};
    Rectangle gun_r = {coords, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        Gun gun_info = gun->get_gun_info();
        gun_r.coords.x = gun_info.x;
        gun_r.coords.y = gun_info.y;
        Coordenades coords =
                collisions
                        .check_near_blocks_collision(gun_r, gun_info.x, gun_info.y + GUN_FALL_SPEED)
                        .last_valid_position;
        gun->set_new_coords(coords.x, coords.y);
    }
}

void CollectablesManager::add_guns_to_snapshot(Snapshot& snapshot) {
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        Gun snapshot_gun = gun->get_gun_info();
        if (snapshot_gun.type == None) {
            continue;
        }
        snapshot.guns.push_back(snapshot_gun);
    }
    bullets.add_bullets_to_snapshot(snapshot);
}
