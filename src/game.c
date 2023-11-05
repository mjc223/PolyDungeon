#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_particle.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "agumon.h"
#include "cube.h"
#include "potion.h"

#include "player.h"
#include "world.h"

extern int __DEBUG;

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    
    Sprite *mouse = NULL;
    Sprite *playerFace = NULL;
    Sprite *blockImage = NULL;

    int mousex,mousey;
    //Uint32 then;
    float mouseFrame = 0;
    World *w;
    Entity *agu, *cube, *cube2, *cube3, *cube4, *cube5,
        *potionHealth, *potionCursedHealth,
        *potionDamage, *potionCursedDamage,
        *potionSpeed,
        *plr;
    

    Particle particle[100];
    Matrix4 skyMat;
    Model *sky;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }
    
    init_logger("gf3d.log",0);    
    gfc_input_init("config/input.cfg");
    slog("gf3d begin");
    gf3d_vgraphics_init("config/setup.cfg");
    gf2d_font_init("config/font.cfg");
    gf2d_draw_manager_init(1000);
    
    slog_sync();
    
    entity_system_init(1024);
    
    
    mouse = gf2d_sprite_load("images/pointer.png",32,32, 16);
    playerFace = gf2d_sprite_load("images/playerFace.png", 120, 120, 1);
    blockImage = gf2d_sprite_load("images/shield.png", 262, 320, 1);
    
    //Entities
    agu = agumon_new(vector3d(100, 100, 20));
    cube = cube_new(vector3d(0, 0, -5));
    cube2 = cube_wall_new(vector3d(2500, 0, -500));
    cube3 = cube_wall_new(vector3d(-2500, 0, -500));
    cube4 = cube_wall_new(vector3d(0, 2500, -500));
    cube5 = cube_wall_new(vector3d(0, -2500, -500));
    
    //Potions
    potionHealth = potion_health_new(vector3d(400, 400, 20));
    potionCursedHealth = potion_cursed_health_new(vector3d(450, 450, 20));
    potionDamage = potion_damage_new(vector3d(500, 500, 20));
    potionCursedDamage = potion_cursed_damage_new(vector3d(550, 500, 20));
    potionSpeed = potion_speed_new(vector3d(600, 600, 20));
    

    if (agu)agu->selected = 1;
    if (cube)cube->selected = 1;
    //if (potionAttack) potionAttack->selected = 1;
    
    w = world_load("config/testworld.json");
    
    slog("After world load");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));
    plr = player_new(vector3d(0,0,15));
    
    
    for (a = 0; a < 100; a++)
    {
        particle[a].position = vector3d(gfc_crandom() * 100,gfc_crandom() * 100,gfc_crandom() * 100);
        particle[a].color = gfc_color(0,0,0,1);
//        particle[a].color = gfc_color(gfc_random(),gfc_random(),gfc_random(),1);
        particle[a].size = 100 * gfc_random();
    }
    
    a = 0;
    sky = gf3d_model_load("models/sky.model");
    gfc_matrix_identity(skyMat);
    gfc_matrix_scale(skyMat,vector3d(100,100,100));
    
    // main game loop
    slog("gf3d main loop begin");
    while(!done)
    {
        gfc_input_update();
        gf2d_font_update();
        SDL_GetMouseState(&mousex,&mousey);
        
        
        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;
        world_run_updates(w);
        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        gf3d_vgraphics_render_start();

            //3D draws
                gf3d_model_draw_sky(sky,skyMat,gfc_color(1,1,1,1));
                world_draw(w);
                entity_draw_all();
                
                /*
                for (a = 0; a < 100; a++)
                {
                    gf3d_particle_draw(&particle[a]);
                }
                */
            //2D draws
                gf2d_draw_rect_filled(gfc_rect(10,10,1000,32),gfc_color8(128,128,128,255));
                gf2d_font_draw_line_tag("Press ALT+F4 to exit",FT_H1,gfc_color(1,1,1,1), vector2d(10,10));
                
                gf2d_draw_rect_filled(gfc_rect(10,100,300,32),gfc_color8(60,60,60,255));
                char buffer[50];
                int max_len = sizeof buffer;
                snprintf(buffer, max_len, "Health: %d / %d", plr->currHealth, plr->maxHealth);
                gf2d_font_draw_line_tag(buffer,FT_H1,gfc_color(1,1,1,1), vector2d(10,100));
                
                
                gf2d_draw_rect_filled(gfc_rect(10,125,800,32),gfc_color8(60,60,60,255));
                char buffer_velocity[100];
                int max_len_velocity = sizeof buffer_velocity;
                PlayerData *playData;
                playData = plr->customData;
                snprintf(buffer_velocity, max_len_velocity, "X Rot %f, Y Rot %f, Z Rot %f",
                    plr->rotation.x, plr->rotation.y, plr->rotation.z);
                gf2d_font_draw_line_tag(buffer_velocity,FT_H1,gfc_color(1,1,1,1), vector2d(10,125));

                gf2d_draw_rect_filled(gfc_rect(10,150,400,32),gfc_color8(60,60,60,255));
                char buffer_speed[50];
                int max_len_speed = sizeof buffer_speed;
                snprintf(buffer_speed, max_len_speed, "Speed %f ",
                    playData->speedMult);
                gf2d_font_draw_line_tag(buffer_speed,FT_H1,gfc_color(1,1,1,1), vector2d(10,150));

                gf2d_draw_rect_filled(gfc_rect(10, 175, 300,32),gfc_color8(60,60,60,255));
                char buffer_mana[50];
                int max_len_mana = sizeof buffer_mana;
                snprintf(buffer_mana, max_len_mana, "Mana %d / %d",
                    playData->currMana, playData->maxMana);
                gf2d_font_draw_line_tag(buffer_mana,FT_H1,gfc_color(1,1,1,1), vector2d(10,175));

                gf2d_draw_rect_filled(gfc_rect(10, 200, 500,32),gfc_color8(60,60,60,255));
                char buffer_position[50];
                int max_len_position = sizeof buffer_position;
                Vector3D positionVect;
                positionVect = get_player_position();
                snprintf(buffer_position, max_len_position, "My Position: X: %f Y: %f Z: %f",
                positionVect.x, positionVect.y, positionVect.z);
                gf2d_font_draw_line_tag(buffer_position,FT_H1,gfc_color(1,1,1,1), vector2d(10,200));

                if(plr->isBlocking == 1)
                {
                    gf2d_sprite_draw(blockImage, vector2d(800, 300), vector2d(2,2), vector3d(8, 8, 0), gfc_color(1, 1, 1, 0.9), (Uint32)1);    
                }

                if(plr->inStats == 1)
                {
                    gf2d_draw_rect_filled(gfc_rect(600, 10, 500, 500),gfc_color8(60,60,60,255));
                    char buffer_stat[50];
                    int max_len_stat = sizeof buffer_stat;
                    snprintf(buffer_stat, max_len_stat, "Defense: X: %f",playData->defense);

                    char buffer_stat2[50];
                    int max_len_stat2 = sizeof buffer_stat2;
                    snprintf(buffer_stat2, max_len_stat2, "Magic Mult: %f", playData->magicMult);

                    char buffer_stat3[50];
                    int max_len_stat3 = sizeof buffer_stat3;
                    snprintf(buffer_stat3, max_len_stat3, "Phys Mult: %f", playData->physicalMult);

                    char buffer_stat4[50];
                    int max_len_stat4 = sizeof buffer_stat4;
                    snprintf(buffer_stat4, max_len_stat4, "Arrow Mult: %f", playData->arrowMult);

                    char buffer_stat5[50];
                    int max_len_stat5 = sizeof buffer_stat5;
                    snprintf(buffer_stat5, max_len_stat5, "Arrow Count: %d / %d", playData->currArrow, playData->maxArrow);

                    gf2d_font_draw_line_tag(buffer_stat,FT_H1,gfc_color(1,1,1,1), vector2d(600,10));
                    gf2d_font_draw_line_tag(buffer_stat2,FT_H1,gfc_color(1,1,1,1), vector2d(600,30));
                    gf2d_font_draw_line_tag(buffer_stat3,FT_H1,gfc_color(1,1,1,1), vector2d(600,50));
                    gf2d_font_draw_line_tag(buffer_stat4,FT_H1,gfc_color(1,1,1,1), vector2d(600,70));
                    gf2d_font_draw_line_tag(buffer_stat5,FT_H1,gfc_color(1,1,1,1), vector2d(600,90));
                }

                gf2d_sprite_draw(playerFace, vector2d(1000, 10), vector2d(2,2), vector3d(8, 8, 0), gfc_color(1, 1, 1, 0.9), (Uint32)1);

                gf2d_sprite_draw(mouse,vector2d(mousex,mousey),vector2d(2,2),vector3d(8,8,0),gfc_color(0.3,.9,1,0.9),(Uint32)mouseFrame);
        gf3d_vgraphics_render_end();

        if (gfc_input_command_down("exit"))done = 1; // exit condition
    }    
    
    world_delete(w);
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
