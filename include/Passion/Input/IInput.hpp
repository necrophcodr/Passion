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

#ifndef PASSION_IINPUT_HPP
#define PASSION_IINPUT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Input/IBaseInput.hpp>
#include <SFML/Window.hpp>

namespace Passion
{	
	////////////////////////////////////////////////////////////
	// SFML implementation of IBaseInput
	////////////////////////////////////////////////////////////

	class IInput : public IBaseInput
	{
	public:
		IInput() { m_wheelDelta = 0; }
		
		void SetWindow( Window* window );
		bool GetEvents();

		int GetMouseX();
		int GetMouseY();
		int GetMouseWheel();

		bool IsMouseDown( MouseButton button );

	private:
		sf::Window* m_window;
		sf::Input* m_input;

		int m_wheelDelta;
	};
}

#endif