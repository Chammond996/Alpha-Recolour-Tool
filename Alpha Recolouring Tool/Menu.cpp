#include "Menu.h"
#include <iostream>
#include <fstream>
#include <vector>

Menu::Menu(unsigned size_x, unsigned size_y, sf::Color bg, sf::Color bg_hover)
{
	//this->icon_image = icon_image;
	this->menu_bar.setSize(sf::Vector2f(size_x, size_y));
	this->menu_bar_hover.setSize(sf::Vector2f(size_x, size_y));

	this->menu_bar.setFillColor(bg);
	this->menu_bar_hover.setFillColor(bg_hover);

	// Load Datapack
	bool loaded_data = this->LoadData();


	if (!loaded_data)
	{
		std::cout << "Menu failed to load properly..\n";
		return;
	}
	Button* load_btn = new Button(sf::Vector2i(0, 0), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Load", this->GetFont());
	this->buttons.emplace_back(load_btn);

	Button* save_btn = new Button(sf::Vector2i(100, 0), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Save", this->GetFont());
	this->buttons.emplace_back(save_btn);

	Button* palette_btn = new Button(sf::Vector2i(200, 0), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Palette", this->GetFont());
	this->buttons.emplace_back(palette_btn);

	Button* preview_btn = new Button(sf::Vector2i(300, 0), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Preview", this->GetFont());
	this->buttons.emplace_back(preview_btn);

	Button* help_btn = new Button(sf::Vector2i(400, 0), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "Help", this->GetFont());
	this->buttons.emplace_back(help_btn);


	// Palette shit

	Button* palette_new_btn = new Button(sf::Vector2i(200, 40), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), "New Palette", this->GetFont());
	palette_new_btn->show = false;
	palette_new_btn->palleteDropDown = true;
	this->buttons.emplace_back(palette_new_btn);

	this->LoadPaletteColours();
	this->LoadPalettes();

	this->loaded = true;

}
void Menu::Save()
{
	this->SavePaletteColours();
}
bool Menu::LoadData()
{
	std::ifstream inputFile("data.pack", std::ios::binary);
	if (inputFile)
	{
		// Read the width and height of the image

		inputFile.read(reinterpret_cast<char*>(&this->icon_dimensions.x), sizeof(this->icon_dimensions.x));
		inputFile.read(reinterpret_cast<char*>(&this->icon_dimensions.y), sizeof(this->icon_dimensions.y));

		// Read the pixel data
		std::size_t size = this->icon_dimensions.x * this->icon_dimensions.y * 4;
		this->icon_pixels.resize(size);
		inputFile.read(reinterpret_cast<char*>(this->icon_pixels.data()), size);
		inputFile.close();
		//this->window.setIcon(width, height, this->icon_pixels.data());
	}

	if (!this->font.loadFromFile("font.ttf"))
		return false;

	return true;
}

void Menu::HelpWindow()
{

	sf::RenderWindow helpWindow;
	
	std::string helpInfo = "Load - Loads all images from the 'images/' folder. \n";
				helpInfo += "Save - Saves all modified images to the 'recolours/' folder. \n";
				helpInfo += "Palette - Select a palette or create new ones \n";
				helpInfo += "Preview - Apply the changes. \n";
				helpInfo += "Left + Right Arrows - Scrolls the slider\n";


	sf::Text helpText;
	helpText.setString(helpInfo);
	helpText.setCharacterSize(18);
	helpText.setFillColor(sf::Color::Black);//sf::Color(99, 99, 99, 255));
	helpText.setFont(this->font);
	helpText.setPosition(10, 10);

	helpWindow.create(sf::VideoMode(helpText.getLocalBounds().width+20, helpText.getLocalBounds().height+20), "Alpha Recolouring Tool Help", sf::Style::Close);
							

	while (helpWindow.isOpen())
	{
		sf::Event event;
		while (helpWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				helpWindow.close();
				break;
			}
		}

		helpWindow.clear(this->menu_bar_hover.getFillColor()); //sf::Color(15, 15, 15));
		helpWindow.draw(helpText);
		helpWindow.display();
		sf::sleep(sf::milliseconds(100));
		
	}
}

void Menu::NewPalette(bool force)
{
	if (this->action_timer.getElapsedTime() < sf::seconds(3) && !force)
		return;

	this->action_timer.restart();

	if (this->palettes.size() >= 10)
	{
		this->PopUpWindow("Uhh Oh!!", "Maximum palettes reached (10)");
		return;
	}
	Palette* new_palette = new Palette("Palette " + std::to_string(this->palettes.size()+1), this->palettes.size()+1, this->font);
	this->palettes.emplace_back(new_palette);

	this->LoadPalettes();

	this->CursorMoved(this->cursorPos);
}
void Menu::LoadPalettes()
{
	int _count = 0;
	int yOffset = 80;
	for (auto palette : this->palettes)
	{
		palette->Wipe();
		_count++;
		yOffset = 40 * _count;
		Button* palette_new_btn = new Button(sf::Vector2i(200, 40 + yOffset), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), palette->GetName(), this->GetFont());
		palette_new_btn->show = false;
		palette_new_btn->palleteDropDown = true;
		this->buttons.emplace_back(palette_new_btn);

		for (auto& colour : this->palette_colours[palette->GetID()])
			palette->AddColour(colour);
	}
}

void Menu::OpenPalette(std::string name)
{
	for (auto palette : this->palettes)
	{
		if (palette->GetName() == name)
		{
			palette->Open();
			return;
		}
	}


}

Menu::~Menu()
{
	for (auto palette : this->palettes)
		delete palette;
	this->palettes.clear();
}

void Menu::TickPalettes()
{

	for (auto palette : this->palettes)
	{
		palette->Tick();

		if (palette->IsColourUpdated())
		{
			this->paletteColourTargeted = palette->GetTargetedColour();
			std::cout << "Menu Colour update! New colour targeted from palette " << palette->GetName() << "\n";
			this->paletteTargetColourUpdated = true;
		}
	}
}
bool Menu::PaletteTargetColourUpdated()
{
	if (this->paletteTargetColourUpdated)
	{
		this->paletteTargetColourUpdated = false;
		return true;
	}
	return false;
}

void Menu::Draw(sf::RenderWindow& window)
{

	if (this->hover)
		window.draw(this->menu_bar_hover);
	else
		window.draw(this->menu_bar);

	for (auto btn : this->buttons)
	{
		if (!btn->show)
			continue;

		if (btn->hover)
			window.draw(btn->bg_hover);
		else
			window.draw(btn->bg);

		window.draw(btn->text);

	}

}

void Menu::CursorMoved(sf::Vector2i pos)
{
	this->cursorPos = pos;

	bool show_palette_subs = false;
	for (Button* btn : this->buttons)
	{
		if (btn->bg.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
			btn->hover = true;
		else
			btn->hover = false;

		if (btn->bg.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)) && btn->GetName().find("Palette") != std::string::npos)
		{
			show_palette_subs = true;
		}

	}
	for (auto sub : this->buttons)
	{
		if (sub->palleteDropDown)
			sub->show = show_palette_subs ? true : false;
	}
}

