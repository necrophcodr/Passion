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

		// Get display resolution
		RECT size;
		GetClientRect( GetDesktopWindow(), &size );

		// Calculate appropriate window size
		RECT windowSize;
		windowSize.left = 0;
		windowSize.top = 0;
		windowSize.right = width;
		windowSize.bottom = height;
		AdjustWindowRect( &windowSize, WS_POPUPWINDOW | WS_CAPTION, false );
		int width2 = windowSize.right - windowSize.left;
		int height2 = windowSize.bottom - windowSize.top;

		m_window =  CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, "OpenGLWindow", title, WS_POPUPWINDOW | WS_CAPTION, size.right / 2 - width2 / 2, size.bottom / 2 - height2 / 2, width2, height2, NULL, NULL, GetModuleHandle( NULL ), this );

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

		m_cursorState = LoadCursor( 0, IDC_ARROW );
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
        m_display = XOpenDisplay( NULL );

        // Find the appropiate color format (first try double buffered, then single buffered)
        XVisualInfo* vi = glXChooseVisual( m_display, DefaultScreen( m_display ), const_cast<int*>( dblBuf ) );
        m_doubleBuffered = true;

        if ( !vi )
        {
            vi = glXChooseVisual( m_display, DefaultScreen( m_display ), const_cast<int*>( snglBuf ) );
            m_doubleBuffered = false;
        }

        // Create OpenGL context
        GLXContext cx = glXCreateContext( m_display, vi, None, true );

        // Create color map
        Colormap cmap = XCreateColormap( m_display, RootWindow( m_display, vi->screen ), vi->visual, AllocNone );
        XSetWindowAttributes swa;
        swa.colormap = cmap;
        swa.border_pixel = 0;
        swa.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

        // Create window
        m_window = XCreateWindow( m_display, RootWindow( m_display, vi->screen ), ( DisplayWidth( m_display, DefaultScreen( m_display ) ) - width ) / 2, ( DisplayHeight( m_display, DefaultScreen( m_display ) ) - height ) / 2, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa );

        // Set title
        XStoreName( m_display, m_window, title );

        // Set window properties
        Atom windowHints = XInternAtom( m_display, "_MOTIF_WM_HINTS", false );

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
        XChangeProperty( m_display, m_window, windowHints, windowHints, 32, PropModeReplace, hintsPtr, 5 );

        // Activate context
        glXMakeCurrent( m_display, m_window, cx );

        // Show window
        XMapWindow( m_display, m_window );

		// Create hidden cursor
		Pixmap cursorPixmap = XCreatePixmap( m_display, m_window, 1, 1, 1 );
		GC graphicsContext = XCreateGC( m_display, cursorPixmap, 0, NULL );
		XDrawPoint( m_display, cursorPixmap, graphicsContext, 0, 0 );
		XFreeGC( m_display, graphicsContext );

		XColor color;
		color.flags = DoRed | DoGreen | DoBlue;
		color.red = color.blue = color.green = 0;
		m_hiddenCursor = XCreatePixmapCursor( m_display, cursorPixmap, cursorPixmap, &color, &color, 0, 0 );

		XFreePixmap( m_display, cursorPixmap );
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

        char buffer[32];
        KeySym sym;

        while ( XPending( m_display ) )
        {
            XNextEvent( m_display, &event );

            switch ( event.type )
            {
                case KeyPress:
                XLookupString( &event.xkey, buffer, 32, &sym, NULL);
                m_keys[ MapKey( sym, event.xkey.keycode ) ] = true;
                break;

                case KeyRelease:
                XLookupString( &event.xkey, buffer, 32, &sym, NULL);
                m_keys[ MapKey( sym, event.xkey.keycode ) ] = false;
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

	void RenderWindow::ShowMouseCursor( bool show )
	{
#ifdef WIN32
		m_cursorState = show ? LoadCursor( 0, IDC_ARROW ) : 0;
		SetCursor( m_cursorState );
#else
		XDefineCursor( m_display, m_window, show ? None : m_hiddenCursor );
		XFlush( m_display );
#endif
	}

	void RenderWindow::SetMousePos( int x, int y )
	{
#ifdef WIN32
		POINT pos = { x, y };
		ClientToScreen( m_window, &pos );
		SetCursorPos( pos.x, pos.y );
#else
		XWarpPointer( m_display, None, m_window, 0, 0, 0, 0, x, y );
		XFlush( m_display );
#endif
	}

	void RenderWindow::Present()
	{
#ifdef WIN32
		SwapBuffers( m_dc );
#else
        if ( m_doubleBuffered )
            glXSwapBuffers( m_display, m_window );
        else
            glFlush();
#endif
	}

	int RenderWindow::MapKey( int key, int raw )
	{
#ifdef WIN32
		switch ( key )
		{
			case VK_BACK:		return Backspace;
			case VK_TAB:		return Tab;
			case VK_RETURN:		return Enter;
			case VK_PAUSE:		return Pause;
			case VK_CAPITAL:	return Capslock;
			case VK_ESCAPE:		return Escape;
			case VK_SPACE:		return Space;
			case VK_PRIOR:		return PageUp;
			case VK_NEXT:		return PageDown;
			case VK_END:		return End;
			case VK_HOME:		return Home;
			case VK_LEFT:		return Left;
			case VK_UP:			return Up;
			case VK_RIGHT:		return Right;
			case VK_DOWN:		return Down;
			case VK_SELECT:		return Select;
			case VK_PRINT:		return Print;
			case VK_EXECUTE:	return Execute;
			case VK_SNAPSHOT:	return PrintScreen;
			case VK_INSERT:		return Insert;
			case VK_DELETE:		return Delete;
			case VK_HELP:		return Help;
			case VK_LWIN:		return LWindows;
			case VK_RWIN:		return RWindows;
			case VK_NUMPAD0:	return Numpad0;
			case VK_NUMPAD1:	return Numpad1;
			case VK_NUMPAD2:	return Numpad2;
			case VK_NUMPAD3:	return Numpad3;
			case VK_NUMPAD4:	return Numpad4;
			case VK_NUMPAD5:	return Numpad5;
			case VK_NUMPAD6:	return Numpad6;
			case VK_NUMPAD7:	return Numpad7;
			case VK_NUMPAD8:	return Numpad8;
			case VK_NUMPAD9:	return Numpad9;
			case VK_MULTIPLY:	return Multiply;
			case VK_ADD:		return Add;
			case VK_SEPARATOR:	return Seperator;
			case VK_SUBTRACT:	return Subtract;
			case VK_DECIMAL:	return Comma;
			case VK_DIVIDE:		return Divide;
			case VK_F1:			return F1;
			case VK_F2:			return F2;
			case VK_F3:			return F3;
			case VK_F4:			return F4;
			case VK_F5:			return F5;
			case VK_F6:			return F6;
			case VK_F7:			return F7;
			case VK_F8:			return F8;
			case VK_F9:			return F9;
			case VK_F10:		return F10;
			case VK_F11:		return F11;
			case VK_F12:		return F12;
			case VK_F13:		return F13;
			case VK_F14:		return F14;
			case VK_F15:		return F15;
			case VK_NUMLOCK:	return Numlock;
			case VK_SCROLL:		return Scrolllock;
			case VK_LSHIFT:		return LShift;
			case VK_RSHIFT:		return RShift;
			case VK_LCONTROL:	return LControl;
			case VK_RCONTROL:	return RControl;
			case VK_LMENU:		return LAlt;
			case VK_RMENU:		return RAlt;
			case '0':			return Num0;
			case '1':			return Num1;
			case '2':			return Num2;
			case '3':			return Num3;
			case '4':			return Num4;
			case '5':			return Num5;
			case '6':			return Num6;
			case '7':			return Num7;
			case '8':			return Num8;
			case '9':			return Num9;
			case 'A':			return A;
			case 'B':			return B;
			case 'C':			return C;
			case 'D':			return D;
			case 'E':			return E;
			case 'F':			return F;
			case 'G':			return G;
			case 'H':			return H;
			case 'I':			return I;
			case 'J':			return J;
			case 'K':			return K;
			case 'L':			return L;
			case 'M':			return M;
			case 'N':			return N;
			case 'O':			return O;
			case 'P':			return P;
			case 'Q':			return Q;
			case 'R':			return R;
			case 'S':			return S;
			case 'T':			return T;
			case 'U':			return U;
			case 'V':			return V;
			case 'W':			return W;
			case 'X':			return X;
			case 'Y':			return Y;
			case 'Z':			return Z;
		}
#else
        KeySym lower, realkey;
        XConvertCase( key, &lower, &realkey );

        // Standard keys
		switch ( realkey )
		{
			case XK_BackSpace:return Backspace;
			case XK_Tab:		return Tab;
			case XK_Return:		return Enter;
			case XK_Pause:		return Pause;
			case XK_End:		return End;
			case XK_Home:		return Home;
            case XK_Insert:		return Insert;
			case XK_Delete:		return Delete;
			case XK_space:		return Space;
			case XK_KP_0:   	return Numpad0;
			case XK_KP_1:	    return Numpad1;
			case XK_KP_2:	    return Numpad2;
			case XK_KP_3:	    return Numpad3;
			case XK_KP_4:	    return Numpad4;
			case XK_KP_5:	    return Numpad5;
			case XK_KP_6:	    return Numpad6;
			case XK_KP_7:	    return Numpad7;
			case XK_KP_8:	    return Numpad8;
			case XK_KP_9:	    return Numpad9;
			case XK_KP_Multiply:return Multiply;
			case XK_KP_Add:		return Add;
			case XK_KP_Subtract:return Subtract;
			case XK_comma:	    return Comma;
			case XK_KP_Divide:	return Divide;
			case XK_F1:			return F1;
			case XK_F2:			return F2;
			case XK_F3:			return F3;
			case XK_F4:			return F4;
			case XK_F5:			return F5;
			case XK_F6:			return F6;
			case XK_F7:			return F7;
			case XK_F8:			return F8;
			case XK_F9:			return F9;
			case XK_F10:		return F10;
			case XK_F11:		return F11;
			case XK_F12:		return F12;
			case XK_F13:		return F13;
			case XK_F14:		return F14;
			case XK_F15:		return F15;
			case XK_Shift_L:	return LShift;
			case XK_Shift_R:	return RShift;
			case XK_Control_L:	return LControl;
			case XK_Control_R:	return RControl;
			case XK_Alt_L:		return LAlt;
			case XK_Alt_R:		return RAlt;
			case XK_0:			return Num0;
			case XK_1:			return Num1;
			case XK_2:			return Num2;
			case XK_3:			return Num3;
			case XK_4:			return Num4;
			case XK_5:			return Num5;
			case XK_6:			return Num6;
			case XK_7:			return Num7;
			case XK_8:			return Num8;
			case XK_9:			return Num9;
			case XK_A:			return A;
			case XK_B:			return B;
			case XK_C:			return C;
			case XK_D:			return D;
			case XK_E:			return E;
			case XK_F:			return F;
			case XK_G:			return G;
			case XK_H:			return H;
			case XK_I:			return I;
			case XK_J:			return J;
			case XK_K:			return K;
			case XK_L:			return L;
			case XK_M:			return M;
			case XK_N:			return N;
			case XK_O:			return O;
			case XK_P:			return P;
			case XK_Q:			return Q;
			case XK_R:			return R;
			case XK_S:			return S;
			case XK_T:			return T;
			case XK_U:			return U;
			case XK_V:			return V;
			case XK_W:			return W;
			case XK_X:			return X;
			case XK_Y:			return Y;
			case XK_Z:			return Z;
		}

		// Other keys
		switch ( raw )
		{
            case 66:	        return Capslock;
			case 9:		        return Escape;
			case 112:		    return PageUp;
			case 117:		    return PageDown;
			case 113:		    return Left;
			case 111:			return Up;
			case 114:		    return Right;
			case 116:		    return Down;
			case 78:            return Scrolllock;
			case 77:            return Numlock;
			case 133:		    return LWindows;
			case 134:		    return RWindows;
		}
#endif

		return key;
	}

#ifdef WIN32
	LRESULT RenderWindow::Event( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		switch ( msg )
		{
		case WM_DESTROY:
			m_open = false;
			break;

		case WM_SETCURSOR:
			if ( LOWORD( lParam ) == HTCLIENT )
				SetCursor( m_cursorState );
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
			m_keys[ MapKey( wParam ) ] = true;
			break;

		case WM_KEYUP:
			m_keys[ MapKey( wParam ) ] = false;
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
