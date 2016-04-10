#include "resourceloader.h"

using namespace BQ;

ResourceLoader::ResourceLoader()
{
    baseDirectory = "var";
    fontDirectory = "fonts";
    textureDirectory = "textures";
}

DebugUtils *ResourceLoader::getDebug() const
{
    return debug;
}

void ResourceLoader::setDebug(DebugUtils *value)
{
    debug = value;
}

sf::Font &ResourceLoader::getFont(std::string name)
{
    std::map<std::string,sf::Font>::const_iterator it = fonts.find(name);
    bool exists = it!=fonts.end();
    if(exists) {return fonts[name];}

    //the font is not in the map

    sf::Font font;

    //only load the font if it's not present in the map
    if(!font.loadFromFile(baseDirectory + "/" + fontDirectory + "/" + name))
    {
        debug->println("font not found");
        debug->printerr("font " + name + " not found");
    }
    fonts[name] = font;
    return fonts[name];
}

sf::Texture &ResourceLoader::getTexture(std::string name)
{
    std::map<std::string,sf::Texture>::const_iterator it = textures.find(name);
    bool exists = it!=textures.end();
    if(exists) {return textures[name];}

    //the texture is not in the map

    sf::Texture texture;

    if(!texture.loadFromFile(baseDirectory + "/" + textureDirectory + "/" + name))
    {
        debug->println("texture not found");
        debug->printerr("texture " + name + " not found");
    }

    textures[name] = texture;
    return textures[name];

}

