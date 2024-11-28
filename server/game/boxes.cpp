#include "boxes.h"

#include <cstdint>

#include "common/config.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"

static Config& config = Config::get_instance();

const float MAX_BOX_HP = config.get_initial_box_hp();

BoxEntity::BoxEntity(float x, float y, uint32_t id, CollisionChecks& collisions):
        info(), hitbox(), health(MAX_BOX_HP), collisions(collisions) {
    info.box_id = id;
    info.status = NoDamage;
    info.x = static_cast<int16_t>(x);
    info.y = static_cast<int16_t>(y);
    hitbox.coords = {x, y};
    hitbox.height = BOX_HITBOX_HEIGHT;
    hitbox.width = BOX_HITBOX_WIDTH;
}

bool BoxEntity::get_hit(const Rectangle& b_rec, uint8_t damage) {
    Collision coll = collisions.rectangles_collision(hitbox, b_rec);
    if (coll.horizontal_collision || coll.vertical_collision) {
        if (!damage || !health) {  // Para que la banana no se rompa si le pega a una caja
            return false;          // Ni que absorba todas las balas de una explosion en una iteracion
        }
        if (damage > health) {
            health = 0;
        } else {
            health -= damage;
        }
        if (health > MAX_BOX_HP * .75) {
            info.status = NoDamage;
        } else if (health > MAX_BOX_HP * .5) {
            info.status = LittleDamage;
        } else if (health > MAX_BOX_HP * .25) {
            info.status = MediumDamage;
        } else if (health >= 0) {
            info.status = HeavyDamage;
        }
        return true;
    }
    return false;
}

bool BoxEntity::destroyed() { return health == 0; }

Coordenades& BoxEntity::get_coords() { return hitbox.coords; }

Box BoxEntity::get_info() { return info; }
