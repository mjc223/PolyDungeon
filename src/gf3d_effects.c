#include "simple_logger.h"
#include "gf3d_effects.h"

typedef struct
{
    Uint32      effect_max;
    GF3DEffect *effect_list;
}GF3DEventManager;

static GF3DEventManager gf3d_effect_manager = {0};

void gf3d_effect_manager_close()
{
    int i;
    for (i = 0; i < gf3d_effect_manager.effect_max;i++)
    {
        if (!gf3d_effect_manager.effect_list[i]._inuse)continue;
        gf3d_effect_free(&gf3d_effect_manager.effect_list[i]);
    }
    if (gf3d_effect_manager.effect_list)free(gf3d_effect_manager.effect_list);
    memset(&gf3d_effect_manager,0,sizeof(GF3DEventManager));
}

void gf3d_effect_manager_init(Uint32 maxEffects)
{
    if (!maxEffects)
    {
        slog("cannot initialize effect manager with zero effects");
        return;
    }
    gf3d_effect_manager.effect_list = gfc_allocate_array(sizeof(GF3DEffect),maxEffects);
    if (!gf3d_effect_manager.effect_list)
    {
        slog("effect manager could not allocate memory for effects");
        return;
    }
    gf3d_effect_manager.effect_max = maxEffects;
    atexit(gf3d_effect_manager_close);
}

void gf3d_effect_update(GF3DEffect *effect,Uint32 now)
{
    if (!effect)return;
    if ((effect->ttd > 0)&&(effect->ttd < now))
    {//if we time out   AND we have timed out
        gf3d_effect_free(effect);
        return;
    }
    gfc_color_add(&effect->color,effect->color,effect->colorVector);
    gfc_color_add(&effect->colorVector,effect->colorVector,effect->colorAcceleration);
    //do fade in/ fade out
    
    //do physcial updates
    switch (effect->eType)
    {
        case GF3D_ET_Particle:
            vector3d_add(effect->particle.position,effect->particle.position,effect->velocity);
            vector3d_add(effect->velocity,effect->velocity,effect->acceleration);
            gfc_color_copy(effect->particle.color,effect->color);
            effect->particle.size *= effect->sizeDelta;
            break;
        case GF3D_ET_Model:
            gf3d_model_mat_move(&effect->mat,effect->mat.positionDelta);
            gf3d_model_mat_rotate(&effect->mat,effect->mat.rotationDelta);
            gf3d_model_mat_scale(&effect->mat,effect->mat.scaleDelta);
            vector3d_add(effect->mat.positionDelta,effect->mat.positionDelta,effect->acceleration);
            gfc_matrix4_from_vectors(
                effect->mat.mat,
                effect->mat.position,
                effect->mat.rotation,
                effect->mat.scale);
            break;
        default:
            break;
    }
}

void gf3d_effect_draw(GF3DEffect *effect)
{
    if (!effect)return;
    switch (effect->eType)
    {
        case GF3D_ET_Particle:
            gf3d_particle_draw(effect->particle);
            break;
        case GF3D_ET_Model:
            gf3d_model_draw(effect->mat.model,0,effect->mat.mat,gfc_color_to_vector4f(effect->color),vector4d(1,1,1,1),vector4d(1,1,1,1));
            break;
        default:
            break;
    }
}

void gf3d_effect_manager_draw_all()
{
    int i;
    Uint32 now = SDL_GetTicks();
    for (i = 0; i < gf3d_effect_manager.effect_max;i++)
    {
        if (!gf3d_effect_manager.effect_list[i]._inuse)continue;
        gf3d_effect_update(&gf3d_effect_manager.effect_list[i],now);
        gf3d_effect_draw(&gf3d_effect_manager.effect_list[i]);
    }    
}

GF3DEffect *gf3d_effect_new()
{
    int i;
    for (i = 0; i < gf3d_effect_manager.effect_max;i++)
    {
        if (gf3d_effect_manager.effect_list[i]._inuse)continue;
        gf3d_effect_manager.effect_list[i]._inuse = 1;
        gf3d_effect_manager.effect_list[i].color = gfc_color8(255,255,255,255);
        gf3d_model_mat_reset(&gf3d_effect_manager.effect_list[i].mat);
        gf3d_effect_manager.effect_list[i].sizeDelta = 1;
        return &gf3d_effect_manager.effect_list[i];
    }
    return NULL;
}

void gf3d_effect_free(GF3DEffect *effect)
{
    if (!effect)return;
    if (effect->mat.model)gf3d_model_free(effect->mat.model);
    memset(effect,0,sizeof(GF3DEffect));
}

GF3DEffect *gf3d_effect_new_particle(
    Vector3D position,
    Vector3D velocity,
    Vector3D acceleration,
    float size,
    float sizeDelta,
    Color color,
    Color colorVector,
    Color colorAcceleration,
    Sint32 ttl)
{
    GF3DEffect *effect;
    effect = gf3d_effect_new();
    if (!effect)return NULL;
    effect->eType = GF3D_ET_Particle;
    if (ttl < 0)effect->ttd = -1;
    else effect->ttd = SDL_GetTicks() + ttl;
    effect->particle.position = position;
    effect->velocity = velocity;
    effect->acceleration = acceleration;
    effect->particle.size = size;
    effect->sizeDelta = sizeDelta;
    gfc_color_copy(effect->particle.color,color);
    gfc_color_copy(effect->color,color);
    gfc_color_copy(effect->colorVector,colorVector);
    gfc_color_copy(effect->colorAcceleration,colorAcceleration);
    return effect;
}

GF3DEffect *gf3d_effect_from_config(SJson *config)
{
    GF3DEffect *effect;
    if (!config)return NULL;
    effect = gf3d_effect_new();
    if (!effect)return NULL;
    
    return effect;
}

/*eol@eof*/
