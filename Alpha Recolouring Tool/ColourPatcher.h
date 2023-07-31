#pragma once
#include "SFML/Main.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <filesystem>

#include "Menu.h"
class ColourPatcher
{

private:

	sf::RenderWindow window;
	sf::Clock deltaClock;


	sf::Vector2i cursor_pos;

	Menu* menu = nullptr;

	struct Actions
	{
		sf::Color original_colour;
		sf::Color new_colour;
	};
	std::vector<Actions> actions;

	struct PaletteSquare
	{
		sf::RectangleShape palette_box;
		sf::Color palette_colour;
	};
	struct Images
	{
		sf::Image original_image;
		sf::Image new_image;

		sf::Sprite original_sprite;
		sf::Sprite new_sprite;

		sf::Texture original_tx;
		sf::Texture new_tx;
		std::string name;
		std::vector<PaletteSquare* > original_palette;
		std::vector<PaletteSquare* > new_palette;
	};
	std::vector<Images*> images;
	unsigned loaded = 0;

	// Window Specs
	std::string window_name = "ART 0.0.1";
	unsigned window_size_x = 1000;
	unsigned window_size_y = 800;
	bool window_resizeable = true;


	void RunLoop();

	void CursorMoved(sf::Vector2i pos);
	void LeftClick(sf::Vector2i pos);
	void RightClick(sf::Vector2i pos);

	void DoActions();
	void LoadImages();

	void Draw(sf::RenderWindow& window);

	void UpdateSprite(std::string name, Images* image);

	void GetPalette(Images* img, bool updated = false);


	int current_selection = 0;
	sf::RectangleShape image_slider;
	sf::RectangleShape image_slider_notch;
	struct SliderTexts
	{
		sf::Font font;
		sf::Text txt;
	};
	bool update_slider = false;
	sf::Font font;
	std::vector<SliderTexts*> slider_texts;
	sf::Text current_slide;


	sf::Text original_img_label;
	sf::Text new_img_label;
	sf::Text lastColourClicked;


public:
	void Start();
};

