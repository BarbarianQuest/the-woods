#include "collisionengine.h"

#include<eventfactory.h>
#include<componentloader.h>
#include<game.h>

using namespace BQ;

CollisionEngine::CollisionEngine() : Engine()
{
    placeholder = "auto_coll_";
    quadtree.setRegion(sf::FloatRect(0,0,480,270));
    quadtree.initialise();
    rectShape.setOutlineThickness(1.0);
    rectShape.setFillColor(sf::Color::Transparent);
}

Collidable & CollisionEngine::addCollidable()
{
    collidables.resize(collidables.size()+1);
    return collidables.back();
}

bool CollisionEngine::checkCollision(Collidable & a,Collidable & b)
{
    if(a.bBox.intersects(b.bBox))
    {
        a.colliding = true;
        b.colliding = true;
        a.setBBoxRectColor(sf::Color::Cyan);
        b.setBBoxRectColor(sf::Color::Cyan);
        return true;
    }
}

void CollisionEngine::run()
{

    quadtree.clear();
    ComponentLoader& components = *componentLoader;
    for(unsigned int i=0; i<collidables.size(); i++)
    {
        collidables[i].setBBox(components.getTransform(collidables[i].getTransform()).getBBox());
        collidables[i].update();
        quadtree.addObject(&collidables[i]);
    }

    quadtree.build();

    for(unsigned int i=0; i<quadtree.flatNodes.size(); i++)
    {
        QuadtreeNode & node = quadtree.flatNodes[i];
        int indexChecked = 0;
        for(unsigned int j=0; j<node.objects.size();j++)
        {
            int jLevel = node.objects[j]->quadtreeLevel;
            if(node.objects[j]->quadtreeLevel == node.level) //primary nodes only fool
            {
                for(unsigned int k=0; k<node.objects.size();k++)
                {
                    if(k==j){continue;}
                    int kLevel = node.objects[k]->quadtreeLevel;
                    if(kLevel == jLevel && k < indexChecked) {continue;}
                    checkCollision(*node.objects[j],*node.objects[k]);
                }
                indexChecked = j;
            }
        }
    }

}

void CollisionEngine::drawDebug()
{
    if(gameWindow != NULL){
        GameWindow& window = *gameWindow;
        rectShape.setOutlineThickness(1.0);
        for(unsigned int i=0; i<quadtree.flatNodes.size(); i++)
        {
            sf::FloatRect bBox = quadtree.flatNodes[i].bounds;
            rectShape.setOutlineColor(sf::Color::Cyan);
            rectShape.setPosition(bBox.left+1,bBox.top+1);
            rectShape.setSize(sf::Vector2f(bBox.width-2,bBox.height-2));
            window.draw(rectShape);
        }
        rectShape.setOutlineThickness(1.0);
        for(unsigned int i=0; i<collidables.size(); i++)
        {
            sf::FloatRect bBox = collidables[i].getBBox();
            rectShape.setOutlineColor(sf::Color::Red);
            rectShape.setPosition(bBox.left+1,bBox.top+1);
            rectShape.setSize(sf::Vector2f(bBox.width-2,bBox.height-2));
            if(collidables[i].colliding)
            {
                rectShape.setOutlineColor(sf::Color::Green);
            }
            window.draw(rectShape);
        }


    }
}

