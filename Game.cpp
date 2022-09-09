#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	scene.init(-1);
}

bool Game::update(int deltaTime)	
{
	scene.update(deltaTime);
	if (Game::instance().checkIfKeyPressed('R')) {
		scene.init(1);
		}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int sceneToLoad = scene.render();
	if (sceneToLoad != 99) {
		scene.init(sceneToLoad);
		if(sceneToLoad > 0 && sceneToLoad < 4)
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::checkIfKeyPressed(char key) {
	bool pressed = false;
	
	if (key >= 65 && key <= 90) {
		return keys[int(key)] || keys[int(key) + 32];
	}
	else if (key >= 97 && key <= 122) {
		return keys[int(key)] || keys[int(key) - 32];
	}
	else {
		return keys[int(key)];
	}
}

bool Game::anyKeyPressed() {
	for each (bool isPressed in keys) {
		if (isPressed)
			return true;
	}
	return false;
	
}
bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





