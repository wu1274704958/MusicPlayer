#include "FuncBox.h"
#include <SFML\Graphics.hpp>
#include <SFGUI\ScrolledWindow.hpp>
#include <SFGUI\Adjustment.hpp>


void fb::handlerScrolledWindow(sf::Event & event, sfg::ScrolledWindow & sw)
{
	sf::FloatRect rect = { sw.GetAbsolutePosition().x, sw.GetAbsolutePosition().y ,sw.GetAllocation().width, sw.GetAllocation().height };
	if (rect.contains(event.mouseWheel.x, event.mouseButton.y))
	{
		sw.GetVerticalAdjustment()->SetValue(sw.GetVerticalAdjustment()->GetValue() + 30.f * -event.mouseWheel.delta);
	}
}
