#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "BossView.h"
#include "Game.h"

#define PLAYER_POSITION_OFFSET 40
#define CAMERA_LIMIT_OFFSET_TOP 5
#define CAMERA_LIMIT_OFFSET_BOTTOM 15 

using namespace std;

void BossView::init(const glm::vec2& initialPosition, const glm::ivec2& mapSize, int levelTileSize)
{
	tileSize = levelTileSize;
	mapSizeX = mapSize[0] * tileSize;
	mapSizeY = mapSize[1] * tileSize;

	left = 224;
	right = left + float(SCREEN_WIDTH - 1);

	calculateCamera(initialPosition);
}

void BossView::update(int deltaTime, const glm::vec2& playerPosition)
{
	calculateCamera(playerPosition);
}

void BossView::calculateCamera(const glm::vec2& playerPosition)
{
	bottom = float(playerPosition[1]) + float(SCREEN_HEIGHT) / 2;
	if (bottom > mapSizeY - 1 + CAMERA_LIMIT_OFFSET_BOTTOM) {
		bottom = mapSizeY - 1 + CAMERA_LIMIT_OFFSET_BOTTOM;
		top = bottom - float(SCREEN_HEIGHT - 1);
	}
	top = bottom - float(SCREEN_HEIGHT);
	if (top < 0.0 + CAMERA_LIMIT_OFFSET_TOP) {
		top = 0.0 + CAMERA_LIMIT_OFFSET_TOP;
		bottom = top + float(SCREEN_HEIGHT - 1);
	}
}

float BossView::getLeft() {
	return left;
}

float BossView::getRight() {
	return right;
}

float BossView::getBottom() {
	return bottom;
}

float BossView::getTop() {
	return top;
}