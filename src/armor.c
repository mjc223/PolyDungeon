#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "armor.h"

Entity *armor_new(Vector3D position, ArmorType equipSlot)
{
    ArmorData *ad;
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no potion for you!");
        return NULL;
    }
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->model = gf3d_model_load("models/armor.model");
    ent->scale = vector3d(4.0, 4.0, 4.0);
    
    ad = gfc_allocate_array(sizeof(ArmorData), 1);
    ad->equipSlot = equipSlot;
    ad->defense = 2;
    ad->durability = 100;
    ent->type = ENT_EQUIPMENT;

    ent->customData = ad;

    Box b = gfc_box(position.x, position.y, position.z, 5, 5, 20);
    ent->bounds = b;

    vector3d_copy(ent->position,position);
    vector3d_copy(ent->bounds, ent->position);
    return ent;
}