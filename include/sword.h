#ifndef __SWORD_H_
#define __SWORD_H__

#include "entity.h"

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *sword_new(Vector3D position);


#endif
