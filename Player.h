#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Enums.h"
#include "Enums.h"
#include "include/irrKlang.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

using namespace irrklang;

class Player
{	

	enum SidePlayerAnims
	{
		STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, LOOK_UPRIGHT, LOOK_UPLEFT, JUMP, DIAGONAL_RIGHT, DIAGONAL_LEFT, CROUCH_RIGHT, CROUCH_LEFT, STAND_DIAGONAL_LEFT, STAND_DIAGONAL_RIGHT
	};

	enum TopDownPlayerAnims {
		STAND_UP_TOPDOWN, STAND_RIGHT_TOPDOWN, STAND_DOWN_TOPDOWN, STAND_LEFT_TOPDOWN, MOVE_UP_TOPDOWN, MOVE_RIGHT_TOPDOWN, MOVE_DOWN_TOPDOWN, MOVE_LEFT_TOPDOWN
	};

	enum class KeyMovementCombo
	{
		NOTHING, LEFT, RIGHT, UP, DOWN, LEFT_UP, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN, LEFT_DIAGONAL, RIGHT_DIAGONAL
	};

public:
	void sideViewPlayer(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool spreadGunActivated);
	void topDownViewPlayer(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool spreadGunActivated);
	vector<Bullet*> update(int deltaTime, bool timeScale);
	bool render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	ShaderProgram getShaderProgram();
	void inflictDamage();
	void activateSpreadGun();
	int getAnimation();
	
private:
	void shootingManagement(float deltaTime, bool timeScale);
	void movementManagement(bool timeScale);
	void jumpManagement(bool timeScale);
	void die();
	int jumpSpeed;
	bool jumping;
	bool lookingRight;
	bool falling;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Texture topDownSpritesheet;
	Texture bulletsSpritesheet;
	Sprite *sprite;
	Sprite *bulletSprite;
	ISoundEngine *engine;
	TileMap *map;
	ShaderProgram shaderP;
	vector<Bullet *> bullets;
	float time;
	float timeLastShot;
	bool spreadGun;
	CameraType cameraType;
	int health;
	bool dead;
	int lastTimeDamage;
};


#endif // _PLAYER_INCLUDE


