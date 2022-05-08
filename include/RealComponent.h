/** -----------------------------------------------------------------------------
  * FILE:    RealComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Components, some physical representation (for visuals or physics)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "WorldComponent.h"

namespace engine{


class RealComponent: public WorldComponent{
    friend class Component;
    static void RegisterRealComponentParts(RealComponent* component);
    static void UnregisterRealComponentParts(RealComponent* component);
public:
    RealComponent(const std::string& name);
    RealComponent(const RealComponent& other);
    RealComponent(RealComponent&& other) = delete;
    virtual ~RealComponent();
    RealComponent& operator=(const RealComponent &other) = delete;
    RealComponent& operator=(RealComponent &&other) = delete;
    bool operator==(const RealComponent &other);


    /**
     * @brief GetVertices: Returns the vertices in counter-clockwise order.
     */
    virtual std::vector<pt::math::float3> GetVertices() = 0;

};


}

