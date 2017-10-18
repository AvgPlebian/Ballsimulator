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
    std::cout << "Welcome to this 2D Ball Collision Simulator.\nSpace to pause and unpause.\nNumpad+ to add a ball.\nNumpad- to remove a ball.\n\n\n";

    sf::RenderWindow window(sf::VideoMode(WWIDTH, WHEIGHT), "Ball Collision Simulator");
    window.setPosition(sf::Vector2i(XPOS, YPOS));

    sf::Mouse::setPosition(sf::Vector2i(WWIDTH/2, WHEIGHT/2), window);

    sf::Clock deltaClock;
    srand (time(NULL));

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
                    CircleManager<Circle>::addData();
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
                {
                    CircleManager<Circle>::deleteData();
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    play = !play;
                }
            }
        }
    if(play)
        CircleManager<Circle>::updateData(&window, dt);
    }
    return 0;
}
