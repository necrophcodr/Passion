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

#ifndef PASSION_BASESCRIPTVALUE_HPP
#define PASSION_BASESCRIPTVALUE_HPP

#include <memory>

namespace Passion
{
	#define SCRIPT_FUNCTION( name ) static int name( void* p )

	typedef int ( *ScriptFunction ) ( void* p );

	////////////////////////////////////////////////////////////
	// Base script value
	////////////////////////////////////////////////////////////

	class BaseScriptValue
	{
	public:
		virtual ~BaseScriptValue() { }

		virtual bool IsNumber() = 0;
		virtual bool IsBoolean() = 0;
		virtual bool IsTable() = 0;
		virtual bool IsString() = 0;
		virtual bool IsFunction() = 0;
		virtual bool IsUserData() = 0;
		virtual bool IsNil() = 0;

		virtual int GetInteger() = 0;
		virtual float GetFloat() = 0;
		virtual double GetDouble() = 0;
		virtual bool GetBoolean() = 0;
		virtual const char* GetString() = 0;
		virtual void* GetUserData() = 0;

		virtual std::auto_ptr<BaseScriptValue> GetMember( const char* key ) = 0;
		virtual std::auto_ptr<BaseScriptValue> GetMember( int key ) = 0;
		virtual std::auto_ptr<BaseScriptValue> GetMember( BaseScriptValue* key ) = 0;

		virtual void Set( int value ) = 0;
		virtual void Set( float value ) = 0;
		virtual void Set( double value ) = 0;
		virtual void Set( bool value ) = 0;
		virtual void Set( const char* value ) = 0;
		virtual void Set( std::auto_ptr<BaseScriptValue> value ) = 0;
		virtual void Set( BaseScriptValue* value ) = 0;
		virtual void Set( ScriptFunction value ) = 0;

		virtual void SetMetaTable( std::auto_ptr<BaseScriptValue> metatable ) = 0;
		virtual std::auto_ptr<BaseScriptValue> GetMetaTable() = 0;

		virtual bool Equals( std::auto_ptr<BaseScriptValue> value ) = 0;

		virtual void Push() = 0;
	};
}

#endif