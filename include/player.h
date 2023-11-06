#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "armor.h"

typedef struct PlayerData
{
    ArmorData *head;
    ArmorData *chest;
    ArmorData *legs;
    ArmorData *boots;
    ArmorData *ring;

    float defense;

    int currArrow;
    int maxArrow;

    int currMana;
    int maxMana;

    float speedMult;
    float physicalMult;
    float magicMult;
    float arrowMult;
}PlayerData;

/**
 * @brief Create a new player entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new(Vector3D position);

void player_damage(Entity *self, int damage, Entity *inflictor);

Vector3D get_player_position();

void change_player_speed(float newMult);

void set_player_data();

#endif
