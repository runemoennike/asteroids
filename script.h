#ifndef __AI_H
#define __AI_H

#include <string>
#include <lua.hpp>

namespace scrpt
{

using std::string;

enum VMS {
    VM_LEVEL, VM_ENEMY1, VM_ENEMY2, __VM_COUNT
};

bool initVM(VMS vm, string scriptfile);
void deinitVM(VMS vm);

int l_debugout(lua_State * luaVM);
int l_getRandomInt(lua_State * luaVM);

}

#endif /* ifndef __AI_H */
