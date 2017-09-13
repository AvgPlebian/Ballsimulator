#ifndef CIRCLE_H
#define CIRCLE_H
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>


class Circle
{
private:
    int sqr(int);

public:
    sf::Vector2f speed;
    sf::Vector2f position;
    float radius;
    sf::CircleShape circle;
    sf::Vector2f collisionUpdate;
    bool collision;

    void initialize(int, int, int, sf::Vector2i);
    void relocate(sf::Vector2u, sf::Time);
    void Update();


    bool Collision(Circle);
    bool CollisionTemplate(sf::Vector2i, int);
    sf::Vector2f CollisionSpeed(Circle);
};


#endif // CIRCLE_H
