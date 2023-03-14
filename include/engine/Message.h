#pragma once


//TODO: remove this


class Message
{
public:
    Message();
    Message( const Message& other );
    Message( Message&& source );

    virtual ~Message();

    Message& operator=( const Message& other );
    Message& operator=( Message&& source );

    bool operator==( const Message& other ) const;
protected:
private:
};


