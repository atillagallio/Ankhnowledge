/*
 * Map.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: makoto
 */

#include "Map.h"
#include "BlockWater.h"
#include "BlockMovable.h"
#include "BlockTreasure.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SkillDig.h"
#include "SkillSand.h"
#include "CharacterSuti.h"
#include "CharacterAlan.h"


using namespace std;

Map::Map(Sprite * tile, Sprite * block, string mapLink, float x, float y):GameObject(x,y) {
	this->tile = tile;
	this->tile->incNumRef();
	this->block = block;
	this->block->incNumRef();
	this->columns = 20;
	this->rows = 20;

	SDLBase::initializeSDLTTF();
	color.r = 255;
	color.g = 255;
	color.b = 255;
	string font_path = SDLBase::resourcesPath + "quicksandbold.ttf";
	font = SDLBase::loadFont(font_path.c_str(),40);
	tileMap = new TileMap(mapLink,tile,block,getX(),getY());
	this->tiles = tileMap->getTiles();
	this->columns = tileMap->getColumns();
	this->rows = tileMap->getRows();

	/*
	 * Connecting all the tiles
	 */
	for(int i = 0; i < (columns*rows); i++)
	{
		if(!isLastColumn(i))
			tiles.at(i)->setRightTile(tiles.at(i+1));

		if(!isFirstColumn(i))
			tiles.at(i)->setLeftTile(tiles.at(i-1));

		if(!isFirstRow(i))
			tiles.at(i)->setUpTile(tiles.at(i-columns));

		if(!isLastRow(i))
			tiles.at(i)->setDownTile(tiles.at(i+columns));
	}
	cout<<mapLink<<endl;
	player1 = new CharacterAlan(tiles.at(tileMap->getP1Position()), 1);
 	player2 = new CharacterSuti(tiles.at(tileMap->getP2Position()), 2);
	player1->setMap(this);
	player2->setMap(this);
	currentPlayer = player1;
	currentPlayer->setTurn(true);
	changePlayer = false;
	this->endButton = new Button(
									new Sprite(SDLBase::resourcesPath + "endturn_2.png"),
									new Sprite(SDLBase::resourcesPath + "endturn_1.png"),
									new Sprite(SDLBase::resourcesPath + "endturn_3.png"),
									660,
									460);
	deltaEnd = 0;
}

Map::~Map() {

	cout<<"sai3"<<endl;
	for(int i = 0; i < (columns*rows); i++)
	{
		tiles.at(i)->cleanReferences();
	}
	delete player1;
	delete player2;
	this->tile->decNumRef();
	this->tile = 0;
	this->block->decNumRef();
	this->block = 0;
	delete tileMap;


	// TODO Auto-generated destructor stub
}

void Map::render(float cameraX, float cameraY){
	for(unsigned int i = 0; i < tiles.size(); i++)
	{
		tiles.at(i)->render(0,0);
	}
	for(unsigned int i = 0; i < tiles.size(); i++)
	{
		if(tiles.at(i)->getBlock())
			tiles.at(i)->getBlock()->render(getX(), getY());
		if(tiles.at(i)->isClickable())
			tiles.at(i)->renderClickableTiles();
	}




	if(player1->getY() > player2->getY())
	{
		player2->render(0,0);
		player1->render(0,0);
	}
	else
	{
		player1->render(0,0);
		player2->render(0,0);
	}

	if(currentPlayer)
		currentPlayer->getHud()->render(0,0);
	stringstream st;
	st << ((int)(currentPlayer->getStamina()));
	SDLBase::renderText(font, st.str(),color,720,60);

	endButton->render(0,0);

}

int Map::update(int dt)
{
	InputManager* input;
	input = InputManager::getInstance();

	for(unsigned int i = 0; i < tiles.size(); i++)
	{
		tiles.at(i)->update(dt);
		if(tiles.at(i)->isClickable())
			tiles.at(i)->animateClickableTiles(dt);
	}
	
	if(currentPlayer->isUsingSkill())
	{
		if(input->isMousePressed(1))
		{
			Tile* currentTile = getPressedTile();
			currentPlayer->setSkillDestTile(currentTile);
		}		
	}

	player1->update(dt);
	player2->update(dt);

	deltaEnd += dt;
	mouseOver(endButton, input);
	mousePressed(endButton, input);
	this->endButton->update(dt);

	if(!currentPlayer->getWin())
	{
		if(currentPlayer->getStamina() <= 0 && !changePlayer)
		{
			changePlayer = true;
		}

		if(changePlayer)
		{
			changeCurrentPlayer();
		}
	}else
	{
		currentPlayer->setTurn(false);
	}

	for(unsigned int i = 0; i < tiles.size(); i++)
	{
		if(tiles.at(i)->getBlock())
		{
			if (tiles.at(i)->getBlock()->getDestroyedFinalBlock())
			{
			Block* tempBlock = tiles.at(i)->getBlock();
			tiles.at(i)->setBlock(0);
			delete tempBlock;
			}
		}
	}

	return 0;
}

Tile * Map::getPressedTile()
{
	for(unsigned int i = 0; i < tiles.size(); i++)
	{
		if(tiles.at(i)->isClickable())
		{
			if(tiles.at(i)->insideTile())
			{
				return tiles.at(i);
			}
		}
	}

	return 0;
}


bool Map::isFirstRow(int index)
{
	return (index < columns);
}

bool Map::isFirstColumn(int index)
{
	return ((index % columns) == 0);
}

bool Map::isLastRow(int index)
{
	return ((index + columns) > (columns*rows - 1));
}

bool Map::isLastColumn(int index)
{
	return ((index % columns) == (columns - 1));
}

void Map::mousePressed(Button *bt, InputManager* input){

	if((input->isMousePressed(1)) && (bt->insideButton()) == 1)
	{
		bt->mousePressed(true);
		if(deltaEnd > 1000 &&!currentPlayer->isPerformingAction() && currentPlayer->getId() == Network::getID() && !currentPlayer->isUsingSkill())
		{
			deltaEnd = 0;
			changeCurrentPlayer();
			currentPlayer->sendMessage("EndTurn", "-1");
		}

	}
	else
	{
		bt->mousePressed(false);
	}
}

void Map::mouseOver(Button *bt, InputManager * input){
	bt->setMouseCoord(input->mousePosX(),input->mousePosY());
}

void Map::changeCurrentPlayer()
{
	if(!currentPlayer->isPerformingAction() && !currentPlayer->isUsingSkill())
	{
		changePlayer = false;
		if(currentPlayer->getId() == 1)
			currentPlayer = player2;
		else
			currentPlayer = player1;

		player1->setStamina(player1->getInitialStamina());
		player2->setStamina(player2->getInitialStamina());
		player1->toogleTurn();
		player2->toogleTurn();
	}
}

Tile * Map::getTileWithIndex(int index)
{
	return tiles.at(index);
}

