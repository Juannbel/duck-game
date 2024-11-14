#include "collectables_manager.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"
#include "server/game/gun_entity.h"
#include "server/game/guns/death_laser.h"

#include "gun_types.h"


CollectablesManager::CollectablesManager(CollisionChecks& collision,
                                         std::unordered_map<uint8_t, DuckPlayer>& ducks,
                                         std::unordered_map<uint32_t, BoxEntity>& boxes):
        collisions(collision), bullets(collision, ducks, boxes), collectable_id() {}

void CollectablesManager::reset_collectables() {
    collectable_id = 0;
    picked_up_guns.clear();
    for (auto& [id, gun]: guns) {
        gun.reset();
    }
    guns.clear();
    bullets.clear_bullets();
}

std::shared_ptr<GunEntity> CollectablesManager::add_death_laser(Gun& gun) {
    guns.emplace(++collectable_id, new DeathLaserG(gun, &bullets, collisions));
    picked_up_guns.insert(collectable_id);
    return guns[collectable_id];
}

void CollectablesManager::new_gun(Gun& gun) {
    uint32_t id = gun.gun_id;
    switch (gun.type) {
        case Grenade:
            guns.emplace(id, new GrenadeG(gun, &bullets, collisions, false));
            break;
        case GunTypeCount:
            gun.type = Grenade;
            guns.emplace(id, new GrenadeG(gun, &bullets, collisions, true));
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

uint32_t CollectablesManager::add_gun(Gun& gun) {
    if (gun.type == None) {
        return 0;
    }
    if (gun.gun_id == 0) {
        gun.gun_id = ++collectable_id;
    }
    new_gun(gun);
    return collectable_id;
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
    float new_x = duck_hitbox.coords.x;
    float new_y = duck_hitbox.coords.y + duck_hitbox.height;
    gun->drop_gun(new_x, new_y);
    picked_up_guns.erase(gun_info.gun_id);
    if (gun_info.type == None) {
        guns[gun_info.gun_id].reset();
        guns.erase(gun_info.gun_id);
    }
}

void CollectablesManager::update_guns_and_bullets() {
    bullets.update_bullets();
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        gun->update_status();
    }
}

void CollectablesManager::add_guns_to_snapshot(Snapshot& snapshot) {
    std::vector<uint32_t> id_to_eliminate;
    for (auto& [id, gun]: guns) {
        if (picked_up_guns.find(id) != picked_up_guns.end()) {
            continue;
        }
        Gun snapshot_gun = gun->get_gun_info();
        if (snapshot_gun.type == None) {
            id_to_eliminate.push_back(id);
            continue;
        }
        snapshot.guns.push_back(snapshot_gun);
    }
    bullets.add_bullets_to_snapshot(snapshot);
    for (uint32_t id: id_to_eliminate) {
        guns.erase(id);
    }
}
