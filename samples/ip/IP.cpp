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

#include <Passion/Network.hpp>
#include <iostream>
#include <time.h>
#include <stdio.h>

////////////////////////////////////////////////////////////
// Entry point of the application
////////////////////////////////////////////////////////////

int main()
{
	////////////////////////////////////////////////////////////
	// Set up the network interface
	////////////////////////////////////////////////////////////

	#ifdef _DEBUG_
		Passion::IBaseNetwork* network = Passion::CreateInterface<Passion::IBaseNetwork>( "../../lib/network-d" );
	#else
		Passion::IBaseNetwork* network = Passion::CreateInterface<Passion::IBaseNetwork>( "../../lib/network" );
	#endif

	////////////////////////////////////////////////////////////
	// Retrieve the external IP address
	////////////////////////////////////////////////////////////

	Passion::BaseTCPSocket* socket = network->TCPSocket();
	socket->Connect( "www.whatismyip.org", 80 );

	clock_t timeout = clock() + CLOCKS_PER_SEC;
	clock_t start = clock();

	while ( !socket->IsConnected() && clock() < timeout );

	if ( !socket->IsConnected() )
	{
		std::cout << "Could not connect to www.whatismyip.org!\n";

		#ifndef PASSION_PLATFORM_LINUX
            getchar();
        #endif

		return 1;
	}

	clock_t ping = clock() - start;
	std::cout << "Connected after " << (float)ping / (float)CLOCKS_PER_SEC * 1000.0f << " milliseconds." << std::endl;

	char request[] = "GET / HTTP/1.1\r\nHost: www.whatismyip.org\r\n\r\n";
	socket->Send( request, sizeof( request ) );

	char response[512];
	unsigned int received = 0;

	while ( socket->IsConnected() )
	{
		socket->Receive( response, 512, received );
		response[received] = 0;

		if ( received > 0 )
		{
			if ( network->IsIPAddress( response ) )
			{
				std::cout << "Your IP address is: " << response << "\n";
				socket->Disconnect();

				break;
			}

			received = 0;
		}
	}

	////////////////////////////////////////////////////////////
	// Clean up
	////////////////////////////////////////////////////////////

	Passion::DestroyInterface<Passion::IBaseNetwork>( network );

	#ifndef PASSION_PLATFORM_LINUX
        getchar();
    #endif

	return 0;
}
