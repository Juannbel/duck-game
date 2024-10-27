#include "collectables_manager.h"

#include "common/shared_constants.h"
#include "ticks.h"
#include "gun_types.h"

#define GUN_FALL_SPEED (120/TICKS)
const int16_t NEAR_CELLS = 3;

CollectablesManager::CollectablesManager(CollisionChecks& collision) : collisions(collision), collectable_id() {}

uint32_t CollectablesManager::get_and_inc_collectable_id() { return ++collectable_id; }

GunEntity CollectablesManager::new_gun(Gun& gun) {
    switch (gun.type){
    case Grenade:
        return GrenadeG(gun);
        break;
    case Banana:
        return BananaG(gun);
        break;
    case PewPewLaser:
        return PewPewLaserG(gun);
        break;
    case LaserRifle:
        return LaserRifleG(gun);
        break;
    case Ak47:
        return Ak47G(gun);
        break;
    case DuelingPistol:
        return DuelingPistolG(gun);
        break;
    case CowboyPistol:
        return CowboyPistolG(gun);
        break;
    case Magnum:
        return MagnumG(gun);
        break;
    case Shootgun:
        return ShootgunG(gun);
        break;
    case Sniper:
        return SniperG(gun);
        break;
    case Helmet:
        return HelmetG(gun);
        break;
    case Armor:
        return ArmorG(gun);
        break;
    default:
        break;
    }
    return GunEntity(gun);
}

void CollectablesManager::add_gun(Gun& gun) {
    if (gun.type == None) { return; }
    guns[gun.gun_id] = std::move(new_gun(gun));
}

void CollectablesManager::add_gun(GunEntity&& gun) {
    Gun gun_info = gun.get_gun_info();
    if (gun_info.type == None) { return; }
    guns[gun_info.gun_id] = std::move(gun);
}

GunEntity CollectablesManager::pickup(const Rectangle &duck) {
    Rectangle gun_r = {0, 0, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto& [id, gun] : guns) {
        Gun gun_info = gun.get_gun_info();
        gun_r.coords.x = gun_info.x;
        gun_r.coords.y = gun_info.y;
        Collision collision = collisions.rectangles_collision(duck, gun_r);
        if(collision.horizontal_collision && collision.vertical_collision) {
            GunEntity new_gun = std::move(guns[id]);
            guns.erase(id);
            return new_gun;
        }
    }
    return GunEntity();
}

void CollectablesManager::drop_gun(GunEntity &&gun, const Rectangle& duck_hitbox){
    Gun gun_info = gun.get_gun_info();
    if (gun_info.type == None) {
        return;
    }
    gun.set_new_coords(duck_hitbox.coords.x-(duck_hitbox.width/2), duck_hitbox.coords.y+duck_hitbox.height-COLLECTABLE_HITBOX_HEIGHT);
    add_gun(std::move(gun));
    gun.drop();
}

void CollectablesManager::move_guns_falling() {
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
}
