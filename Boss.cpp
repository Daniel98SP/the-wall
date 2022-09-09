#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Boss.h"
#include "Game.h"
#include "include/irrKlang.h"
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>


using namespace std;
using namespace irrklang;


#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace std;

#define SCREEN_X 32
#define SCREEN_Y 16

#define BOSS_SIZE_X 100
#define BOSS_SIZE_Y 100

enum class ReboundType
{
	LEFT, RIGHT, TOP, BOTTOM, NONE
};

enum BossAnims
{
	LEFT, RIGHT
};

ShaderProgram Boss::getShaderProgram() {
	return shaderP;
}

bool Boss::isDead() {
	return dead;
}

void Boss::inflictDamage() {

	if (time >= timeLastHit + 4000) {
		health--;
		timeLastHit = time;

		if (health <= 0) {
			die();
			engine->stopAllSounds();
			engine->play2D("./Sounds/trumpDeath.mp3");
			engine->play2D("./Sounds/explosion.wav");
			currentSentence = engine->play2D("./Sounds/winAudio.wav", false, false, true);


		}
		else {
			int randNumb = rand() %6 + 1;
			string str = "./Sounds/trump";
			char fileName[1024];
			strcpy(fileName, (str + to_string(soundCounter) + ".mp3").c_str());

			engine->stopAllSounds();
			currentSentence = engine->play2D(fileName, false, false, true);
			engine->play2D("./Sounds/hitBoss.wav");

			soundCounter++;
		}
	}

}

void Boss::die() {
	dead = true;
	timeDead = time;
}

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	engine = createIrrKlangDevice(); 
	currentSentence = engine->play2D("./Sounds/trump6.mp3", false, false, true);

	soundCounter = 1;
	sentenceSlowed = false;
	srand((time));
	inMovement = false;
	dead = false;
	health = 6;
	shaderP = shaderProgram;
	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(BOSS_SIZE_X, BOSS_SIZE_Y), glm::vec2(0.2, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	
		sprite->setAnimationSpeed(LEFT, 8);
		sprite->addKeyframe(LEFT, glm::vec2(0.0, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.2, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.4, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.6, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.8, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.6, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.4, 0.0));
		sprite->addKeyframe(LEFT, glm::vec2(0.2, 0.0));

		sprite->setAnimationSpeed(RIGHT, 8);
		sprite->addKeyframe(RIGHT, glm::vec2(0.0, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.2, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.4, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.6, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.8, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.6, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.4, 0.5));
		sprite->addKeyframe(RIGHT, glm::vec2(0.2, 0.5));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));	
}

glm::ivec2 Boss::getPosition() {
	return posBoss;
}

bool Boss::update(int deltaTime, const glm::vec4 &cameraPosition, bool timeScale)
{
	time += deltaTime;

	if (timeScale && !sentenceSlowed) {
		if(currentSentence)
			currentSentence->setPlaybackSpeed(0.5f);
		sentenceSlowed = true;
	}
	else if (!timeScale && sentenceSlowed) {
		if (currentSentence)
			currentSentence->setPlaybackSpeed(1.f);
		sentenceSlowed = false;
	}



	if (!dead) {

		sprite->update(deltaTime);

		movementManagement(cameraPosition, timeScale);

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	
	}
	else if (dead && time > timeDead +25000)
		return true;
	
	return false;
}

void Boss::render()
{
	if (!dead) {

		if (time <= timeLastHit + 3000) {

			if ((int)time % 2 == 0 || (int)time % 3 == 0)
				sprite->render();
		}
		else sprite->render();
	}
}

void Boss::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Boss::setPosition(const glm::vec2 &pos)
{
	posBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::movementManagement(const glm::vec4& cameraPosition, bool timeScale) {
	if (inMovement) {
		//posBoss.x += timeScale ? glm::normalize(direction.x) : direction.x;
		//posBoss.y += timeScale ? glm::normalize(direction.y) : direction.y;
		posBoss += timeScale ? glm::normalize(direction) : direction;

		if (time - movementStart > 50000) {
			inMovement = false;
			movementEnd = time;
			posBoss.x = -200;
			posBoss.y = -200;
		}

		else {
			ReboundType rebound = ReboundType::NONE;
			if (posBoss.x + BOSS_SIZE_X >= cameraPosition[1]) {
				rebound = ReboundType::RIGHT;
				posBoss.x = cameraPosition[1] - BOSS_SIZE_X - 10;
			}
			else if (posBoss.y + BOSS_SIZE_Y >= cameraPosition[2]) {
				rebound = ReboundType::BOTTOM;
				posBoss.y = cameraPosition[2] - BOSS_SIZE_Y - 10;
			}
			else if (posBoss.x <= cameraPosition[0]) {
				rebound = ReboundType::LEFT;
				posBoss.x = cameraPosition[0] + 10;
			}
			else if (posBoss.y <= cameraPosition[3]) {
				rebound = ReboundType::TOP;
				posBoss.y = cameraPosition[3] + 10;
			}

			if (rebound != ReboundType::NONE) {
				glm::vec2 oldDirection;
				oldDirection.x = direction.x;
				oldDirection.y = direction.y;

				direction.x = (rand() % 3) + 2;
				direction.y = (rand() % 3) + 2;

				if ((rebound == ReboundType::LEFT || rebound == ReboundType::RIGHT) && oldDirection.x > 0) {
					direction.x *= (-1);
					if (oldDirection.y < 0) {
						direction.y *= (-1);
					}
				}
				else if ((rebound == ReboundType::TOP || rebound == ReboundType::BOTTOM) && oldDirection.y > 0) {
					direction.y *= (-1);
					if (oldDirection.x < 0) {
						direction.x *= (-1);
					}
				}

				if (rebound != ReboundType::NONE && direction.x > 0 && oldDirection.x < 0) {
					sprite->changeAnimation(1);
				}
				else if (rebound != ReboundType::NONE && direction.x < 0 && oldDirection.x > 0) {
					sprite->changeAnimation(0);
				}
			}
		}
	}

	else if (time - movementEnd > 20000) {
		direction.x = (rand() % 3) + 2;
		direction.y = (rand() % 3) + 2;

		posBoss.x = (rand() % int(cameraPosition[1]) - 1);
		posBoss.y = rand() % ((int(cameraPosition[2]) - 1) - (int(cameraPosition[3]) - 1) + 1) + (int(cameraPosition[3]) - 1);

		inMovement = true;
		movementStart = time;
		sprite->changeAnimation(1);
	}
}




