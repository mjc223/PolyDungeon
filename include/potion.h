#ifndef __POTION_H_
#define __POTION_H__

#include "entity.h"

/**
 * @brief Create a new agumon entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an agumon entity pointer on success
 */
Entity *potion_new(Vector3D position);


#endif
