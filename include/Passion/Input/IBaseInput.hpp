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

#ifndef PASSION_IBASEINPUT_HPP
#define PASSION_IBASEINPUT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

namespace Passion
{
	typedef void Window;

	enum MouseButton
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MIDDLE
	};

	enum Key
	{
		Backspace,
		Tab,
		Enter,
		Pause,
		Capslock,
		Escape,
		Space,
		PageUp,
		PageDown,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Select,
		Print,
		Execute,
		PrintScreen,
		Insert,
		Delete,
		Help,
		LWindows,
		RWindows,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Multiply,
		Add,
		Seperator,
		Subtract,
		Comma,
		Divide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Numlock,
		Scrolllock,
		LShift,
		RShift,
		LControl,
		RControl,
		LAlt,
		RAlt,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z
	};

	////////////////////////////////////////////////////////////
	// Base input interface
	////////////////////////////////////////////////////////////

	class IBaseInput
	{
	public:
		virtual void SetWindow( Window* window ) = 0;
		virtual bool GetEvents() = 0;

		virtual int GetMouseX() = 0;
		virtual int GetMouseY() = 0;
		virtual int GetMouseWheel() = 0;

		virtual bool IsMouseDown( MouseButton button ) = 0;
		virtual bool IsKeyDown( Key key ) = 0;

		virtual void ShowMouseCursor( bool show ) = 0;
		virtual void SetMousePos( int x, int y ) = 0;
	};
}

#endif