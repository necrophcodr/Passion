#ifndef PASSION_HPP
#define PASSION_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <string>

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

	inline void* CreateInterface( const char* filename )
	{
		std::string fullPath = filename;
		typedef void* (*Factory) ();

		#ifdef PASSION_PLATFORM_LINUX
			fullPath += ".so";

			void* library = dlopen( fullPath.c_str(), RTLD_LAZY );
			Factory libFactory = (Factory)dlsym( library, "CreateInterface" );
			return (*libFactory) ();
		#else
			fullPath += ".dll";

			HMODULE library = LoadLibrary( fullPath.c_str() );
			Factory libFactory = (Factory)GetProcAddress( library, "CreateInterface" );
			return (*libFactory) ();
		#endif
	}
}

#endif