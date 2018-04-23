#pragma once
#include<SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace fv {

	class PopMenu
	{
	private:
		//sf::Vector2f m_pos;
		std::vector<std::shared_ptr<sf::Text>> m_v;
		bool m_visible;
		//float m_unit_w;
		//float m_unit_h;
		float m_space;
		sf::Font* font;
		sf::Color m_textColor;
		sf::Color m_hover_Color;
		sf::Color m_bgColor;
		std::function<void(int)> m_onSelectFunc;
		int b_w; //最开始的宽
		int b_h;
		float n_w; //现在的宽
		float n_h;
		int last_hover_i;
	public:
		PopMenu();
		void init(int w,int h,const sf::Font *f);
		bool handlerEvent(sf::Event& event,bool auto_pop = true);
		void draw(sf::RenderWindow& w);
		void add(sf::String&& str);
		bool& visible();
		void setOnSelected(std::function<void(int)>&& func);
		void setFont(const sf::Font *f);
	private:
		void updatePos(float x, float y);
		void updateBgColor(sf::Vector2f&& p);
	};

}