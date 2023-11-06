#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *slime_new(Vector3D position);

void slime_update(Entity *self);

void slime_think(Entity *self);

void slime_damage(Entity *self, int damage, Entity *inflictor);



Entity *bird_new(Vector3D position);

void bird_update(Entity *self);

void bird_think(Entity *self);

void bird_damage(Entity *self, int damage, Entity *inflictor);


Entity *turret_new(Vector3D position);

void turret_update(Entity *self);

void turret_think(Entity *self);

void turret_damage(Entity *self, int damage, Entity *inflictor);

#endif
