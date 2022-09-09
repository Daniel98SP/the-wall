#ifndef _BossView_INCLUDE
#define _BossView_INCLUDE

class BossView
{

public:
	void init(const glm::vec2& initialPosition, const glm::ivec2& mapSize, int levelTileSize);
	void update(int deltaTime, const glm::vec2& playerPosition);
	float getLeft();
	float getRight();
	float getBottom();
	float getTop();

private:
	void calculateCamera(const glm::vec2& playerPosition);

	float left;
	float right;
	float bottom;
	float top;
	int mapSizeX;
	int mapSizeY;
	int tileSize;
};

#endif // _BossView_INCLUDE