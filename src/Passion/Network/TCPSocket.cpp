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

#include <Passion/Network/TCPSocket.hpp>

namespace Passion
{
	void TCPThread( void* userdata )
	{
		TCPSocket* sock = (TCPSocket*)userdata;

		while ( true )
		{
			if ( sock->queue.size() > 0 )
			{
				TCPOperation op = sock->queue[0];
				
				switch ( op.op )
				{
				case 0:
					sock->connected = false;
					sock->sock.Close();
					sock->connected = sock->sock.Connect( op.arg2, op.arg1, op.arg4 ) == sf::Socket::Done;
					break;

				case 1:
					sock->sock.Send( op.arg1, op.arg3 );
					break;
				}

				sock->queue.pop_front();
			}

			sf::Sleep( 0.001f );
		}
	}

	TCPSocket::TCPSocket()
	{
		connected = false;

		thread = new sf::Thread( &TCPThread, this );
		thread->Launch();
	}

	TCPSocket::~TCPSocket()
	{
		delete thread;
	}

	void TCPSocket::Connect( const char* host, unsigned short port, float timeout )
	{
		TCPOperation op;
		op.op = 0;
		op.arg1 = const_cast<char*>( host );
		op.arg2 = port;
		op.arg4 = timeout;
		queue.push_back( op );
	}

	void TCPSocket::Disconnect()
	{
		thread->Terminate();

		sock.Close();
		connected = false;
	}

	void TCPSocket::Send( char* data, unsigned int length )
	{
		TCPOperation op;
		op.op = 1;
		op.arg1 = data;
		op.arg3 = length;
		queue.push_back( op );
	}

	void TCPSocket::Receive( char* buffer, unsigned int size, unsigned int& received )
	{
		if ( !connected ) { received = 0; return; }

		sf::SelectorTCP selector;
		selector.Add( sock );
		if ( selector.Wait( 0 ) > 0 )
		{
			sock.Receive( buffer, size, received );
		} else {
			received = 0;
		}
	}

	bool TCPSocket::IsConnected()
	{
		return connected;
	}
}
