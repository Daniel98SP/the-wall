#ifndef _Bullet_INCLUDE
#define _Bullet_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Bullet is basically a Sprite that represents the Bullet. As such it has
// all properties it needs to track its movement and collisions.


class Bullet
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Sprite *sprite);
	bool update(int deltaTime, bool timeScale, bool player);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos, const glm::vec2 &dir);
	glm::ivec2 getPosition();
	void stop();

private:
	glm::vec2 direction;
	glm::ivec2 tileMapDispl, posBullet;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool stopped;


};


#endif // _Bullet_INCLUDE


