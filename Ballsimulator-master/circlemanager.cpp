#include "circlemanager.h"

CircleManager::CircleManager()
{
    root = nullptr;
    last = nullptr;
}

CircleManager::CircleManager(Circle c)
{
    root = new node(c);
    last = root;
}

void CircleManager::newCircle(Circle c)
{
    if(last)
    {
        last->next = new node(c);
        last = last->next;
        return;
    }
    root = new node(c);
    last = root;
}

void CircleManager::addCircle()
{
    bool placeable = true;

    //
    for(float i = MAXRAD;i <= WHEIGHT-MAXRAD;i++)
    {
        for(float j = MAXRAD;j <= WWIDTH-MAXRAD;j++)
        {
            current = root;
            while(current)
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
                Circle newCir;
                newCir.initialize(MAXRAD, MINRAD, STARTSPD, sf::Vector2i(j, i));
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
    while(current)
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
    while(current)
    {
        current->circle.relocate(window->getSize(), dt);
        current = current->next;
    }
    //end relocation

    //draw circles
    window->clear();
    current = root;
    while(current)
    {
        window->draw(current->circle.circle);
        current = current->next;
    }
    window->display();
}

void CircleManager::deleteCircle()
{
    //if(!root)
        //return;

    if(root->next)
    {
        node *temp = root->next;
        delete root;
        root = temp;

    }
    else if(root)
    {
        delete root;
        root = nullptr;
        last = nullptr;
    }

}

