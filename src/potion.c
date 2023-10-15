#include "simple_logger.h"
#include "potion.h"
#include "player.h"

typedef enum
{
    PT_Attack,
    PT_Health,
    PT_Defense,
    PT_Magic,
    PT_Speed
}PotionType;

typedef struct PotionData
{
    PotionType pt;
    float timer;
}PotionData;

void potion_update(Entity *self);

void potion_think(Entity *self);

Entity *potion_attack_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/potion_attack.model");
    ent->scale = vector3d(5.0, 5.0, 5.0);
    ent->think = potion_think;
    ent->update = potion_update;
    
    pd = gfc_allocate_array(sizeof(PotionData), 1);
    pd->pt = PT_Attack;
    ent->customData = pd;

    vector3d_copy(ent->position,position);
    Collider col = collider_sphere_new(ent->position, gfc_sphere(ent->position.x, ent->position.y, ent->position.z, 10), 1);
    ent->col = col;
    return ent;
}

Entity *potion_defense_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/potion_defense.model");
    ent->scale = vector3d(5.0, 5.0, 5.0);
    ent->think = potion_think;
    ent->update = potion_update;

    pd = gfc_allocate_array(sizeof(PotionData), 1);
    pd->pt = PT_Defense;
    ent->customData = pd;
    
    vector3d_copy(ent->position,position);
    Collider col = collider_sphere_new(ent->position, gfc_sphere(ent->position.x, ent->position.y, ent->position.z, 10), 1);
    ent->col = col;
    return ent;
}

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
    ent->customData = pd;

    vector3d_copy(ent->position,position);
    Collider col = collider_sphere_new(ent->position, gfc_sphere(ent->position.x, ent->position.y, ent->position.z, 10), 1);
    ent->col = col;
    return ent;
}

Entity *potion_magic_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/potion_magic.model");
    ent->scale = vector3d(5.0, 5.0, 5.0);
    ent->think = potion_think;
    ent->update = potion_update;
    
    pd = gfc_allocate_array(sizeof(PotionData), 1);
    pd->pt = PT_Magic;
    ent->customData = pd;

    vector3d_copy(ent->position,position);
    Collider col = collider_sphere_new(ent->position, gfc_sphere(ent->position.x, ent->position.y, ent->position.z, 10), 1);
    ent->col = col;
    return ent;
}

Entity *potion_speed_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/potion_speed.model");
    ent->scale = vector3d(5.0, 5.0, 5.0);
    ent->think = potion_think;
    ent->update = potion_update;

    pd = gfc_allocate_array(sizeof(PotionData), 1);
    pd->pt = PT_Speed;
    ent->customData = pd;

    vector3d_copy(ent->position,position);
    Collider col = collider_sphere_new(ent->position, gfc_sphere(ent->position.x, ent->position.y, ent->position.z, 10), 1);
    ent->col = col;
    return ent;
}


void potion_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position,self->position,self->velocity);

    self->col.s.x = self->position.x;
    self->col.s.y = self->position.y;
    self->col.s.z = self->position.z;
    
}

void potion_think(Entity *self)
{
    if (!self)return;
    PotionData *checkPd = self->customData;

    if(gfc_sphere_overlap(self->col.s, get_player_sphere()))
    {
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
        entity_free(self);
    }





}

/*eol@eof*/
