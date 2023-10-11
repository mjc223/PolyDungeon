#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "collider.h"



Collider *collider_sphere_new(Vector3D newVect, Sphere newS, int isTrigger)
{
    Collider *col;
    
    col->s = newS;

    col->s.x = newVect.x;
    col->s.y = newVect.y;
    col->s.z = newVect.z;

    col->isTrigger = isTrigger;

    return col;
}

Collider *collider_box_new(Vector3D newVect, Box newB, int isTrigger)
{
    Collider *col;
    
    col->b = newB;

    col->b.x = newVect.x;
    col->b.y = newVect.y;
    col->b.z = newVect.z;

    col->isTrigger = isTrigger;

    return col;
}

Collider *collider_plane_new(Vector3D newVect, Plane3D newP, int isTrigger)
{
    Collider *col;
    
    col->p = newP;

    col->p.x = newVect.x;
    col->p.y = newVect.y;
    col->p.z = newVect.z;

    col->isTrigger = isTrigger;

    return col;
}
