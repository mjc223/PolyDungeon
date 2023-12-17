#include "simple_logger.h"
#include "gfc_primitives.h"
#include "npc.h"

Vector3D startPos;
NPCData *data;

Entity *npc_new(Vector3D position, int id)
{
    Entity *ent = NULL;
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no agumon for you!");
        return NULL;
    }
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->model = gf3d_model_load("models/dino.model");
    ent->think = npc_think;
    ent->update = npc_update;
    ent->damage = npc_damage;
    ent->scale = vector3d(5,5,5);
    ent->maxHealth = 1;
    ent->currHealth = 1;

    ent->type = ENT_NPC;
    vector3d_copy(startPos, position);
    vector3d_copy(ent->position, position);

    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;

    set_npc_data(ent);

    slog("NPC created for you!");
}

void npc_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }

    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);

}

void npc_think(Entity *self)
{
    if (!self)return;
    switch(self->state)
    {
        case ES_idle:
            //look for player
            break;
        case ES_hunt:
            // set move towards player
            break;
        case ES_dead:
            // remove myself from the system
            break;
        case ES_attack:
            // run through attack animation / deal damage
            break;
    }

    if(self->currHealth <= 0)
    {
        entity_free(self);
    }
}

void npc_damage(Entity *self, int damage, Entity *inflictor)
{
    if (self == inflictor)
        return;
    slog("Was I hit");
    self->currHealth -= damage;
}

void set_npc_data(Entity *self)
{
    data = gfc_allocate_array(sizeof(NPCData), 1);

    data->name = "Jeff";
    data->message = "What's good.";

    self->customData = data;
}

/*eol@eof*/
