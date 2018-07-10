#include <cstring>
#include "usersystem.h"

using namespace std;

bool getusername(int uid, char* username)
{
    if(uid == 0)
        strcpy(username, "root");
    return true;
}
