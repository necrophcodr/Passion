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
	TCPSocket::TCPSocket()
	{
#ifdef WIN32
		WSADATA wsaData;
		WSAStartup( MAKEWORD( 1, 1 ), &wsaData );

		m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		u_long nonblocking = 1;
		ioctlsocket( m_socket, FIONBIO, &nonblocking );
#endif
	}

	TCPSocket::~TCPSocket()
	{
#ifdef WIN32
		WSACleanup();
#endif;
	}

	void TCPSocket::Connect( const char* address, unsigned short port )
	{
#ifdef WIN32
		LPHOSTENT hostEntry = (LPHOSTENT)gethostbyname( address );

		SOCKADDR_IN serverInfo;
		serverInfo.sin_family = AF_INET;
		serverInfo.sin_addr = *( (LPIN_ADDR)*hostEntry->h_addr_list );
		serverInfo.sin_port = htons( port );

		connect( m_socket, (LPSOCKADDR)&serverInfo, sizeof( struct sockaddr ) );
		m_connected = true;
#endif
	}

	void TCPSocket::Disconnect()
	{
#ifdef WIN32
		closesocket( m_socket );
		m_connected = false;
#endif;
	}

	void TCPSocket::Send( const char* buffer, size_t length )
	{
#ifdef WIN32
		if ( length == 0 ) length = strlen( buffer );
		send( m_socket, buffer, length, 0 );
#endif;
	}

	unsigned int TCPSocket::Receive( char* buffer, size_t length )
	{
#ifdef WIN32
		if ( !Available() )
			return 0;
		else
			return recv( m_socket, buffer, length, 0 );
#endif
	}

	bool TCPSocket::IsConnected()
	{
#ifdef WIN32
		if ( !m_connected ) return false;

		fd_set set;
		set.fd_count = 1;
		set.fd_array[0] = m_socket;

		timeval time = { 0, 0 };

		return select( 0, 0, &set, 0, &time ) != 0;
#endif
	}

	bool TCPSocket::Available()
	{
#ifdef WIN32
		if ( !IsConnected() ) return false;

		fd_set set;
		set.fd_count = 1;
		set.fd_array[0] = m_socket;

		timeval time = { 0, 0 };
	
		if ( select( 0, &set, 0, 0, &time ) > 0 ) {
			char temp;
			return recv( m_socket, &temp, 1, MSG_PEEK ) == 1;
		} else {
			return 0;
		}
#endif
	}
}
