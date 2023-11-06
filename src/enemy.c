#include "simple_logger.h"
#include "gfc_primitives.h"
#include "enemy.h"
#include "projectile.h"
#include "player.h"

Vector3D startingPos;
int turretTimer = 0;

Entity *slime_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/slime.model");
    ent->think = slime_think;
    ent->update = slime_update;
    ent->damage = slime_damage;
    ent->scale = vector3d(5,5,5);
    ent->maxHealth = 1;
    ent->currHealth = 1;

    ent->type = ENT_NPC;
    vector3d_copy(startingPos, position);
    vector3d_copy(ent->position, position);
    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;

    slog("Slime created for you!");
}

void slime_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }

    if(gfc_random() > 0.5)
        self->position.x += gfc_random() * 0.3;        
    else
        self->position.x += gfc_random() * -0.3;
    
    if(gfc_random() > 0.5)
        self->position.y += gfc_random() * 0.3;
    else
        self->position.y += gfc_random() * -0.3;

    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);

}

void slime_think(Entity *self)
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

void slime_damage(Entity *self, int damage, Entity *inflictor)
{
    if (self == inflictor)
        return;
    slog("Was I hit");
    self->currHealth -= damage;
}


Entity *bird_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/slime.model");
    ent->think = bird_think;
    ent->update = bird_update;
    ent->damage = bird_damage;
    ent->scale = vector3d(5,5,5);
    ent->maxHealth = 1;
    ent->currHealth = 1;

    ent->type = ENT_NPC;
    vector3d_copy(startingPos, position);
    vector3d_copy(ent->position, position);
    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;

    slog("Bird created for you!");
}

void bird_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }

    if(gfc_random() > 0.5)
        self->position.x += gfc_random() * 0.4;        
    else
        self->position.x += gfc_random() * -0.4;
    
    if(gfc_random() > 0.5)
        self->position.y += gfc_random() * 0.4;
    else
        self->position.y += gfc_random() * -0.4;

    if(gfc_random() > 0.5)
        self->position.z += gfc_random() * 0.7;
    else
        self->position.z += gfc_random() * -0.7;        

    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);
}

void bird_think(Entity *self)
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

void bird_damage(Entity *self, int damage, Entity *inflictor)
{
    if (self == inflictor)
        return;
    self->currHealth -= damage;
}


Entity *turret_new(Vector3D position)
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
    ent->model = gf3d_model_load("models/slime.model");
    ent->think = turret_think;
    ent->update = turret_update;
    ent->damage = turret_damage;
    ent->scale = vector3d(5,5,5);
    ent->maxHealth = 1;
    ent->currHealth = 1;

    ent->type = ENT_NPC;
    vector3d_copy(startingPos, position);
    vector3d_copy(ent->position, position);
    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 50);
    ent->bounds = b;

    slog("Turret created for you!");
}

void turret_update(Entity *self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }

    vector3d_add(self->position,self->position,self->velocity);
    vector3d_copy(self->bounds, self->position);
}

void turret_think(Entity *self)
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

    turretTimer++;
    if(turretTimer >= 3000)
    {
        Vector2D vect;
        Vector3D dir;
        vect = vector2d_from_angle(self->rotation.z);
        dir.x = vect.x;
        dir.y = vect.y;
        dir.z += 0.3;
        projectile_new(self, self->position, dir, 1, 1);
        turretTimer = 0;
    }
}

void turret_damage(Entity *self, int damage, Entity *inflictor)
{
    if (self == inflictor)
        return;
    slog("Was I hit");
    self->currHealth -= damage;
}
/*eol@eof*/
