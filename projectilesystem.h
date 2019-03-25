/** -----------------------------------------------------------------------------
  * FILE:    projectilesystem.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <vector>
#include "projectile.h"

#include "entity.h"
#include "realcomponent.h"

namespace pttoth{

class ProjectileSystem: public engine::Entity{

    std::vector<Projectile> _projectiles;

public:
    ProjectileSystem();
    ProjectileSystem(const ProjectileSystem& other);
    ProjectileSystem(ProjectileSystem&& other);
    virtual ~ProjectileSystem();
    ProjectileSystem& operator=(const ProjectileSystem &other);
    ProjectileSystem& operator=(ProjectileSystem &&other);
    bool operator==(const ProjectileSystem &other)const;

    void spawnProjectile(math::float2& pos,
                         math::float2& dir);

private:
    int _findAvailableProjectileInstance();
    void _hitdetection();       //

    // Entity interface
protected:
    void tick(float t, float dt) override;

public:
    void OnRegister() override;
    void OnUnregister() override;
};

}

