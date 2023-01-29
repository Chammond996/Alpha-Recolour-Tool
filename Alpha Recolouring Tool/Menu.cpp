#include "Menu.h"
#include <iostream>
#include <fstream>
#include <vector>

Menu::Menu(unsigned size_x, unsigned size_y, sf::Color bg, sf::Color bg_hover)
{
	this->menu_bar.setSize(sf::Vector2f(size_x, size_y));
	this->menu_bar_hover.setSize(sf::Vector2f(size_x, size_y));

	this->menu_bar.setFillColor(bg);
	this->menu_bar_hover.setFillColor(bg_hover);

	// Load font
	bool can_load_buttons = true;// this->LoadData();

	this->font.loadFromFile("font.ttf");

	if (!can_load_buttons)
	{
		std::cout << "Menu failed to load properly..\n";
		return;
	}
	Button* load_btn = new Button(sf::Vector2i(0, 0), sf::Vector2f(70, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Load", this->GetFont());
	this->buttons.emplace_back(load_btn);

	Button* save_btn = new Button(sf::Vector2i(70, 0), sf::Vector2f(70, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Save", this->GetFont());
	this->buttons.emplace_back(save_btn);

	Button* palette_btn = new Button(sf::Vector2i(140, 0), sf::Vector2f(70, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Palette", this->GetFont());
	this->buttons.emplace_back(palette_btn);

	Button* preview_btn = new Button(sf::Vector2i(210, 0), sf::Vector2f(70, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Preview", this->GetFont());
	this->buttons.emplace_back(preview_btn);

}
bool Menu::LoadData()
{
	// Open input file and read size
	std::ifstream inputFile("font.dat", std::ios::binary);
	if (!inputFile) {
		std::cerr << "Could not open font.dat for reading\n";
		return false;
	}

	unsigned int dataSize;
	inputFile.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));  // read size

	// Read font data
	std::vector<char> dataBuffer(dataSize);
	if (!inputFile.read(dataBuffer.data(), dataSize))
	{
		std::cerr << "Error reading font data from font.dat\n";
		return false;
	}

	if (!this->font.loadFromMemory(dataBuffer.data(), dataBuffer.size()))
		return false;
	
	return true;
}

void Menu::Draw(sf::RenderWindow& window)
{

	if (this->hover)
		window.draw(this->menu_bar_hover);
	else
		window.draw(this->menu_bar);

	for (auto btn : this->buttons)
	{
		if (btn->hover)
			window.draw(btn->bg_hover);
		else
			window.draw(btn->bg);

		window.draw(btn->text);

	}

}

void Menu::CursorMoved(sf::Vector2i pos)
{

	for (Button* btn : this->buttons)
	{
		if (btn->bg.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
			btn->hover = true;
		else
			btn->hover = false;

	}
}

void Menu::LeftClick(sf::Vector2i pos)
{
	for (Button* btn : this->buttons)
	{
		if (btn->bg.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
		{
			if (btn->GetName() == "Load")
				this->actions_to_call.push_back(btn->GetName());

			//std::cout << "Clicked button - " << btn->GetName() << "\n";
		}

	}
}
