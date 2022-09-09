#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "include/irrKlang.h"
#include <conio.h>
#include <Windows.h>

using namespace std;
using namespace irrklang;


#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


using namespace std;

#define SCREEN_X 32
#define SCREEN_Y 16

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define FIRE_RATE 1 //segundos entre disparos
#define ENEMY_SIZE_OFFSET_X 18.5
#define ENEMY_SIZE_OFFSET_Y 5.0

ShaderProgram Enemy::getShaderProgram() {
	return shaderP;
}


void Enemy::inflictDamage() {
	health--;
	if (health <= 0) {
		die();
		engine->play2D("./Sounds/dieEnemy.wav");
		timeDeath = time;
		if (cameraType == CameraType::SIDE_VIEW) {
			sprite->changeAnimation(DEAD);
		}
		else {
			sprite->changeAnimation(DEAD_TOPDOWN);
		}
	}
	engine->play2D("./Sounds/hitEnemy.wav");

}


void Enemy::die() {

	dead = true;
}

enum BulletAnims
{
	SPAWN, MOVE, COLLIDE
};

void Enemy::sideViewEnemy(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	timeLastShot = 0;
	health = 3;
	shaderP = shaderProgram;
	cameraType = CameraType::SIDE_VIEW;
	moving = false;
	dead = false;
	
	engine = createIrrKlangDevice();
  
  spritesheet.loadFromFile("images/enemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(37, 48), glm::vec2(0.16667, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
		sprite->setAnimationSpeed(STAND_STILL, 8);
		sprite->addKeyframe(STAND_STILL, glm::vec2(0.16667, 0.f));	

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.8333, 0.75));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6667, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3333, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.16667, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.8333, 0.25));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6667, 0.25));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.25));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3333, 0.25));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.16667, 0.25));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25));

		sprite->setAnimationSpeed(DEAD, 6);
		sprite->addKeyframe(DEAD, glm::vec2(0.0, 0.5));
		sprite->addKeyframe(DEAD, glm::vec2(0.1667, 0.5));
		sprite->addKeyframe(DEAD, glm::vec2(0.3333, 0.5));
		sprite->addKeyframe(DEAD, glm::vec2(0.5, 0.5));
		sprite->addKeyframe(DEAD, glm::vec2(0.6667, 0.5));
		sprite->addKeyframe(DEAD, glm::vec2(0.8333, 0.5));
		
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));	
}

void Enemy::topDownViewEnemy(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	timeLastShot = 0;
	health = 3;
	shaderP = shaderProgram;
	cameraType = CameraType::TOPDOWN_VIEW;
	moving = false;
	dead = false;
  
  engine = createIrrKlangDevice();

	topdownSpritesheet.loadFromFile("images/topdown_enemy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(25, 35), glm::vec2(0.0833, 0.5), &topdownSpritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_UP_TOPDOWN, 8);
	sprite->addKeyframe(STAND_UP_TOPDOWN, glm::vec2(0.9167, 0.0));

	sprite->setAnimationSpeed(STAND_RIGHT_TOPDOWN, 8);
	sprite->addKeyframe(STAND_RIGHT_TOPDOWN, glm::vec2(0.8333, 0.0));

	sprite->setAnimationSpeed(STAND_DOWN_TOPDOWN, 8);
	sprite->addKeyframe(STAND_DOWN_TOPDOWN, glm::vec2(0.6667, 0.0));

	sprite->setAnimationSpeed(STAND_LEFT_TOPDOWN, 8);
	sprite->addKeyframe(STAND_LEFT_TOPDOWN, glm::vec2(0.75, 0.0));

	sprite->setAnimationSpeed(MOVE_UP_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.9167, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.50, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.9167, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.5833, 0.0));

	sprite->setAnimationSpeed(MOVE_RIGHT_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.8333, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.3333, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.8333, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.4167, 0.0));

	sprite->setAnimationSpeed(MOVE_DOWN_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.6667, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.0, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.6667, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.0833, 0.0));

	sprite->setAnimationSpeed(MOVE_LEFT_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.75, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.1667, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.75, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.25, 0.0));

	sprite->setAnimationSpeed(DEAD_TOPDOWN, 6);
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.0, 0.5));
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.0833, 0.5));
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.1667, 0.5));
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.25, 0.5));
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.3333, 0.5));
	sprite->addKeyframe(DEAD_TOPDOWN, glm::vec2(0.4167, 0.5));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::ivec2 Enemy::getPosition() {
	return posEnemy;
}

