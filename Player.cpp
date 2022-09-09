#include <cmath>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "include/irrKlang.h"
#include <conio.h>
#include <Windows.h>

using namespace std;
using namespace irrklang;


#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


#define SCREEN_X 32
#define SCREEN_Y 16

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define FIRE_RATE 0.15 //segundos entre disparos

ShaderProgram Player::getShaderProgram() {
	return shaderP;
}

enum BulletAnims
{
	SPAWN, MOVE, COLLIDE
};

void Player::sideViewPlayer(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool spreadGunActivated)
{
    engine = createIrrKlangDevice();
	jumpSpeed = 96;
	lastTimeDamage = -3000;
	timeLastShot = 0;
	spreadGun = spreadGunActivated;
	shaderP = shaderProgram;
	jumping = false;
	cameraType = CameraType::SIDE_VIEW;
	dead = false;
	health = 10;

	bulletsSpritesheet.loadFromFile("images/bullets.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bulletSprite = Sprite::createSprite(glm::ivec2(12, 12), glm::vec2(0.0288, 0.0284), &bulletsSpritesheet, &shaderProgram);
	bulletSprite->setNumberAnimations(1);
	bulletSprite->setAnimationSpeed(SPAWN, 8);
	bulletSprite->addKeyframe(SPAWN, glm::vec2((0.8245, 0.288)));
	bulletSprite->changeAnimation(0);


	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(30, 44), glm::vec2(0.1667, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(13);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.1667, 0.0));

		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0, 0.0));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		//sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0, 0.1667));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6667, 0.75));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8333, 0.75));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		//sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.0));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0, 0.75));		
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1667, 0.75));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3333, 0.75));
		

		sprite->setAnimationSpeed(JUMP, 16);
		sprite->addKeyframe(JUMP, glm::vec2(0.3333, 0.25));
		sprite->addKeyframe(JUMP, glm::vec2(0.5, 0.25));
		sprite->addKeyframe(JUMP, glm::vec2(0.6667, 0.25));
		sprite->addKeyframe(JUMP, glm::vec2(0.8333, 0.25));

		sprite->setAnimationSpeed(LOOK_UPRIGHT, 8);
		sprite->addKeyframe(LOOK_UPRIGHT, glm::vec2(0.3333, 0.0));
		
		sprite->setAnimationSpeed(LOOK_UPLEFT, 8);
		sprite->addKeyframe(LOOK_UPLEFT, glm::vec2(0.5, 0.0));

		sprite->setAnimationSpeed(DIAGONAL_LEFT, 8);
		sprite->addKeyframe(DIAGONAL_LEFT, glm::vec2(0.8333, 0.0));
		sprite->addKeyframe(DIAGONAL_LEFT, glm::vec2(0.8333, 0.5));
		sprite->addKeyframe(DIAGONAL_LEFT, glm::vec2(0.6667, 0.5));
		sprite->addKeyframe(DIAGONAL_LEFT, glm::vec2(0.5, 0.5));

		sprite->setAnimationSpeed(DIAGONAL_RIGHT, 8);
		sprite->addKeyframe(DIAGONAL_RIGHT, glm::vec2(0.6667, 0.0));
		sprite->addKeyframe(DIAGONAL_RIGHT, glm::vec2(0.3333, 0.5));
		sprite->addKeyframe(DIAGONAL_RIGHT, glm::vec2(0.1667, 0.5));
		sprite->addKeyframe(DIAGONAL_RIGHT, glm::vec2(0.0, 0.5));

		sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
		sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.0, 0.25));

		sprite->setAnimationSpeed(CROUCH_LEFT, 8);
		sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.1667, 0.25));

		sprite->setAnimationSpeed(STAND_DIAGONAL_LEFT, 8);
		sprite->addKeyframe(STAND_DIAGONAL_LEFT, glm::vec2(0.8333, 0.0));

		sprite->setAnimationSpeed(STAND_DIAGONAL_RIGHT, 8);
		sprite->addKeyframe(STAND_DIAGONAL_RIGHT, glm::vec2(0.6667, 0.0));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));	
}


void Player::inflictDamage() {
	if (time >= lastTimeDamage + 3000) {
		health--;
		lastTimeDamage = time;

		if (health <= 0) {
			die();
		}
		else engine->play2D("./Sounds/hitPlayer.wav");
	}

}

