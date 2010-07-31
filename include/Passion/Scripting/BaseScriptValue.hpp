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
	////////////////////////////////////////////////////////////
	// Base scripting state
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
		virtual bool IsNil() = 0;

		virtual operator int() = 0;
		virtual operator float() = 0;
		virtual operator double() = 0;
		virtual operator bool() = 0;
		virtual operator const char*() = 0;

		virtual std::auto_ptr<BaseScriptValue> GetMember( const char* key ) = 0;
		virtual std::auto_ptr<BaseScriptValue> GetMember( int key ) = 0;
		virtual std::auto_ptr<BaseScriptValue> GetMember( BaseScriptValue* key ) = 0;

		virtual void operator= ( int val ) = 0;
		virtual void operator= ( float val ) = 0;
		virtual void operator= ( double val ) = 0;
		virtual void operator= ( bool val ) = 0;
		virtual void operator= ( const char* val ) = 0;
		virtual void operator= ( BaseScriptValue* val ) = 0;

		virtual void Push() = 0;
	};
}

#endif