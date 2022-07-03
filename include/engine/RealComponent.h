/** -----------------------------------------------------------------------------
  * FILE:    RealComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component with physical representation (visual or physical)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "WorldComponent.h"

namespace engine{


class RealComponent: public WorldComponent{
    friend class Component;
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

