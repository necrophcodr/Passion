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

	std::auto_ptr<BaseScriptValue> ScriptState::Globals()
	{
		lua_getglobal( m_lua, "_G" );

		int ref = luaL_ref( m_lua, LUA_REGISTRYINDEX );
		lua_pushstring( m_lua, "_G" );
		int key = luaL_ref( m_lua, LUA_REGISTRYINDEX );

		return std::auto_ptr<BaseScriptValue>( new ScriptValue( m_lua, ref, ref, key ) );
	}

	void ScriptState::Push( const char* value ) { lua_pushstring( m_lua, value ); }
	void ScriptState::Push( const char* value, unsigned int length ) { lua_pushlstring( m_lua, value, length ); }
	void ScriptState::Push( bool value ) { lua_pushboolean( m_lua, value ); }
	void ScriptState::Push( double value ) { lua_pushnumber( m_lua, value ); }
	void ScriptState::Push( float value ) { lua_pushnumber( m_lua, value ); }
	void ScriptState::Push( int value ) { lua_pushinteger( m_lua, value ); }

	void ScriptState::Pop( int values ) { lua_pop( m_lua, values ); }

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
