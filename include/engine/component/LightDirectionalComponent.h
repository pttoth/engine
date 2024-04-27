#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( LightDirectionalComponent )


class LightDirectionalComponent
{
public:
    LightDirectionalComponent();
    //LightDirectionalComponent( const std::string& name );

    LightDirectionalComponent( const LightDirectionalComponent& other );
    LightDirectionalComponent( LightDirectionalComponent&& source );
    virtual ~LightDirectionalComponent();
    LightDirectionalComponent& operator=( const LightDirectionalComponent& other );
    LightDirectionalComponent& operator=( LightDirectionalComponent&& source );

    bool operator==( const LightDirectionalComponent& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
