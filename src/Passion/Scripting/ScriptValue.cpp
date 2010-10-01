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
#include <iostream>

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
		if ( m_ref > 0 ) luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		if ( m_ref != m_tbl ) luaL_unref( m_lua, LUA_REGISTRYINDEX, m_tbl );
		if ( m_key > 0 ) luaL_unref( m_lua, LUA_REGISTRYINDEX, m_key );
	}

	bool ScriptValue::IsNumber()
	{
		bool t;
		Push();
			t = lua_isnumber( m_lua, -1 ) == 1;
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsBoolean()
	{
		bool t;
		Push();
			t = lua_isboolean( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsNil()
	{
		bool t;
		Push();
			t = lua_isnil( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsTable()
	{
		bool t;
		Push();
			t = lua_istable( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsString()
	{
		bool t;
		Push();
			t = lua_isstring( m_lua, -1 ) == 1;
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsFunction()
	{
		bool t;
		Push();
			t = lua_isfunction( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return t;
	}

	bool ScriptValue::IsUserData()
	{
		bool t;
		Push();
			t = lua_isuserdata( m_lua, -1 ) == 1;
		lua_pop( m_lua, 1 );
		return t;
	}

	int ScriptValue::GetInteger()
	{
		int val;
		Push();
		val = lua_tointeger( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return val;
	}

	float ScriptValue::GetFloat()
	{
		float val;
		Push();
		val = static_cast<float>( lua_tonumber( m_lua, -1 ) );
		lua_pop( m_lua, 1 );
		return val;
	}

	double ScriptValue::GetDouble()
	{
		double val;
		Push();
		val = lua_tonumber( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return val;
	}

	bool ScriptValue::GetBoolean()
	{
		bool val;
		Push();
		val = lua_toboolean( m_lua, -1 ) == 1;
		lua_pop( m_lua, 1 );
		return val;
	}

	const char* ScriptValue::GetString()
	{
		const char* val;
		Push();
		val = lua_tostring( m_lua, -1 );
		lua_pop( m_lua, 1 );
		return val;
	}

	const char* ScriptValue::GetString( size_t& length )
	{
		const char* val;
		Push();
		val = lua_tolstring( m_lua, -1, &length );
		lua_pop( m_lua, 1 );
		return val;
	}

	void* ScriptValue::GetUserData()
	{
		void* val;
		Push();
		val = (void*)lua_touserdata( m_lua, -1 );
		lua_pop( m_lua, 1 );
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

		lua_pop( m_lua, 1 );

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

		lua_pop( m_lua, 1 );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, selfcopy, keyref ) );
	}

	void ScriptValue::Set( int value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushinteger( m_lua, value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushinteger( m_lua, value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( float value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushnumber( m_lua, value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushnumber( m_lua, value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( double value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushnumber( m_lua, value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushnumber( m_lua, value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( bool value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushboolean( m_lua, value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushboolean( m_lua, value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( const char* value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushstring( m_lua, value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushstring( m_lua, value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( std::auto_ptr<BaseScriptValue> value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		value->Push();
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		value->Push();
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( BaseScriptValue* value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		value->Push();
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		value->Push();
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::Set( ScriptFunction value )
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_tbl );
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_key );
		lua_pushcfunction( m_lua, (lua_CFunction)value );
		lua_settable( m_lua, -3 );
		lua_pop( m_lua, 1 );

		luaL_unref( m_lua, LUA_REGISTRYINDEX, m_ref );
		lua_pushcfunction( m_lua, (lua_CFunction)value );
		m_ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
	}

	void ScriptValue::SetMetaTable( std::auto_ptr<BaseScriptValue> metatable )
	{
		Push();
		metatable->Push();
		lua_setmetatable( m_lua, -2 );
		lua_pop( m_lua, 1 );
	}
	
	std::auto_ptr<BaseScriptValue> ScriptValue::GetMetaTable()
	{
		Push();
		lua_getmetatable( m_lua, -1 );
		
		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, 0 ) );
	}

	bool ScriptValue::Equals( std::auto_ptr<BaseScriptValue> value )
	{
		Push();
		value->Push();
		bool eq = lua_equal( m_lua, -1, -2 ) == 1;
		lua_pop( m_lua, 2 );
		return eq;
	}

	void ScriptValue::Push()
	{
		lua_rawgeti( m_lua, LUA_REGISTRYINDEX, m_ref );
	}
}