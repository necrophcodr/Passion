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

#ifndef PASSION_SCRIPTSTATE_HPP
#define PASSION_SCRIPTSTATE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Scripting/BaseScriptState.hpp>
#include <Passion/Scripting/ScriptValue.hpp>

extern "C"
{
	#include <Lua/lua.h>
	#include <Lua/lauxlib.h>
	#include <Lua/lualib.h>
}

namespace Passion
{	
	////////////////////////////////////////////////////////////
	// Lua implementation of BaseScriptState
	////////////////////////////////////////////////////////////

	class ScriptState : public BaseScriptState
	{
	public:
		ScriptState();
		~ScriptState();

		bool DoString( const char* code );
		bool DoFile( const char* path );

		std::auto_ptr<BaseScriptValue> NewTable();
		std::auto_ptr<BaseScriptValue> UserData( void* data, std::auto_ptr<BaseScriptValue> metatable );

		void SetMetaTable( std::auto_ptr<BaseScriptValue> value, std::auto_ptr<BaseScriptValue> metatable );
		std::auto_ptr<BaseScriptValue> GetMetaTable( std::auto_ptr<BaseScriptValue> value );

		std::auto_ptr<BaseScriptValue> Globals();

		void Push( BaseScriptValue* value );
		void Push( std::auto_ptr<BaseScriptValue> value );
		void Push( const char* value );
		void Push( const char* value, unsigned int length );
		void Push( bool value );
		void Push( double value );
		void Push( float value );
		void Push( int value );
		void Push( void* value, std::auto_ptr<BaseScriptValue> metatable );

		void Pop( int values );
		std::auto_ptr<BaseScriptValue> Get( int index );

		int Top();

		bool Call( int args, int returns );

		const char* Error();

	private:
		lua_State* m_lua;
		char* m_error;

		void SetError( const char* error = "" );
	};
}

#endif