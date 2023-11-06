#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *projectile_new(Entity* parent, Vector3D position, Vector3D dir, float speed, int damage);

void projectile_think(Entity *self);

void projectile_update(Entity *self);

#endif
