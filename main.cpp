#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test - Working!");
    window.setFramerateLimit(60);

    // Create a circle
    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Cyan);
    circle.setOutlineThickness(3.f);
    circle.setPosition(375.f, 275.f);

    // Load font for text
    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf"); // Make sure you have a font file

    sf::Text statusText;
    if (fontLoaded) {
        statusText.setFont(font);
        statusText.setString("SFML is working!");
        statusText.setCharacterSize(30);
        statusText.setFillColor(sf::Color::Green);
        statusText.setPosition(250.f, 50.f);
    }

    // Ball velocity for bouncing
    sf::Vector2f velocity(3.f, 3.f);

    std::cout << "SFML Window opened successfully!\n";
    std::cout << "Close the window to exit.\n";

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        // Move circle
        circle.move(velocity);

        // Bounce off walls
        sf::Vector2f pos = circle.getPosition();
        float radius = circle.getRadius();

        if (pos.x <= 0 || pos.x + radius * 2 >= 800)
            velocity.x = -velocity.x;
        if (pos.y <= 0 || pos.y + radius * 2 >= 600)
            velocity.y = -velocity.y;

        // Change color on bounce (fun visual feedback)
        circle.setFillColor(sf::Color(
            rand() % 100 + 100,
            rand() % 100 + 100,
            rand() % 200 + 55
        ));

        // Draw
        window.clear(sf::Color(20, 20, 40));
        window.draw(circle);
        if (fontLoaded)
            window.draw(statusText);
        window.display();
    }

    std::cout << "SFML test completed successfully!\n";
    return 0;
}