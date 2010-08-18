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

#include <Passion/Scripting/ScriptState.hpp>
#include <string.h>
#include <iostream>

namespace Passion
{
	ScriptState::ScriptState()
	{
		m_lua = lua_open();
		luaL_openlibs( m_lua );

		m_error = 0;
		SetError( "" );
	}

	ScriptState::~ScriptState()
	{
		delete [] m_error;
		lua_close( m_lua );
	}

	bool ScriptState::DoString( const char* code )
	{
		int err = luaL_loadstring( m_lua, code ) || lua_pcall( m_lua, 0, -1, 0 );

		if ( err )
		{
			SetError( lua_tostring( m_lua, -1 ) );
			lua_pop( m_lua, 1 );
		} else
			SetError();

		return err == 0;
	}

	bool ScriptState::DoFile( const char* path )
	{
		int err = luaL_loadfile( m_lua, path ) || lua_pcall( m_lua, 0, 0, 0 );

		if ( err )
		{
			SetError( lua_tostring( m_lua, -1 ) );
			lua_pop( m_lua, 1 );
		} else
			SetError();

		return err == 0;
	}

	std::auto_ptr<BaseScriptValue> ScriptState::NewTable()
	{
		lua_newtable( m_lua );

		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, 0 ) );
	}

	std::auto_ptr<BaseScriptValue> ScriptState::UserData( void* data, std::auto_ptr<BaseScriptValue> metatable )
	{
		Push( &data, sizeof( void* ), metatable );

		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, 0 ) );
	}

	std::auto_ptr<BaseScriptValue> ScriptState::Globals()
	{
		lua_pushvalue( m_lua, LUA_GLOBALSINDEX );

		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		lua_pushstring( m_lua, "_G" );
		int key = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, key ) );
	}

	std::auto_ptr<BaseScriptValue> ScriptState::Registry()
	{
		lua_pushvalue( m_lua, LUA_REGISTRYINDEX );

		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, 0 ) );
	}

	void ScriptState::Push( BaseScriptValue* value ) { value->Push(); }
	void ScriptState::Push( std::auto_ptr<BaseScriptValue> value ) { value->Push(); }
	void ScriptState::Push( const char* value ) { lua_pushstring( m_lua, value ); }
	void ScriptState::Push( const char* value, unsigned int length ) { lua_pushlstring( m_lua, value, length ); }
	void ScriptState::Push( bool value ) { lua_pushboolean( m_lua, value ); }
	void ScriptState::Push( double value ) { lua_pushnumber( m_lua, value ); }
	void ScriptState::Push( float value ) { lua_pushnumber( m_lua, value ); }
	void ScriptState::Push( int value ) { lua_pushinteger( m_lua, value ); }

	void ScriptState::Push( void* data, size_t length, std::auto_ptr<BaseScriptValue> metatable )
	{
		void* block = lua_newuserdata( m_lua, length );
		memcpy( block, data, length );

		metatable->Push();
		lua_setmetatable( m_lua, -2 );
	}

	void ScriptState::Pop( int values ) { lua_pop( m_lua, values ); }

	std::auto_ptr<BaseScriptValue> ScriptState::Get( int index )
	{
		lua_pushvalue( m_lua, index );
		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, 0 ) );
	}

	int ScriptState::Top() { return lua_gettop( m_lua ); }

	bool ScriptState::Call( int args, int returns )
	{
		int err = lua_pcall( m_lua, args, returns, 0 );

		if ( err != 0 )
		{
			SetError( lua_tostring( m_lua, -1 ) );
			lua_pop( m_lua, 1 );
		}
		else
			SetError();

		return err == 0;
	}

	const char* ScriptState::Error()
	{
		return m_error;
	}

	void ScriptState::Error( const char* error )
	{
		Push( error );
		lua_error( m_lua );
	}

	void ScriptState::Error( int arg, const char* expectedType )
	{
		luaL_error( m_lua, "bad argument #%d (%s expected, got %s)", arg, expectedType, lua_typename( m_lua, lua_type( m_lua, arg ) ) );
	}

	void ScriptState::SetError( const char* error )
	{
		if ( error )
		{
			unsigned int l = strlen( error );
		
			if ( m_error ) delete [] m_error;
			m_error = new char[l+1];

			strcpy( m_error, error );
		}
	}
}
