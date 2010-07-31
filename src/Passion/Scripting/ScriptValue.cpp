////////////////////////////////////////////////////////////
//
//  Copyright (C) 2010 Alexander Overvoorde (overv161@gmail.com)
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Scripting/ScriptValue.hpp>

namespace Passion
{
	ScriptValue::ScriptValue( lua_State* state, int reference, int table, int key )
	{
		m_lua = state;
		m_ref = reference;
		m_tbl = table;
		m_key = key;
	}

	ScriptValue::ScriptValue( const ScriptValue& val )
	{
		m_lua = val.m_lua;

		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, val.m_ref );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, val.m_tbl );
		m_tbl = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, val.m_key );
		m_key = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	ScriptValue::~ScriptValue()
	{
		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		if ( m_ref != m_tbl ) luaL_unref( m_lua, LUA_REGISTRYINDEX, m_tbl );
		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_key );
	}

	bool ScriptValue::IsNumber()
	{
		bool t;
		Push();
			t = lua_isnumber( m_lua, -1 ) == 1;
		lua_pop( m_lua, -1 );
		return t;
	}

	bool ScriptValue::IsBoolean()
	{
		bool t;
		Push();
			t = lua_isboolean( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return t;
	}

	bool ScriptValue::IsNil()
	{
		bool t;
		Push();
			t = lua_isnil( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return t;
	}

	bool ScriptValue::IsTable()
	{
		bool t;
		Push();
			t = lua_istable( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return t;
	}

	bool ScriptValue::IsString()
	{
		bool t;
		Push();
			t = lua_isstring( m_lua, -1 ) == 1;
		lua_pop( m_lua, -1 );
		return t;
	}

	bool ScriptValue::IsFunction()
	{
		bool t;
		Push();
			t = lua_isfunction( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return t;
	}

	ScriptValue::operator int()
	{
		int val;
		Push();
		val = lua_tointeger( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return val;
	}

	ScriptValue::operator float()
	{
		float val;
		Push();
		val = static_cast<float>( lua_tonumber( m_lua, -1 ) );
		lua_pop( m_lua, -1 );
		return val;
	}

	ScriptValue::operator double()
	{
		double val;
		Push();
		val = lua_tonumber( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return val;
	}

	ScriptValue::operator bool()
	{
		bool val;
		Push();
		val = lua_toboolean( m_lua, -1 ) == 1;
		lua_pop( m_lua, -1 );
		return val;
	}

	ScriptValue::operator const char*()
	{
		const char* val;
		Push();
		val = lua_tostring( m_lua, -1 );
		lua_pop( m_lua, -1 );
		return val;
	}

	std::auto_ptr<BaseScriptValue> ScriptValue::GetMember( const char* key )
	{
		Push();
		int selfcopy = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		lua_pushstring( m_lua, key );
		int keyref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		Push();
		lua_pushstring( m_lua, key );
		lua_gettable( m_lua, -2 );
		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		lua_pop( m_lua, 1 );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, selfcopy, keyref ) );
	}

	std::auto_ptr<BaseScriptValue> ScriptValue::GetMember( int key )
	{
		
		Push();
		int selfcopy = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		lua_pushinteger( m_lua, key );
		int keyref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		Push();
		lua_pushinteger( m_lua, key );
		lua_gettable( m_lua, -2 );
		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		lua_pop( m_lua, -1 );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, selfcopy, keyref ) );
	}

	std::auto_ptr<BaseScriptValue> ScriptValue::GetMember( BaseScriptValue* key )
	{
		Push();
		int selfcopy = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		key->Push();
		int keyref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		Push();
		key->Push();
		lua_gettable( m_lua, -2 );
		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		lua_pop( m_lua, -1 );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, selfcopy, keyref ) );
	}

	void ScriptValue::operator= ( int val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushinteger( m_lua, val );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushinteger( m_lua, val );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::operator= ( float val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushnumber( m_lua, val );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushnumber( m_lua, val );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::operator= ( double val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushnumber( m_lua, val );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushnumber( m_lua, val );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::operator= ( bool val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushboolean( m_lua, val );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushboolean( m_lua, val );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::operator= ( const char* val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushstring( m_lua, val );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushstring( m_lua, val );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::operator= ( BaseScriptValue* val )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		val->Push();
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, -1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		val->Push();
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Push()
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_ref );
	}
}