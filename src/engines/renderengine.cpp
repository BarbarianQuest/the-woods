#include "renderengine.h"

#include<game.h>

using namespace BQ;

RenderEngine::RenderEngine()
{

}


std::vector<SpriteRenderer> RenderEngine::getSprites() const
{
    return sprites;
}

void RenderEngine::setSprites(const std::vector<SpriteRenderer> &value)
{
    sprites = value;
}

BQ::SpriteRenderer &BQ::RenderEngine::addSpriteRenderer()
{
    sprites.resize(sprites.size()+1);
    sprites.back().index = sprites.size()-1;
    sprites.back().name = placeholder + std::to_string(sprites.size());
    return sprites.back();
}

SpriteRenderer &RenderEngine::getSpriteRenderer(int index)
{
    if(index >=0 && index < sprites.size())
    {
        return sprites[index];
    }
    else
    {
        debug->printerr("requested spriterenderer out of bounds");
        return sprites[0]; //todo: this could cause a segfault! Very bad >:(
    }
}

void RenderEngine::wake()
{
    //load sprite sheets

    //load and parse textures.json
    rapidjson::Document tconfig = resourceLoader->loadJsonFile("config/textures.json");

    //get the parent directory for the spritesheets
    assert(tconfig["parentdirectory"].IsString());
    std::string tDir = tconfig["parentdirectory"].GetString();

    //load the spritesheets array
    const rapidjson::Value & files = tconfig["spritesheets"];
    assert(files.IsArray());

    for (rapidjson::SizeType i = 0; i < files.Size(); i++)
    {
        std::string fileName = files[i].GetString();
        debug->println(tDir + "/" + fileName);
        rapidjson::Document spritesheetJson = resourceLoader->loadJsonFile(tDir + "/" + files[i].GetString());

        if(!spritesheetJson.HasMember("name") || !spritesheetJson.HasMember("path"))
        {
            continue;
        }

        assert(spritesheetJson["name"].IsString());
        assert(spritesheetJson["path"].IsString());
        std::string sheetName = spritesheetJson["name"].GetString();
        std::string sheetPath = spritesheetJson["path"].GetString();

        //construct a spritesheet object here
        if ( spriteSheets.find(sheetName) != spriteSheets.end() ) {
            debug->printwarn("duplicate sprite sheet defined in config: " + sheetName);
          // not found
        }
        else
        {
            SpriteSheet s;
            s.texture = sheetPath;

            if(!spritesheetJson.HasMember("sprites") && !spritesheetJson.HasMember("split"))
            {
                debug->println("parser error in " + fileName + ": no sprite data found");
                continue;
            }

            if(spritesheetJson.HasMember("sprites"))
            {

                const rapidjson::Value & animationsJson = spritesheetJson["sprites"];

                assert(animationsJson.IsArray());



                for (rapidjson::SizeType j=0 ; j<animationsJson.Size(); j++)
                {

                    assert(animationsJson[j]["name"].IsString());
                    std::string sprName = animationsJson[j]["name"].GetString();
                    const rapidjson::Value & frames = animationsJson[j]["frames"];
                    assert(frames.IsArray());
                    std::vector<sf::IntRect> rects;
                    for (rapidjson::SizeType k=0 ; k<frames.Size(); k++)
                    {
                        sf::IntRect r;
                        r.left = frames[k]["l"].GetInt();
                        r.top = frames[k]["t"].GetInt();
                        r.width = frames[k]["w"].GetInt();
                        r.height = frames[k]["h"].GetInt();
                        rects.push_back(r);
                    }
                    s.addSprite(sprName,rects);
                }
            }

            if(spritesheetJson.HasMember("split"))
            {
                const rapidjson::Value & split = spritesheetJson["split"].GetObject();
                if(!split.HasMember("w") || !split.HasMember("h") || !split.HasMember("x_count") || !split.HasMember("y_count"))
                {
                    debug->println("parser error in " + fileName + ": split data incomplete");
                    continue;
                }

                int xCount = split["x_count"].GetInt();
                int yCount = split["y_count"].GetInt();
                int width = split["w"].GetInt();
                int height = split["h"].GetInt();

                for(int j = 0; j<yCount; j++)
                {
                    for(int k=0; k<xCount; k++)
                    {
                        std::vector<sf::IntRect> rects;
                        int sprIndex = j*xCount + k;
                        std::string sprName = std::to_string(sprIndex);
                        sf::IntRect r;
                        r.left = k*width;
                        r.top = j*height;
                        r.width = width;
                        r.height = height;
                        rects.push_back(r);
                        s.addSprite(sprName,rects);
                    }
                }

            }


            spriteSheets[sheetName] = s;
        }

    }



}