void Player::die() {
	dead = true;
}
void Player::topDownViewPlayer(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool spreadGunActivated)
{

	engine = createIrrKlangDevice();

	timeLastShot = 0;
	spreadGun = spreadGunActivated;
	shaderP = shaderProgram;
	jumping = false;
	cameraType = CameraType::TOPDOWN_VIEW;
	dead = false;
	health = 10;

	bulletsSpritesheet.loadFromFile("images/bullets.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bulletSprite = Sprite::createSprite(glm::ivec2(12, 12), glm::vec2(0.0288, 0.0284), &bulletsSpritesheet, &shaderProgram);
	bulletSprite->setNumberAnimations(1);
	bulletSprite->setAnimationSpeed(SPAWN, 8);
	bulletSprite->addKeyframe(SPAWN, glm::vec2((0.8245, 0.288)));
	bulletSprite->changeAnimation(0);

	topDownSpritesheet.loadFromFile("images/topdown_player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(23, 34), glm::vec2(0.05, 1.00), &topDownSpritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(STAND_UP_TOPDOWN, 8);
	sprite->addKeyframe(STAND_UP_TOPDOWN, glm::vec2(0.80, 0.0));

	sprite->setAnimationSpeed(STAND_RIGHT_TOPDOWN, 8);
	sprite->addKeyframe(STAND_RIGHT_TOPDOWN, glm::vec2(0.85, 0.0));

	sprite->setAnimationSpeed(STAND_DOWN_TOPDOWN, 8);
	sprite->addKeyframe(STAND_DOWN_TOPDOWN, glm::vec2(0.90, 0.0));

	sprite->setAnimationSpeed(STAND_LEFT_TOPDOWN, 8);
	sprite->addKeyframe(STAND_LEFT_TOPDOWN, glm::vec2(0.95, 0.0));

	sprite->setAnimationSpeed(MOVE_UP_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.80, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.00, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.20, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.80, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.40, 0.0));
	sprite->addKeyframe(MOVE_UP_TOPDOWN, glm::vec2(0.60, 0.0));

	sprite->setAnimationSpeed(MOVE_RIGHT_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.85, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.05, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.25, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.85, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.45, 0.0));
	sprite->addKeyframe(MOVE_RIGHT_TOPDOWN, glm::vec2(0.65, 0.0));

	sprite->setAnimationSpeed(MOVE_DOWN_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.90, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.10, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.30, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.90, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.50, 0.0));
	sprite->addKeyframe(MOVE_DOWN_TOPDOWN, glm::vec2(0.70, 0.0));

	sprite->setAnimationSpeed(MOVE_LEFT_TOPDOWN, 8);
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.95, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.15, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.35, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.95, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.55, 0.0));
	sprite->addKeyframe(MOVE_LEFT_TOPDOWN, glm::vec2(0.75, 0.0));

	sprite->changeAnimation(STAND_UP_TOPDOWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

vector<Bullet*> Player::update(int deltaTime, bool timeScale)
{
	if (!dead) {


		time += deltaTime;
		sprite->update(deltaTime);
		

		shootingManagement(deltaTime, timeScale);

		movementManagement(timeScale);

		jumpManagement(timeScale);

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

		return bullets;
	}
}


bool Player::render()
{
	if (!dead) {

		if (time <= lastTimeDamage + 3000) {

			if((int)time %2 == 0 || (int)time % 3 == 0)
				sprite->render();
		}
		else sprite->render();

		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->render();
		}
		return false;
	}
	
	else
		return true;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::shootingManagement(float deltaTime, bool timeScale)
{
	vector<Bullet*> bulletsAux;
	for (int i = 0; i < bullets.size(); i++) {
		bool destroyed = bullets[i]->update(deltaTime, timeScale, true);
		if (!destroyed)
			bulletsAux.push_back(bullets[i]);
	}
	bullets = bulletsAux;

	if ((Game::instance().checkIfKeyPressed('M') || Game::instance().checkIfKeyPressed('Z')) && (time >= (timeLastShot + (FIRE_RATE * 1000)))) {
		glm::vec2 dir;
		glm::vec2* offset = new glm::vec2(0, 0);
		timeLastShot = time;
		vector<glm::vec2> directions;


		// Shooting in SIDE VIEW
		if (cameraType == CameraType::SIDE_VIEW) {

			// Shooting in SIDE VIEW with the SPREAD GUN ACTIVATED
			if (spreadGun) {
				engine->play2D("./Sounds/shot2.wav");

				directions.resize(5);

				if (sprite->animation() == LOOK_UPLEFT || sprite->animation() == LOOK_UPRIGHT) {
					directions[0] = glm::vec2(-2, -2);
					directions[1] = glm::vec2(-1, -3);
					directions[2] = glm::vec2(0, -3);
					directions[3] = glm::vec2(1, -3);
					directions[4] = glm::vec2(2, -2);
				}
				else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_LEFT) {
					directions[0] = glm::vec2(-2, -2);
					directions[1] = glm::vec2(-3, -1);
					directions[2] = glm::vec2(-3, 0);
					directions[3] = glm::vec2(-3, 1);
					directions[4] = glm::vec2(-2, 2);
				}
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_RIGHT) {
					directions[0] = glm::vec2(2, -2);
					directions[1] = glm::vec2(3, -1);
					directions[2] = glm::vec2(3, 0);
					directions[3] = glm::vec2(3, 1);
					directions[4] = glm::vec2(2, 2);
				}
				else if (sprite->animation() == DIAGONAL_LEFT || sprite->animation() == STAND_DIAGONAL_LEFT) {
					directions[0] = glm::vec2(0, -3);
					directions[1] = glm::vec2(-1, -3);
					directions[2] = glm::vec2(-2, -2);
					directions[3] = glm::vec2(-3, -1);
					directions[4] = glm::vec2(-3, 0);
				}
				else if (sprite->animation() == DIAGONAL_RIGHT || sprite->animation() == STAND_DIAGONAL_RIGHT) {
					directions[0] = glm::vec2(0, -3);
					directions[1] = glm::vec2(1, -3);
					directions[2] = glm::vec2(2, -2);
					directions[3] = glm::vec2(3, -1);
					directions[4] = glm::vec2(3, 0);
				}
				else {
					if (lookingRight) {
						directions[0] = glm::vec2(2, -2);
						directions[1] = glm::vec2(3, -1);
						directions[2] = glm::vec2(3, 0);
						directions[3] = glm::vec2(3, 1);
						directions[4] = glm::vec2(2, 2);
					}
					else{
						directions[0] = glm::vec2(-2, -2);
						directions[1] = glm::vec2(-3, -1);
						directions[2] = glm::vec2(-3, 0);
						directions[3] = glm::vec2(-3, 1);
						directions[4] = glm::vec2(-2, 2);
					}
				}
			}

			// Shooting in SIDE VIEW with the SPREAD GUN NOT ACTIVATED
			else {
				engine->play2D("./Sounds/shot.wav");

				directions.resize(1);

				if (sprite->animation() == LOOK_UPRIGHT || sprite->animation() == LOOK_UPLEFT)
					directions[0].x = 0;
				else if (lookingRight)
					directions[0].x = 3;
				else directions[0].x = -3;

				if (sprite->animation() == DIAGONAL_LEFT || sprite->animation() == DIAGONAL_RIGHT || sprite->animation() == LOOK_UPRIGHT || sprite->animation() == LOOK_UPLEFT
					|| sprite->animation() == STAND_DIAGONAL_LEFT || sprite->animation() == STAND_DIAGONAL_RIGHT) {
					directions[0].y = -3;
				}
			}

			if (sprite->animation() == STAND_RIGHT)
				offset = new glm::vec2(30, 11);

			else if (sprite->animation() == STAND_LEFT)
				offset = new glm::vec2(-5, 13);

			else if (sprite->animation() == MOVE_RIGHT)
				offset = new glm::vec2(30, 13);

			else if (sprite->animation() == MOVE_LEFT)
				offset = new glm::vec2(-10, 13);

			else if (sprite->animation() == LOOK_UPLEFT)
				offset = new glm::vec2(13, -26);

			else if (sprite->animation() == LOOK_UPRIGHT)
				offset = new glm::vec2(13, -26);

			else if (sprite->animation() == DIAGONAL_RIGHT)
				offset = new glm::vec2(28, -8);

			else if (sprite->animation() == DIAGONAL_LEFT)
				offset = new glm::vec2(0, -11);

			else if (sprite->animation() == CROUCH_LEFT)
				offset = new glm::vec2(-7, 29);

			else if (sprite->animation() == CROUCH_RIGHT)
				offset = new glm::vec2(33, 29);
			
			else if (sprite->animation() == STAND_DIAGONAL_LEFT)
				offset = new glm::vec2(0, -11);
			
			else if (sprite->animation() == STAND_DIAGONAL_RIGHT)
				offset = new glm::vec2(28, -8);

			else if (sprite->animation() == JUMP)
				offset = new glm::vec2(28, 24);
		}

		// Shooting in TOP-DOWM VIEW
		else {
			
			// Shooting in TOP-DOWN VIEW with the SPREAD GUN ACTIVATED
			if (spreadGun) {
				engine->play2D("./Sounds/shot2.wav");

				directions.resize(5);

				if (sprite->animation() == STAND_UP_TOPDOWN || sprite->animation() == MOVE_UP_TOPDOWN) {
					directions[0] = glm::vec2(-2, -2);
					directions[1] = glm::vec2(-1, -3);
					directions[2] = glm::vec2(0, -3);
					directions[3] = glm::vec2(1, -3);
					directions[4] = glm::vec2(2, -2);
					offset = new glm::vec2(15, -10);
				}
				else if (sprite->animation() == STAND_RIGHT ||  sprite->animation() == MOVE_RIGHT_TOPDOWN) {
					directions[0] = glm::vec2(2, -2);
					directions[1] = glm::vec2(3, -1);
					directions[2] = glm::vec2(3, 0);
					directions[3] = glm::vec2(3, 1);
					directions[4] = glm::vec2(2, 2);
					offset = new glm::vec2(20, 14);
				}
				if (sprite->animation() == STAND_DOWN_TOPDOWN || sprite->animation() == MOVE_DOWN_TOPDOWN) {
					directions[0] = glm::vec2(-2, 2);
					directions[1] = glm::vec2(-1, 3);
					directions[2] = glm::vec2(0, 3);
					directions[3] = glm::vec2(1, 3);
					directions[4] = glm::vec2(2, 2);
					offset = new glm::vec2(5, 25);
				}
				else if (sprite->animation() == STAND_LEFT_TOPDOWN || sprite->animation() == MOVE_LEFT || sprite->animation() == MOVE_LEFT_TOPDOWN) {
					directions[0] = glm::vec2(-2, -2);
					directions[1] = glm::vec2(-3, -1);
					directions[2] = glm::vec2(-3, 0);
					directions[3] = glm::vec2(-3, 1);
					directions[4] = glm::vec2(-2, 2);
					offset = new glm::vec2(0, 0);
				}
			}

			// Shooting in TOP-DOWN VIEW with the SPREAD GUN NOT ACTIVATED
			else {
				engine->play2D("./Sounds/shot.wav");

				directions.resize(1);

				if (sprite->animation() == STAND_UP_TOPDOWN || sprite->animation() == MOVE_UP_TOPDOWN) {
					directions[0] = glm::vec2(0, -3);
					offset = new glm::vec2(15, -10);
				}
				else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT_TOPDOWN) {
					directions[0] = glm::vec2(3, 0);
					offset = new glm::vec2(20, 14);
				}
				if (sprite->animation() == STAND_DOWN_TOPDOWN || sprite->animation() == MOVE_DOWN_TOPDOWN) {
					directions[0] = glm::vec2(0, 3);
					offset = new glm::vec2(5, 25);
				}
				else if (sprite->animation() == STAND_LEFT_TOPDOWN || sprite->animation() == MOVE_LEFT || sprite->animation() == MOVE_LEFT_TOPDOWN) {
					directions[0] = glm::vec2(-3, 0);
					offset = new glm::vec2(0, 0);
				}
			}
		}

		for (int i = 0; i < directions.size(); ++i) {

			Bullet* bullet = new Bullet();
			bullet->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderP, bulletSprite);
			bullet->setPosition(glm::vec2(posPlayer.x + offset->x, posPlayer.y + offset->y), directions[i]);
			bullet->setTileMap(map);
			bullets.push_back(bullet);
		}
	}
}

