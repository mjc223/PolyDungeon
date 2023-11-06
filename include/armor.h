#ifndef __ARMOR_H_
#define __ARMOR_H__

#include "entity.h"

typedef enum
{
    AR_HEAD,
    AR_CHEST,
    AR_LEG,
    AR_BOOT,
    AR_RING,
}ArmorType;

typedef struct ArmorData
{
    float defense;
    float durability;
    ArmorType equipSlot;
}ArmorData;

/**
 * @brief Create a new health potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
Entity *armor_new(Vector3D position, ArmorType equipSlot);

#endif
