#ifndef __STATION_FACILITY_H__
#define __STATION_FACILITY_H__

#include "simple_json.h"

#include "gfc_text.h"
#include "gfc_list.h"

typedef struct
{
    TextLine    name;  //its name identifier
    TextLine    facilityType;
    float       damage;   //keeps track of damage.  Damaged facilities lave lower output  0 is no damage, 100 is destroyed anything else can be repaired.
    int         housing;        //how much housing is provided by this facility
    int         staffRequired;  //how many people are needed to run the facility
    int         staffAssigned;     //how many people are actually hired to do so
    int         energyDraw;
    int         energyOutput;
    int         inactive;//if the facility cannot run
    int         disabled;//if the player has chosen to turn it off
    int         storage;// how many commodities can be stored here
    TextLine    officer;//if there is an officer assigned to help run this facility
    List        *upkeep;    //what resources are needed to keep this running every update
    List        *produces;  //what this produces every update cycle
}StationFacility;

/**
 * @brief allocate and initialize a new station facility
 * @return NULL on error or the station facility
 */
StationFacility *station_facility_new();

/**
 * @brief create a new default facility based on its definition name.
 * @param name the name of the facility in question
 * @return NULL if not found or other error, the newly created facility otherwise
 */
StationFacility *station_facility_new_by_name(const char *name);

/**
 * @brief load a station facility based on config
 * @param config the config containing the data
 * @return NULL on error or the facility loaded
 */
StationFacility *station_facility_load(SJson *config);

/**
 * @brief free from memory a station facility
 * @param facility the facility to free
 */
void station_facility_free(StationFacility *facility);

/**
 * @brief save a station facility to config
 * @param facility the facility to write to config
 * @return NULL on error a json object containing facility data
 */
SJson *station_facility_save(StationFacility *facility);

/**
 * @brief free a list of facilities
 * @param list a list containing pointers to facilities
 */
void station_facility_free_list(List *list);

/**
 * @brief get the display name, given a facilities name
 * @param name the name id
 * @return NULL if not found, the name otherwise
 */
const char *station_facility_get_display_name(const char *name);

const char *station_facility_get_name_from_display(const char *display);

/**
 * @brief run the upkeep on the facility
 */
void station_facility_update(StationFacility *facility);

List *station_facility_get_resource_cost(const char *name,const char *resource_type);

#endif
