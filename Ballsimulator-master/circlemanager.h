#ifndef CIRCLEMANAGER_H
#define CIRCLEMANAGER_H
#include "circle.h"
#include <iostream>


struct node
{
    node(Circle c){next = nullptr; circle = c;}
    Circle circle;
    node *next;
//    node *previous;
};

const float pi = 3.1415926535897932384626433832795;

const int WHEIGHT = 500;
const int WWIDTH = 700;
const int YPOS = 20;
const int XPOS = 20;
const int MAXRAD = 50;
const int MINRAD = 40;
const int STARTSPD = 50;

class CircleManager
{
private:
    node *root, *last, *current;

public:
    CircleManager();
    CircleManager(Circle);
    void newCircle(Circle);
    void updateCircles(sf::RenderWindow*, sf::Time);
    void deleteCircle();
    void addCircle();
};

#endif // CIRCLEMANAGER_H
