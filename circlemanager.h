#ifndef CIRCLEMANAGER_H
#define CIRCLEMANAGER_H
#include "circle.h"
#include <iostream>

template <class T>
struct node
{
    node(T d){next = nullptr; data = d;}
    T data;
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

template <class T>
class CircleManager
{
private:
    static node<T> *root, *last, *current;

public:
    static void newData(T);
    static void updateData(sf::RenderWindow*, sf::Time);
    static void deleteData();
    static void addData();
};

#endif // CIRCLEMANAGER_H
