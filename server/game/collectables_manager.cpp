#include "collectables_manager.h"

#include <unordered_map>
#include <vector>

#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"
#include "server/game/gun_entity.h"

#include "gun_types.h"


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
            guns.emplace(id, new GrenadeG(gun, &bullets, collisions));
            break;
        case Banana:
            guns.emplace(id, new BananaG(gun, &bullets, collisions));
            break;
        case PewPewLaser:
            guns.emplace(id, new PewPewLaserG(gun, &bullets, collisions));
            break;
        case LaserRifle:
            guns.emplace(id, new LaserRifleG(gun, &bullets, collisions));
            break;
        case Ak47:
            guns.emplace(id, new Ak47G(gun, &bullets, collisions));
            break;
        case DuelingPistol:
            guns.emplace(id, new DuelingPistolG(gun, &bullets, collisions));
            break;
        case CowboyPistol:
            guns.emplace(id, new CowboyPistolG(gun, &bullets, collisions));
            break;
        case Magnum:
            guns.emplace(id, new MagnumG(gun, &bullets, collisions));
            break;
        case Shootgun:
            guns.emplace(id, new ShootgunG(gun, &bullets, collisions));
            break;
        case Sniper:
            guns.emplace(id, new SniperG(gun, &bullets, collisions));
            break;
        case Helmet:
            guns.emplace(id, new HelmetG(gun, &bullets, collisions));
            break;
        case Armor:
            guns.emplace(id, new ArmorG(gun, &bullets, collisions));
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
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        const Rectangle& gun_r = gun->get_hitbox();
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
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        gun->check_movement();
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
