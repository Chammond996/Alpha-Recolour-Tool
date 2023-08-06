#include "Palette.h"
#include <iostream>

void Palette::Tick()
{
	if (this->needsClose)
	{
		this->palette_window.close();
		this->needsClose = false;
		this->isOpen = false;
		return;
	}

	if (this->palette_window.isOpen())
	{
		this->palette_window.clear(sf::Color::Black);

		sf::Event event;
		while (this->palette_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				this->palette_window.close();
				this->isOpen = false;
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (this->paletteActionClock.getElapsedTime().asMilliseconds() < 100)
					return;

				this->paletteActionClock.restart();

				std::cout << "Shifting colour picker hue [ <--- ] on palette [" << this->name << "]\n";
				this->colourPickerHue -= 0.012345;

				if (this->colourPickerHue < 0)
					this->colourPickerHue = 0.0;

				this->ModulateColourPicker(this->colourPickerHue);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (this->paletteActionClock.getElapsedTime().asMilliseconds() < 100)
					return;

				this->paletteActionClock.restart();
				std::cout << "Shifting colour picker hue [ ---> ] on palette [" << this->name << "]\n";
				this->colourPickerHue += 0.012345;

				if (this->colourPickerHue > 1)
					this->colourPickerHue = 1.0;

				this->ModulateColourPicker(this->colourPickerHue);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (this->paletteActionClock.getElapsedTime().asMilliseconds() < 100)
					return;

				this->paletteActionClock.restart();

				if (this->addingNewColour)
				{

					// Get the mouse position in window coordinates
					sf::Vector2i mousePosition = sf::Mouse::getPosition(this->palette_window);

					// Check if the mouse is within the color wheel's boundaries
					if (mousePosition.x >= 0 && mousePosition.x < 256 && mousePosition.y >= 0 && mousePosition.y < 256)
					{
						this->AddColour(this->activeColour);
						this->CreatePaletteSquares();
						this->addingNewColour = false;
						break;
					}
				}
				else
				{
					bool updatePaletteColours = false;
					for (auto &boxes : this->palette_squares)
					{
						auto &box = boxes;
						if (box->palette_box.getGlobalBounds().contains(sf::Mouse::getPosition(this->palette_window).x, sf::Mouse::getPosition(this->palette_window).y))
						{
							if (box->targeted)
							{
								this->DeleteColour(box->palette_colour);
								updatePaletteColours = true;
								break; // Exit the loop early, no need to continue after erasing.
							}
							else
							{
								// If the box is not targeted, mark it as targeted.
								box->targeted = true;
								//box->palette_box.setOutlineThickness(1.5);
								break;

							}
						}
					}
					
					if(updatePaletteColours)
						this->CreatePaletteSquares();

				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (this->addingNewColour)
				{
					if (this->paletteActionClock.getElapsedTime().asMilliseconds() < 100)
						return;

					this->paletteActionClock.restart();

					// Get the mouse position in window coordinates
					sf::Vector2i mousePosition = sf::Mouse::getPosition(this->palette_window);

					// Check if the mouse is within the color wheel's boundaries
					if (mousePosition.x >= 0 && mousePosition.x < 256 && mousePosition.y >= 0 && mousePosition.y < 256)
					{
						// Calculate the vertex index corresponding to the mouse position
						int vertexIndex = mousePosition.y * 256 + mousePosition.x;

						// Retrieve the color of the clicked vertex
						sf::Color clickedColor = this->colourWheel[vertexIndex].color;
						this->activeColour = clickedColor;
						this->colourWheelText.setString("Use arrows to change hue\n           <-- /  --> \nLeft click to sample\nRight click to set colour\n   R: " + std::to_string(clickedColor.r) + ", G: " + std::to_string(clickedColor.g) + ", B : " + std::to_string(clickedColor.b));
						this->colourWheelDemo.setFillColor(clickedColor);
						break;
					}
				}
				std::cout << "Clicked Palette " << this->name << "\n";
				for (auto &btn : this->buttons)
				{
					if(!btn->bg.getGlobalBounds().contains(sf::Mouse::getPosition(this->palette_window).x, sf::Mouse::getPosition(this->palette_window).y))
						continue;
					std::cout << "Clicked button " << btn->name << "\n";
					if (btn->name == "Add Colour") // should be the only one for now
					{
						this->addingNewColour = true;
						this->ModulateColourPicker(this->colourPickerHue);
					}
				}
				
				if (this->addingNewColour) continue;

				for (auto& box : this->palette_squares)
				{
					box->palette_box.setOutlineThickness(0.0);
					if (box->palette_box.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->palette_window).x, sf::Mouse::getPosition(this->palette_window).y)))
					{
						box->palette_box.setOutlineThickness(1.5);
						box->palette_box.setOutlineColor(sf::Color::Yellow);
						this->paletteColourUpdated = true;
						this->colourTargeted = box->palette_box.getFillColor();
					}
				}
			}
		}

		if (!this->addingNewColour)
		{
			for (auto& palette_square : this->palette_squares)
				this->palette_window.draw(palette_square->palette_box);

			for (auto &btn : this->buttons)
			{
				if (!btn->show)
					continue;

				if (btn->hover)
					this->palette_window.draw(btn->bg_hover);
				else
					this->palette_window.draw(btn->bg);

				this->palette_window.draw(btn->text);
			}
		}
		else
		{
			this->palette_window.draw(this->colourWheel);
			this->palette_window.draw(this->colourWheelText);
			this->palette_window.draw(this->colourWheelDemo);
		}

		if (this->paletteActionClock.getElapsedTime().asSeconds() > 5)
		{
			for (auto& box : this->palette_squares)
				box->targeted = false;
		}

		//this->ModulateColourPicker(this->paletteActionClock.getElapsedTime().asSeconds() / 5);

		this->palette_window.display();
	}
}
sf::Color Palette::GetTargetedColour()
{
	this->paletteColourUpdated = false;

	return this->colourTargeted;
}
void Palette::Open()
{
	if (!this->isOpen)
	{
		this->isOpen = true;
		this->palette_window.create(sf::VideoMode(500, 300), name, sf::Style::Close);
		this->palette_window.setIcon(this->icon_dimensions.x, this->icon_dimensions.y, this->icon_pixels.data());

		Button* btn_new = new Button(sf::Vector2i(5, 5), sf::Vector2f(130, 30), this->BUTTON_BACKGROUND, this->BUTTON_BACKGROUND_HOVER, "Add Colour", this->font);
		this->buttons.emplace_back(btn_new);

		this->CreatePaletteSquares();
	}
}

