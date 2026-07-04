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
    float radius;
    sf::Color color;
    sf::CircleShape shape;
    std::vector<Obstacle> obsticals;


    Player(double x_, double y_,float radius_, sf::Color color_, std::vector<Obstacle> obsticals_)
        : x(x_), y(y_),  radius(radius_), color(color_), obsticals(obsticals_)
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            shape.move({0.f, 0.2f});
        }
        sf::Vector2f pos = shape.getPosition();
        x = pos.x;
        y = pos.y;
        //boundry
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
        return pos;

    }

    sf::Vector2f check_collisions(float x_prev, float y_prev) {
        for (auto& stuff :obsticals) {
            double dx = stuff.x - x;
            double dy = stuff.y - y;
            double minDist = radius + stuff.shape.getRadius();
            if ((dx*dx + dy*dy) <= minDist) {
                x = x_prev;
                y = y_prev;
                shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
                break;
            }

        }
        return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
    }
};




int main() {
    sf::RenderWindow window(sf::VideoMode({900u, 600u}), "SFML Test");
    unsigned int width = 600;
    unsigned int height = 360;



    //render obstitalcs
    std::vector<Obstacle> stuff;
    for (int i = 0 ; i < 50 ; i++) {
        Obstacle thing(rand() %900, rand() %600, sf::Color(0,0,255));
        stuff.push_back(thing);
    }

    // render the player
    Player p(150, 150, 40.f, sf::Color(0,255,0), stuff);


    double prev_x = p.x;
    double prev_y = p.y;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        prev_x = p.x;
        prev_y = p.y;
        //player movement
        p.move_ment();
        p.check_collisions(static_cast<float>(prev_x), static_cast<float>(prev_y));


        window.clear();
        p.draw_player(window);
        for ( auto& dude: stuff) {
            dude.draw(window);
        }

        window.display();

    }
    return 0;
}