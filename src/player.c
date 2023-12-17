#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "npc.h"
#include "hud.h"
#include "player.h"
#include "potion.h"
#include "object.h"
#include "projectile.h"

static int thirdPersonMode = 0;
void player_think(Entity *self);
void player_update(Entity *self);

static char* filename = "config/player.def";
SJson *jsonPlayer;

Entity *plr;
PlayerData *pd;
NPCData *pastData = {0};
Vector3D lastPos, arrowOffset;
int dodgeTimer = 300;
int arrowTimer = 200;
int npcRetrigger = 0;
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
    plr->damage = player_damage;
    vector3d_copy(plr->position,position);
    vector3d_copy(lastPos, plr->position);
    plr->rotation.x = -GFC_PI;
    plr->rotation.z = -GFC_HALF_PI;
    plr->hidden = 1;
    plr->isCrouching = 0;
    plr->isWalking = 0;
    plr->isBlocking = 0;
    plr->isGrounded = 0;

    Box b = gfc_box(position.x, position.y, position.z, 3, 3, 50);
    plr->bounds = b;

    set_player_data();
    atexit(player_save);

    return plr;
}


void player_think(Entity *self)
{
    PlayerData *checkPd = self->customData;
    Vector3D forward = {0};
    Vector3D right = {0};
    Vector3D dodgeRightVect = {0};
    Vector3D dodgeForwardVect = {0};
    Vector2D w,mouse;
    Entity *other;

    float sprintMult = 1.0;
    int mx,my;
    SDL_GetRelativeMouseState(&mx,&my);
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

    mouse.x = mx;
    mouse.y = my;

    if(self->currHealth <= 0)
    {
        slog("PLAYER DEATH!");
    }

    w = vector2d_from_angle(self->rotation.z);

    if (keys[SDL_SCANCODE_LSHIFT])
        sprintMult = 2;
    else if (keys[SDL_SCANCODE_LALT])
        sprintMult = 0.5;

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
    vector3d_copy(dodgeRightVect, right);
    vector3d_copy(dodgeForwardVect, forward);

    if(other != NULL)
    {
        switch(other->type)
        {
            case ENT_EQUIPMENT:
                slog("We picked up an Equipment");
                ArmorData *armData;
                armData = other->customData;

                checkPd = plr->customData;
                if(armData->equipSlot == AR_HEAD)
                {
                    slog ("Helmet Armor");
                    checkPd->head = armData;
                }
                else if (armData->equipSlot == AR_CHEST)
                {
                    slog ("Chest Armor");
                    checkPd->chest = armData;
                }
                else if (armData->equipSlot == AR_LEG)
                {
                    slog ("Leg Armor");
                    checkPd->legs = armData;
                }
                else if (armData->equipSlot == AR_BOOT)
                {
                    slog ("Boot Armor");
                    checkPd->boots = armData;
                }
                else if (armData->equipSlot == AR_RING)
                {
                    slog ("Ring Armor");
                    checkPd->ring = armData;
                    checkPd->speedMult += 0.05;
                }

                checkPd->defense += armData->defense;

                entity_free(other);
                break;
            case ENT_ITEM:
                slog ("We picked up an item");

                PotionData *potData;
                potData = other->customData;

                checkPd = plr->customData;
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
                    checkPd->physicalMult = 2;
                }
                else if (potData->pt == PT_Damage_Boost_Cursed)
                {
                    slog ("Cursed Damage Potion");
                    checkPd->physicalMult = 0.1;
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
                ObjectData *objData;
                slog ("prepare to set custom data");
                objData = other->customData;

                checkPd = plr->customData;
                if(objData->objType == OBJ_DOOR)
                {
                    slog ("Object Door");
                    vector3d_copy(self->position, lastPos);
                }
                else if (objData->objType == OBJ_FAKEWALL)
                {
                    slog ("Object Fake Wall");
                }
                else if (objData->objType == OBJ_INVISWALL)
                {
                    slog ("Object Invis Wall");
                    vector3d_copy(self->position, lastPos);
                }
                else if (objData->objType == OBJ_SWITCH)
                {
                    slog ("Object Switch");
                    vector3d_copy(self->position, lastPos);
                    entity_free(other->target);
                    entity_free(other);
                }
                else if (objData->objType == OBJ_TRICKEDDOOR)
                {
                    slog ("Object Tricked Door");
                    vector3d_copy(self->position, lastPos);
                    plr->currHealth -= 3;
                    entity_free(other);
                }
                
                break;
            case ENT_NPC:
                if(npcRetrigger == 0)
                {
                    NPCData *data = other->customData;
                    slog ("---BEGIN NPC DIALOGUE---");
                    slog (data->name);
                    slog (data->message);
                    pastData = data;
                    npcRetrigger = 1;
                }
                iterate_npc_message();

                break;
            case ENT_PROJ:
                break;
            default:
                break;
        }
    }
    else
    {
        npcRetrigger = 0;
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
    
    if (keys[SDL_SCANCODE_SPACE] && self->isGrounded)
    {
        vector3d_add(self->position, self->position, vector3d(0, 0, 40));
        self->isGrounded = 0;
    }
    //if (keys[SDL_SCANCODE_SPACE])self->position.z += 1 * checkPd->speedMult;
    //if (keys[SDL_SCANCODE_LCTRL])self->position.z -= 1 * checkPd->speedMult;
    if (keys[SDL_SCANCODE_C] || keys[SDL_SCANCODE_LCTRL])self->isCrouching = 1; else self->isCrouching = 0;

    if(keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT] 
    || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN])
    {
        

        dodgeRightVect.x = dodgeRightVect.x * 150;
        dodgeRightVect.y = dodgeRightVect.y * 150;
        
        dodgeForwardVect.x = dodgeForwardVect.x * 150;
        dodgeForwardVect.y = dodgeForwardVect.y * 150;

        if(dodgeTimer >= 3000)
        {
            dodgeTimer = 0;
            if (keys[SDL_SCANCODE_LEFT]) vector3d_add(self->position, self->position, -dodgeRightVect);
            if (keys[SDL_SCANCODE_RIGHT]) vector3d_add(self->position, self->position, dodgeRightVect);
            if (keys[SDL_SCANCODE_UP]) vector3d_add(self->position, self->position, dodgeForwardVect);
            if (keys[SDL_SCANCODE_DOWN]) vector3d_add(self->position, self->position, -dodgeForwardVect);
        }
    }

    arrowTimer += 1;
    if (keys[SDL_SCANCODE_F])self->isBlocking = 1; else self->isBlocking = 0;
    if (keys[SDL_SCANCODE_TAB])self->inStats = 1; else self->inStats = 0;
    
    //Gravity function
    if(self->position.z > 5)
    {
        vector3d_add(self->position, self->position, vector3d(0, 0, -0.25));
    }
    else
        self->isGrounded = 1;
    
    dodgeTimer++;
    if(self->isGrounded)
    {
        dodgeTimer++;
    }
    
    if (mouse.x != 0)self->rotation.z -= (mouse.x * 0.001);
    if (mouse.y != 0)self->rotation.x += (mouse.y * 0.001);

    if (keys[SDL_SCANCODE_F3])
    {
        thirdPersonMode = !thirdPersonMode;
        self->hidden = !self->hidden;
    }

    if(keys[SDL_SCANCODE_E])
    {
        if(arrowTimer >= 200 && pd->currArrow > 1)
        {
            pd->currArrow--;
            projectile_new(self, self->position, forward, 1, 1);
            arrowTimer = 0;
        }
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
    
    Vector3D cameraVect;
    vector3d_copy(cameraVect, position);
    if(self->isCrouching == 1)
    {
        cameraVect.z += 10;
        gf3d_camera_set_position(cameraVect);
    }
    else
    {
        cameraVect.z += 20;
        gf3d_camera_set_position(cameraVect);
    }
    vector3d_copy(arrowOffset, cameraVect);
    arrowOffset.z += 5;
    gf3d_camera_set_rotation(rotation);

}

