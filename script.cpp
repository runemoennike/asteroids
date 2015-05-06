#include <vector>
#include <string>
#include "script.h"
#include "settings.h"
#include "debugout.h"
#include "commons.h"
#include "math.h"

namespace scrpt
{

using std::string;

lua_State* luaVMs[__VM_COUNT + 1];

bool initVM(VMS vm, string scriptfile)
{
    luaVMs[vm] = lua_open();

    if(luaVMs[vm] == NULL)
    {
        return false;
    }

    lua_register(luaVMs[vm], "print", l_debugout);
    lua_register(luaVMs[vm], "getRandomInt", l_getRandomInt);

    string filename = OPT_PATH_SCRIPTS + scriptfile + OPT_PATHEXT_SCRIPTS;
    luaL_dofile(luaVMs[vm], filename.c_str());

    string initstr = "init(" + intToStr(vm) + ")";
    luaL_dostring(luaVMs[vm], initstr.c_str());

    return true;
}

void deinitVM(VMS vm)
{
    if(luaVMs[vm] != NULL)
    {
        debugoutput("Script: Deiniting VM:" + intToStr(vm));
        lua_close(luaVMs[vm]);
    }
}

int l_debugout(lua_State * luaVM)
{
    string msg = "Script(VM:" + (string)lua_tostring(luaVM, -2) + "): " + lua_tostring(luaVM, -1);
    debugoutput(msg);
    lua_pushnumber(luaVM, 0);
    return 1;
}

int l_getRandomInt(lua_State * luaVM)
{
    lua_pushnumber(luaVM, getRndInt(lua_tointeger(luaVM, -2), lua_tointeger(luaVM, -1)));
    return 1;
}

} /* namespace */
