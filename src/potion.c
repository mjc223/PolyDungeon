#include "simple_logger.h"
#include "potion.h"
#include "player.h"



void potion_update(Entity *self);

void potion_think(Entity *self);


Entity *potion_health_new(Vector3D position)
{
    PotionData *pd;
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no potion for you!");
        return NULL;
    }
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->model = gf3d_model_load("models/potion_health.model");
    ent->scale = vector3d(5.0, 5.0, 5.0);
    ent->think = potion_think;
    ent->update = potion_update;
    
    pd = gfc_allocate_array(sizeof(PotionData), 1);
    pd->pt = PT_Health;
    ent->type = ENT_ITEM;

    ent->customData = pd;

    Box b = gfc_box(position.x, position.y, position.z, 5, 5, 5);
    ent->bounds = b;

    vector3d_copy(ent->position,position);
    vector3d_copy(ent->bounds, ent->position);
    return ent;
}

void potion_think(Entity *self)
{
    if (!self)return;
    /*
    switch(checkPd->pt)
    {
        case PT_Attack:
            slog("POTION ATTACK COLLISION DETECTED");
            break;
        case PT_Defense:
            slog("POTION DEFENSE COLLISION DETECTED");
            break;
        case PT_Health:
            slog("POTION HEALTH COLLISION DETECTED");
            break;
        case PT_Magic:
            slog("POTION MAGIC COLLISION DETECTED");
            break;
        case PT_Speed:
            slog("POTION SPEED COLLISION DETECTED");
            change_player_speed(0.5);
            break;
    }
    */
    //entity_free(self);

}

void potion_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position,self->position,self->velocity);    
}

/*eol@eof*/