int Player::getAnimation() {
	
	return sprite->animation();
}

void Player::movementManagement(bool timeScale) {
	// Determine the combination of keys pressed
	KeyMovementCombo keyMovementCombo = KeyMovementCombo::NOTHING;

	if ((Game::instance().checkIfKeyPressed('A') || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) && (Game::instance().checkIfKeyPressed('W') || Game::instance().getSpecialKey(GLUT_KEY_UP))) {
		keyMovementCombo = KeyMovementCombo::LEFT_UP;
	}
	else if ((Game::instance().checkIfKeyPressed('A') || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) && (Game::instance().checkIfKeyPressed('S') || Game::instance().getSpecialKey(GLUT_KEY_DOWN))) {
		keyMovementCombo = KeyMovementCombo::LEFT_DOWN;
	}
	else if (Game::instance().checkIfKeyPressed('A') || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		keyMovementCombo = KeyMovementCombo::LEFT;
	}
	else if ((Game::instance().checkIfKeyPressed('D') || Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) && (Game::instance().checkIfKeyPressed('W') || Game::instance().getSpecialKey(GLUT_KEY_UP))) {
		keyMovementCombo = KeyMovementCombo::RIGHT_UP;
	}
	else if ((Game::instance().checkIfKeyPressed('D') || Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) && (Game::instance().checkIfKeyPressed('S') || Game::instance().getSpecialKey(GLUT_KEY_DOWN))) {
		keyMovementCombo = KeyMovementCombo::RIGHT_DOWN;
	}
	else if (Game::instance().checkIfKeyPressed('D') || Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		keyMovementCombo = KeyMovementCombo::RIGHT;
	}
	else if (Game::instance().checkIfKeyPressed('W') || Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		keyMovementCombo = KeyMovementCombo::UP;
	}
	else if (Game::instance().checkIfKeyPressed('S') || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		keyMovementCombo = KeyMovementCombo::DOWN;
	}
	else if (Game::instance().checkIfKeyPressed('Q')) {
		keyMovementCombo = KeyMovementCombo::LEFT_DIAGONAL;
	}
	else if (Game::instance().checkIfKeyPressed('E')) {
		keyMovementCombo = KeyMovementCombo::RIGHT_DIAGONAL;
	}

	int speed = timeScale ? 1 : 2;

	// Action depending of the combination of keys pressed in side view
	if (cameraType == CameraType::SIDE_VIEW) {		
		if (keyMovementCombo == KeyMovementCombo::LEFT) {
			lookingRight = false;
			posPlayer.x -= speed;
			if (sprite->animation() != MOVE_LEFT && !jumping && !falling) {
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::LEFT_UP) {
			lookingRight = false;
			posPlayer.x -= speed;
			if (sprite->animation() != DIAGONAL_LEFT && !jumping && !falling) {
				sprite->changeAnimation(DIAGONAL_LEFT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::LEFT_DOWN) {
			lookingRight = false;
			posPlayer.x -= speed;
			if (sprite->animation() != MOVE_LEFT && !jumping && !falling) {
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
		if (keyMovementCombo == KeyMovementCombo::RIGHT) {
			lookingRight = true;
			posPlayer.x += speed;
			if (sprite->animation() != MOVE_RIGHT && !jumping && !falling) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT_UP) {
			lookingRight = true;
			posPlayer.x += speed;
			if (sprite->animation() != DIAGONAL_RIGHT && !jumping && !falling) {
				sprite->changeAnimation(DIAGONAL_RIGHT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT_DOWN) {
			lookingRight = true;
			posPlayer.x += speed;
			if (sprite->animation() != MOVE_RIGHT && !jumping && !falling) {
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::UP) {
			if (lookingRight && sprite->animation() != LOOK_UPRIGHT && !jumping && !falling) {
				sprite->changeAnimation(LOOK_UPRIGHT);
			}
			else if (!lookingRight && sprite->animation() != LOOK_UPLEFT && !jumping && !falling) {
				sprite->changeAnimation(LOOK_UPLEFT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::DOWN) {
			if (lookingRight && sprite->animation() != CROUCH_RIGHT && !jumping && !falling) {
				sprite->changeAnimation(CROUCH_RIGHT);
			}
			else if (!lookingRight && sprite->animation() != CROUCH_LEFT && !jumping && !falling) {
				sprite->changeAnimation(CROUCH_LEFT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::LEFT_DIAGONAL) {
			if (sprite->animation() != DIAGONAL_LEFT && !jumping && !falling) {
				lookingRight = false;
				sprite->changeAnimation(STAND_DIAGONAL_LEFT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT_DIAGONAL) {
			if (sprite->animation() != DIAGONAL_RIGHT && !jumping && !falling) {
				lookingRight = true;
				sprite->changeAnimation(STAND_DIAGONAL_RIGHT);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::NOTHING && !jumping && !falling){
			if (lookingRight) {
				sprite->changeAnimation(STAND_RIGHT);
			}
			else {
				sprite->changeAnimation(STAND_LEFT);
			}
		}

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(30, 44))) {
			posPlayer.x += speed;
			sprite->changeAnimation(STAND_LEFT);
		}

		if (map->collisionMoveRight(posPlayer, glm::ivec2(30, 44))) {
			posPlayer.x -= speed;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}

	// Action depending of the combination of keys pressed in top-down view
	else {
		if (keyMovementCombo == KeyMovementCombo::LEFT) {
			posPlayer.x -= speed;
			if (sprite->animation() != MOVE_LEFT_TOPDOWN) {
				sprite->changeAnimation(MOVE_LEFT_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::LEFT_UP) {
			posPlayer.y -= speed;
			if (sprite->animation() != MOVE_UP_TOPDOWN) {
				sprite->changeAnimation(MOVE_UP_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::LEFT_DOWN) {
			posPlayer.y += speed;
			if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
				sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT) {
			posPlayer.x += speed;
			if (sprite->animation() != MOVE_RIGHT_TOPDOWN) {
				sprite->changeAnimation(MOVE_RIGHT_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT_UP) {
			posPlayer.y -= speed;
			if (sprite->animation() != MOVE_UP_TOPDOWN) {
				sprite->changeAnimation(MOVE_UP_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::RIGHT_DOWN) {
			posPlayer.y += speed;
			if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
				sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::UP) {
			posPlayer.y -= speed;
			if (sprite->animation() != MOVE_UP_TOPDOWN) {
				sprite->changeAnimation(MOVE_UP_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::DOWN) {
			posPlayer.y += speed;
			if (sprite->animation() != MOVE_DOWN_TOPDOWN) {
				sprite->changeAnimation(MOVE_DOWN_TOPDOWN);
			}
		}
		else if (keyMovementCombo == KeyMovementCombo::NOTHING) {
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

		if (map->collisionMoveUp(posPlayer, glm::ivec2(23, 34))) {
			posPlayer.y += speed;
			sprite->changeAnimation(STAND_UP_TOPDOWN);
		}

		if (map->collisionMoveDown(posPlayer, glm::ivec2(23, 34))) {
			posPlayer.y -= speed;
			sprite->changeAnimation(STAND_DOWN_TOPDOWN);
		}

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(23, 34))) {
			posPlayer.x += speed;
			sprite->changeAnimation(STAND_LEFT_TOPDOWN);
		}

		if (map->collisionMoveRight(posPlayer, glm::ivec2(23, 34))) {
			posPlayer.x -= speed;
			sprite->changeAnimation(STAND_RIGHT_TOPDOWN);
		}
	}
}

void Player::jumpManagement(bool timeScale) {
	if (cameraType == CameraType::SIDE_VIEW) {
		
		if (jumping) {
			if (sprite->animation() != JUMP)
				sprite->changeAnimation(JUMP);

			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180) {
				jumping = false;
				posPlayer.y = startY;
			}
			else {
				
				posPlayer.y = int(startY  -96* sin(3.14159f  * jumpAngle / 180.f));
				if (jumpAngle > 90) {
					jumping = !map->collisionOnFall(posPlayer, glm::ivec2(30, 44), &posPlayer.y);

				}
			}
		}
		else {
			posPlayer.y += FALL_STEP;
			if (map->collisionOnFall(posPlayer, glm::ivec2(30, 44), &posPlayer.y))
			{
				falling = false;

				if (Game::instance().checkIfKeyPressed(' '))
				{
					engine->play2D("./Sounds/jump.wav");
					jumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}

			else {
				falling = true;
				if (sprite->animation() != JUMP) {
					sprite->changeAnimation(JUMP);
				}
			}
		}
	}
}

void Player::activateSpreadGun() {
	spreadGun = true;
}