#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "potion.h"

static int thirdPersonMode = 0;
void player_think(Entity *self);
void player_update(Entity *self);

Entity *plr;

Vector3D lastPos;
float teleportTimer = 30;

Entity *player_new(Vector3D position)
{   
    plr = NULL;
    
    plr = entity_new();
    if (!plr)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    plr->isRigidBody = 1;

    plr->model = gf3d_model_load("models/dino.model");
    plr->think = player_think;
    plr->update = player_update;
    vector3d_copy(plr->position,position);
    vector3d_copy(lastPos, plr->position);
    plr->rotation.x = -GFC_PI;
    plr->rotation.z = -GFC_HALF_PI;
    plr->hidden = 1;

    Box b = gfc_box(position.x, position.y, position.z, 20, 20, 20);
    plr->bounds = b;

    set_player_data();

    return plr;
}


void player_think(Entity *self)
{
    PlayerData *checkPd = self->customData;
    Vector3D forward = {0};
    Vector3D right = {0};
    Vector2D w,mouse;
    Entity *other;

    float sprintMult = 1.0;
    int mx,my;
    SDL_GetRelativeMouseState(&mx,&my);
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    mouse.x = mx;
    mouse.y = my;

    if(self->currHealth == 0)
    {
        slog("PLAYER DEATH!");
    }

    w = vector2d_from_angle(self->rotation.z);

    if (keys[SDL_SCANCODE_LSHIFT])
        sprintMult = 2;

    forward.x = w.x * checkPd->speedMult * sprintMult;
    forward.y = w.y * checkPd->speedMult * sprintMult;
    w = vector2d_from_angle(self->rotation.z - GFC_HALF_PI);
    right.x = w.x * checkPd->speedMult * sprintMult;
    right.y = w.y * checkPd->speedMult * sprintMult;
    
    other = entity_get_collision_entity(self);

    if(other != NULL && other->type == ENT_WALL)
    {
        vector3d_copy(self->position, lastPos);
    }
    else
    {
        if(other != NULL)
        {
            switch(other->type)
            {
                case ENT_EQUIPMENT:
                    slog("We picked up an Equipment");
                    break;
                case ENT_ITEM:
                    slog ("We picked up an item");

                    PotionData *potData;
                    potData = other->customData;

                    PlayerData *playerData;
                    playerData = plr->customData;
                    if(potData->pt == PT_Health)
                    {
                        slog ("Health Potion");
                        plr->currHealth = plr->maxHealth;
                    }
                    else if (potData->pt == PT_Health_Cursed)
                    {
                        slog ("Cursed Health Potion");
                        plr->currHealth = 1;
                    }
                    else if (potData->pt == PT_Damage_Boost)
                    {
                        slog ("Damage Potion");
                        playerData->physicalMult = 2;
                    }
                    else if (potData->pt == PT_Damage_Boost_Cursed)
                    {
                        slog ("Cursed Damage Potion");
                        playerData->physicalMult = 0.1;
                    }
                    else if (potData->pt == PT_Speed)
                    {
                        slog("Speed potion");
                        change_player_speed(0.75);
                    }
                    
                    entity_free(other);
                    break;
                case ENT_INTERACT:
                    slog ("We are trying to interact");
                    break;
                case ENT_NPC:
                    slog ("We are trying to talk to an NPC");
                    break;
                case ENT_PROJ:
                    slog ("We are being hit by an enemy projectile");
                    break;
                default:
                    break;
            }
        }

        vector3d_copy(lastPos, self->position);   
        if (keys[SDL_SCANCODE_W])
        {   
            vector3d_add(self->position,self->position,forward);
        }
        if (keys[SDL_SCANCODE_S])
        {
            vector3d_add(self->position,self->position,-forward);        
        }
        if (keys[SDL_SCANCODE_D])
        {
            vector3d_add(self->position,self->position,right);
        }
        if (keys[SDL_SCANCODE_A])    
        {
            vector3d_add(self->position,self->position,-right);
        }
        if (keys[SDL_SCANCODE_SPACE])self->position.z += 1 * checkPd->speedMult;
        if (keys[SDL_SCANCODE_LCTRL])self->position.z -= 1 * checkPd->speedMult;
    }
             
    if (mouse.x != 0)self->rotation.z -= (mouse.x * 0.001);
    if (mouse.y != 0)self->rotation.x += (mouse.y * 0.001);

    if (keys[SDL_SCANCODE_F3])
    {
        thirdPersonMode = !thirdPersonMode;
        self->hidden = !self->hidden;
    }
}

void player_update(Entity *self)
{
    Vector3D forward = {0};
    Vector3D position;
    Vector3D rotation;
    Vector2D w;
    
    if (!self)return;


    vector3d_copy(position,self->position);
    vector3d_copy(self->bounds, self->position);
    vector3d_copy(rotation,self->rotation);
    


    if (thirdPersonMode)
    {
        position.z += 100;
        rotation.x += M_PI*0.125;
        w = vector2d_from_angle(self->rotation.z);
        forward.x = w.x * 100;
        forward.y = w.y * 100;
        vector3d_add(position,position,-forward);
    }
    
    gf3d_camera_set_position(position);
    gf3d_camera_set_rotation(rotation);
}

Sphere get_player_sphere()
{
    return plr->col.s;
}

Vector3D get_player_position()
{
    return plr->position;
}

void change_player_speed(float newMult)
{
    PlayerData *pd = plr->customData;
    pd->speedMult = newMult;
}

void set_player_data()
{
    plr->currHealth = 10;
    plr->maxHealth = 10;

    PlayerData *pd;
    pd = gfc_allocate_array(sizeof(PlayerData), 1);

    pd->currArrow = 20;
    pd->maxArrow = 20;

    pd->currMana = 10;
    pd->maxMana = 10;

    pd->arrowMult = 1.0;
    pd->magicMult = 1.0;
    pd->physicalMult = 1.0;
    pd->speedMult = 0.05;

    plr->customData = pd;
}

/*eol@eof*/
