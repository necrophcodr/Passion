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

#include <Passion/Render/RenderWindow.hpp>

#include <cstring>
#include <cstdio>

namespace Passion
{
	RenderWindow::RenderWindow( const char* title, unsigned int width, unsigned int height )
	{
        // Create window
#ifdef WIN32
		WNDCLASSEX windowClass;

		windowClass.cbSize = sizeof( WNDCLASSEX );
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = WindowEvent;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hIcon =  LoadIcon( NULL, IDI_APPLICATION );
		windowClass.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
		windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
		windowClass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "OpenGLWindow";
		windowClass.hInstance = 0;

		RegisterClassEx( &windowClass );

		RECT size;
		GetClientRect( GetDesktopWindow(), &size );

		m_window =  CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "OpenGLWindow", title, WS_POPUPWINDOW | WS_CAPTION, size.right / 2 - width / 2, size.bottom / 2 - height / 2, width, height, NULL, NULL, GetModuleHandle( NULL ), this );

		ShowWindow( m_window, SW_SHOWNORMAL );
		UpdateWindow( m_window );

		// Create OpenGL context
		m_dc = GetDC( m_window );

		// Set pixel format
		PIXELFORMATDESCRIPTOR pfd;

		ZeroMemory( &pfd, sizeof( PIXELFORMATDESCRIPTOR ) );
		pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int iFormat = ChoosePixelFormat( m_dc, &pfd );

		SetPixelFormat( m_dc, iFormat, &pfd );

		m_context = wglCreateContext( m_dc );

		wglMakeCurrent( m_dc, m_context );
#else
        static const int snglBuf[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, None };
        static const int dblBuf[]  = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };

        static const unsigned long HINTS_FUNCTIONS = 1 << 0;
        static const unsigned long HINTS_DECORATIONS = 1 << 1;

        static const unsigned long DECOR_BORDER = 1 << 1;
        static const unsigned long DECOR_TITLE = 1 << 3;
        static const unsigned long DECOR_MENU = 1 << 4;
        static const unsigned long DECOR_MINIMIZE = 1 << 5;

        static const unsigned long FUNC_MOVE = 1 << 2;
        static const unsigned long FUNC_MINIMIZE = 1 << 3;
        static const unsigned long FUNC_CLOSE = 1 << 5;

        // Connect to X server
        m_dpy = XOpenDisplay( NULL );

        // Find the appropiate color format (first try double buffered, then single buffered)
        XVisualInfo* vi = glXChooseVisual( m_dpy, DefaultScreen( m_dpy ), const_cast<int*>( dblBuf ) );
        m_doubleBuffered = true;

        if ( !vi )
        {
            vi = glXChooseVisual( m_dpy, DefaultScreen( m_dpy ), const_cast<int*>( snglBuf ) );
            m_doubleBuffered = false;
        }

        // Create OpenGL context
        GLXContext cx = glXCreateContext( m_dpy, vi, None, true );

        // Create color map
        Colormap cmap = XCreateColormap( m_dpy, RootWindow( m_dpy, vi->screen ), vi->visual, AllocNone );
        XSetWindowAttributes swa;
        swa.colormap = cmap;
        swa.border_pixel = 0;
        swa.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

        // Create window
        m_win = XCreateWindow( m_dpy, RootWindow( m_dpy, vi->screen ), ( DisplayWidth( m_dpy, DefaultScreen( m_dpy ) ) - width ) / 2, ( DisplayHeight( m_dpy, DefaultScreen( m_dpy ) ) - height ) / 2, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa );

        // Set title
        XStoreName( m_dpy, m_win, title );

        // Set window properties
        Atom windowHints = XInternAtom( m_dpy, "_MOTIF_WM_HINTS", false );

        struct WMHints
        {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long inputMode;
            unsigned long state;
        };

        WMHints hints;
        hints.flags = HINTS_FUNCTIONS | HINTS_DECORATIONS;
        hints.decorations = DECOR_BORDER | DECOR_TITLE | DECOR_MINIMIZE | DECOR_MENU;
        hints.functions = FUNC_MOVE | FUNC_MINIMIZE | FUNC_CLOSE;

        const unsigned char* hintsPtr = reinterpret_cast<const unsigned char*>( &hints );
        XChangeProperty( m_dpy, m_win, windowHints, windowHints, 32, PropModeReplace, hintsPtr, 5 );

        // Activate context
        glXMakeCurrent( m_dpy, m_win, cx );

        // Show window
        XMapWindow( m_dpy, m_win );
