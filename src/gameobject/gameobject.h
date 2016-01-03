#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<transform.h>

#include<component.h>
#include<componentloader.h>
#include<gamelogic.h>
#include <iostream>
#include <vector>
#include<functional>
#include<memory>
#include <map>

namespace BQ{
class GameObject
{
public:
    GameObject();

    int transform;

    ComponentLoader * componentLoader;

    std::string name;
    int uniqueId;

    void addComponent(Component&);
    void addComponent(std::string,Component&);

    Transform & loadTransform();
    int getTransform() const;
    void setTransform(int value);

    ComponentLoader *getComponentLoader() const;
    void setComponentLoader(ComponentLoader *value);

    std::map<std::string, std::map<std::string, int> > getComponents() const;

    std::map<std::string,std::map<std::string,int>> components;

private:



};
}

#endif // GAMEOBJECT_H
