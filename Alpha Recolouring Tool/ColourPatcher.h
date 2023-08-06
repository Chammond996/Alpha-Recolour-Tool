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

	struct ColourAction
	{
	private:
	public:
		ColourAction(sf::Color original, sf::Color new_colour) : original_colour(original), new_colour(new_colour) {};
		sf::Color original_colour;
		sf::Color new_colour;
		bool completed = false;
	};
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
		bool edited = false;
	};
	std::vector<ColourAction*> colourActions;
	std::vector<Images*> images;
	unsigned loaded = 0;

	// Window Specs
	std::string window_name = "ART 0.0.1";
	unsigned window_size_x = 1000;
	unsigned window_size_y = 800;
	bool window_resizeable = false;

	sf::Clock clickClock;


	void RunLoop();

	void CursorMoved(sf::Vector2i pos);
	void LeftClick(sf::Vector2i pos);
	void RightClick(sf::Vector2i pos);

	void DoActions();
	void LoadImages();
	void SaveImages();

	void Undo();

	void Draw(sf::RenderWindow& window);

	void UpdateSprite(std::string name, Images* image);

	void GetPalette(Images* img, bool updated = false);

	void ChangeColour(Images* image, sf::Color original, sf::Color newColour);

	sf::Font font;

	// Sprite selection
	int current_selection = 0;
	sf::RectangleShape image_slider;
	sf::RectangleShape image_slider_notch;
	struct SliderTexts
	{
		sf::Font font;
		sf::Text txt;
	};
	bool update_slider = false;
	std::vector<SliderTexts*> slider_texts;
	sf::Text current_slide;

	// Sprite Scale
	int current_scale = 1;
	enum SCALES {
		MIN = 1,
		MAX = 10
	};
	sf::Text ScaleText;
	bool updateScale = false;


	sf::Text original_img_label;
	sf::Text new_img_label;
	sf::Text lastColourClicked;
	sf::RectangleShape lastColourClickedBox;
	sf::Text paletteColourTargetedText;
	sf::RectangleShape paletteColourTargetedBox;
	sf::Color paletteColourTargerted;

	sf::Text test;


public:
	void Start();
};

