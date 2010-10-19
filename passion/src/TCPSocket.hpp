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

#include "Interfaces.hpp"
#include <stdlib.h>

////////////////////////////////////////////////////////////
// TCP socket object
////////////////////////////////////////////////////////////

class TCPSocket
{
public:
	SCRIPT_FUNCTION( Constructor )
	{
		Passion::BaseTCPSocket* socket = g_Network->TCPSocket();
		g_Lua->Push( &socket, sizeof( Passion::BaseTCPSocket* ), g_Lua->Registry()->GetMember( "TCPSocket" ) );

		return 1;
	}

	SCRIPT_FUNCTION( Destructor )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );

		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();
		delete socket;

		return 0;
	}

	SCRIPT_FUNCTION( Connect )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );
		if ( !g_Lua->Get( 2 )->IsString() ) g_Lua->Error( 2, "string" );
		if ( !g_Lua->Get( 3 )->IsNumber() ) g_Lua->Error( 3, "number" );

		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();
		socket->Connect( g_Lua->Get( 2 )->GetString(), g_Lua->Get( 3 )->GetInteger() );

		return 0;
	}

	SCRIPT_FUNCTION( Disconnect )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );
		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();

		socket->Disconnect();

		return 0;
	}

	SCRIPT_FUNCTION( Send )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );
		if ( !g_Lua->Get( 2 )->IsString() ) g_Lua->Error( 2, "string" );

		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();

		size_t len;
		const char* data = g_Lua->Get( 2 )->GetString( len );

		socket->Send( data, len );

		return 0;
	}

	SCRIPT_FUNCTION( Receive )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );
		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();

		char* buffer = reinterpret_cast<char*>( malloc( 512 ) );
		unsigned int offset = 0;
		unsigned int length = 0;

		do
		{
			length = socket->Receive( buffer + offset, 512 );
			offset += length;
			buffer = reinterpret_cast<char*>( realloc( buffer, offset + 512 ) );
		} while ( length > 0 );

		g_Lua->Push( buffer, offset );

		free( buffer );

		return 1;
	}

	SCRIPT_FUNCTION( IsConnected )
	{
		if ( !g_Lua->Get( 1 )->IsUserData() || !g_Lua->Get( 1 )->GetMetaTable()->Equals( g_Lua->Registry()->GetMember( "TCPSocket" ) ) ) g_Lua->Error( 1, "TCPSocket" );

		Passion::BaseTCPSocket* socket = *(Passion::BaseTCPSocket**)g_Lua->Get( 1 )->GetUserData();
		g_Lua->Push( socket->IsConnected() );

		return 1;
	}

	static void Bind()
	{
		g_Lua->Globals()->GetMember( "TCPSocket" )->Set( Constructor );

		std::auto_ptr<BaseScriptValue> meta = g_Lua->NewTable();
			meta->GetMember( "Connect" )->Set( Connect );
			meta->GetMember( "Disconnect" )->Set( Disconnect );

			meta->GetMember( "Send" )->Set( Send );
			meta->GetMember( "Receive" )->Set( Receive );

			meta->GetMember( "IsConnected" )->Set( IsConnected );

			meta->GetMember( "__gc" )->Set( Destructor );
			meta->GetMember( "__index" )->Set( meta.get() );
		g_Lua->Registry()->GetMember( "TCPSocket" )->Set( meta );
	}
};
