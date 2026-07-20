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
    double dx_p;
    double dy_p;
    double decay;
    float radius;
    sf::Color color;
    sf::CircleShape shape;
    std::vector<Obstacle> obsticals;



    Player(double x_, double y_, double dx_p_, double dy_p_, double decay_, float radius_, sf::Color color_, std::vector<Obstacle> obsticals_)
        : x(x_), y(y_), dx_p(dx_p_), dy_p(dy_p_), decay(decay_), radius(radius_), color(color_), obsticals(obsticals_)
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
        sf::Vector2f pos = shape.getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            dx_p += 0.002;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            dx_p -= 0.002;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            dy_p -= 0.002;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            dy_p += 0.002;
        }

        // friction: glide to a stop when no key is held
        dx_p *= (1 - decay);
        dy_p *= (1 - decay);

        // cap speed at 4, keep direction
        double speed_mag = std::hypot(dx_p, dy_p);
        if (speed_mag > 4) {
            dx_p *= 4 / speed_mag;
            dy_p *= 4 / speed_mag;
        }

        // apply velocity to position
        pos.x += static_cast<float>(dx_p);
        pos.y += static_cast<float>(dy_p);

        // boundary: clamp and kill velocity on the axis you hit
        if (pos.x < radius) {
            pos.x = radius;
            dx_p = 0;
        } else if (pos.x > 900 - radius) {
            pos.x = 900 - radius;
            dx_p = 0;
        }
        if (pos.y < radius) {
            pos.y = radius;
            dy_p = 0;
        } else if (pos.y > 600 - radius) {
            pos.y = 600 - radius;
            dy_p = 0;
        }

        shape.setPosition(pos);
        x = pos.x;   // keep members in sync check_collisions reads these
        y = pos.y;
        return pos;
    }

    sf::Vector2f check_collisions(float x_prev, float y_prev) {
        for (auto& stuff : obsticals) {
            double dx = stuff.x - x;
            double dy = stuff.y - y;
            double minDist = radius + stuff.shape.getRadius();
            if ((dx*dx + dy*dy) <= minDist*minDist) {
                x = x_prev;
                y = y_prev;
                dx_p = -dx_p * 0.01;
                dy_p = -dy_p * 0.01;
                shape.setPosition({static_cast<float>(x), static_cast<float>(y)});
                break;
            }

        }
        return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
    }
};




int main() {
    sf::RenderWindow window(sf::VideoMode({900u, 600u}), "SFML Test");



    //render obstitalcs
    std::vector<Obstacle> stuff;
    for (int i = 0 ; i < 50 ; i++) {
        Obstacle thing(rand() %900, rand() %600, sf::Color(0,0,255));
        stuff.push_back(thing);
    }

    // render the player
    Player p(150, 150, 0, 0, 0.02, 40.f, sf::Color(0,255,0), stuff);


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