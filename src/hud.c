
#include "player.h"
#include "hud.h"

//Retrieve Player Data, Draw relevant Player Data
char *currMessage;
void hud_init()
{
    //gf2d_sprite_draw_image(bomb,vector2d(600,800));
}

void hud_update_all()
{
    hud_update_main(currMessage);
}

void hud_update_main()
{
    //gf2d_draw_rect_filled(gfc_rect(60,60,1000,32),gfc_color8(128,128,128,255));
    //gf2d_font_draw_line_tag(currMessage,FT_H1,gfc_color(1,1,1,1), vector2d(10,10));
}

void hud_update_message(char *msg)
{
    //currMessage = &msg;
    
}