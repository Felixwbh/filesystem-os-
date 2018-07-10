#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include "defin.h"
#include "cstring"

using namespace std;

struct user
{
    char username[32];
    char password[32];
    bool valid;
    user()
    {
        memset((char*)this->username, 0, sizeof(user));
    }
    user(const user &u)
    {
        memcpy((char*)this->username, (char*)u.username, sizeof(user));
    }
};


#endif // USER_H_INCLUDED
