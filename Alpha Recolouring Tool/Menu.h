#pragma once
#include "SFML/Graphics.hpp"


class Menu
{

	private:

		sf::Clock action_timer;
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
			bool show = false;
			bool palleteDropDown = false;

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

				this->show = true;
			};
			std::string GetName() { return this->name; }

		};
		struct PaletteSquare
		{
			sf::RectangleShape palette_box;
			sf::Color palette_colour;
		};

		class Palette
		{
		private:
		public:
			Palette(std::string name) : name(name){}

			std::string name = "";
			std::vector<PaletteSquare*> palette_squares;
			sf::RenderWindow palette_window;
			bool isOpen = false;
			bool needsClose = false;
		};

		std::vector<Palette*> palettes;

		std::vector<Button*> buttons;

		bool LoadData();

		void HelpWindow();

		void NewPalette();
		void LoadPalettes();
		void PopUpWindow(std::string title, std::string msg);
		void OpenPalette(std::string name);

public:
	Menu(unsigned size_x, unsigned size_y, sf::Color bg, sf::Color bg_hover);
	void TickPalettes();

	sf::Font GetFont() { return this->font; }

	void Draw(sf::RenderWindow& window);

	void CursorMoved(sf::Vector2i pos);
	int LeftClick(sf::Vector2i pos);
	void RightClick(sf::Vector2i pos);

	std::vector<std::string> actions_to_call;
};


