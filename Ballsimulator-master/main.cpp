#define _USE_MATH_DEFINES
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "circle.h"
#include "circlemanager.h"



double atan(int x, int y)
{
    double angle = atan2(y, x);
    if(angle < 0)
        return (2*pi)+angle;
    return angle;
}

int main()
{
    //sf::RenderWindow log(sf::VideoMode(200, 200), "Log");
    //log.setPosition(sf::Vector2i(XPOS + WWIDTH + 50, YPOS + WHEIGHT/2));

    sf::RenderWindow window(sf::VideoMode(WWIDTH, WHEIGHT), "Derp");
    window.setPosition(sf::Vector2i(XPOS, YPOS));

    sf::Mouse::setPosition(sf::Vector2i(WWIDTH/2, WHEIGHT/2), window);

    sf::Clock deltaClock;
    srand (time(NULL));

    CircleManager cm;

    bool play = true;


    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
                {
                    cm.addCircle();
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
                {
                    cm.deleteCircle();
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    play = !play;
                }
            }
        }
    if(play)
        cm.updateCircles(&window, dt);
    }
    return 0;
}
