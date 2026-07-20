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
    int score;
    sf::Color color;
    sf::CircleShape shape;

    Player(double x_, double y_, double dx_p_, double dy_p_, double decay_, float radius_, int score_, sf::Color color_)
        : x(x_), y(y_), dx_p(dx_p_), dy_p(dy_p_), decay(decay_), radius(radius_), score(score_), color(color_)
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
            dx_p += 0.2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            dx_p -= 0.2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            dy_p -= 0.2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            dy_p += 0.2;
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

    void check_collisions(std::vector<Obstacle>& obs) {
        for (size_t i = 0; i < obs.size(); i++) {
            double dx = obs[i].x - x;
            double dy = obs[i].y - y;
            double minDist = radius + obs[i].shape.getRadius();
            if (dx*dx + dy*dy <= minDist*minDist) {
                score += 1;
                obs.erase(obs.begin() + i);
                break;
            }
        }
    }
};




int main() {
    sf::RenderWindow window(sf::VideoMode({900u, 600u}), "SFML Test");
    window.setFramerateLimit(60);

    //render obstitalcs
    std::vector<Obstacle> stuff;
    for (int i = 0 ; i < 50 ; i++) {
        Obstacle thing(rand() %900, rand() %600, sf::Color(0,0,255));
        stuff.push_back(thing);
    }

    // render the player
    Player p(150, 150, 0, 0, 0.02, 40.f, 0, sf::Color(0,255,0));
    std::string str = std::to_string(p.score);


    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Error loading font file!" << std::endl;
        return -1;
    }

    sf::Text text(font);                     // Associate the font with the text
    text.setString(str);          // Set the message
    text.setCharacterSize(24);               // Set size in pixels (not points)
    text.setFillColor(sf::Color::White);     // Set text color
    text.setStyle(sf::Text::Bold);           // Set text style (Bold, Italic, etc.)
    text.setPosition({100.f, 100.f});







    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (stuff.size()< 50) {
            Obstacle thing(rand() %900, rand() %600, sf::Color(0,0,255));
            stuff.push_back(thing);
        }
        std::string str_ = std::to_string(p.score);
        text.setString(str_);
        //player movement
        p.move_ment();
        p.check_collisions(stuff);


        window.clear();
        p.draw_player(window);
        for ( auto& dude: stuff) {
            dude.draw(window);
        }
        window.draw(text);


        window.display();

    }
    return 0;
}