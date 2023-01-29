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


	// Window Specs
	std::string window_name = "ART 0.0.1";
	unsigned window_size_x = 1000;
	unsigned window_size_y = 800;
	bool window_resizeable = true;

	void RunLoop();

	void CursorMoved(sf::Vector2i pos);
	void LeftClick(sf::Vector2i pos);

	void DoActions();
	void LoadImages();

	void Draw(sf::RenderWindow& window);

	struct Actions
	{
		sf::Color original_colour;
		sf::Color new_colour;
	};
	std::vector<Actions> actions;

	struct Images
	{
		sf::Image original;
		sf::Image image;
		std::string name;
	};
	std::vector<Images*> images;
	unsigned loaded = 0;


	unsigned current_selection = 0;
	sf::RectangleShape image_slider;
	struct SliderTexts
	{
		sf::Font font;
		sf::Text txt;
	};
	sf::Font font;
	std::vector<SliderTexts*> slider_texts;
	sf::Text current_slide;


public:
	void Start();
};

