#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "object.h"

Entity *object_new(Vector3D position, ObjectType objType)
{
    Entity *ent = NULL;
    ObjectData *objData;
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no object for you!");
        return NULL;
    }
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->scale = vector3d(10.0, 10.0, 10.0);
    ent->type = ENT_INTERACT;

    ent->rotation = vector3d(0, 0, 0);

    objData = gfc_allocate_array(sizeof(objData), 1);
    objData->objType = objType;
    ent->customData = objData;

    if(objType != OBJ_INVISWALL)
        ent->model = gf3d_model_load("models/door.model");
    else
    {
        ent->model = gf3d_model_load("models/door.model");
        ent->scale = vector3d(0, 0, 0);
    }

    if(objType != OBJ_FAKEWALL)
    {
        Box b = gfc_box(position.x, position.y, position.z, 10, 10, 20);
        ent->bounds = b;
    }

    vector3d_copy(ent->position,position);
    vector3d_copy(ent->bounds, ent->position);
    return ent;
}

Entity *object_switch_new(Vector3D position, Entity *trigger)
{
    Entity *ent = NULL;
    ObjectData *objData;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no object for you!");
        return NULL;
    }
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->scale = vector3d(10.0, 10.0, 10.0);
    ent->rotation = vector3d(0, 0, 0);

    ent->type = ENT_INTERACT;

    objData = gfc_allocate_array(sizeof(objData), 1);
    objData->objType = OBJ_SWITCH;

    ent->customData = objData;

    ent->model = gf3d_model_load("models/door.model");
    ent->target = trigger;
    Box b = gfc_box(position.x, position.y, position.z, 10, 10, 20);
    ent->bounds = b;

    vector3d_copy(ent->position,position);
    vector3d_copy(ent->bounds, ent->position);
    return ent;
}