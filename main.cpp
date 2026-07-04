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


class Obstacle {
public:
    double x;
    double y;
    sf::Color color;
    sf::CircleShape shape;

    Obstacle(double x_, double y_, sf::Color color_, float radius = 20.f)
        : x(x_), y(y_), color(color_)
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius}); // center origin
        shape.setFillColor(color);
    }

    void draw(sf::RenderWindow& window) {
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
        window.draw(shape);
    }

};
class Player {
public:
    double x;
    double y;
    sf::Color color;
    sf::CircleShape shape;

    Player(double x_, double y_, sf::Color color_, float radius = 20.f)
        : x(x_), y(y_), color(color_)
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius}); // center origin
        shape.setFillColor(color);
        shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
    }

    void draw_player(sf::RenderWindow& window) {

        window.draw(shape);
    }
    sf::Vector2f move_ment() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            shape.move({0.2f, 0.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            shape.move({-0.2f, 0.f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            shape.move({0.f, -0.2f});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            shape.move({0.f, 0.2f});
        }
        sf::Vector2f pos = shape.getPosition();
        x = pos.x;
        y = pos.y;

        return pos;

    }


};




int main() {
    sf::RenderWindow window(sf::VideoMode({900u, 600u}), "SFML Test");
    unsigned int width = 600;
    unsigned int height = 360;

    std::vector<Obstacle> stuff;
    for (int i = 0 ; i < 50 ; i++) {
        Obstacle thing(rand() %900, rand() %600, sf::Color(0,0,255));
        stuff.push_back(thing);
    }


    sf::CircleShape shape(50.f);
    shape.setOrigin({50.f, 50.f});
    shape.setPosition({450.f, 300.f});
    shape.setFillColor(sf::Color(100, 250, 50));

    sf::CircleShape circle(64.f);
    circle.setOrigin(circle.getGeometricCenter());
    circle.setPosition({width / 2.0f, height / 2.0f});
    circle.setFillColor(sf::Color::Red );
    circle.setPointCount(35);
    double prev_x;
    double prev_y;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        sf::Vector2f pos = shape.getPosition();
        sf::Vector2f pos_circle = circle.getPosition();


        if (std::pow((pos.x-300),2) + std::pow((pos.y-180),2) <= std::pow((115),2)){
            pos.x = prev_x;
            pos.y = prev_y;
            std::cout << "in" <<"\n";
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

        for ( auto& dude: stuff) {
            dude.draw(window);
        }
        window.draw(shape);
        window.draw(circle);
        window.display();
        prev_x = pos.x;
        prev_y = pos.y;
    }
    return 0;
}