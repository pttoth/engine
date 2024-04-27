#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( LightComponent )


class LightComponent
{
public:
    LightComponent();
    //LightComponent( const std::string& name );

    LightComponent( const LightComponent& other );
    LightComponent( LightComponent&& source );
    virtual ~LightComponent();
    LightComponent& operator=( const LightComponent& other );
    LightComponent& operator=( LightComponent&& source );

    bool operator==( const LightComponent& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
