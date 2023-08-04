// Alpha Recolouring Tool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ColourPatcher.h"
#include "SFML/Main.hpp"

#include <fstream>

//#include "DataPacket.h"

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
   

#ifdef _GENERATEPACK

    sf::Image icon;
    icon.loadFromFile("Patcher_Icon.png");

    const sf::Uint8* pixels = icon.getPixelsPtr();
    std::size_t size = icon.getSize().x * icon.getSize().y * 4;

    unsigned int width = icon.getSize().x;
    unsigned int height = icon.getSize().y;

    std::ofstream outputFile("data.pack", std::ios::binary);

    // Get the current write position before writing width and height
    outputFile.write(reinterpret_cast<char*>(&width), sizeof(width));
    outputFile.write(reinterpret_cast<char*>(&height), sizeof(height));
    outputFile.write(reinterpret_cast<const char*>(pixels), size);
    outputFile.close();
#endif


    ColourPatcher patcher;
    patcher.Start();


    return 0;
}
/*
* 
#include "SFML/Graphics.hpp"
#include <iostream>

void modulate(sf::VertexArray& points, double hue) {
    // First, Let's "sanitize" inputs a bit.
    // Don't accept negative numbers.
    if (hue < 0)
        hue = 0;
    // Lazy overflow by subtracting the integer portion of the number.
    else if (hue > 1)
        hue -= static_cast<int>(hue);

    // Now iterate over all "pixels" and upate their colors.
    for (unsigned int y = 0; y <= 255; ++y) {
        for (unsigned int x = 0; x <= 255; ++x) {
            // "Calculate" our missing HSV components with ranges from 0 to 1.
            const double s = x / 255.; // x is our saturation
            const double v = y / 255.; // y is our value

            // Pick the correct case based on our position on the color wheel.
            const int cs = hue * 6;

            // Calculate some helper values used in our cases below.
            const double f = hue * 6 - cs;
            const double p = v * (1 - s);
            const double q = v * (1 - s * f);
            const double t = v * (1 - s * (1 - f));

            switch (cs) {
            case 0:
            case 6:
                points[y * 256 + x].color = sf::Color(v * 255, t * 255, p * 255);
                break;
            case 1:
                points[y * 256 + x].color = sf::Color(q * 255, v * 255, p * 255);
                break;
            case 2:
                points[y * 256 + x].color = sf::Color(p * 255, v * 255, t * 255);
                break;
            case 3:
                points[y * 256 + x].color = sf::Color(p * 255, q * 255, v * 255);
                break;
            case 4:
                points[y * 256 + x].color = sf::Color(t * 255, p * 255, v * 255);
                break;
            case 5:
                points[y * 256 + x].color = sf::Color(v * 255, p * 255, q * 255);
                break;
            }
        }
    }
}

int main(int argc, char** argv) {
    // Setup a render window
    sf::RenderWindow window(sf::VideoMode(500, 456), "Color Picker");

    // We're using a clock to change the hue dynamically.
    sf::Clock timer;

    // This vertex array is going to be used for drawing.
    // It includes one vertex/point/pixel per color.
    sf::VertexArray colors(sf::Points, 256 * 256);
    for (unsigned int y = 0; y <= 255; ++y) {
        for (unsigned int x = 0; x <= 255; ++x) {
            sf::Vertex& vertex(colors[y * 256 + x]);

            // Note that I "flip" the displayed texture here, by subtracting
            // x/y from 255 rather than just using x/y, but that's really just
            // to get the same orientation that you have.
            vertex.position.x = 255 - x;
            vertex.position.y = 255 - y;
        }
    }

    double ran = 0.0;
    sf::Clock test;
    sf::Clock test2;
    while (window.isOpen()) {
        sf::Event event;
        // Your typical event loop
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && test.getElapsedTime().asMilliseconds() > 100)
        {
            test.restart();
            std::cout << "here: ran "<< ran<<"\n";

            ran += 0.012345;
            if (ran > 1)
                ran = 1;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && test.getElapsedTime().asMilliseconds() > 100)
        {
            test.restart();
            std::cout << "here: ran " << ran << "\n";

            ran -= 0.012345;

            if (ran < 0)
                ran = 0;
           
        }

        // Update our colors based on the time passed.
        // I want to cycle all hues in 5 seconds, so dividing the timer.
        modulate(colors, ran);
        //std::cout << timer.getElapsedTime().asSeconds() / 10 << "\n";

        // Draw and display our colors
        window.clear();
        window.draw(colors);
        window.display();
    }

    return 0;
}
*/







// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
