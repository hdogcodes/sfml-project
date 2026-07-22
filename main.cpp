#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <complex>
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
        shape.setOutlineColor(sf::Color::Blue);
        shape.setOutlineThickness(5.0f);
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
    float mag = std::hypot(dx,dy);
    float minDist = one.radius + two.radius;
    if (dx*dx + dy*dy <= minDist*minDist) {
        float movement_amount = minDist - mag;
        one.x += (dx * movement_amount)/mag;
        one.y += (dy * movement_amount)/mag;
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
        float prev_x;
        float prev_y;
        float radius;
        double decay;
        std::vector<Player>& blobs;   // reference to main's vector, not a copy
        sf::CircleShape magnet;
        attraction_point(float x_, float y_ , float dx_m_, float dy_m_,float prev_x_, float prev_y_, float radius_ , double decay_,std::vector<Player>& blobs_)
            : x(x_), y(y_), dx_m(dx_m_), dy_m(dy_m_),prev_x(prev_x_), prev_y(prev_y_), decay(decay_),radius(radius_),blobs(blobs_)
        {
            magnet.setRadius(radius);
            magnet.setOrigin({radius, radius});
            magnet.setPosition({x,y});


        }
        sf::Vector2f movement_magnet() {
            sf::Vector2f pos = magnet.getPosition();

            prev_x = x;
            prev_y = y;

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
        void split() {
            int n = blobs.size();
            if (n < 8) {
                for (int i = 0; i < n; i++) {
                    //pi*r^2= A/2
                    //r = sqrt(A/(2*pi))
                    //r = r/sqrt(2)
                    blobs[i].radius = blobs[i].radius / std::sqrt(2);
                    Player dude = blobs[i];
                    //norm the shot vector
                    float dx = x - prev_x;
                    float dy = y - prev_y;
                    float mag = std::hypot(dx,dy);
                    dx = dx / mag;
                    dy = dy / mag;
                    dude.score = 0;
                    dude.x += dx * (dude.radius  +2)* 2;
                    dude.y += dy * (dude.radius  +2 )* 2;
                    dude.dx_p = 50 * dx;
                    dude.dy_p = 50 * dy;
                    dude.shape.setRadius(blobs[i].radius);
                    blobs[i].shape.setRadius(blobs[i].radius);
                    blobs.push_back(dude);
                }
            }
        }


        void attraction() {
            float baseSpeed  = 4.0f;
            float baseRadius = 40.f;
            float k          = 0.44f;
            int n = blobs.size();
            for (int i = 0 ; i < n; i++) {
                float speed = baseSpeed * std::pow(baseRadius / blobs[i].radius, k);
                blobs[i].prev_x = blobs[i].x;
                blobs[i].prev_y = blobs[i].y;
                float dx =  x - blobs[i].x;
                float dy =  y - blobs[i].y;
                float mag = std::hypot(dx,dy);
                if (n > 1) {
                    for (int j = 0; j < n; j++) {
                        if (j == i) continue;
                        //std::cout << "blob i " <<blobs[i].prev_x << "," << blobs[i].prev_y<< "\n";
                        //std::cout << blobs[j].prev_x << "," << blobs[j].prev_y<< "\n";
                        blobs[i] = collision(blobs[i], blobs[j]);
                    }
                }
                if (mag > 0) {
                    blobs[i].x += dx/mag * speed;
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
    window.setKeyRepeatEnabled(false); // one KeyPressed event per physical press
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


    attraction_point thing(150, 150, 0, 0,0,0, 1 ,0.02,blobs);


    int score = 0;

    while (window.isOpen()) {
        bool splitPressed = false;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Space)
                    splitPressed = true; // fires ONCE per press, not while held
            }
        }


        if (stuff.size()< 50) {
            Obstacle thing(rand() %1800, rand() %1200, sf::Color(0,0,255));
            stuff.push_back(thing);
        }
        //update
        thing.movement_magnet();
        thing.attraction();
        if (splitPressed) thing.split();
        for (auto& agar : blobs) {
            agar.check_collisions(stuff);
        }

        sf::View partialView(sf::FloatRect({thing.x - 450, thing.y - 300}, {900.f, 600.f}));
        text.setPosition({thing.x, thing.y});
        for (const auto& bruv :blobs) {
            score+= bruv.score;
        }
        text.setString(std::to_string(blobs.empty() ? 0 : score));
        score = 0;
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