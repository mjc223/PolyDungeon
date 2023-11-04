#ifndef __POTION_H_
#define __POTION_H__

#include "entity.h"

typedef enum
{
    PT_Health,
    PT_Health_Cursed,
    PT_Damage_Boost,
    PT_Damage_Boost_Cursed,
    PT_Water,
    PT_Speed
}PotionType;

typedef struct PotionData
{
    PotionType pt;
    float timer;
}PotionData;

/**
 * @brief Create a new attack potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
//Entity *potion_attack_new(Vector3D position);

/**
 * @brief Create a new defense potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
//Entity *potion_defense_new(Vector3D position);

/**
 * @brief Create a new health potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
Entity *potion_health_new(Vector3D position);

/**
 * @brief Create a new magic potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
//Entity *potion_magic_new(Vector3D position);

/**
 * @brief Create a new speed potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
//Entity *potion_speed_new(Vector3D position);


#endif
