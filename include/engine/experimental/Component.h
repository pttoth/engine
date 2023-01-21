#pragma once





class Component
{
public:
    Component();
    Component( const Component& other );
    Component( Component&& source );

    virtual ~Component();

    Component& operator=( const Component& other );
    Component& operator=( Component&& source );

    bool operator==( const Component& other ) const;
protected:
private:
};