void Palette::Close()
{
	// Let the tick close it aye
	this->needsClose = true;
}
void Palette::DeleteColour(sf::Color colour)
{
	std::vector<sf::Color> temp_colours = this->paletteColours;
	this->paletteColours.clear();
	
	for (auto temp_c : temp_colours)
	{
		if (temp_c != colour)
			this->paletteColours.emplace_back(temp_c);
	}
	

	PaletteSquare* square_to_remove = nullptr;
	for (auto& square : this->palette_squares)
	{
		if (square->palette_colour == colour)
			square_to_remove = square;
	}
	if (square_to_remove)
	{
		this->palette_squares.erase(std::remove(this->palette_squares.begin(), this->palette_squares.end(), square_to_remove));
		delete square_to_remove;
	}
	
}
void Palette::AddColour(sf::Color colour)
{
	PaletteSquare* new_square = new PaletteSquare();
	new_square->palette_colour = colour;
	new_square->palette_box.setSize(sf::Vector2f(20, 20));
	new_square->palette_box.setFillColor(colour);

	this->palette_squares.emplace_back(new_square);
	this->paletteColours.emplace_back(colour);
}
Palette::~Palette()
{

}
void Palette::Wipe()
{
	for (auto& square : this->palette_squares)
		delete square;
	for (auto& btn : this->buttons)
		delete btn;

	this->palette_squares.clear();
	this->paletteColours.clear();
	this->buttons.clear();
}
int Palette::GetSize()
{
	return this->palette_squares.size() > 0 ? this->palette_squares.size() : 0;
}
std::vector<sf::Color> Palette::GetColours()
{
	return this->paletteColours;
}
void Palette::ModulateColourPicker(double hue)
{
	// First, Let's "sanitize" inputs a bit.
	// Don't accept negative numbers.
	if (hue < 0)
		hue = 0;
	// Lazy overflow by subtracting the integer portion of the number.
	else if (hue > 1)
		hue -= static_cast<int>(hue);

	// Now iterate over all "pixels" and upate their colours.
	for (unsigned int y = 0; y <= 255; ++y) {
		for (unsigned int x = 0; x <= 255; ++x) {
			// "Calculate" our missing HSV components with ranges from 0 to 1.
			const double s = x / 255.; // x is our saturation
			const double v = y / 255.; // y is our value

			// Pick the correct case based on our position on the colour wheel.
			const int cs = hue * 6;

			// Calculate some helper values used in our cases below.
			const double f = hue * 6 - cs;
			const double p = v * (1 - s);
			const double q = v * (1 - s * f);
			const double t = v * (1 - s * (1 - f));

			switch (cs) {
			case 0:
			case 6:
				this->colourWheel[y * 256 + x].color = sf::Color(v * 255, t * 255, p * 255);
				break;
			case 1:
				this->colourWheel[y * 256 + x].color = sf::Color(q * 255, v * 255, p * 255);
				break;
			case 2:
				this->colourWheel[y * 256 + x].color = sf::Color(p * 255, v * 255, t * 255);
				break;
			case 3:
				this->colourWheel[y * 256 + x].color = sf::Color(p * 255, q * 255, v * 255);
				break;
			case 4:
				this->colourWheel[y * 256 + x].color = sf::Color(t * 255, p * 255, v * 255);
				break;
			case 5:
				this->colourWheel[y * 256 + x].color = sf::Color(v * 255, p * 255, q * 255);
				break;
			}
		}
	}
}
void Palette::CreatePaletteSquares()
{
	int y_ = 50;
	int x_ = 0;
	int row_spacing = 30;
	int column_spacing = 30;

	for (size_t i = 0; i < this->palette_squares.size(); i++)
	{
		// Check if the index is within the valid range of the vector.
		if (i >= this->palette_squares.size() || this->palette_squares[i] == nullptr)
			continue;

		// Calculate the row and column positions based on 'i' (count_ in your original code).
		int row = i / 10;      // Integer division to determine the row (0 to 9).
		int column = i % 10;   // Modulo to determine the column (0 to 9).

		// Calculate the actual position using row and column information, along with spacing.
		x_ = column * row_spacing;
		y_ = row * column_spacing;

		if (i % 10 == 0)
			x_ = 0;

		x_ += 10;
		y_ += 50;

		this->palette_squares[i]->palette_box.setPosition(x_, y_);
		this->palette_squares[i]->palette_box.setOutlineColor(sf::Color::Yellow);
	}
}