int Menu::LeftClick(sf::Vector2i pos)
{
	for (Button* btn : this->buttons)
	{
		if (btn->bg.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
		{
			if (btn->GetName() == "Load")
				this->actions_to_call.push_back(btn->GetName());
			else if (btn->GetName() == "Help")
				this->HelpWindow();
			else if (btn->GetName() == "New Palette")
				this->NewPalette();

			for (auto palette : this->palettes)
			{
				if (btn->GetName() == palette->GetName())
				{
					if (palette->IsOpen())
					{
						palette->Close();
						break;
					}
					this->OpenPalette(btn->GetName());
					break;
				}
			}

			//std::cout << "Clicked button - " << btn->GetName() << "\n";
			break;
		}

	}
	return 0;
}

void Menu::RightClick(sf::Vector2i pos)
{

	
}

void Menu::SavePaletteColours()
{

	std::ofstream outputFile("data.pack", std::ios::binary);

	// store the icon regardless
	unsigned int icon_width = this->icon_dimensions.x;
	unsigned int icon_height = this->icon_dimensions.y;
	outputFile.write(reinterpret_cast<char*>(&icon_width), sizeof(icon_width));
	outputFile.write(reinterpret_cast<char*>(&icon_height), sizeof(icon_height));
	outputFile.write(reinterpret_cast<const char*>(this->icon_pixels.data()), icon_width * icon_height * 4);

	char pal_count = 1;
	for (auto& palette : this->palettes)
	{
		std::vector<sf::Color> palette_colours = palette->GetColours();

		outputFile.write(reinterpret_cast<char*>(&pal_count), 1); // pal ID/number

		// Assuming palette_colours is a std::vector<sf::Color>
		unsigned char num_colors = static_cast<unsigned char>(palette_colours.size());
		outputFile.write(reinterpret_cast<char*>(&num_colors), 1); // should be less than 200 colours per palette

		std::vector<sf::Color> dupe_check_list;
		for (sf::Color &colour : palette_colours)
		{
			bool dupe_found = false;
			for (auto& dupe_check : dupe_check_list)
			{
				if (dupe_check == colour)
				{
					dupe_found = true;
					break;
				}
			}
			if (dupe_found)
				continue;

			outputFile.write(reinterpret_cast<char*>(&colour.r), 1);
			outputFile.write(reinterpret_cast<char*>(&colour.g), 1);
			outputFile.write(reinterpret_cast<char*>(&colour.b), 1);
			outputFile.write(reinterpret_cast<char*>(&colour.a), 1);

			std::cout << "saving colour on palette " << static_cast<int>(pal_count) << " R: " << static_cast<int>(colour.r) << ", G: " << static_cast<int>(colour.g) << ", B: " << static_cast<int>(colour.b) << ", A: " << static_cast<int>(colour.a) << "\n";
		}

		pal_count++;
	}




	outputFile.close();
}
void Menu::LoadPaletteColours()
{
	std::ifstream inputFile("data.pack", std::ios::binary);
	if (inputFile)
	{
		// Read the width and height of the image
		unsigned int width, height;
		inputFile.read(reinterpret_cast<char*>(&width), sizeof(width));
		inputFile.read(reinterpret_cast<char*>(&height), sizeof(height));

		// Skip the icon
		inputFile.seekg(width * height * 4, std::ios::cur);

		while (inputFile.peek() != EOF)
		{
			unsigned char pal_number = 0;   // Use unsigned char for 1-byte value
			unsigned char pal_colour_amt = 0;  // Use unsigned char for 1-byte value
			inputFile.read(reinterpret_cast<char*>(&pal_number), 1);
			inputFile.read(reinterpret_cast<char*>(&pal_colour_amt), 1);

			for (int i = 0; i < pal_colour_amt; i++)
			{
				unsigned char r, g, b, a;
				inputFile.read(reinterpret_cast<char*>(&r), 1);
				inputFile.read(reinterpret_cast<char*>(&g), 1);
				inputFile.read(reinterpret_cast<char*>(&b), 1);
				inputFile.read(reinterpret_cast<char*>(&a), 1);
				this->palette_colours[pal_number].push_back(sf::Color(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a)));
			}

			std::cout << "FOUND Palette " << static_cast<int>(pal_number) << " storing " << static_cast<int>(pal_colour_amt) << " colours!\n";
			this->NewPalette(true);

		}

		inputFile.close();
	}
}

void Menu::PopUpWindow(std::string title, std::string msg)
{
	sf::RenderWindow popUpWindow;

	std::string popUpMsg = msg;


	sf::Text popUpText;
	popUpText.setString(popUpMsg);
	popUpText.setCharacterSize(18);
	popUpText.setFillColor(sf::Color::Black);//sf::Color(99, 99, 99, 255));
	popUpText.setFont(this->font);
	popUpText.setPosition(10, 10);

	unsigned _width = std::min(int(popUpText.getLocalBounds().width + 20), 300);

	popUpWindow.create(sf::VideoMode(_width, popUpText.getLocalBounds().height + 20), title, sf::Style::Close);


	while (popUpWindow.isOpen())
	{
		sf::Event event;
		while (popUpWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				popUpWindow.close();
				break;
			}
		}

		popUpWindow.clear(this->menu_bar_hover.getFillColor()); //sf::Color(15, 15, 15));
		popUpWindow.draw(popUpText);
		popUpWindow.display();
		sf::sleep(sf::milliseconds(100));

	}
}
