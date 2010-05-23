////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <Passion/Input/IInput.hpp>

namespace Passion
{
	void IInput::SetWindow( Window* window )
	{
		m_window = (sf::Window*)window;
		m_input = const_cast<sf::Input*>( &m_window->GetInput() );
	}

	bool IInput::GetEvents()
	{
		m_wheelDelta = 0;

		sf::Event Event;
		while ( m_window->GetEvent( Event ) )
		{
			switch ( Event.Type )
			{
			case sf::Event::Closed:
				return false;
			case sf::Event::MouseWheelMoved:
				m_wheelDelta += Event.MouseWheel.Delta;
				break;
			}
        }

		return true;
	}

	int IInput::GetMouseX()
	{
		return m_input->GetMouseX();
	}

	int IInput::GetMouseY()
	{
		return m_input->GetMouseY();
	}

	int IInput::GetMouseWheel()
	{
		return m_wheelDelta;
	}

	bool IInput::IsMouseDown( MouseButton button )
	{
		return m_input->IsMouseButtonDown( (sf::Mouse::Button)button );
	}
}