#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <set>












class Obstacle {
public:
    float x;
    float y;
    sf::Color color;
    sf::CircleShape shape;

    Obstacle(float x_, float y_, sf::Color color_, float radius = 20.f)
        : x(x_), y(y_), color(color_)
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius}); // center origin
        shape.setFillColor(color);
    }

    void draw(sf::RenderWindow& window) {
        shape.setPosition({x, y});
        window.draw(shape);
    }










};
class Player {
public:
    float x;
    float y;
    double dx_p;
    double dy_p;
    double decay;
    float prev_x;
    float prev_y;
    float radius;
    int score;
    sf::Color color;
    sf::CircleShape shape;

    Player(float x_, float y_, double dx_p_, double dy_p_, double decay_,float prev_x_,float prev_y_ ,float radius_, int score_, sf::Color color_)
        : x(x_), y(y_), dx_p(dx_p_), dy_p(dy_p_), decay(decay_),prev_x(prev_x_), prev_y(prev_y_), radius(radius_), score(score_), color(color_)
    {
        shape.setRadius(radius);
        shape.setOrigin({radius, radius}); // center origin
        shape.setFillColor(color);
        shape.setPosition({x, y});
    }

    void draw_player(sf::RenderWindow& window) {

        window.draw(shape);
    }



    void check_collisions(std::vector<Obstacle>& obs) {
        for (size_t i = 0; i < obs.size(); i++) {
            double dx = obs[i].x - x;
            double dy = obs[i].y - y;
            double minDist = radius + obs[i].shape.getRadius();
            if (dx*dx + dy*dy <= minDist*minDist) {
                score += 1;
                radius = std::sqrt(radius*radius + 400);
                shape.setRadius(radius);
                shape.setOrigin({radius, radius});
                obs.erase(obs.begin() + i);
                break;
            }
        }
    }
};

// function
Player collision(Player one, Player two) {
    float dx = one.x - two.x;
    float dy = one.y - two.y;
    float minDist = one.radius + two.radius;
    if (dx*dx + dy*dy <= minDist*minDist) {
        one.x = one.prev_x;
        one.y = one.prev_y;
        one.dx_p  = -one.dx_p *0.01;
        one.dy_p  = -one.dy_p *0.01;
        one.shape.setPosition({one.x, one.y});
        return one;
    }
    return one;

}



class attraction_point {
    public:
        float x;
        float y;
        float dx_m;
        float dy_m;
        float radius;
        double decay;
        std::vector<Player>& blobs;   // reference to main's vector, not a copy
        sf::CircleShape magnet;
        attraction_point(float x_, float y_ , float dx_m_, float dy_m_,float radius_ , double decay_,std::vector<Player>& blobs_)
            : x(x_), y(y_), dx_m(dx_m_), dy_m(dy_m_), decay(decay_),radius(radius_),blobs(blobs_)
        {
            magnet.setRadius(radius);
            magnet.setOrigin({radius, radius});
            magnet.setPosition({x,y});

        }
        sf::Vector2f movement_magnet() {
            sf::Vector2f pos = magnet.getPosition();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                dx_m += 0.2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                dx_m -= 0.2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                dy_m -= 0.2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                dy_m += 0.2;
            }

            // friction: glide to a stop when no key is held
            dx_m *= (1 - decay);
            dy_m *= (1 - decay);

            // cap speed at 4, keep direction
            double speed_mag = std::hypot(dx_m, dy_m);
            if (speed_mag > 4) {
                dx_m *= 4 / speed_mag;
                dy_m *= 4 / speed_mag;
            }

            // apply velocity to position
            pos.x += dx_m;
            pos.y += dy_m;

            // boundary: clamp and kill velocity on the axis you hit
            if (pos.x < radius) {
                pos.x = radius;
                dx_m = 0;
            } else if (pos.x > 1800 - radius) {
                pos.x = 1800 - radius;
                dx_m = 0;
            }
            if (pos.y < radius) {
                pos.y = radius;
                dy_m = 0;
            } else if (pos.y > 1200 - radius) {
                pos.y = 1200 - radius;
                dy_m = 0;
            }

            magnet.setPosition(pos);
            x = pos.x;   // keep members in sync check_collisions reads these
            y = pos.y;
            return pos;

        }
        void attraction() {
            int speed = 3.5;
            int n = blobs.size();
            for (int i = 0 ; i < n; i++) {
                float dx =  x - blobs[i].x;
                float dy =  y - blobs[i].y;
                float mag = std::hypot(dx,dy);
                if (n > 1) {
                    for (int j = 0; j < n; j++) {
                        if (j == i) continue;
                        blobs[i] = collision(blobs[i], blobs[j]);
                    }
                }
                if (mag > 0) {
                    blobs[i].x += dx/mag * speed ;   // speed is 2
                    blobs[i].y += dy/mag * speed;
                    blobs[i].shape.setPosition({blobs[i].x, blobs[i].y});
                }
            }
        }

        // do this
        //current job making player stored in a vector

    };





int main() {
    sf::RenderWindow window(sf::VideoMode({1800u, 1200u}), "SFML Test");
    window.setFramerateLimit(60);
    //window size 900, 600




    //render obstitalcs
    std::vector<Obstacle> stuff;
    for (int i = 0 ; i < 50 ; i++) {
        Obstacle thing(rand() %1800u, rand() %1200u, sf::Color(0,0,255));
        stuff.push_back(thing);
    }

    // render the player
    Player p(150, 150, 0, 0, 0.02, 150, 150, 40.f, 0, sf::Color(0,255,0));
    std::string str = std::to_string(p.score);
    std::vector<Player> blobs = {p};

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


    attraction_point thing(150, 150, 0, 0, 1 ,0.02,blobs);





    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (stuff.size()< 50) {
            Obstacle thing(rand() %1800, rand() %1200, sf::Color(0,0,255));
            stuff.push_back(thing);
        }
        //update
        thing.movement_magnet();
        thing.attraction();
        for (auto& agar : blobs) {
            agar.check_collisions(stuff);
        }

        sf::View partialView(sf::FloatRect({thing.x - 450, thing.y - 300}, {900.f, 600.f}));
        text.setPosition({thing.x, thing.y});
        text.setString(std::to_string(blobs.empty() ? 0 : blobs[0].score));

        //draw
        window.clear();
        window.setView(partialView);
        for (auto& dude: stuff) {
            dude.draw(window);
        }
        for (auto& agar : blobs) {
            agar.draw_player(window);
        }
        window.draw(thing.magnet);
        window.draw(text);
        window.display();

    }
    return 0;
}