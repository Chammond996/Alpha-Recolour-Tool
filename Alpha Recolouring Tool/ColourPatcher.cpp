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

			if (!this->window.hasFocus())
				continue;

			else if (event.type == sf::Event::MouseMoved)
			{
				this->CursorMoved(sf::Mouse::getPosition(window));
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->LeftClick(sf::Mouse::getPosition(window));
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				this->RightClick(sf::Mouse::getPosition(window));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (++this->current_selection > this->images.size()-1)
				this->current_selection = 0;

				this->update_slider = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (--this->current_selection < 0)
					this->current_selection = this->images.size() -1;

				this->update_slider = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				for (auto pal : this->images[this->current_selection]->original_palette)
					std::cout << std::to_string(pal->palette_box.getFillColor().r) << "," << std::to_string(pal->palette_box.getFillColor().b) << ","
					<< std::to_string(pal->palette_box.getFillColor().b) << "," << std::to_string(pal->palette_box.getFillColor().a) << "\n";
			}
		}

		if (this->update_slider)
		{
			this->update_slider = false;
			int position_to_mark = 100 + ((this->window_size_x - 200) / this->images.size()) * this->current_selection;

			this->current_slide.setString("Current Slide: " + std::to_string(this->current_selection));
			this->image_slider_notch.setPosition(position_to_mark, 90);
		}
		

		this->window.clear(sf::Color(15, 15, 15));// sf::Color(18, 33, 43)); // Color background


		this->Draw(window);

		if (this->menu)
		{
			this->menu->TickPalettes();
			this->menu->Draw(this->window);
		}

		this->window.display();

		sf::sleep(sf::milliseconds(60));
	}
}

