#pragma once
#include "Button.h"

struct PaletteSquare
{
	sf::RectangleShape palette_box;
	sf::Color palette_colour;
	bool targeted = false;
};

class Palette
{
private:

	int id = 0;

	std::string name = "";
	std::vector<PaletteSquare*> palette_squares;
	sf::RenderWindow palette_window;
	std::vector<sf::Color> paletteColours;
	bool isOpen = false;
	bool needsClose = false;
	std::vector<Button*> buttons;
	sf::Color activeColour;
	
	sf::Font font;
	const sf::Color BUTTON_BACKGROUND = sf::Color(99, 99, 99, 255);
	const sf::Color BUTTON_BACKGROUND_HOVER = sf::Color(135, 135, 135, 255);



	// Colour picker shit
	bool addingNewColour = false;
	double colourPickerHue = 0.0;
	sf::VertexArray colourWheel;

	void ModulateColourPicker(double hue);
	void CreatePaletteSquares();

	sf::Text colourWheelText;
	

	sf::Clock paletteActionClock;

	bool paletteColourUpdated = false;
	sf::Color colourTargeted;


	void DeleteColour(sf::Color colour);

public:
	Palette(std::string name, char id, sf::Font font) : name(name), font(font), id(id)
	{
		this->colourWheel = sf::VertexArray(sf::Points, 256 * 256);
		for (unsigned int y = 0; y <= 255; ++y)
		{
			for (unsigned int x = 0; x <= 255; ++x)
			{
				sf::Vertex& vertex(this->colourWheel[y * 256 + x]);

				vertex.position.x = x;
				vertex.position.y = y;
			}
		}
		this->colourWheelText.setPosition(290, 30);
		this->colourWheelText.setFont(this->font);
		this->colourWheelText.setCharacterSize(16);
		this->colourWheelText.setFillColor(this->BUTTON_BACKGROUND);
		this->colourWheelText.setString("Use arrows to change hue\n           <-- /  --> \nLeft click to sample\nRight click to set colour\n   R: n/a, G: n/a, B: n/a");
	}
	~Palette();

	bool IsColourUpdated() { return this->paletteColourUpdated; }
	sf::Color GetTargetedColour();

	void Tick();
	void Open();
	void Close();
	void Wipe();
	void AddColour(sf::Color colour);

	bool IsOpen() { return this->isOpen; }
	std::string GetName() { return this->name; }

	int GetID() { return this->id; }

	int GetSize();

	std::vector<sf::Color> GetColours();

};
