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

#ifndef PASSION_RENDERWINDOW_HPP
#define PASSION_RENDERWINDOW_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#ifdef WIN32
    #include <windows.h>
    #include <windowsx.h>
    #include <gl/GL.h>
    #include <gl/GLU.h>
#else
    #include <GL/glx.h>
    #include <GL/gl.h>
    #include <GL/glu.h>

    #include <X11/X.h>
    #include <X11/keysym.h>
#endif

#include <Passion/Input/IBaseInput.hpp>

namespace Passion
{
	////////////////////////////////////////////////////////////
	// Cross-platform render window
	////////////////////////////////////////////////////////////

	class RenderWindow
	{
	public:
		RenderWindow( const char* title, unsigned int width, unsigned int height );
		~RenderWindow();

		bool GetEvents();

		unsigned int GetHeight();
		unsigned int GetWidth();

		int MouseX();
		int MouseY();

		bool IsMouseDown( MouseButton button );
		bool IsKeyDown( Key key );

		int MouseWheel();

		void Present();

#ifdef WIN32
		static LRESULT CALLBACK WindowEvent( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
#endif

	private:
#ifdef WIN32
		HWND	m_window;
		HDC		m_dc;
		HGLRC	m_context;
#else
        Display*    m_dpy;
        ::Window    m_win;

        bool        m_doubleBuffered;
#endif

        // Window size
        int m_w, m_h;

		// Window open?
		bool	m_open;

		// Mouse coordinates
		int m_x, m_y;

		// Mouse states
		bool m_mouseLeft, m_mouseRight, m_mouseMiddle;
		int m_wheelDelta;

		// Key states
		bool m_keys[256];

#ifdef WIN32
		LRESULT Event( UINT msg, WPARAM wParam, LPARAM lParam );
#endif
	};
}

#endif