void ColourPatcher::LeftClick(sf::Vector2i pos)
{
	if (!this->menu)
		return;
	this->menu->LeftClick(pos);

	for (auto txt : this->slider_texts)
	{
		if (txt->txt.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
		{
			this->current_selection = std::stoi(txt->txt.getString().toAnsiString());
			this->update_slider = true;
			break;
		}
	}
	if (this->loaded > 0)
	{
		for (auto box : this->images[this->current_selection]->original_palette)
		{
			if (box->palette_box.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
			{
				sf::Color box_colour = box->palette_box.getFillColor();
				this->lastColourClicked.setString("Last Colour Clicked -- R:" + std::to_string(box_colour.r) + ", G:" + std::to_string(box_colour.g) + ", B:" + std::to_string(box_colour.b));
				break;
			}
		}
		for (auto box : this->images[this->current_selection]->new_palette)
		{
			if (box->palette_box.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
			{
				sf::Color box_colour = box->palette_box.getFillColor();
				this->lastColourClicked.setString("Last Colour Clicked -- R:" + std::to_string(box_colour.r) + ", G:" + std::to_string(box_colour.g) + ", B:" + std::to_string(box_colour.b));
				break;
			}
		}
	}
}
void ColourPatcher::RightClick(sf::Vector2i pos)
{
	if (!this->menu)
		return;

	this->menu->RightClick(pos);
}
void ColourPatcher::DoActions()
{
	for (auto act : this->menu->actions_to_call)
	{
		std::cout << "Completing action : " << act << "\n";

		if (act == "Load")
			this->LoadImages();

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

					img->original_image.loadFromFile("images/" + entry.path().filename().string());
					img->original_tx.create(img->original_image.getSize().x, img->original_image.getSize().y);
					img->original_tx.update(img->original_image);
					img->original_sprite.setTexture(img->original_tx);
					img->original_sprite.setPosition((this->window_size_x / 2) - (100 + img->original_image.getSize().x), 200);
					img->name = entry.path().filename().string();

					img->new_image = img->original_image;
					img->new_tx.create(img->original_image.getSize().x, img->original_image.getSize().y);
					img->new_tx.update(img->original_image);
					img->new_sprite.setTexture(img->new_tx);
					img->new_sprite.setPosition((this->window_size_x / 2) + (100 - img->new_image.getSize().x), 200);

					this->images.emplace_back(img);

					this->GetPalette(img);
					this->GetPalette(img, true);

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

		this->image_slider_notch.setSize(sf::Vector2f(5, 40));
		this->image_slider_notch.setPosition(100, 90);
		this->image_slider_notch.setFillColor(sf::Color(99, 99, 99, 255));


		this->current_slide.setFont(this->font);
		this->current_slide.setCharacterSize(14);
		this->current_slide.setFillColor(sf::Color(99, 99, 99, 255));
		this->current_slide.setString("Current Slide: " + std::to_string(this->current_selection));
		this->current_slide.setPosition(sf::Vector2f((this->window_size_x / 2) - 20 , 110));

		this->original_img_label.setFont(this->font);
		this->original_img_label.setCharacterSize(16);
		this->original_img_label.setFillColor(sf::Color(99, 99, 99, 255));
		this->original_img_label.setString("Original Image");
		this->original_img_label.setPosition((this->window_size_x / 2) - (this->original_img_label.getLocalBounds().width + 100), 130);

		this->new_img_label.setFont(this->font);
		this->new_img_label.setCharacterSize(16);
		this->new_img_label.setFillColor(sf::Color(99, 99, 99, 255));
		this->new_img_label.setString("Updated Image");
		this->new_img_label.setPosition((this->window_size_x / 2) + 100, 130);

		this->lastColourClicked.setFont(this->font);
		this->lastColourClicked.setCharacterSize(16);
		this->lastColourClicked.setFillColor(sf::Color(99, 99, 99, 255));
		this->lastColourClicked.setString("Last Colour Clicked: n/a");
		this->lastColourClicked.setPosition(10, 45);

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
				num_to_display = this->loaded -1;

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

		window.draw(this->image_slider_notch);

		window.draw(this->current_slide);

		window.draw(this->original_img_label);
		window.draw(this->new_img_label);

		window.draw(this->images[this->current_selection]->original_sprite);
		window.draw(this->images[this->current_selection]->new_sprite);

		for (auto palette_squares : this->images[this->current_selection]->original_palette)
			window.draw(palette_squares->palette_box);

		for (auto palette_squares : this->images[this->current_selection]->new_palette)
			window.draw(palette_squares->palette_box);

		window.draw(this->lastColourClicked);
	}
}

void ColourPatcher::UpdateSprite(std::string name, Images* image)
{

	
}

void ColourPatcher::GetPalette(Images* img, bool updated)
{
    int colours_found = 0;
    std::vector<sf::Color> colours;
    sf::Image image;

    std::vector<PaletteSquare*> palette_squares;
    image = updated ? img->new_image : img->original_image;

px_check_loop:

	for (int x = 0; x < image.getSize().x; x++)
	{
		for (int y = 0; y < image.getSize().y; y++)
		{
			sf::Color px_colour = image.getPixel(x, y);


			bool found = false;
			for (sf::Color px_check : colours)
			{
				if (px_colour.r == px_check.r && px_colour.b == px_check.b && px_colour.g == px_check.g && px_check.a == px_colour.a)
					found = true;
			}

			if (!found)
			{
				colours.push_back(px_colour);


				PaletteSquare* square = new PaletteSquare();
				square->palette_box.setSize(sf::Vector2f(10, 10));
				square->palette_box.setFillColor(px_colour);
				square->palette_colour = px_colour;

				int y_ = 0;
				int x_ = 0;

				if (palette_squares.size() < 25)
				{
					y_ = 160;
					x_ = updated ? (this->window_size_x / 2) + (15 * palette_squares.size()) : 15 * palette_squares.size();
				}
				else if (palette_squares.size() < 50)
				{
					y_ = 180;
					x_ = updated ? (this->window_size_x / 2) + (15 * (palette_squares.size() - 25)) : 15 * (palette_squares.size() - 25);
				}
				else if (palette_squares.size() < 75)
				{
					y_ = 200;
					x_ = updated ? (this->window_size_x / 2) + (15 * (palette_squares.size() - 50)) : 15 * (palette_squares.size() - 50);
				}
				else
				{
					std::cout << "Palette size exceeded for image " << img->name << "\n";
					return;
				}

				square->palette_box.setPosition(x_, y_);

				palette_squares.emplace_back(square);

				goto px_check_loop;
			}
		}
	}   


    std::cout << "Image " << img->name << ", Palette size : " << palette_squares.size() << "\n";

    if (!updated)
        img->original_palette = palette_squares;
    else
        img->new_palette = palette_squares;
}


