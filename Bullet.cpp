#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Bullet.h"
#include "Game.h"

using namespace std;


#define VELOCITY 2

enum BulletAnims
{
	MOVE
};

void Bullet::stop() {
	stopped = true;
}

void Bullet::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Sprite *spr)
{
	if (spr == nullptr) {
		spritesheet.loadFromFile("images/enemy_bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(MOVE, 1);
		sprite->addKeyframe(MOVE, glm::vec2((0.0, 0.0)));
		sprite->addKeyframe(MOVE, glm::vec2((0.2, 0.0)));
		sprite->addKeyframe(MOVE, glm::vec2((0.4, 0.0)));
		sprite->addKeyframe(MOVE, glm::vec2((0.6, 0.0)));
		sprite->addKeyframe(MOVE, glm::vec2((0.8, 0.0)));
		sprite->addKeyframe(MOVE, glm::vec2((0.0, 0.2)));
		sprite->addKeyframe(MOVE, glm::vec2((0.2, 0.2)));
		sprite->addKeyframe(MOVE, glm::vec2((0.4, 0.2)));
		sprite->addKeyframe(MOVE, glm::vec2((0.6, 0.2)));
		sprite->addKeyframe(MOVE, glm::vec2((0.8, 0.2)));
		sprite->addKeyframe(MOVE, glm::vec2((0.0, 0.4)));
		sprite->addKeyframe(MOVE, glm::vec2((0.2, 0.4)));

		sprite->changeAnimation(0);
	}
	else {
		sprite = spr;
	}
	
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}

glm::ivec2 Bullet::getPosition() {
	return posBullet;
}

bool Bullet::update(int deltaTime, bool timeScale, bool player)
{
	if (!stopped) {
		sprite->update(deltaTime);
		float deltaX;
		float deltaY;

		if (timeScale) {
			if (!player) {
				deltaX = direction.x * VELOCITY / 3;
				deltaY = direction.y * VELOCITY / 3;
			}
			else {
				deltaX = direction.x * VELOCITY / 2;
				deltaY = direction.y * VELOCITY / 2;
			}
		}

		else {
			deltaX = direction.x * VELOCITY;
			deltaY = direction.y * VELOCITY;

		}

		if (!stopped && deltaX > 0) {
			if (!map->collisionMoveRight(posBullet, glm::ivec2(12, 12))) 

				posBullet.x += deltaX;
			
			else 
				stop();
			
		}

		else if (!stopped && deltaX < 0) {
			if (!map->collisionMoveLeft(posBullet, glm::ivec2(12, 12))) 
				posBullet.x += deltaX;
			
			else 
				stop();
			

		}

		if (!stopped && deltaY < 0 ) {
			if (!map->collisionMoveUp(posBullet, glm::ivec2(12, 12)))
				posBullet.y += deltaY;
			else
				stop();

		}

		else if (!stopped && deltaY > 0) {

			if (!map->collisionMoveDown(posBullet, glm::ivec2(12, 12)))
				posBullet.y += deltaY;
			else
				stop();

		}

		return false;
	}
	else return true;
}

void Bullet::render()
{
	if (!stopped) {
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
		sprite->render();
	}
}

void Bullet::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Bullet::setPosition(const glm::vec2 &pos, const glm::vec2 &dir)
{
	posBullet = pos;
	direction = dir;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBullet.x), float(tileMapDispl.y + posBullet.y)));
}




