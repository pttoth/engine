/** -----------------------------------------------------------------------------
  * FILE:    projectile.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "math/float2.h"

namespace pttoth{

struct Projectile{
    bool is_active;
    math::float2 pos, dir;

public:
    Projectile(){
        is_active = false;
    }
    Projectile(const Projectile& other);
    Projectile(Projectile&& other);
    virtual ~Projectile();
    Projectile& operator=(const Projectile &other);
    Projectile& operator=(Projectile &&other);
    bool operator==(const Projectile &other)const;

};


}

