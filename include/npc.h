#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

typedef struct NPCData
{
    char *name;
    char *message;
}NPCData;

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *npc_new(Vector3D position, int id);

void npc_update(Entity *self);

void npc_think(Entity *self);

void npc_damage(Entity *self, int damage, Entity *inflictor);

void set_npc_data();

void iterate_npc_message();

#endif
