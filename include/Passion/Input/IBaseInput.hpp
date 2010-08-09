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
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
        Num0 = '0',
        Num1 = '1',
        Num2 = '2',
        Num3 = '3',
        Num4 = '4',
        Num5 = '5',
        Num6 = '6',
        Num7 = '7',
        Num8 = '8',
        Num9 = '9', 
        Escape = 256,
        LControl,
        LShift,
        LAlt,
        LSystem,
        RControl,
        RShift,
        RAlt,
        RSystem,
        Menu,
        LBracket,
        RBracket,
        SemiColon,
        Comma,
        Period,
        Quote,
        Slash,
        BackSlash,
        Tilde,
        Equal,
        Dash,
        Space,
        Return,
        Back,
        Tab,
        PageUp,
        PageDown,
        End,
        Home,
        Insert,
        Delete,
        Add,
        Subtract,
        Multiply,
        Divide,
        Left,
        Right,
        Up,
        Down,
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
        Pause
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
	};
}

#endif