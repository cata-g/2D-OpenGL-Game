#pragma once

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

class GameObject {
public:
	float x, y;
	float speed;
	int multiplierX, multiplierY;
	float size;
	GameObject() {};
	GameObject(float x, float y, float speed) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->multiplierX = 1;
		this->multiplierY = 1;
		this->size = 0;
	}

	float getX() const {
		return this->x;
	}

	float getY() const {
		return this->y;
	}

	float getSpeed() const {
		return this->speed;
	}

	void setX(float x) {
		this->x += x;
	}

	void setY(float y) {
		this->y += y;
	}

	int getMultiplierX(){
		return this->multiplierX;
	}
	int getMultiplierY(){
		return this->multiplierY;
	}
	void setMultiplierX(int mX){
		this->multiplierX = mX;
	}
	void setMultiplierY(int mY){
		this->multiplierY = mY;
	}

	float getSize() const {
		return this->size;
	}

};

class Bullet : public GameObject {
public:
	Bullet(float x, float y, float speed) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->multiplierX = 1;
		this->multiplierY = 1;
		this->size = 0.02f;
	}
};

class Point : public GameObject {
public:
	bool collected;
	Point(float x, float y, bool collected) {
		this->x = x;
		this->y = y;
		this->speed = 0;
		this->collected = collected;
		this->multiplierX = 1;
		this->multiplierY = 1;
		this->size = 0.1;
	}

	float getCollected() {
		return this->collected;
	}
};

class Opponent : public GameObject {
public:
	bool active; 
	Direction direction;
	Opponent(float x, float y, float speed) {
		this->x = x;
		this->y = y;
		this->speed = speed;
		this->active = true;
		this->direction = Direction::Up;
		this->multiplierX = 1;
		this->multiplierY = 1;
		this->size = 0.2f;
	}
};

class Player: public GameObject {

public:
	Player() {
		this->x = 0;
		this->y = 0;
		this->speed = 0.01f;
		this->multiplierX = 1;
		this->multiplierY = 1;
		this->size = 0.4f;
	}

};