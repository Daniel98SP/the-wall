#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "SideView.h"
#include "Game.h"

#define PLAYER_POSITION_OFFSET 40
#define CAMERA_LIMIT_OFFSET 70

using namespace std;

void SideView::init(const glm::vec2& initialPosition, const glm::ivec2& mapSize, int levelTileSize)
{
	tileSize = levelTileSize;
	mapSizeX = mapSize[0] * tileSize;
	mapSizeY = mapSize[1] * tileSize;
	calculateCamera(initialPosition);
}

void SideView::update(int deltaTime, const glm::vec2& playerPosition)
{
	calculateCamera(playerPosition);
}

void SideView::calculateCamera(const glm::vec2& playerPosition)
{
	left = float(playerPosition[0]) - float(SCREEN_WIDTH)/2 + PLAYER_POSITION_OFFSET;
	if (left < 0) {
		left = 0;
	}
	right = left + float(SCREEN_WIDTH);
	if (right > mapSizeX - 1 + CAMERA_LIMIT_OFFSET) {
		right = mapSizeX - 1 + CAMERA_LIMIT_OFFSET;
		left = right - float(SCREEN_WIDTH);
	}
	bottom = float(playerPosition[1]) + float(SCREEN_HEIGHT)/2;
	if (bottom > mapSizeY - 1 + CAMERA_LIMIT_OFFSET) {
		bottom = mapSizeY - 1 + CAMERA_LIMIT_OFFSET;
		top = bottom - float(SCREEN_HEIGHT);
	}
	top = bottom - float(SCREEN_HEIGHT);
}

float SideView::getLeft() {
	return left;
}

float SideView::getRight() {
	return right;
}

float SideView::getBottom() {
	return bottom;
}

float SideView::getTop() {
	return top;
}