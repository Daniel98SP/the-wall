#ifndef _TopDownView_INCLUDE
#define _TopDownView_INCLUDE

class TopDownView
{

public:
	void init(const glm::vec2& initialPosition, const glm::ivec2& mapSize, int levelTileSize);
	void update(int deltaTime, const glm::vec2& playerPosition);
	float getLeft();
	float getRight();
	float getBottom();
	float getTop();
	float getAngle();

private:
	void calculateCamera(const glm::vec2& playerPosition);

	float left;
	float right;
	float bottom;
	float top;
	int mapSizeX;
	int mapSizeY;
	int tileSize;
	float angle;
};

#endif // _TopDownView_INCLUDE