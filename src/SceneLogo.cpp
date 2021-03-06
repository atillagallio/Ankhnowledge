/*
 * SceneLogo.cpp
 *
 */

#include "SceneLogo.h"

SceneLogo::SceneLogo(string scene_name) : Scene (scene_name) {

}

SceneLogo::~SceneLogo() {
}

void SceneLogo::Enter()
{
	AudioHandler * audio = AudioHandler::getInstance();
 	audio->stopMusic();
 	audio->set_music("intro.ogg");
 	audio->play_music(1);

	this->splashSprite = new Sprite(SDLBase::resourcesPath + "splashlogo.png");
	string nextScene = "SceneTechnology";
	SplashScreen * splashScreen =  new SplashScreen(splashSprite,nextScene);

	this->addGameObject(splashScreen);
	//GameManager::fade_screen->fade_out(0,2);
}

void SceneLogo::Exit()
{
	this->clean_game_objects();
}
