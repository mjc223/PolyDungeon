#include "simple_logger.h"

#include "gfc_list.h"
#include "gfc_input.h"
#include "gfc_callbacks.h"

#include "gf3d_camera.h"

#include "config_def.h"
#include "player.h"
#include "player_history.h"
#include "event_menu.h"
#include "event_manager.h"

static Window *event_window = {0};

void event_manager_handle_choice(SJson *event, int choice)
{
    
}

void event_manager_event_end()
{
    event_window = NULL;
}

void event_manager_update()
{
    SJson *event,*history;
    Bool once = 0;
    const char *name;
    int startDate,endDate;
    int day;
    int i,c;
    day = player_get_day();
    history = player_get_history();
    if (event_window)return;
    if (!history)return;
    c = config_def_get_resource_count("events");
    for (i = 0; i < c; i++)
    {
        event = config_def_get_by_index("events",i);
        if (!event)continue;
        name = sj_object_get_value_as_string(event,"name");
        if (!name)continue;
        if (sj_object_get_value_as_bool(event,"once",&once) && once)
        {
            if (player_history_get_event("event",name) != NULL)continue;
        }
        
        // other tests
        
        if ((sj_object_get_value_as_int(event,"startDate",&startDate))&&
            (sj_object_get_value_as_int(event,"endDate",&endDate)))
        {
            if ((day >= startDate)&&(day <= endDate))
            {
                event_window = event_menu(NULL,name);
                return;// only one event can happen at a time
            }
        }
    }
}


/*eol@eof*/
