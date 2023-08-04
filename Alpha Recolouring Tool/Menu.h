#pragma once

#include "Palette.h"
#include "Button.h"


class Menu
{

	private:

		sf::Clock action_timer;
		sf::RectangleShape menu_bar, menu_bar_hover;
		bool hover = false;

		sf::Font font;

		std::vector<Palette*> palettes;

		std::vector<Button*> buttons;

		sf::Vector2i cursorPos;

		bool LoadData();

		void HelpWindow();

		sf::Color paletteColourTargeted;
		bool paletteTargetColourUpdated = false;

		void NewPalette(bool force = false);
		void LoadPalettes();
		void PopUpWindow(std::string title, std::string msg);
		void OpenPalette(std::string name);
		~Menu();


		void LoadPaletteColours();
		void SavePaletteColours();
		std::vector<sf::Color> palette_colours[10];
		

public:
	Menu(unsigned size_x, unsigned size_y, sf::Color bg, sf::Color bg_hover);

	std::vector<sf::Uint8> icon_pixels;
	sf::Vector2i icon_dimensions;
	bool loaded = false;


	void TickPalettes();

	sf::Font GetFont() { return this->font; }

	void Draw(sf::RenderWindow& window);

	void CursorMoved(sf::Vector2i pos);
	int LeftClick(sf::Vector2i pos);
	void RightClick(sf::Vector2i pos);

	std::vector<std::string> actions_to_call;

	bool PaletteTargetColourUpdated();

	sf::Color GetPaletteTargetColour() { return this->paletteColourTargeted; }

	void Save();
};


