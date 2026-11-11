#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({900, 400}), "Pianova");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                std::cout << "Key pressed: " << static_cast<int>(key->code) << "\n";
            }
        }

        window.clear(sf::Color(30, 30, 40));
        window.display();
    }
    return 0;
}