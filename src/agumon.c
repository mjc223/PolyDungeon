
#include "simple_logger.h"
#include "gfc_primitives.h"
#include "player.h"
#include "agumon.h"


Entity *agumon_new(Vector3D position)
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
    ent->think = agumon_think;
    ent->update = agumon_update;

    ent->isRigidBody = 1;
    ent->type = ENT_WALL;
    
    vector3d_copy(ent->position, position);
    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;
    
    return ent;
}

void agumon_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);

    //self->rotation.z += 0.01;
}

void agumon_think(Entity *self)
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

    /*

    if(gfc_sphere_overlap(self->col.s, get_player_sphere_after_move()))
    {
        slog("AGU: COLLISION DETECTED");
    }
    */
}

/*eol@eof*/
