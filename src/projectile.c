#include "simple_logger.h"
#include "gfc_primitives.h"
#include "projectile.h"

int projectileTimer = 0;

Entity *projectile_new(Entity* parent, Vector3D position, Vector3D dir, float speed, int damage)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no projectile for you!");
        return NULL;
    }
    ent->parent = parent;
    ent->selectedColor = gfc_color(0.1,1,0.1,1);
    ent->color = gfc_color(1,1,1,1);
    ent->model = gf3d_model_load("models/cube.model");

    ent->think = projectile_think;
    ent->update = projectile_update;

    ent->isRigidBody = 1;
    ent->type = ENT_PROJ;
    
    vector3d_copy(ent->position, position);
    vector3d_normalize(&dir);
    vector3d_scale(dir, dir, speed * 0.2);
    vector3d_copy(ent->velocity, dir);
    ent->damageDealt = damage;
    

    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;
    
    return ent;
}

void projectile_think(Entity *self)
{
    projectileTimer++;
    if(projectileTimer >= 600)
    {
        projectileTimer = 0;
        entity_free(self);
    }
    Entity *other;
    if (!self)return;

    other = entity_get_collision_entity(self);
    if(other)
    {
        if(other->damage != NULL)
            other->damage(other, self->damageDealt, self->parent);

        entity_free(self);
    }
}
void projectile_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);
}

/*eol@eof*/
