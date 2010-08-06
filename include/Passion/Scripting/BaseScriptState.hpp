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

#ifndef PASSION_BASESCRIPTSTATE_HPP
#define PASSION_BASESCRIPTSTATE_HPP

#include <Passion/Scripting/BaseScriptValue.hpp>
#include <memory>

namespace Passion
{
	////////////////////////////////////////////////////////////
	// Base scripting state
	////////////////////////////////////////////////////////////

	class BaseScriptState
	{
	public:
		virtual bool DoString( const char* code ) = 0;
		virtual bool DoFile( const char* path ) = 0;

		virtual std::auto_ptr<BaseScriptValue> NewTable() = 0;
		virtual std::auto_ptr<BaseScriptValue> UserData( void* data, std::auto_ptr<BaseScriptValue> metatable ) = 0;

		virtual std::auto_ptr<BaseScriptValue> Globals() = 0;
		virtual std::auto_ptr<BaseScriptValue> Registry() = 0;

		virtual void Push( std::auto_ptr<BaseScriptValue> value ) = 0;
		virtual void Push( BaseScriptValue* value ) = 0;
		virtual void Push( const char* value ) = 0;
		virtual void Push( const char* value, unsigned int length ) = 0;
		virtual void Push( bool value ) = 0;
		virtual void Push( double value ) = 0;
		virtual void Push( float value ) = 0;
		virtual void Push( int value ) = 0;
		virtual void Push( void* value, std::auto_ptr<BaseScriptValue> metatable ) = 0;

		virtual void Pop( int values ) = 0;
		virtual std::auto_ptr<BaseScriptValue> Get( int index ) = 0;

		virtual int Top() = 0;

		virtual bool Call( int args, int returns = -1 ) = 0;

		virtual const char* Error() = 0;
	};
}

#endif