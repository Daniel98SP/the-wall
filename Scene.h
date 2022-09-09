#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SideView.h"
#include "TopDownView.h"
#include "BossView.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Enums.h"
#include "Enums.h"
#include "include/irrKlang.h"

using namespace irrklang;


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init(int levelNumber);
	void update(int deltaTime);
	int render();
	void setTimeScale(float timeScale);

private:
	void initShaders();

private:
	TileMap *map;
	vector<TileMap*> extraMap;
	ISound* level1musicSound;
	ISound* level2musicSound;
	ISound* level3musicSound;

	float time;
	float timeSlowMo;
	float timeScale;
	Player *player;
	vector<Enemy*> enemies;
	Boss *boss;
	SideView *sideView;
	TopDownView *topDownView;
	Texture spritesheet;
	Texture spritesheet2;
	Sprite *sprite;
	Sprite *sprite2;
	BossView *bossView;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::ivec2 levelEndTile;
	ISoundEngine *engine;
	glm::ivec2 spreadGunTile;
	int currentLevel;
	bool spreadGunActivated;
	CameraType cameraType;
};

#endif // _SCENE_INCLUDE