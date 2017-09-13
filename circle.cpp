#include "circle.h"

int Circle::sqr(int x)
{
    return x*x;
}

void Circle::initialize(int maxRad, int minRad, int spd, sf::Vector2i position)
{
    circle.setRadius(rand()%(maxRad-minRad)+minRad);
    radius = circle.getRadius();
    circle.setOrigin(radius, radius);
    circle.setPosition(position.x, position.y);
    circle.setFillColor(sf::Color(rand()%256, rand()%256, rand()%256));
    collision = false;

    //total speed remains <= 70
    speed.x = rand()%(spd);
    int temp = sqrt(sqr(spd)-sqr(speed.x));
    speed.y = temp;
    int neg = rand()%3;
    switch (neg)
    {
    case 0:
        speed.x*=-1;
        break;
    case 1:
        speed.y*=-1;
        break;
    case 2:
        speed.x*=-1;
        speed.y*=-1;
        break;
    }
}

void Circle::Update()
{

    speed = collisionUpdate;
    collision = false;          //keeps it from calling Update without a collision or anything update-worthy happening
}

void Circle::relocate(sf::Vector2u windowSize, sf::Time dt)
{
    if(collision)
    {
        Update();
    }
    sf::Vector2f position = circle.getPosition();

    //keep circle within window boundaries
    if(position.x <= radius && speed.x < 0)
        speed.x *= -1;
    if(position.x >= windowSize.x - radius && speed.x > 0)
        speed.x *= -1;
    if(position.y <= radius && speed.y < 0)
        speed.y *= -1;
    if(position.y >= windowSize.y - radius && speed.y > 0)
        speed.y *= -1;

    circle.move(speed * dt.asSeconds());
}

bool Circle::Collision(Circle circle2)
{

    //check collisions as according to pythagoras
    sf::Vector2f c2pos = circle2.circle.getPosition();


    float distX = circle.getPosition().x - c2pos.x;
    float distY = circle.getPosition().y - c2pos.y;

    float futDistX = circle.getPosition().x + speed.x - c2pos.x;
    float futDistY = circle.getPosition().y + speed.y - c2pos.y;

        if(distX < 0)
            distX *= -1;
        if(distY < 0)
            distY *= -1;

        if(distX*distX + distY*distY <= (radius + circle2.radius)*(radius + circle2.radius))
        {
            if(distX*distX + distY*distY > futDistX*futDistX + futDistY*futDistY)
            {
                return true;
            }
        }
    return false;
}

bool Circle::CollisionTemplate(sf::Vector2i position, int rad)
{
    //check collisions as according to pythagoras
    float distX = circle.getPosition().x - position.x;
    float distY = circle.getPosition().y - position.y;

    if(distX < 0)
        distX *= -1;
    if(distY < 0)
        distY *= -1;

    if(distX*distX + distY*distY <= (radius + rad)*(radius + rad))
    {
        return true;
    }
    return false;
}

sf::Vector2f Circle::CollisionSpeed(Circle circle2)
{
    //find normal, unit normal, unit tangent
    sf::Vector2f normal = circle.getPosition() - circle2.circle.getPosition();
    float normalMag = sqrt((normal.x * normal.x)+(normal.y * normal.y));
    sf::Vector2f unitNorm = normal / normalMag;
    sf::Vector2f unitTang = sf::Vector2f(-unitNorm.y, unitNorm.x);
    //end

    //initiate velocity calculation
    int vn = (unitNorm.x * speed.x)+(unitNorm.y * speed.y);
    int vt = (unitTang.x * speed.x)+(unitTang.y * speed.y);
    int c2vn = (unitNorm.x * circle2.speed.x)+(unitNorm.y * circle2.speed.y);
    //end

    //finalize velocity calculations
    vn = (vn * (radius - circle2.radius) + 2 * circle2.radius * c2vn)/(radius + circle2.radius);
    //end

    //reverse type to Vector2f
    sf::Vector2f Vn = sf::Vector2f(vn * unitNorm.x, vn * unitNorm.y);
    sf::Vector2f Vt = sf::Vector2f(vt * unitTang.x, vt * unitTang.y);
    //end

    collision = true;                    //cause a speed update to happen


    //return sf::Vector2f(-speed.x, -speed.y);
    return sf::Vector2f(Vn.x + Vt.x, Vn.y + Vt.y);
}