void player_damage(Entity *self, int damage, Entity *inflictor)
{
    if (self == inflictor)
        return;
    slog("Ouchies");
    self->currHealth -= damage;
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
    
    plr->currHealth = 9;
    plr->maxHealth = 10;

    pd = gfc_allocate_array(sizeof(PlayerData), 1);

    plr->customData = pd;

    SJson *playerObj;

    if (!filename) return;

    jsonPlayer = sj_load(filename); //have a default player json
    if(!jsonPlayer) return;

    playerObj = sj_object_get_value(jsonPlayer, "player");
    if(!playerObj)
    {
        slog("file %s missing player object", filename);
        sj_free(jsonPlayer);
        return;
    }

    sj_object_get_value_as_float(playerObj, "xPos", &plr->position.x);
    sj_object_get_value_as_float(playerObj, "yPos", &plr->position.y);
    sj_object_get_value_as_float(playerObj, "zPos", &plr->position.z);

    sj_object_get_value_as_float(playerObj, "speedMult", &pd->speedMult);

    sj_object_get_value_as_float(playerObj, "physicalMult", &pd->physicalMult);
    sj_object_get_value_as_float(playerObj, "magicMult", &pd->magicMult);

}

void player_save()
{
    SJson *jsonCopy, *playerNew;

    if (!filename) return;

    if(!jsonPlayer) return;

    jsonCopy = sj_copy(jsonPlayer);

    playerNew = player_save_writer();

    sj_object_delete_key(jsonCopy, "player");

    sj_object_insert(jsonCopy, "player", playerNew);

    sj_save(jsonCopy, "config/player.def");
}

SJson* player_save_writer()
{
    SJson *newXPos, *newYPos, *newZPos, *newSpeedMult, *newPhysicalMult, *newMagicMult;

    SJson *newPlayer;
    newPlayer = sj_object_new();

    PlayerData *pd = plr->customData;

    newXPos = sj_new_float(plr->position.x);
    newYPos = sj_new_float(plr->position.y);
    newZPos = sj_new_float(plr->position.z);

    newSpeedMult = sj_new_float(pd->speedMult);

    newPhysicalMult = sj_new_float(pd->physicalMult);
    newMagicMult = sj_new_float(pd->magicMult);
    
    sj_object_insert(newPlayer, "xPos", newXPos);
    sj_object_insert(newPlayer, "yPos", newYPos);
    sj_object_insert(newPlayer, "zPos", newZPos);
    sj_object_insert(newPlayer, "speedMult", newSpeedMult);
    sj_object_insert(newPlayer, "physicalMult", newPhysicalMult);
    sj_object_insert(newPlayer, "magicMult", newMagicMult); 

    return newPlayer;
}

Entity *plr_copy()
{
    return plr;
}

/*eol@eof*/
