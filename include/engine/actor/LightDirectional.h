#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( LightDirectional )


class LightDirectional
{
public:
    LightDirectional();
    //LightDirectional( const std::string& name );

    LightDirectional( const LightDirectional& other );
    LightDirectional( LightDirectional&& source );
    virtual ~LightDirectional();
    LightDirectional& operator=( const LightDirectional& other );
    LightDirectional& operator=( LightDirectional&& source );

    bool operator==( const LightDirectional& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
