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

namespace Passion
{
	RenderWindow::RenderWindow( const char* title, unsigned int width, unsigned int height )
	{
		// Create window
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

		m_open = true;

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

		// Initialize input states
		m_x = 0;
		m_y = 0;

		m_mouseLeft = false;
		m_mouseRight = false;
		m_mouseMiddle = false;

		m_wheelDelta = 0;

		memset( m_keys, 0, 256 );
	}

	RenderWindow::~RenderWindow()
	{
		wglDeleteContext( m_context );
		wglMakeCurrent( NULL, NULL );
		ReleaseDC( m_window, m_dc );
	}

	bool RenderWindow::GetEvents()
	{
		if ( !m_open ) return false;

		m_wheelDelta = 0;

		MSG msg;
		
		while ( PeekMessage( &msg, m_window, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		return true;
	}

	unsigned int RenderWindow::GetWidth()
	{
		RECT size;
		GetClientRect( m_window, &size );

		return size.right;
	}

	unsigned int RenderWindow::GetHeight()
	{
		RECT size;
		GetClientRect( m_window, &size );

		return size.bottom;
	}

	int RenderWindow::GetX()
	{
		RECT pos;
		GetWindowRect( m_window, &pos );

		return pos.left;
	}

	int RenderWindow::GetY()
	{
		RECT pos;
		GetWindowRect( m_window, &pos );

		return pos.top;
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
		SwapBuffers( m_dc );
	}

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
}