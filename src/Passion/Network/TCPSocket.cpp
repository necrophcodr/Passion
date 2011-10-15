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

#ifndef WIN32
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

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
#else
         m_socket = socket( AF_INET, SOCK_STREAM, 0 );

        int flags = fcntl( m_socket, F_GETFL, 0 );
        fcntl( m_socket, F_SETFL, flags | O_NONBLOCK );
#endif
	}

	TCPSocket::~TCPSocket()
	{
#ifdef WIN32
		WSACleanup();
#endif
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
#else
        struct sockaddr_in server;
        struct hostent* hp;

        server.sin_family = AF_INET;
        hp = gethostbyname( address );
        memcpy( (char*)&server.sin_addr, (char*)hp->h_addr, hp->h_length );
        server.sin_port = htons( port );

        connect( m_socket, (struct sockaddr*)&server, sizeof( server ) );
#endif

        m_connected = true;
	}

	void TCPSocket::Disconnect()
	{
#ifdef WIN32
		closesocket( m_socket );
#else
        close( m_socket );
#endif
        m_connected = false;
	}

	void TCPSocket::Send( const char* buffer, size_t length )
	{
		if ( length == 0 ) length = strlen( buffer );
		send( m_socket, buffer, length, 0 );
	}

	unsigned int TCPSocket::Receive( char* buffer, size_t length )
	{
		if ( !Available() )
			return 0;
		else
			return recv( m_socket, buffer, length, 0 );
	}

	bool TCPSocket::IsConnected()
	{
		if ( !m_connected ) return false;

        fd_set set;
		FD_ZERO( &set );
		FD_SET( m_socket, &set );

		timeval time = { 0, 0 };

		return select( m_socket + 1, 0, &set, 0, &time ) != 0;
	}

	bool TCPSocket::Available()
	{
		if ( !IsConnected() ) return false;

		fd_set set;
		FD_ZERO( &set );
		FD_SET( m_socket, &set );

		timeval time = { 0, 0 };

		if ( select( m_socket + 1, &set, 0, 0, &time ) > 0 ) {
			char temp;
			return recv( m_socket, &temp, 1, MSG_PEEK ) == 1;
		} else {
			return 0;
		}
	}
}
