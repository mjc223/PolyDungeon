#ifndef __CUBE_H__
#define __CUBE_H__

#include "entity.h"

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *cube_new(Vector3D position);

Entity *cube_wall_new(Vector3D position);

void cube_update(Entity *self);

void cube_think(Entity *self);


#endif