void BQ::RenderEngine::start()
{

}

void BQ::RenderEngine::run()
{

    for(int i=0; i<activeComponents.size(); i++)
    {
        SpriteRenderer & s = sprites[activeComponents[i]];
        s.update();
        s.depth = componentLoader->getTransform(s.transform).position.y + s.depthOffset;
    }
}

void BQ::RenderEngine::finish()
{

}

void BQ::RenderEngine::drawDebug()
{
    std::vector<SpriteRenderer> renderList;

    Level & l = game->levels["butterfly_demo"];

    sf::Sprite tSpr;

    for(int j = 0; j<l.tileMap.tileLayers.size(); j++)
    {
        TileLayer & layer = l.tileMap.tileLayers[j];
        SpriteSheet & tSheet = spriteSheets[layer.tileset];
        sf::Texture & layerTexture = resourceLoader->getTexture(tSheet.texture);
        tSpr.setTexture(layerTexture);

        for(int i=0; i<grid->activeSquares.size(); i++)
        {
            GridSquare & g = grid->activeSquares[i];

            Tile & t = l.tileMap.getTile(j,g.position);
            if(t.index == -1)
            {
                continue;
            }
            tSpr.setTextureRect(tSheet.getSprite(t.index)[0]);
            tSpr.setPosition(grid->getCentre(g.position));
            game->gameWindow->draw(tSpr);
        }

    }

    for(int i=0; i<activeComponents.size(); i++)
    {
        renderList.push_back(sprites[activeComponents[i]]);
    }

    std::sort(renderList.begin(),renderList.end());

    std::vector<int> renderOrder;

    for(int i=0; i<renderList.size(); i++)
    {
        renderOrder.push_back(renderList[i].index);
    }

    for(int i=0; i<renderOrder.size(); i++)
    {
        SpriteRenderer & s = sprites[renderOrder[i]];
        sf::Sprite spr;
        if ( spriteSheets.find(s.spritesheet) == spriteSheets.end() ) {
            spr.setTexture(resourceLoader->getTexture(""));
        }
        else
        {
            SpriteSheet& sheet = spriteSheets[s.spritesheet];
            spr.setTexture(resourceLoader->getTexture(sheet.texture));
            std::string clip = "";

            if(sheet.spriteFrames.find(s.clip) != sheet.spriteFrames.end())
            {
                clip = s.clip;
            }
            else if(sheet.spriteFrames.size() > 0)
            {
                clip = sheet.spriteFrames.begin()->first;
            }

            if(clip != "")
            {
                bool spritefound = true;
                if(s.frame < 0 || s.frame >= sheet.spriteFrames[clip].size())
                {
                    if(sheet.spriteFrames[clip].size() > 0)
                    {
                        s.frame = 0;
                    }
                    else
                    {
                        spritefound = false;
                    }
                }

                if(spritefound)
                {
                    spr.setTextureRect(sheet.spriteFrames[clip][s.frame]);
                }
            }


        }

        sf::Vector2f pos = componentLoader->getTransform(s.transform).position;
        if(s.centreOrigin)
        {
            pos.x -= 0.5*spr.getLocalBounds().width;
            pos.y -= 0.5*spr.getLocalBounds().height;
        }
        spr.setPosition(maths->round(pos + s.offset));
        gameWindow->draw(spr);
    }
}
