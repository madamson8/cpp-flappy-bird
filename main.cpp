#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>

using namespace std;

/*
 * Made by Matthew Adamson.
 * You can contribute to this project on github!
 */

float max(float a, float b) {
    if(a > b)
        return a;
    else
        return b;
}

float min(float a, float b) {
    if(a < b)
        return a;
    else
        return b;
}

bool checkCRCollision(float rx, float ry, float rw, float rh, float cx, float cy, float cr) {
    // see https://yal.cc/rectangle-circle-intersection-test/ for details on why this works.
    float delta_x = cx - max(rx, min(cy, rx + rw));
    float delta_y = cy - max(ry, min(cy, ry + rh));
    return (delta_x * delta_x + delta_y * delta_y) < (cr * cr);
}

int main() {
    // Possible speed improvements
    /*
     * Move all pipes checking into the same loop
     */

    // Player Variables
    const float p_x = 75; // The player only ever stays at one point on the screen.
    float p_y = 300;
    float p_radius = 15;
    sf::CircleShape player(p_radius, 100);
    player.setFillColor(sf::Color::Blue);

    // Velocity variables
    float v_y = 0;

    // Set up gamestate initially

    bool is_paused = false;

    // Seed random generator
    srand (static_cast <unsigned> (time(0)));

    sf::Color background(sf::Color::Black);
    player.setPosition(sf::Vector2f(p_x, p_y));

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Flappy Bird");
    window.setFramerateLimit(60);

    vector<std::array<float, 2>> pipes;

    // Begin window looping.
    int iteration = 0;
    int frame = 0;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::KeyReleased) {
                if(event.key.code == sf::Keyboard::Escape) {
                    if(!is_paused)
                        is_paused = true;
                    else
                        is_paused = false;
                }
                if(event.key.code == sf::Keyboard::R) {
                    player.setFillColor(sf::Color::Blue);
                    pipes.clear();
                    p_y = 300;
                    v_y = 0;
                    is_paused = false;
                }
            }
        }

        if(!is_paused) {
            frame++;
            if(frame > 60) {
                iteration += 1;
                frame = 1;
            }
            

            // Loop Gamestate
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if(v_y < 0)
                    v_y = 0;
                if(v_y < 5)
                    v_y += 0.4;
            }

            // Gravity

            if(v_y > -5.0) {
                v_y -= 0.1;
            } else {
                v_y = -5.0;
            }

            // Pipe handling
            // Create new pipe every fps * 3 seconds
            if(iteration % 3 == 0 && frame == 1) {
                float pmd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/600));
                array<float, 2> new_int = { 800, pmd };
                pipes.push_back(new_int);
            }

            for(int i = 0; i < pipes.size(); i++) {
                pipes.at(i)[0] -= 2; // control speed of scrolling by adjusting this value
            }

            if(pipes.size() > 8) {
                pipes.erase(pipes.begin());
            }
            // End Pipe handling

            p_y -= v_y;

            player.setPosition(p_x, p_y);

            // Collision Detection
            if(p_y - p_radius < 0 || p_y + p_radius > 600) {
                player.setFillColor(sf::Color::Red);
                is_paused = true;
            }

            //float rx, float ry, float rw, float rh, float cx, float cy, float cr
            for(int i = 0; i < pipes.size(); i++) {
                float distance = 600 - pipes.at(i)[1];
                float h1 = 600 - (distance + 50);
                float s2 = 600 - (distance - 50);
                float h2 = 600 - s2;
                if(checkCRCollision(pipes.at(i)[0]-50, 0, 50, h1, p_x, p_y, p_radius)) {
                    player.setFillColor(sf::Color::Red);
                    is_paused = true;
                }
                if(checkCRCollision(pipes.at(i)[0]-50, s2, 50, h2, p_x, p_y, p_radius)) {
                    player.setFillColor(sf::Color::Red);
                    is_paused = true;
                }
            }
        }

        // only drawing should go past this point.  No calculations!

        window.clear(background);

        window.draw(player);

        for(int i = 0; i < pipes.size(); i++) {
            // height of container is 600
            float distance = 600 - pipes.at(i)[1];
            float h1 = 600 - (distance + 50);
            float s2 = 600 - (distance - 50);
            float h2 = 600 - s2;
            sf::RectangleShape pipe1(sf::Vector2f(50, h1));
            sf::RectangleShape pipe2(sf::Vector2f(50, h2));
            pipe1.setFillColor(sf::Color::Green);
            pipe1.setPosition(pipes.at(i)[0], 0);
            pipe2.setFillColor(sf::Color::Green);
            pipe2.setPosition(pipes.at(i)[0], s2);
            window.draw(pipe1);
            window.draw(pipe2);
        }

        window.display();
    }

    return 0;
}