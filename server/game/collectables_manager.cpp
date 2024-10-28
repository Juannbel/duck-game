#include "collectables_manager.h"

#include "common/shared_constants.h"
#include "ticks.h"
#include "gun_types.h"

#define GUN_FALL_SPEED (120/TICKS)
const int16_t NEAR_CELLS = 3;

CollectablesManager::CollectablesManager(CollisionChecks& collision) : collisions(collision), bullets(collision), collectable_id() {}

uint32_t CollectablesManager::get_and_inc_collectable_id() { return ++collectable_id; }

void CollectablesManager::new_gun(Gun& gun) {
    uint32_t id = gun.gun_id;
    switch (gun.type){
    case Grenade:
        guns.emplace(id, GrenadeG(gun, collisions, &bullets));
        break;
    case Banana:
        guns.emplace(id, BananaG(gun, collisions, &bullets));
        break;
    case PewPewLaser:
        guns.emplace(id, PewPewLaserG(gun, collisions, &bullets));
        break;
    case LaserRifle:
        guns.emplace(id, LaserRifleG(gun, collisions, &bullets));
        break;
    case Ak47:
        guns.emplace(id, Ak47G(gun, collisions, &bullets));
        break;
    case DuelingPistol:
        guns.emplace(id, DuelingPistolG(gun, collisions, &bullets));
        break;
    case CowboyPistol:
        guns.emplace(id, CowboyPistolG(gun, collisions, &bullets));
        break;
    case Magnum:
        guns.emplace(id, MagnumG(gun, collisions, &bullets));
        break;
    case Shootgun:
        guns.emplace(id, ShootgunG(gun, collisions, &bullets));
        break;
    case Sniper:
        guns.emplace(id, SniperG(gun, collisions, &bullets));
        break;
    case Helmet:
        guns.emplace(id, HelmetG(gun, collisions, &bullets));
        break;
    case Armor:
        guns.emplace(id, ArmorG(gun, collisions, &bullets));
        break;
    default:
        break;
    }
}

void CollectablesManager::add_gun(Gun& gun) {
    if (gun.type == None) { return; }
    new_gun(gun);
}

GunEntity CollectablesManager::empty_gun() { return GunEntity(collisions, &bullets); }


GunEntity CollectablesManager::pickup(const Rectangle &duck) {
    Rectangle gun_r = {0, 0, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto& [id, gun] : guns) {
        Gun gun_info = gun.get_gun_info();
        gun_r.coords.x = gun_info.x;
        gun_r.coords.y = gun_info.y;
        Collision collision = collisions.rectangles_collision(duck, gun_r);
        if(collision.horizontal_collision && collision.vertical_collision) {
            GunEntity new_gun(std::move(gun));
            guns.erase(id);
            return GunEntity(std::move(new_gun));
        }
    }
    return GunEntity(std::move(empty_gun()));
}

void CollectablesManager::drop_gun(GunEntity&& gun, const Rectangle& duck_hitbox){
    Gun gun_info = gun.get_gun_info();
    if (gun_info.type == None) {
        return;
    }
    gun.set_new_coords(duck_hitbox.coords.x-(duck_hitbox.width/2), duck_hitbox.coords.y+duck_hitbox.height-COLLECTABLE_HITBOX_HEIGHT);
    uint32_t id = gun_info.gun_id;
    guns.emplace(id, GunEntity(std::move(gun)));
    gun.drop();
}

void CollectablesManager::move_guns_falling() {
    bullets.update_bullets();
    Rectangle gun_r = {0, 0, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto &[id, gun] : guns) {
        Gun gun_info = gun.get_gun_info();
        gun_r.coords.x = gun_info.x;
        gun_r.coords.y = gun_info.y;
        Coordenades coords = collisions.check_near_blocks_collision(gun_r, gun_info.x, gun_info.y+GUN_FALL_SPEED).last_valid_position;
        gun.set_new_coords(coords.x, coords.y);
    }
    
}

void CollectablesManager::add_guns_to_snapshot(Snapshot& snapshot) {
    for (auto &[id, gun] : guns) {
        Gun snapshot_gun = gun.get_gun_info();
        snapshot.guns.push_back(snapshot_gun);
    }
    bullets.add_bullets_to_snapshot(snapshot);
}
