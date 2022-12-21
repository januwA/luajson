extern "C" {
#include "lua.h"
}
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#ifdef WIN32
#define EXPORTED __declspec(dllexport)
#else
#define EXPORTED
#endif

#ifdef WIN32
#define CALL __stdcall
#else
#define CALL
#endif

void lua_pushjson(lua_State* L, const json& data) {
  switch (data.type()) {
    case json::value_t::null:
      lua_pushnil(L);
      break;
    case json::value_t::object: {
      lua_newtable(L);
      size_t i = 0;
      for (auto& [key, value] : data.items()) {
        lua_pushstring(L, key.c_str());
        lua_pushjson(L, value);
        i++;
        lua_settable(L, -3);
      }
      break;
    }
    case json::value_t::array: {
      lua_newtable(L);
      size_t i = 0;
      for (auto& element : data) {
        lua_pushinteger(L, i);
        lua_pushjson(L, element);
        i++;
        lua_settable(L, -3);
      }
      break;
    }
    case json::value_t::string:
      lua_pushstring(L, data.get<std::string>().c_str());
      break;
    case json::value_t::boolean:
      lua_pushboolean(L, data.get<bool>());
      break;
    case json::value_t::number_integer:
      lua_pushinteger(L, data.get<int>());
      break;
    case json::value_t::number_unsigned:
      lua_pushnumber(L, data.get<unsigned int>());
      break;
    case json::value_t::number_float:
      lua_pushnumber(L, data.get<double>());
      break;
    default:
      break;
  }
}

json lua_tojson(lua_State* L, int idx) {
  switch (lua_type(L, idx)) {
    case LUA_TNIL:
      return nullptr;

    case LUA_TNUMBER: {
      if (lua_isinteger(L, idx))
        return lua_tointeger(L, idx);
      else
        return lua_tonumber(L, idx);
    }

    case LUA_TBOOLEAN:
      return (bool)lua_toboolean(L, idx);

    case LUA_TSTRING:
      return lua_tostring(L, idx);

    case LUA_TTABLE: {
      bool isarr = true;
      lua_gettable(L, idx);

      lua_pushnil(L);
      while (lua_next(L, idx - 1) != 0) {
        isarr = lua_isinteger(L, -2);
        lua_pop(L, 1);
        break;
      }
      lua_pop(L, 1);

      json table;
      if (isarr) {
        table = json::array();
        lua_pushnil(L);
        while (lua_next(L, idx - 1) != 0) {
          table.push_back(lua_tojson(L, -1));
          lua_pop(L, 1);
        }
      } else {
        table = json::object();
        lua_pushnil(L);
        while (lua_next(L, idx - 1) != 0) {
          auto k = lua_tostring(L, -2);
          auto v = lua_tojson(L, -1);
          table.emplace(k, v);
          lua_pop(L, 1);
        }
      }
      return table;
    }

    case LUA_TFUNCTION:
    case LUA_TUSERDATA:
    case LUA_TTHREAD:
    case LUA_TLIGHTUSERDATA:
      return nullptr;
  }
  return nullptr;
}

extern "C" {

int json_parse(lua_State* L) {
  auto stackSize = lua_gettop(L);
  if (stackSize == 0 || stackSize > 1) return 0;

  if (!lua_isstring(L, -1)) return 0;

  json data = json::parse(lua_tostring(L, -1));
  lua_pushjson(L, data);

  return 1;
}

int lua_stringify(lua_State* L) {
  auto stackSize = lua_gettop(L);

  if (stackSize == 0 || stackSize > 2) return 0;

  int indent = -1;
  if (stackSize == 2 && lua_isinteger(L, -1)) {
    indent = (int)lua_tointeger(L, -1);
  }

  json j = lua_tojson(L, -stackSize);

  lua_pushstring(L, j.dump(indent).c_str());

  return 1;
}

EXPORTED LUALIB_API int CALL luaopen_luajson(lua_State* L) {
  lua_newtable(L);

  lua_pushstring(L, "parse");
  lua_pushcclosure(L, json_parse, 0);
  lua_settable(L, -3);

  lua_pushstring(L, "stringify");
  lua_pushcclosure(L, lua_stringify, 0);
  lua_settable(L, -3);

  return 1;
}
}