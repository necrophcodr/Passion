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

#ifndef PASSION_SCRIPTVALUE_HPP
#define PASSION_SCRIPTVALUE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Scripting/BaseScriptValue.hpp>

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

	class ScriptValue : public BaseScriptValue
	{
	public:
		ScriptValue( lua_State* state, int reference, int table, int key );
		ScriptValue( const ScriptValue& val );
		~ScriptValue();

		bool IsNumber();
		bool IsBoolean();
		bool IsTable();
		bool IsString();
		bool IsFunction();
		bool IsNil();

		operator int();
		operator float();
		operator double();
		operator bool();
		operator const char*();

		std::auto_ptr<BaseScriptValue> GetMember( const char* key );
		std::auto_ptr<BaseScriptValue> GetMember( int key );
		std::auto_ptr<BaseScriptValue> GetMember( BaseScriptValue* key );

		void operator= ( int val );
		void operator= ( float val );
		void operator= ( double val );
		void operator= ( bool val );
		void operator= ( const char* val );
		void operator= ( BaseScriptValue* val );

		void Push();

	private:
		lua_State* m_lua;
		int m_ref;

		int m_tbl;
		int m_key;
	};
}

#endif