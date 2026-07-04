#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <set>

double vecs(const double& v1, const double& v2) {
    double dif = (v1 - v2)/144;
    return dif;
}
//random points
std::set<std::pair<int, int>> random(int n) {
    std::set<std::pair<int, int>> pos;
    for (int i = 0 ;i < n; i++) {
        pos.insert({rand() % 900, rand() % 600});
    }
    return pos;
}

class obsticals {
    public:
        double x;
        double y;
    void draw() {


    }


};




int main() {
    sf::RenderWindow window(sf::VideoMode({900u, 600u}), "SFML Test");
    unsigned int width = 600;
    unsigned int height = 360;
    const sf::Font font("/System/Library/Fonts/Supplemental/Arial.ttf");
    sf::Text text(font, "Hello SFML", 50);

    // random circle generator
    std::set<std::pair<int, int>> randomPoints = random(50);


    sf::CircleShape shape(50.f);
    shape.setOrigin({50.f, 50.f});
    shape.setPosition({450.f, 300.f});
    shape.setFillColor(sf::Color(100, 250, 50));

    sf::CircleShape circle(64.f);
    circle.setOrigin(circle.getGeometricCenter());
    circle.setPosition({width / 2.0f, height / 2.0f});
    circle.setFillColor(sf::Color::Red );
    circle.setPointCount(35);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        sf::Vector2f pos = shape.getPosition();
        std::cout << pos.x << ", " << pos.y << "\n";
        sf::Vector2f pos_circle = circle.getPosition();


        double rad_dif = std::sqrt( std::pow((pos.x- pos_circle.x), 2) + std::pow((pos.y- pos_circle.y), 2));
        std::cout << rad_dif << "\n";
        if (rad_dif <= 114 ) {
            pos.x += 114 * vecs(pos.x, pos_circle.x);
            pos.y += 114 * vecs(pos.y ,pos_circle.y);
        }

        // bounds
        if (pos.x < 0) {
            pos.x = 0;
        } else if (pos.x > 900) {
            pos.x = 900;
        }
        if (pos.y < 0) {
            pos.y = 0;
        } else if (pos.y > 600) {
            pos.y = 600;
        }
        shape.setPosition(pos);



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            shape.move({0.2f, 0.f});

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            shape.move({0.f, -0.2f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            shape.move({0.f, 0.2f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            shape.move({-0.2f, 0.f});
        }

        window.clear();
        for (const auto& item : randomPoints) {
            sf::CircleShape bruh(20.f);
            bruh.setOrigin({20.f, 20.f});
            bruh.setPosition({static_cast<float>(item.first), static_cast<float>(item.second)});
            bruh.setFillColor(sf::Color(100, 250, 50));
            window.draw(bruh);
        }
        window.draw(text);
        window.draw(shape);
        window.draw(circle);
        window.display();
    }
    return 0;
}