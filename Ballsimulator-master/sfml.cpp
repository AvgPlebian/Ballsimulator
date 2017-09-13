#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <cmath>
#include <stdlib.h>

const float pi = 3.1415926535897932384626433832795;

const int WHEIGHT = 500;
const int WWIDTH = 700;
const int YPOS = 20;
const int XPOS = 20;
const int MAXRAD = 50;
const int MINRAD = 40;
const int STARTSPD = 50;

double atan(int x, int y)
{
    double angle = atan2(y, x);
    if(angle < 0)
        return (2*pi)+angle;
    return angle;
}

int sqr(int x)
{
    return x*x;
}

//circle class start
class Circle
{
private:

public:
    sf::Vector2f speed;
    sf::Vector2f position;
    float radius;
    sf::CircleShape circle;
    sf::Vector2f collisionUpdate;
    bool collision;

    void initialize(sf::Vector2i);
    void relocate(sf::Vector2u, sf::Time);
    void Update();


    bool Collision(Circle);
    bool CollisionTemplate(sf::Vector2i, int);
    sf::Vector2f CollisionSpeed(Circle);
};

void Circle::initialize(sf::Vector2i position)
{
    circle.setRadius(rand()%(MAXRAD-MINRAD)+MINRAD);
    radius = circle.getRadius();
    circle.setOrigin(radius, radius);
    circle.setPosition(position.x, position.y);
    circle.setFillColor(sf::Color(rand()%256, rand()%256, rand()%256));
    collision = false;

    //total speed remains <= 70
    speed.x = rand()%(STARTSPD);
    int temp = sqrt(sqr(STARTSPD)-sqr(speed.x));
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
//circle class end


//circlemanager class start
struct node
{
    Circle circle;
    node *next;
    node *previous;
};

class CircleManager
{
private:
    node *root, *last, *current;

public:
    CircleManager(Circle);
    void newCircle(Circle);
    void updateCircles(sf::RenderWindow*, sf::Time);
    void deleteCircle();
    void addCircle();
};

CircleManager::CircleManager(Circle c)
{
    root = new node;
    root->circle = c;
    root->next = NULL;
    root->previous = NULL;
    last = root;
}

void CircleManager::newCircle(Circle c)
{
    current = last;
    current->next = new node;

    current->next->previous = current;
    current = current->next;
    current->next = NULL;

    current->circle = c;
    last = current;
}

void CircleManager::addCircle()
{
    Circle newCir;
    bool placeable = true;

    for(float i = MAXRAD;i <= WHEIGHT-MAXRAD;i++)
    {
        for(float j = MAXRAD;j <= WWIDTH-MAXRAD;j++)
        {
            current = root;
            while(current != last->next)
            {
                if(current->circle.CollisionTemplate(sf::Vector2i(j, i), MAXRAD))
                {
                    placeable = false;
                    current = last;
                }
                current = current->next;
            }

            if(placeable)
            {
                newCir.initialize(sf::Vector2i(j, i));
                newCircle(newCir);
                j = WWIDTH;
                i = WHEIGHT;
            }
            placeable = true;
        }
    }
}

void CircleManager::updateCircles(sf::RenderWindow *window, sf::Time dt)
{
    //check for collisions
    current = root;
    while(current != last->next)
    {
        node* tempCurr = root;
        while(tempCurr != last->next)
        {
                if(current->circle.Collision(tempCurr->circle))
                {
                    current->circle.collisionUpdate = current->circle.CollisionSpeed(tempCurr->circle);
                    tempCurr->circle.collisionUpdate = tempCurr->circle.CollisionSpeed(current->circle);
                }
                tempCurr = tempCurr->next;
        }
        current = current->next;
    }
    //end check for collisions

    //relocate circles
    current = root;
    while(current != last->next)
    {
        current->circle.relocate(window->getSize(), dt);
        current = current->next;
    }
    //end relocation

    //draw circles
    window->clear();
    current = root;
    while(current != last->next)
    {
        window->draw(current->circle.circle);
        current = current->next;
    }
    window->display();
}

void CircleManager::deleteCircle()
{
    if(root->next != NULL)
    {
        root = root->next;
        root->previous = NULL;
    }
}
//circlemanager class end

int main()
{
    sf::RenderWindow log(sf::VideoMode(200, 200), "Log");
    log.setPosition(sf::Vector2i(XPOS + WWIDTH + 50, YPOS + WHEIGHT/2));

    sf::RenderWindow window(sf::VideoMode(WWIDTH, WHEIGHT), "Derp");
    window.setPosition(sf::Vector2i(XPOS, YPOS));

    sf::Mouse::setPosition(sf::Vector2i(WWIDTH/2, WHEIGHT/2), window);

    sf::Clock deltaClock;
    srand (time(NULL));

    Circle c1;
    c1.initialize(sf::Vector2i(200, 300));
    CircleManager cm(c1);

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
