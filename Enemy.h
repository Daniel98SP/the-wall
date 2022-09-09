#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Bullet.h"
#include "Enums.h"
#include "include/irrKlang.h"
using namespace irrklang;

class Enemy
{

	enum SideEnemyAnims
	{
		STAND_STILL, STAND_LEFT, MOVE_LEFT, STAND_RIGHT, MOVE_RIGHT, DEAD
	};

	enum TopDownEnemyAnims
	{
		STAND_UP_TOPDOWN, STAND_RIGHT_TOPDOWN, STAND_DOWN_TOPDOWN, STAND_LEFT_TOPDOWN, MOVE_UP_TOPDOWN, MOVE_RIGHT_TOPDOWN, MOVE_DOWN_TOPDOWN, MOVE_LEFT_TOPDOWN, DEAD_TOPDOWN
	};

public:
	void sideViewEnemy(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void topDownViewEnemy(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	vector<Bullet*> update(int deltaTime, const glm::vec2& playerPosition, bool timeScale);
	bool render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	ShaderProgram getShaderProgram();
	void inflictDamage();
	bool isDead();
	
private:
	float distanceToPlayer(const glm::vec2& playerPosition);
	glm::vec2 directionToPlayer(const glm::vec2& playerPosition);
	void shootingManagement(float deltaTime, const glm::vec2& playerPosition, float distToPlayer, bool timeScale);
	void movementManagement(const glm::vec2& playerPosition, float distToPlayer, bool timeScale);
	void die();
	int frames;

	ISoundEngine *engine;
	bool jumping;
	bool falling;
	bool moving;

	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Texture topdownSpritesheet;
	Texture bulletSpritesheet;
	Sprite *sprite;
	TileMap *map;
	ShaderProgram shaderP;
	
	vector<Bullet*> bullets;
	Sprite *bulletSprite;
	CameraType cameraType;
	int health;
	float time;
	float timeLastShot;
	bool dead;
	float timeDeath;
};


#endif // _Enemy_INCLUDE


