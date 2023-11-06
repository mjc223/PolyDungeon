#ifndef __OBJECT_H_
#define __OBJECT_H__

#include "entity.h"

typedef enum
{
    OBJ_DOOR,
    OBJ_SWITCH,
    OBJ_INVISWALL,
    OBJ_FAKEWALL,
    OBJ_TRICKEDDOOR,
}ObjectType;

typedef struct ObjectData
{
    ObjectType objType;
}ObjectData;


/**
 * @brief Create a new health potion entity
 * @param position where to spawn the potion at
 * @return NULL on error, or an potion entity pointer on success
 */
Entity *object_new(Vector3D position, ObjectType objType);

Entity *object_switch_new(Vector3D position, Entity *trigger);

#endif
