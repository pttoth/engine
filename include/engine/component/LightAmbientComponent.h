#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( LightAmbientComponent )


class LightAmbientComponent
{
public:
    LightAmbientComponent();
    //LightAmbientComponent( const std::string& name );

    LightAmbientComponent( const LightAmbientComponent& other );
    LightAmbientComponent( LightAmbientComponent&& source );
    virtual ~LightAmbientComponent();
    LightAmbientComponent& operator=( const LightAmbientComponent& other );
    LightAmbientComponent& operator=( LightAmbientComponent&& source );

    bool operator==( const LightAmbientComponent& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
