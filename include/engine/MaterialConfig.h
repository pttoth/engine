#pragma once



#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( MaterialConfig )


class MaterialConfig
{
public:
    MaterialConfig();
    //MaterialConfig( const std::string& name );

    MaterialConfig( const MaterialConfig& other );
    MaterialConfig( MaterialConfig&& source );
    virtual ~MaterialConfig();
    MaterialConfig& operator=( const MaterialConfig& other );
    MaterialConfig& operator=( MaterialConfig&& source );

    bool operator==( const MaterialConfig& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
