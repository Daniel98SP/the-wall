#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "TopDownView.h"
#include "Game.h"

#define PLAYER_POSITION_OFFSET 40
#define CAMERA_LIMIT_OFFSET 70

using namespace std;

void TopDownView::init(const glm::vec2& initialPosition, const glm::ivec2& mapSize, int levelTileSize)
{
	tileSize = levelTileSize;
	mapSizeX = mapSize[0] * tileSize;
	mapSizeY = mapSize[1] * tileSize;
	angle = 0.0;
	calculateCamera(initialPosition);
}

void TopDownView::update(int deltaTime, const glm::vec2& playerPosition)
{
	calculateCamera(playerPosition);
}

void TopDownView::calculateCamera(const glm::vec2& playerPosition)
{
	left = float(playerPosition[0]) - float(SCREEN_WIDTH)/2 + PLAYER_POSITION_OFFSET;
	right = left + float(SCREEN_WIDTH);
	bottom = float(playerPosition[1]) + float(SCREEN_HEIGHT)/2;
	top = bottom - float(SCREEN_HEIGHT);

	if (Game::instance().checkIfKeyPressed('Q')) {
		angle -= 5.0;
	}
	if (Game::instance().checkIfKeyPressed('E')) {
		angle += 5.0;
	}
}

float TopDownView::getLeft() {
	return left;
}

float TopDownView::getRight() {
	return right;
}

float TopDownView::getBottom() {
	return bottom;
}

float TopDownView::getTop() {
	return top;
}

float TopDownView::getAngle() {
	return angle;
}