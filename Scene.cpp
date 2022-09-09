#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "include/irrKlang.h"
#include <conio.h>
#include <Windows.h>

#define SCREEN_X 32
#define SCREEN_Y 16
#define NUMBER_OF_LEVELS 5
#define SLOWMO_DURATION 5000

using namespace irrklang;


#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
		for (int i = 0; i < extraMap.size(); ++i) {
			delete extraMap[i];
		}
		extraMap.resize(0);
	if(player != NULL)
		delete player;
}

enum title
{
	SPAWN, SPAWN2

};


void Scene::init(int levelNumber)
{
	currentLevel = levelNumber;
	timeScale = false;
	timeSlowMo = -10000;
	enemies.clear();
	delete map;
	for (int i = 0; i < extraMap.size(); ++i) {
		delete extraMap[i];
	}
	extraMap.resize(0);

	initShaders();
	if(engine)engine->stopAllSounds();
	engine = createIrrKlangDevice();

	/*
	irrklang::ISoundSource* song = engine->addSoundSourceFromFile("./Sounds/boss.wav");

	song->setDefaultVolume(0.25f);*/
	


	// Scene info management
	ifstream sceneInfoInput;
	stringstream infoStream;
	string infoString;

	if (currentLevel > 0 && currentLevel < 4) {
	  map = TileMap::createTileMap("levels/level0" + to_string(currentLevel) + "/map.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

		sceneInfoInput.open("levels/level0" + to_string(currentLevel) + "/info.txt");

		getline(sceneInfoInput, infoString);
	infoStream.str(infoString);
	infoStream >> infoString;
	if (infoString == "CAMERA_TYPE") {
		getline(sceneInfoInput, infoString);
		infoStream.clear();
		infoStream.str(infoString);
		string camera;
		infoStream >> camera;
		if (camera == "side") {
			cameraType = CameraType::SIDE_VIEW;
		}
		else if (camera == "topdown") {
			cameraType = CameraType::TOPDOWN_VIEW;
		}
		else {
			cameraType = CameraType::BOSS_VIEW;
		}
	}

	getline(sceneInfoInput, infoString);
	infoStream.str(infoString);
	infoStream >> infoString;
	if (infoString == "PLAYER_POSITION") {
		getline(sceneInfoInput, infoString);
		infoStream.clear();
		infoStream.str(infoString);
		glm::ivec2 playerPosition;
		infoStream >> playerPosition.x >> playerPosition.y;
		player = new Player();
		if (cameraType == CameraType::SIDE_VIEW || cameraType == CameraType::BOSS_VIEW) {
			player->sideViewPlayer(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spreadGunActivated);
		}
		else {
			player->topDownViewPlayer(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, spreadGunActivated);
		}
		player->setPosition(glm::vec2(playerPosition.x * map->getTileSize(), playerPosition.y * map->getTileSize()));
		player->setTileMap(map);
	}

	getline(sceneInfoInput, infoString);
	infoStream.str(infoString);
	infoStream >> infoString;
	if (infoString == "ENEMIES_POSITIONS") {
		getline(sceneInfoInput, infoString);
		while (infoString != "LEVEL_END_TILE") {
			infoStream.clear();
			infoStream.str(infoString);
			glm::ivec2 enemyPosition;
			infoStream >> enemyPosition.x >> enemyPosition.y;
			Enemy *enemy = new Enemy();
			if (cameraType == CameraType::SIDE_VIEW) {
				enemy->sideViewEnemy(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			}
			else {
				enemy->topDownViewEnemy(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			}			
			enemy->setPosition(glm::vec2(enemyPosition.x * map->getTileSize(), enemyPosition.y * map->getTileSize()));
			enemy->setTileMap(map);
			enemies.push_back(enemy);
			getline(sceneInfoInput, infoString);
			infoStream.str(infoString);
		}
	}

	if (infoString == "LEVEL_END_TILE") {
		getline(sceneInfoInput, infoString);
		infoStream.clear();
		infoStream.str(infoString);
		infoStream >> levelEndTile.x >> levelEndTile.y;
	}
	if (cameraType == CameraType::SIDE_VIEW) {
		getline(sceneInfoInput, infoString);
		if (infoString == "SPREAD_GUN") {
			getline(sceneInfoInput, infoString);
			infoStream.clear();
			infoStream.str(infoString);
			infoStream >> spreadGunTile.x >> spreadGunTile.y;
		}
	}

	getline(sceneInfoInput, infoString);
	if (infoString == "EXTRA_MAPS") {
		int numberOfExtraMaps;
		getline(sceneInfoInput, infoString);
		infoStream.clear();
		infoStream.str(infoString);
		infoStream >> numberOfExtraMaps;

		for (int i = 0; i < numberOfExtraMaps; ++i) {
			extraMap.push_back(TileMap::createTileMap("levels/level0" + to_string(currentLevel) + "/extraMap" + to_string(i) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram));
		}
	}


	sceneInfoInput.close();

		if (cameraType == CameraType::BOSS_VIEW) {
			level3musicSound = engine->play2D("Sounds/boss.wav", false, false, true);
			level3musicSound->setVolume(0.25f);


			boss = new Boss();
			boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			boss->setPosition(glm::vec2(-200, -200));
			boss->setTileMap(map);
		}

		glm::ivec2 playerPosition = player->getPosition();
		glm::ivec2 mapSize = map->getMapSizeInTiles();
		int tileSize = map->getTileSize();

		if (cameraType == CameraType::SIDE_VIEW) {
			level1musicSound = engine->play2D("Sounds/level1music.mp3", false, false, true);
			sideView = new SideView();
			sideView->init(glm::vec2(playerPosition[0], playerPosition[1]), mapSize, tileSize);
			projection = glm::ortho(sideView->getLeft(), sideView->getRight(), sideView->getBottom(), sideView->getTop());
		}
		else if (cameraType == CameraType::TOPDOWN_VIEW) {
			level2musicSound = engine->play2D("./Sounds/level2music.mp3");
			topDownView = new TopDownView();
			topDownView->init(glm::vec2(playerPosition[0], playerPosition[1]), mapSize, tileSize);
			projection = glm::ortho(topDownView->getLeft(), topDownView->getRight(), topDownView->getBottom(), topDownView->getTop());
		}
		else {
			bossView = new BossView();
			bossView->init(glm::vec2(playerPosition[0], playerPosition[1]), mapSize, tileSize);
			projection = glm::ortho(bossView->getLeft(), bossView->getRight(), bossView->getBottom(), bossView->getTop());
		}


		currentTime = 0.0f;
	}
	else if(currentLevel == -1) {

	spritesheet.loadFromFile("images/the_wall.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(1904, 990), glm::vec2(1, 1), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(SPAWN, 8);
	sprite->addKeyframe(SPAWN, glm::vec2((0, 0)));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(-50, 100));


	projection = glm::ortho(float(-50), float(1854), float(1090), float(100));

}

	else if (currentLevel == 0) {

	spritesheet.loadFromFile("images/instrucciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(1919, 1083), glm::vec2(1, 1), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(SPAWN, 8);
	sprite->addKeyframe(SPAWN, glm::vec2((0, 0)));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(-50, 100));

	
	projection = glm::ortho(0.f, float(1856), float(1392), 0.f);

}

	else if (currentLevel == 4) {

	spritesheet.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(1919, 1083), glm::vec2(1, 1), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(SPAWN, 8);
	sprite->addKeyframe(SPAWN, glm::vec2((0, 0)));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(-50, 100));


	projection = glm::ortho(0.f, float(1856), float(1392), 0.f);
	engine->play2D("Sounds/credits.mp3");

}

}void Scene::setTimeScale(float ts) {
	timeScale = ts;
}

void Scene::update(int deltaTime)
{

	time += deltaTime;
	if (currentLevel > 0 && currentLevel < 4) {

		if ((Game::instance().checkIfKeyPressed('X') || Game::instance().checkIfKeyPressed('N')) && time > timeSlowMo + SLOWMO_DURATION + 5000) {
			if (!timeScale) {
				timeScale = true;
				timeSlowMo = time;
				if(level1musicSound)
					level1musicSound->setPlaybackSpeed(0.5f);
				else if (level2musicSound)
					level2musicSound->setPlaybackSpeed(0.5f);
				else if (level3musicSound)
					level3musicSound->setPlaybackSpeed(0.5f);
			}
		}

		if (timeScale && time > timeSlowMo + SLOWMO_DURATION) {
			timeScale = false;
			if (level1musicSound)
				level1musicSound->setPlaybackSpeed(1.f);
			else if (level2musicSound)
				level2musicSound->setPlaybackSpeed(1.f);
			else if (level3musicSound)
				level3musicSound->setPlaybackSpeed(1.f);

		
		}

		vector<Bullet*> playerBullets;
		vector<Bullet*> enemyBullets;

		currentTime += deltaTime;
		playerBullets = player->update(deltaTime, timeScale);

		if (playerBullets.size() != 0) {
			for each (Bullet* bullet in playerBullets)
			{
				for each (Enemy* enemy in enemies) {

					if (bullet->getPosition().x > enemy->getPosition().x - 20 && bullet->getPosition().x <= enemy->getPosition().x + 20 && bullet->getPosition().y > enemy->getPosition().y + -30 && bullet->getPosition().y <= enemy->getPosition().y + 30 && !enemy->isDead()) {
						bullet->stop();
						enemy->inflictDamage();
					}

				}

				if (boss && !boss->isDead() && cameraType == CameraType::BOSS_VIEW && bullet->getPosition().x > boss->getPosition().x - 50 && bullet->getPosition().x <= boss->getPosition().x + 50 && bullet->getPosition().y > boss->getPosition().y + -50 && bullet->getPosition().y <= boss->getPosition().y + 50) {
					bullet->stop();
					boss->inflictDamage();
				}
			}
		}

		if (cameraType == CameraType::BOSS_VIEW && !boss->isDead() && player->getPosition().x > boss->getPosition().x - 50 && player->getPosition().x <= boss->getPosition().x + 50 && player->getPosition().y > boss->getPosition().y + -50 && player->getPosition().y <= boss->getPosition().y + 50) {

			player->inflictDamage();
		}
		glm::ivec2 playerPosition = player->getPosition();

		int tileSize = map->getTileSize();

		if (playerPosition.x >= (levelEndTile.x - 1) * tileSize && playerPosition.x < (levelEndTile.x + 1) * tileSize + tileSize
			&& playerPosition.y >= (levelEndTile.y - 1) * tileSize && playerPosition.y < (levelEndTile.y + 1) * tileSize + tileSize
			&& currentLevel != NUMBER_OF_LEVELS) {
			++currentLevel;
			init(currentLevel);
		}
		else {
			if (playerPosition.x >= (spreadGunTile.x - 1) * tileSize && playerPosition.x < (spreadGunTile.x + 1) * tileSize + tileSize
				&& playerPosition.y >= (spreadGunTile.y - 1) * tileSize && playerPosition.y < (spreadGunTile.y + 1) * tileSize + tileSize && !spreadGunActivated && cameraType == CameraType::SIDE_VIEW) {
				spreadGunActivated = true;
				player->activateSpreadGun();
				engine->play2D("./Sounds/railgun.wav");
			}
			int verticalOffset = 30;
			if (player->getAnimation() == 9 || player->getAnimation() == 10) {
				verticalOffset = 0;
			}

			for (int i = 0; i < enemies.size(); ++i) {
				vector<Bullet*> bulletsAux = enemies[i]->update(deltaTime, playerPosition, timeScale);
				for each (Bullet* bullet in bulletsAux) {

					if (bullet->getPosition().x > player->getPosition().x - 20 && bullet->getPosition().x <= player->getPosition().x + 20 && bullet->getPosition().y > player->getPosition().y + -verticalOffset && bullet->getPosition().y <= player->getPosition().y + verticalOffset) {
						bullet->stop();
						player->inflictDamage();
					}
				}
			}


			if (cameraType == CameraType::SIDE_VIEW) {
				sideView->update(deltaTime, playerPosition);
				projection = glm::ortho(sideView->getLeft(), sideView->getRight(), sideView->getBottom(), sideView->getTop());

				if (playerPosition.x >= (spreadGunTile.x - 1) * tileSize && playerPosition.x < (spreadGunTile.x + 1) * tileSize + tileSize
					&& playerPosition.y >= (spreadGunTile.y - 1) * tileSize && playerPosition.y < (spreadGunTile.y + 1) * tileSize + tileSize) {
					spreadGunActivated = true;
					player->activateSpreadGun();
				}
			}
			else if (cameraType == CameraType::TOPDOWN_VIEW) {
				topDownView->update(deltaTime, playerPosition);
				projection = glm::ortho(topDownView->getLeft(), topDownView->getRight(), topDownView->getBottom(), topDownView->getTop());
			}
			else {
				bossView->update(deltaTime, playerPosition);
				projection = glm::ortho(bossView->getLeft(), bossView->getRight(), bossView->getBottom(), bossView->getTop());
				bool bossDead = boss->update(deltaTime, glm::vec4(bossView->getLeft(), bossView->getRight(), bossView->getBottom(), bossView->getTop()), timeScale); 
					if (bossDead)
						init(4);
			}
		}
	}
		
}

int Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	if (currentLevel > 0 && currentLevel < 4) {
    map->render();
	  for (int i = extraMap.size() - 1; i >= 0; --i) {
		  extraMap[i]->render();
	  }
		if (cameraType == CameraType::BOSS_VIEW) {
			boss->render();
		}

		bool playerDead = player->render();
		if (playerDead) {
			spreadGunActivated = false;
			engine->stopAllSounds();

		}


		vector <Enemy*> enemiesAux;
		for (int i = 0; i < enemies.size(); ++i) {
			bool dead = enemies[i]->render();
			if (!dead)
				enemiesAux.push_back(enemies[i]);
		}

		enemies = enemiesAux;

		if (playerDead)
			return 1;
		else return 99;
	}

	else if(currentLevel == -1) {
		sprite->render();
		if (Game::instance().anyKeyPressed())
				return 0;
		else return 99;
	}

	else if (currentLevel == 0) {
		sprite->render();
		if (Game::instance().anyKeyPressed())
			return 1;
		else return 99;
	}
	else if (currentLevel == 4 ) {
		sprite->render();
		if (Game::instance().anyKeyPressed())
			return 99;
		else return 99;
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}