#pragma once

//TODO: probably move this class to ptlib



class Transform
{
public:
    Transform();
    Transform( const Transform& other );
    Transform( Transform&& source );

    virtual ~Transform();

    Transform& operator=( const Transform& other );
    Transform& operator=( Transform&& source );

    bool operator==( const Transform& other ) const;
protected:
private:
};


