#ifndef _Boss_INCLUDE
#define _Boss_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "include/irrKlang.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

using namespace irrklang;

class Boss
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	bool update(int deltaTime, const glm::vec4& cameraPosition, bool timeScale);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	ShaderProgram getShaderProgram();
	void inflictDamage();
	void die();
	bool isDead();

	
private:
	ISound* currentSentence;
	bool sentenceSlowed;
	void movementManagement(const glm::vec4& cameraPosition, bool timeScale);
	int health;
	bool moving;
	glm::ivec2 tileMapDispl, posBoss;
	int timeDead;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	ShaderProgram shaderP;
	float time;
	float timeLastHit;
	bool inMovement;
	float movementStart;
	float movementEnd;
	bool dead;
	glm::vec2 direction;	
	ISoundEngine *engine;
	int soundCounter;

};


#endif // _Boss_INCLUDE