#endif

		// Initialize states
		m_x = 0;
		m_y = 0;

		m_mouseLeft = false;
		m_mouseRight = false;
		m_mouseMiddle = false;

		m_wheelDelta = 0;

		memset( m_keys, 0, 256 );

		m_open = true;
		m_w = width;
		m_h = height;
	}

	RenderWindow::~RenderWindow()
	{
#ifdef WIN32
		wglDeleteContext( m_context );
		wglMakeCurrent( NULL, NULL );
		ReleaseDC( m_window, m_dc );
#endif
	}

	bool RenderWindow::GetEvents()
	{
	    m_wheelDelta = 0;

#ifdef WIN32
		if ( !m_open ) return false;

		MSG msg;

		while ( PeekMessage( &msg, m_window, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		return true;
#else
        XEvent event;

        while ( XPending( m_dpy ) )
        {
            XNextEvent( m_dpy, &event );

            switch ( event.type )
            {
                case KeyPress:
                m_keys[event.xkey.keycode] = true;
                break;

                case KeyRelease:
                m_keys[event.xkey.keycode] = false;
                break;

                case ButtonPress:
                if ( event.xbutton.button == 1 )
                    m_mouseLeft = true;
                else if ( event.xbutton.button == 2 )
                    m_mouseMiddle = true;
                else if ( event.xbutton.button == 3 )
                    m_mouseRight = true;
                break;

                case ButtonRelease:
                if ( event.xbutton.button == 1 )
                    m_mouseLeft = false;
                else if ( event.xbutton.button == 2 )
                    m_mouseMiddle = false;
                else if ( event.xbutton.button == 3 )
                    m_mouseRight = false;
                else if ( event.xbutton.button == 4 || event.xbutton.button == 5 )
                {
                    m_wheelDelta = event.xbutton.button == 4 ? 1 : -1;
                }
                break;

                case MotionNotify:
                m_x = event.xmotion.x;
                m_y = event.xmotion.y;
                break;
            }
        }

        return true;
#endif
	}

	unsigned int RenderWindow::GetWidth()
	{
        return m_w;
	}

	unsigned int RenderWindow::GetHeight()
	{
        return m_h;
	}

	int RenderWindow::MouseX()
	{
		return m_x;
	}

	int RenderWindow::MouseY()
	{
		return m_y;
	}

	bool RenderWindow::IsMouseDown( MouseButton button )
	{
		if ( button == MOUSE_LEFT )
			return m_mouseLeft;
		else if ( button == MOUSE_RIGHT )
			return m_mouseRight;
		else if ( button == MOUSE_MIDDLE )
			return m_mouseMiddle;

		return false;
	}

	bool RenderWindow::IsKeyDown( Key key )
	{
		if ( key < 256 )
		{
			return m_keys[key];
		}
		else
			return false;
	}

	int RenderWindow::MouseWheel()
	{
		return m_wheelDelta;
	}

	void RenderWindow::Present()
	{
#ifdef WIN32
		SwapBuffers( m_dc );
#else
        if ( m_doubleBuffered )
            glXSwapBuffers( m_dpy, m_win );
        else
            glFlush();
#endif
	}

#ifdef WIN32
	LRESULT RenderWindow::Event( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		switch ( msg )
		{
		case WM_DESTROY:
			m_open = false;
			break;

		case WM_MOUSEMOVE:
			m_x = GET_X_LPARAM( lParam );
			m_y = GET_Y_LPARAM( lParam );

			break;

		case WM_MOUSEWHEEL:
			if ( GET_WHEEL_DELTA_WPARAM( wParam ) == 0 )
				m_wheelDelta = 0;
			else if ( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
				m_wheelDelta = 1;
			else
				m_wheelDelta = -1;

			break;

		case WM_LBUTTONDOWN:
			m_mouseLeft = true;
			break;
		case WM_LBUTTONUP:
			m_mouseLeft = false;
			break;
		case WM_RBUTTONDOWN:
			m_mouseRight = true;
			break;
		case WM_RBUTTONUP:
			m_mouseRight = false;
			break;
		case WM_MBUTTONDOWN:
			m_mouseMiddle = true;
			break;
		case WM_MBUTTONUP:
			m_mouseMiddle = false;
			break;

		case WM_KEYDOWN:
			m_keys[wParam] = true;
			break;

		case WM_KEYUP:
			m_keys[wParam] = false;
			break;

		default:
			return DefWindowProc( m_window, msg, wParam, lParam );
		}

		return 0;
	}

	LRESULT CALLBACK RenderWindow::WindowEvent( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		RenderWindow* window;

		if ( msg == WM_NCCREATE )
		{
			window = reinterpret_cast<RenderWindow*>( ((LPCREATESTRUCT)lParam)->lpCreateParams );
			window->m_window = hWnd;

			SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast<long>( window ) );

			return DefWindowProc( hWnd, msg, wParam, lParam );
		} else {
			window = reinterpret_cast<RenderWindow*>( GetWindowLong( hWnd, GWL_USERDATA ) );

			return window->Event( msg, wParam, lParam );
		}
	}
#endif
}
