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

#include <Passion/Scripting.hpp>
#include <iostream>

////////////////////////////////////////////////////////////
// Entry point of the application
////////////////////////////////////////////////////////////

int main()
{
	////////////////////////////////////////////////////////////
	// Set up the scripting interface
	////////////////////////////////////////////////////////////

	#ifdef _DEBUG_
		Passion::IBaseScripting* scripting = (Passion::IBaseScripting*)Passion::CreateInterface( "../../lib/scripting-d" );
	#else
		Passion::IBaseScripting* scripting = (Passion::IBaseScripting*)Passion::CreateInterface( "../../lib/scripting" );
	#endif

	////////////////////////////////////////////////////////////
	// Evaluate lines of code
	////////////////////////////////////////////////////////////

	char code[256];

	while ( true )
	{
		std::cout << "> ";
		std::cin.getline( code, 256 );

		bool success = scripting->DoString( code );

		if ( !success )
		{
			std::cout << scripting->Error() << "\n";
		}
	}

	return 0;
}
