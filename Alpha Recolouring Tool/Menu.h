#pragma once
#include "SFML/Graphics.hpp"


class Menu
{

	private:

		sf::RectangleShape menu_bar, menu_bar_hover;
		bool hover = false;

		sf::Font font;
		class Button
		{
		public:

			sf::Vector2i position;
			sf::Vector2f size;
			sf::RectangleShape bg;
			sf::RectangleShape bg_hover;
			sf::Color bg_colour, bg_colour_hover;
			sf::Text text;
			sf::Font font;
			bool hover = false;
			std::string name = "";

			Button(sf::Vector2i position, sf::Vector2f size, sf::Color bg_colour, sf::Color bg_colour_hover, std::string name, sf::Font font)// :
				//	position(position), size(size), bg_colour(bg_colour), bg_colour_hover(bg_colour_hover)
			{
				this->bg.setPosition(position.x, position.y);
				this->bg_hover.setPosition(position.x, position.y);

				this->bg.setFillColor(bg_colour);
				this->bg_hover.setFillColor(bg_colour_hover);
				this->name = name;
				this->bg.setSize(size);
				this->bg_hover.setSize(size);
				this->font = font;
				this->text.setFont(this->font);
				this->text.setCharacterSize(14);
				this->text.setFillColor(sf::Color::Black);
				this->text.setString(name);
				this->text.setPosition(position.x + ((size.x - this->text.getLocalBounds().width) / 2), position.y + ((size.y - this->text.getLocalBounds().height) / 2));

			};
			std::string GetName() { return this->name; }

		};
		std::vector<Button*> buttons;

		bool LoadData();

public:
	Menu(unsigned size_x, unsigned size_y, sf::Color bg, sf::Color bg_hover);

	sf::Font GetFont() { return this->font; }

	void Draw(sf::RenderWindow& window);

	void CursorMoved(sf::Vector2i pos);
	void LeftClick(sf::Vector2i pos);

	std::vector<std::string> actions_to_call;
};


