#ifndef __HUD_H__
#define __HUD_H__


#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"

#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_draw.h"

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_texture.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_draw.h"

void hud_init();

void hud_update_all();

void hud_update_main();

void hud_update_message(char *msg);

#endif