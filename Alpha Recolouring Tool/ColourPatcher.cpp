#include "ColourPatcher.h""

#include <iostream>
#include <filesystem>

void ColourPatcher::Start()
{
	if(this->window_resizeable)
		this->window.create(sf::VideoMode(this->window_size_x, this->window_size_y), this->window_name, sf::Style::Default);
	else
		this->window.create(sf::VideoMode(this->window_size_x, this->window_size_y), this->window_name, sf::Style::Close);

	this->menu = new Menu(this->window_size_x, 40, sf::Color(99, 99, 99, 255), sf::Color(135, 135, 135, 255));

	this->font.loadFromFile("font.ttf");
	this->RunLoop();
}

void ColourPatcher::RunLoop()
{

	while (this->window.isOpen())
	{

		if (this->menu)
			this->DoActions();

		sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				this->window.close();
				break;
			}

			else if (event.type == sf::Event::MouseMoved)
			{
				this->CursorMoved(sf::Mouse::getPosition(window));
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->LeftClick(sf::Mouse::getPosition(window));
			}
		}
		

		this->window.clear(sf::Color(15, 15, 15));// sf::Color(18, 33, 43)); // Color background

		if (this->menu)
			this->menu->Draw(this->window);

		this->Draw(window);

		this->window.display();

		sf::sleep(sf::milliseconds(100));
	}
}

void ColourPatcher::LeftClick(sf::Vector2i pos)
{
	this->menu->LeftClick(pos);
}
void ColourPatcher::DoActions()
{
	for (auto act : this->menu->actions_to_call)
	{
		if (act == "Load")
			this->LoadImages();

		std::cout << "Completing action : " << act << "\n";
	}
	this->menu->actions_to_call.clear();
}
void ColourPatcher::CursorMoved(sf::Vector2i pos)
{
	this->cursor_pos = pos;

	this->menu->CursorMoved(pos);

}
void ColourPatcher::LoadImages()
{
	std::cout << "Attempting to load images..\n";
	std::string directory_path = "images/";

	namespace fs = std::filesystem;
	
	this->loaded = 0;
	this->current_selection = 0;

	try {
		for (const auto& entry : fs::directory_iterator(directory_path)) {
			if (entry.is_regular_file()) {
				if (entry.path().extension() == ".png" || entry.path().extension() == ".bmp")
				{
					Images* img = new Images();
					img->original.loadFromFile("images/" + entry.path().filename().string());
					this->images.emplace_back(img);
					this->loaded++;
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
	}


	if (this->loaded > 0)
	{
		this->image_slider.setSize(sf::Vector2f(this->window_size_x - 200, 10));
		this->image_slider.setPosition(100, 100);
		this->image_slider.setFillColor(sf::Color(99, 99, 99, 255));


		this->current_slide.setFont(this->font);
		this->current_slide.setCharacterSize(14);
		this->current_slide.setFillColor(sf::Color(99, 99, 99, 255));
		this->current_slide.setString("Current Slide: " + std::to_string(this->current_selection));
		this->current_slide.setPosition(sf::Vector2f((this->window_size_x / 2) - 20 , 110));

		int i = 0;
		int amount = 0;

		while (i < this->loaded)
		{
			i += 5;
			amount += 1;
		}

		// Calculate the distance between each label on the slider
		float distance_between_labels = (this->window_size_x - 200) / static_cast<float>(amount);

		for (int i = 0; i <= amount; i++)
		{
			unsigned num_to_display = i * 5;

			if (i == amount)
				num_to_display = this->loaded;

			SliderTexts* text = new SliderTexts();
			text->font = this->menu->GetFont();
			text->txt.setFont(text->font);
			text->txt.setCharacterSize(14);
			text->txt.setFillColor(sf::Color(99, 99, 99, 255));
			text->txt.setString(std::to_string(num_to_display));
			// Calculate the x position of the label
			float x_position = 100 + i * distance_between_labels;
			text->txt.setPosition(sf::Vector2f(x_position, 80));
			this->slider_texts.emplace_back(text);
		}
	}
}

void ColourPatcher::Draw(sf::RenderWindow& window)
{


	if (this->loaded > 0)
	{
		window.draw(this->image_slider);

		for (auto slider_numbers : this->slider_texts)
			window.draw(slider_numbers->txt);

		window.draw(this->current_slide);
	}
}
