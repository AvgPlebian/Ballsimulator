#include "circlemanager.h"

template <class T>
node<T> * CircleManager<T>::root = nullptr;

template <class T>
node<T> * CircleManager<T>::last = nullptr;

template <class T>
node<T> * CircleManager<T>::current = nullptr;

template <class T>
void CircleManager<T>::newData(T d)
{
    if(last)
    {
        last->next = new node<T>(d);
        last = last->next;
        return;
    }
    root = new node<T>(d);
    last = root;
}

template <class T>
void CircleManager<T>::addData()
{
    bool placeable = true;



    for(float i = MAXRAD;i <= WHEIGHT-MAXRAD;i++)
    {
        for(float j = MAXRAD;j <= WWIDTH-MAXRAD;j++)
        {
            current = root;
            while(current)
            {
                if(current->data.CollisionTemplate(sf::Vector2i(j, i), MAXRAD))
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
                newData(newCir);
                return;
            }
            placeable = true;
        }
    }
}

template <class T>
void CircleManager<T>::updateData(sf::RenderWindow *window, sf::Time dt)
{
    //check for collisions
    current = root;
    while(current)
    {
        node<T>* tempCurr = root;
        while(tempCurr)
        {
                if(current->data.Collision(tempCurr->data))
                {
                    current->data.collisionUpdate = current->data.CollisionSpeed(tempCurr->data);
                    tempCurr->data.collisionUpdate = tempCurr->data.CollisionSpeed(current->data);
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
        current->data.relocate(window->getSize(), dt);
        current = current->next;
    }
    //end relocation

    //draw circles
    window->clear();
    current = root;
    while(current)
    {
        window->draw(current->data.circle);
        current = current->next;
    }
    window->display();
}

template <class T>
void CircleManager<T>::deleteData()
{
    if(root->next)
    {
        current = root->next;
        delete root;
        root = current;

    }
    else if(root)
    {
        delete root;
        root = nullptr;
        last = nullptr;
    }
}


template class CircleManager<Circle>;

