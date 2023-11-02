#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"

static int thirdPersonMode = 0;
void player_think(Entity *self);
void player_update(Entity *self);

Entity *plr;

Vector3D lastPos;

Entity *player_new(Vector3D position)
{   
    PlayerData *pd;
    pd = gfc_allocate_array(sizeof(PlayerData), 1);

    pd->arrowMult = 1.0;
    pd->magicMult = 1.0;
    pd->physicalMult = 1.0;
    pd->speedMult = 0.05;

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

    Box b = gfc_box(position.x, position.y, position.z, 2.0, 2.0, 2.0);
    plr->bounds = b;
    
    plr->customData = pd;

    return plr;
}


void player_think(Entity *self)
{
    PlayerData *checkPd = self->customData;
    Vector3D forward = {0};
    Vector3D right = {0};
    Vector2D w,mouse;


    Entity *other;
    Bool coll = false;

    int mx,my;
    SDL_GetRelativeMouseState(&mx,&my);
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    mouse.x = mx;
    mouse.y = my;

    w = vector2d_from_angle(self->rotation.z);
    forward.x = w.x * checkPd->speedMult;
    forward.y = w.y * checkPd->speedMult;
    w = vector2d_from_angle(self->rotation.z - GFC_HALF_PI);


    right.x = w.x * checkPd->speedMult;
    right.y = w.y * checkPd->speedMult;;
    
    other = entity_get_collision_entity(self);

    if (other == NULL)
        coll = false;
    else
        coll = true;

    if(coll && other->type == ENT_WALL)
    {
        vector3d_copy(self->position, lastPos);
        slog("help me");
    }
    else
    {
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
    }
             

    if (keys[SDL_SCANCODE_SPACE])self->position.z += 1;
    if (keys[SDL_SCANCODE_LCTRL])self->position.z -= 1;
    
    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0050;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0050;
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z -= 0.0050;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z += 0.0050;
    
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

    lastPos = self->position;
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

/*eol@eof*/
