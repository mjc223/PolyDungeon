#ifndef __COLLIDER_H
#define __COLLIDER_H__

#include "gfc_types.h"
#include "gfc_color.h"
#include "gfc_primitives.h"
#include "gfc_shape.h"

#include "gf3d_model.h"


typedef struct Collider
{
    int _inuse;

    int isTrigger;
    
    Sphere s;
    Box b;
    Plane3D p;

}Collider;

/**
 * @brief provide a pointer to a new empty sphere collider
 * @param vec used to provide positional data to collider, no auto update
 * @param s sphere collider parametere
 * @param isTrigger used to determine if this should be wall
 * @return NULL on error or a valid collider pointer otherwise
 */
Collider *collider_sphere_new(Vector3D vect, Sphere s, int isTrigger);

/**
 * @brief provide a pointer to a new empty sphere collider
 * @param vec used to provide positional data to collider, no auto update
 * @param b cube collider parametere
 * @param isTrigger used to determine if this should be wall
 * @return NULL on error or a valid collider pointer otherwise
 */
Collider *collider_box__new(Vector3D vect, Box b, int isTrigger);

/**
 * @brief provide a pointer to a new empty sphere collider
 * @param vec used to provide positional data to collider, no auto update
 * @param p plane collider parametere
 * @param isTrigger used to determine if this should be wall
 * @return NULL on error or a valid collider pointer otherwise
 */
Collider *collider_plane_new(Vector3D vect, Plane3D p, int isTrigger);

#endif
