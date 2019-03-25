#include "projectilesystem.h"

#include "pttoth/std_extension.hpp"

using namespace pttoth;

ProjectileSystem::
        ProjectileSystem(){
    _projectiles.reserve(256);
}

ProjectileSystem::
        ProjectileSystem(const ProjectileSystem &other){
}

ProjectileSystem::
        ProjectileSystem(ProjectileSystem &&other){
}

ProjectileSystem::
        ~ProjectileSystem(){
}

ProjectileSystem &ProjectileSystem::
        operator=(const ProjectileSystem &other){
}

ProjectileSystem &ProjectileSystem::
        operator=(ProjectileSystem &&other){
}

bool ProjectileSystem::
operator==(const ProjectileSystem &other) const{
}

void ProjectileSystem::
        spawnProjectile(math::float2 &pos,
                        math::float2 &dir){
    Projectile proj(pos, dir);
    proj.is_active = true;

    int idx = _findAvailableProjectileInstance();
    if(idx < 0){
        _projectiles.push_back( proj );
    }else{
        _projectiles[idx] = proj;
    }
}

int ProjectileSystem::
        _findAvailableProjectileInstance(){
    return -1;
}

void ProjectileSystem::_hitdetection()
{

}

void ProjectileSystem::
        tick(float t, float dt){
    for(Projectile& p : _projectiles){
        if(p.is_active){
            p.pos = p.pos + p.dir * dt;
        }
        _hitdetection();
    }

}

void ProjectileSystem::
        OnRegister(){
}

void ProjectileSystem::
        OnUnregister(){
}
