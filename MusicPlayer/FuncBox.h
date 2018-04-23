#pragma once

namespace sf {
	class Event;
}

namespace sfg {
	class ScrolledWindow;
}


namespace fb {
	void handlerScrolledWindow(sf::Event& event, sfg::ScrolledWindow &sw);
}