vector<Bullet*> Enemy::update(int deltaTime, const glm::vec2& playerPosition, bool timeScale)
{
	time += deltaTime;
	sprite->update(deltaTime);

	if (!dead) {
		float distToPlayer = distanceToPlayer(playerPosition);

		shootingManagement(deltaTime, playerPosition, distToPlayer, timeScale);

		movementManagement(playerPosition, distToPlayer, timeScale);

		if (cameraType == CameraType::SIDE_VIEW) {
			posEnemy.y += FALL_STEP;
			if (map->collisionOnFall(posEnemy, glm::ivec2(30, 40), &posEnemy.y, true)) {
				falling = false;
			}
			else {
				falling = true;
			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	}
	else {
	}
	return bullets;
}

bool Enemy::render()
{
	if ((!dead) || dead && (time - timeDeath) < 1000) {
		sprite->render();

		if (!dead) {
			for (int i = 0; i < bullets.size(); i++) {
				bullets[i]->render();
			}
		}

		return false;
	}
	else {
		return true;
	}
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

bool Enemy::isDead() {
	return dead;
}

float Enemy::distanceToPlayer(const glm::vec2& playerPosition) {
	float distanceX = abs(playerPosition.x - posEnemy.x + ENEMY_SIZE_OFFSET_X);
	float distanceY = abs(playerPosition.y - posEnemy.y + ENEMY_SIZE_OFFSET_Y);

	return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
}

glm::vec2 Enemy::directionToPlayer(const glm::vec2& playerPosition) {
	glm::vec2 direction;

	float distanceX = playerPosition.x - posEnemy.x + ENEMY_SIZE_OFFSET_X;
	float distanceY = playerPosition.y - posEnemy.y + ENEMY_SIZE_OFFSET_Y;
	float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

	direction.x = distanceX / distance * 3;
	direction.y = distanceY / distance * 3;

	return direction;
}

void Enemy::shootingManagement(float deltaTime, const glm::vec2& playerPosition, float distToPlayer, bool timeScale) {
	if (rand() % 20 == 0 && distToPlayer < SCREEN_WIDTH / 2 && (double(time) - double(timeLastShot)) > FIRE_RATE * 1000) {
		Bullet* bullet = new Bullet();
		bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderP, nullptr);
		glm::vec2 dirToPlayer = directionToPlayer(playerPosition);
		bullet->setPosition(glm::vec2(posEnemy.x + ENEMY_SIZE_OFFSET_X, posEnemy.y + ENEMY_SIZE_OFFSET_Y), dirToPlayer);
		bullet->setTileMap(map);
		bullets.push_back(bullet);
		timeLastShot = time;
	}

	vector<Bullet*> bulletsAux;
	for (int i = 0; i < bullets.size(); i++) {
		bool destroyed = bullets[i]->update(deltaTime, timeScale, false);
		if (!destroyed)
			bulletsAux.push_back(bullets[i]);

	}

	bullets = bulletsAux;
}

void Enemy::movementManagement(const glm::vec2& playerPosition, float distToPlayer, bool timeScale) {
	frames++;
	if (distToPlayer < SCREEN_WIDTH / 2 && distToPlayer > SCREEN_WIDTH / 4 ) {
		
		int distX = playerPosition.x - posEnemy.x;
		int distY = playerPosition.y - posEnemy.y;

		int velocity;

		if (!timeScale || frames % 3 == 0)
			velocity = 1;
		else velocity = 0;
	
		// SIDE VIEW
		if (cameraType == CameraType::SIDE_VIEW) {
			if (distX > 5) {
				posEnemy.x += velocity ;

				if (map->collisionMoveRight(posEnemy, glm::ivec2(30, 40), true))
				{
					posEnemy.x -= velocity ;
						sprite->changeAnimation(STAND_STILL);
				}
				else {
					if (sprite->animation() != MOVE_RIGHT && !timeScale) {
						sprite->changeAnimation(MOVE_RIGHT);
					}
				}
			}
			else if (distX < -5) {
				posEnemy.x -= velocity ;

				if (map->collisionMoveLeft(posEnemy, glm::ivec2(30, 40), true))
				{
					posEnemy.x += velocity ;
						sprite->changeAnimation(STAND_STILL);
				}
				else {
					if (sprite->animation() != MOVE_LEFT && !timeScale) {
						sprite->changeAnimation(MOVE_LEFT);
					}
				}
			}
			else {
				sprite->changeAnimation(STAND_STILL);
			}
		}

		// TOPDOWN VIEW
		else {
			// If the enemy is further in the X axis
			if (abs(distX) >= abs(distY)) {
				// If the player is in the enemy's right
				if (distX >= 0) {
					// If the enemy cannot advance to the right
					if (map->collisionMoveRight(glm::ivec2(posEnemy.x + 5, posEnemy.y), glm::ivec2(30, 40), true)) {
						// If the player is in the enemy's bottom
						if (distY >= 0) {
							// If the enemy cannot advance to the bottom
							if (map->collisionMoveDown(glm::ivec2(posEnemy.x, posEnemy.y + 5), glm::ivec2(30, 40), true)) {
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the bottom
							else {
								posEnemy.y += velocity;
								if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
								}
							}
						}
						// If the player is in the enemy's top
						else {
							// If the enemy cannot advance to the top
							if (map->collisionMoveUp(glm::ivec2(posEnemy.x, posEnemy.y - 5), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the top
							else {
								posEnemy.y -= velocity ;
								if (sprite->animation() != MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(MOVE_UP_TOPDOWN);
								}
							}
						}
					}
					else {
							posEnemy.x += velocity ;
						if (sprite->animation() != MOVE_RIGHT_TOPDOWN) {
							sprite->changeAnimation(MOVE_RIGHT_TOPDOWN);
						}
					}
				}
				// If the player is in the enemy's left
				else {
					// If the enemy cannot advance to the left
					if (map->collisionMoveLeft(glm::ivec2(posEnemy.x - 5, posEnemy.y), glm::ivec2(30, 40), true)) {
						// If the player is in the enemy's bottom
						if (distY >= 0) {
							// If the enemy cannot advance to the bottom
							if (map->collisionMoveDown(glm::ivec2(posEnemy.x, posEnemy.y + 5), glm::ivec2(30, 40), true))	{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the bottom
							else {
								posEnemy.y += velocity ;
								if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
								}
							}
						}
						// If the player is in the enemy's top
						else {
							// If the enemy cannot advance to the top
							if (map->collisionMoveUp(glm::ivec2(posEnemy.x, posEnemy.y - 5), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the top
							else {
								posEnemy.y-= velocity;
								if (sprite->animation() != MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(MOVE_UP_TOPDOWN);
								}
							}
						}
					}
					// If the enemy can advance to the left
					else {
						posEnemy.x -=velocity ;
						if (sprite->animation() != MOVE_LEFT_TOPDOWN) {
							sprite->changeAnimation(MOVE_LEFT_TOPDOWN);
						}
					}
				}
			}
			// If the enemy is further in the Y axis
			else {
				// If the player is in the enemy's bottom
				if (distY >= 0) {
					// If the enemy cannot advance to the bottom
					if (map->collisionMoveDown(glm::ivec2(posEnemy.x, posEnemy.y + 5), glm::ivec2(30, 40), true)) {
						// If the player is in the enemy's right
						if (distX >= 0) {
							// If the enemy cannot advance to the right
							if (map->collisionMoveRight(glm::ivec2(posEnemy.x + 5, posEnemy.y), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the right
							else {
								posEnemy.x+= velocity;
								if (sprite->animation() != MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(MOVE_RIGHT_TOPDOWN);
								}
							}
						}
						// If the player is in the enemy's left
						else {
							// If the enemy cannot advance to the left
							if (map->collisionMoveLeft(glm::ivec2(posEnemy.x - 5, posEnemy.y), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the left
							else {
								posEnemy.x-= velocity;
								if (sprite->animation() != MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(MOVE_LEFT_TOPDOWN);
								}
							}
						}
					}
					// If the enemy can advance to the bottom
					else {
						posEnemy.y += velocity;
						if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
							sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
						}
					}
				}
				// If the player is in the enemy's top
				else {
					// If the enemy cannot advance to the top
					if (map->collisionMoveUp(glm::ivec2(posEnemy.x, posEnemy.y - 5), glm::ivec2(30, 40), true)) {
						// If the player is in the enemy's right
						if (distX >= 0) {
							// If the enemy cannot advance to the right
							if (map->collisionMoveRight(glm::ivec2(posEnemy.x + 5, posEnemy.y), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the right
							else {
								posEnemy.x += velocity;
								if (sprite->animation() != MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(MOVE_RIGHT_TOPDOWN);
								}
							}
						}
						// If the player is in the enemy's left
						else {
							// If the enemy cannot advance to the left
							if (map->collisionMoveLeft(glm::ivec2(posEnemy.x - 5, posEnemy.y), glm::ivec2(30, 40), true))
							{
								if (sprite->animation() == MOVE_UP_TOPDOWN) {
									sprite->changeAnimation(STAND_UP_TOPDOWN);
								}
								if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
									sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
								}
								if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
									sprite->changeAnimation(STAND_DOWN_TOPDOWN);
								}
								if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(STAND_LEFT_TOPDOWN);
								}
							}
							// If the enemy can advance to the left
							else {
								posEnemy.x -=velocity ;
								if (sprite->animation() != MOVE_LEFT_TOPDOWN) {
									sprite->changeAnimation(MOVE_LEFT_TOPDOWN);
								}
							}
						}
					}
					// If the enemy can advance to the top
					else {
						posEnemy.y-= velocity;
						if (sprite->animation() != MOVE_UP_TOPDOWN) {
							sprite->changeAnimation(MOVE_UP_TOPDOWN);
						}
					}
				}
			}
		}
		
	}
	else {
		// SIDE VIEW
		if (cameraType == CameraType::SIDE_VIEW) {
			if (distToPlayer < SCREEN_WIDTH / 2) {
				if (playerPosition.x > posEnemy.x) {
					sprite->changeAnimation(STAND_RIGHT);
				}
				else {
					sprite->changeAnimation(STAND_LEFT);
				}
			}
			else {
				sprite->changeAnimation(STAND_STILL);
			}
		}

		// TOPDOWN VIEW
		else {
			moving = false;
			if (sprite->animation() == MOVE_UP_TOPDOWN) {
				sprite->changeAnimation(STAND_UP_TOPDOWN);
			}
			if (sprite->animation() == MOVE_RIGHT_TOPDOWN) {
				sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
			}
			if (sprite->animation() == MOVE_DOWN_TOPDOWN) {
				sprite->changeAnimation(STAND_DOWN_TOPDOWN);
			}
			if (sprite->animation() == MOVE_LEFT_TOPDOWN) {
				sprite->changeAnimation(STAND_LEFT_TOPDOWN);
			}
		}
	}
}




