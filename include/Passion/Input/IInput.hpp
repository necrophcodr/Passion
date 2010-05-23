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