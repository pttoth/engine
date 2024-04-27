#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( LightAmbient )


class LightAmbient
{
public:
    LightAmbient();
    //LightAmbient( const std::string& name );

    LightAmbient( const LightAmbient& other );
    LightAmbient( LightAmbient&& source );
    virtual ~LightAmbient();
    LightAmbient& operator=( const LightAmbient& other );
    LightAmbient& operator=( LightAmbient&& source );

    bool operator==( const LightAmbient& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
