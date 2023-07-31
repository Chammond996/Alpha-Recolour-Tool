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

	if(!this->font.loadFromFile("font.ttf"))
		can_load_buttons = false;

	if (!can_load_buttons)
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

	this->LoadPalettes();

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

void Menu::NewPalette()
{
	if (this->action_timer.getElapsedTime() < sf::seconds(3))
		return;

	this->action_timer.restart();

	if (this->palettes.size() >= 10)
	{
		this->PopUpWindow("Uhh Oh!!", "Maximum palettes reached (10)");
		return;
	}
	Palette* new_palette = new Palette("Palette " + std::to_string(this->palettes.size()+1), this->font);
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
		_count++;
		yOffset = 40 * _count;
		Button* palette_new_btn = new Button(sf::Vector2i(200, 40 + yOffset), sf::Vector2f(100, 40), this->menu_bar.getFillColor(), this->menu_bar_hover.getFillColor(), palette->GetName(), this->GetFont());
		palette_new_btn->show = false;
		palette_new_btn->palleteDropDown = true;
		this->buttons.emplace_back(palette_new_btn);
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

void Menu::TickPalettes()
{

	for (auto palette : this->palettes)
	{
		palette->Tick();
	}
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
