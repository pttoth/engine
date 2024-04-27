#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( Light )


class Light
{
public:
    Light();
    //Light( const std::string& name );

    Light( const Light& other );
    Light( Light&& source );
    virtual ~Light();
    Light& operator=( const Light& other );
    Light& operator=( Light&& source );

    bool operator==( const Light& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
