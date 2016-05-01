/*
 * ScenePhaseOne.cpp
 *
 *  Created on: Oct 8, 2013
 *      Author: al
 */

#include "ScenePhaseOne.h"
#include "AudioHandler.h"

ScenePhaseOne::ScenePhaseOne(string scene_name) : Scene (scene_name) {


}

ScenePhaseOne::~ScenePhaseOne() {
	// TODO Auto-generated destructor stub
}

void ScenePhaseOne::Enter()
{
	AudioHandler * audio = AudioHandler::getInstance();
	audio->stopMusic();
	audio->set_music("fase1.ogg");
	audio->play_music(-1);

	this->splashSprite = new Sprite(SDLBase::resourcesPath + "interface.png");
	this->tile = new Sprite(SDLBase::resourcesPath + "tile.png");
	this->block = new Sprite(SDLBase::resourcesPath + "areiamovedicabloco.png");
	this->map = new Map(tile, block, SDLBase::resourcesPath +"tileMap.txt", 200, 100);
	string nextScene = "ScenePhaseOne" ;
	SplashScreen * splashScreen =  new SplashScreen(splashSprite,nextScene);
	this->addGameObject(splashScreen);
	this->addGameObject(map);
	GameManager::fadeScreen->fade_out(0,2);
}

void ScenePhaseOne::Exit()
{
	this->clean_game_objects();
}

