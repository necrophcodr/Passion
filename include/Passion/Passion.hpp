#ifndef PASSION_HPP
#define PASSION_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#ifdef PASSION_PLATFORM_LINUX
	#include <dlfcn.h>
#else
	#include <windows.h>
#endif

////////////////////////////////////////////////////////////
// Conveniences
////////////////////////////////////////////////////////////

namespace Passion
{
	////////////////////////////////////////////////////////////
	// Loads an interface from a shared library
	////////////////////////////////////////////////////////////

	template <typename _interface>
	_interface* CreateInterface( const char* filename )
	{
		std::string fullPath = filename;
		typedef void* (*Factory) ();

		#ifdef PASSION_PLATFORM_LINUX
			fullPath += ".so";

			void* library = dlopen( fullPath.c_str(), RTLD_NOW );

			if ( library == 0 ) { std::cout << "Couldn't load library: " << fullPath << "\n"; while( true ); }

			Factory libFactory = (Factory)dlsym( library, "CreateInterface" );

			if ( libFactory == 0 ) { std::cout << "Couldn't load factory in library: " << fullPath << "\n" << dlerror() << "\n"; while( true ); }

			return (*libFactory) ();

			return 0;
		#else
			fullPath += ".dll";

			HMODULE library = LoadLibrary( fullPath.c_str() );
			if ( library == 0 )
			{
				std::cout << "Could not load library: " << fullPath << "\n";
				exit( 1 );
			}

			Factory libFactory = (Factory)GetProcAddress( library, "CreateInterface" );
			return (_interface*) (*libFactory) ();
		#endif
	}

	////////////////////////////////////////////////////////////
	// Unloads an interface from a shared library
	////////////////////////////////////////////////////////////

	template <typename _interface>
	void DestroyInterface( _interface* interface_ )
	{
		if ( interface_ )
			delete interface_;
	}
}

#endif
