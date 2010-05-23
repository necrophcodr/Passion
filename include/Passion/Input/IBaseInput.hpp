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
	};
}

#endif