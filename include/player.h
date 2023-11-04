#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef struct PlayerData
{
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

Vector3D get_player_position();

void change_player_speed(float newMult);

void set_player_data();

#endif
