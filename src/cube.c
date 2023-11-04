#include "simple_logger.h"
#include "gfc_primitives.h"
#include "player.h"
#include "cube.h"


Entity *cube_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/cube.model");
    ent->think = cube_think;
    ent->update = cube_update;
    ent->scale = vector3d(5000, 5000, 5.0);

    ent->isRigidBody = 1;
    ent->type = ENT_WALL;

    vector3d_copy(ent->position, position);
    Box b = gfc_box(position.x, position.y, position.z, 5000, 5000, 5);
    ent->bounds = b;
    
    return ent;
}

void cube_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);
}

void cube_think(Entity *self)
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

}

/*eol@eof*/
