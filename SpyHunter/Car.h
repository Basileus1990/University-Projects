#define BLUE_CAR_PATH "./images/Blue.bmp" // player
#define GREEN_CAR_PATH "./images/Green.bmp" // civilian
#define RED_CAR_PATH "./images/Red.bmp" // enemy

#define CAR_WIDTH 35
#define CAR_HEIGHT 70
#define COLLISION_MARGIN 3

enum CarStatus {
	Player,
	Enemy,
	Civilian
};

// class resonsible for car behaviour
class Car {
	CarStatus _carStatus;
	SDL_Surface* _carPhoto;
	pos _pos;
	double _carSpeed;
	bool _isOffRoad = false;
	bool _isDead = false;

	Screen* _screen;

public:
	Car(Screen* screen, CarStatus status, double speed,int posX, int posY) {
		_screen = screen;
		_carStatus = status;
		
		_carSpeed = speed;

		_pos.x = posX;
		_pos.y = posY;
		setPhoto();
	}

	CarStatus getStatus() {
		return _carStatus;
	}

	void setStatus(CarStatus status) {
		_carStatus = status;
	}

	bool isDead() {
		return _isDead;
	}

	bool isOffRoad() {
		return _isOffRoad;
	}

	void destroy() {
		_isDead = true;
	}

	void setPosition(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}

	pos getPos() {
		return _pos;
	}

	double getSpeed() {
		return _carSpeed;
	}

	void setSpeed(double newSpeed) {
		_carSpeed = newSpeed;
	}
	
	void drawCar() {
		if (_pos.x == NULL || _pos.y == NULL) {
			return;
		}
		_screen->drawSurface(_carPhoto, _pos.x, _pos.y);
	}

	void moveCar(moveVector vector, Car* cars[], int curDistance) {
		_pos.x += vector.x;
		_pos.y += vector.y;
		colisionDetect(cars, curDistance);

		// grass collision check
		_isOffRoad = false;
		int offRoad = howMuchOffRoad(this, curDistance);
		if (offRoad != 0) {
			if (offRoad > OFF_THE_ROAD_DEATH_RADIUS) {
				_isDead = true;
			}
			_isOffRoad = true;
		}
	}

	int getVerticalDistanceToACar(Car* two) {
		return sqrt((_pos.y - two->getPos().y) * (_pos.y - two->getPos().y));
	}

private:
	// sets the photo acording to the status
	void setPhoto() {
		switch (_carStatus) {
		case Player:
			_carPhoto = _screen->getBMP(BLUE_CAR_PATH);
			break;
		case Enemy:
			_carPhoto = _screen->getBMP(RED_CAR_PATH);
			break;
		case Civilian:
			_carPhoto = _screen->getBMP(GREEN_CAR_PATH);
			break;
		}
	}

	// always checks after moving
	void colisionDetect(Car* cars[], int curDistance) {
		for (int i = 0; i < MAX_AMOUNT_OF_CARS; i++) {
			if (cars[i] == NULL || cars[i] == this || getVerticalDistanceToACar(cars[i]) > CAR_HEIGHT) {
				continue;
			}

			int distanceBetweenCars = getVerticalDistanceToACar(cars[i]);
			if (_pos.x - cars[i]->getPos().x <= CAR_WIDTH && _pos.x - cars[i]->getPos().x >= 0) { // this on the  right or center
				if (distanceBetweenCars < CAR_HEIGHT && distanceBetweenCars > CAR_HEIGHT - COLLISION_MARGIN && _pos.y - cars[i]->getPos().y > 0) { // this on the bottom
					if (i == 0) {
						cars[0]->destroy();
						continue;
					}
					moveVector collisionVector = { VSTATIONAL, VUp * _carSpeed - 1};
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}
				else if (distanceBetweenCars < CAR_HEIGHT && distanceBetweenCars > CAR_HEIGHT - COLLISION_MARGIN && cars[i]->getPos().y - _pos.y > 0) { // this on the top
					if (i == 0) {
						destroy();
						continue;
					}
					moveVector collisionVector = { VSTATIONAL, VDown * _carSpeed + 1};
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}

				else if (distanceBetweenCars < CAR_HEIGHT) {
					moveVector collisionVector = { VLeft * _carSpeed - 1, VSTATIONAL };
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}
			}
			else if (cars[i]->getPos().x - _pos.x < CAR_WIDTH && cars[i]->getPos().x - _pos.x > 0) { // this on the left
				if (distanceBetweenCars < CAR_HEIGHT && distanceBetweenCars > CAR_HEIGHT - COLLISION_MARGIN && _pos.y - cars[i]->getPos().y > 0) { // this on the bottom
					if (i == 0) {
						cars[0]->destroy();
						continue;
					}
					moveVector collisionVector = { VSTATIONAL, VUp * _carSpeed - 1 };
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}
				else if (distanceBetweenCars < CAR_HEIGHT && distanceBetweenCars > CAR_HEIGHT - COLLISION_MARGIN && cars[i]->getPos().y - _pos.y > 0) { // this on the top
					if (i == 0) { 
						destroy();
						continue;
					}
					moveVector collisionVector = { VSTATIONAL, VDown * _carSpeed + 1 };
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}

				else if (distanceBetweenCars < CAR_HEIGHT) {
					moveVector collisionVector = { VRight * _carSpeed + 1, VSTATIONAL };
					cars[i]->moveCar(collisionVector, cars, curDistance);
					continue;
				}
			}
		}
	}

	// returns how many pixels the car is off the road
	int howMuchOffRoad(Car* car, int curDistance) {
		int roadLength = int(randomDeterministicNumber(curDistance - car->getPos().y) * ROAD_LENGTH_CALCULATIONS );
		int roadStart = (SCREEN_WIDTH - roadLength) / 2;

		if (car->getPos().x > roadStart && car->getPos().x < roadLength + roadStart) {
			return 0;
		}
		else if (roadStart - car->getPos().x > 0) {
			return roadStart - car->getPos().x;
		}
		else if (car->getPos().x - (roadStart + roadLength) > 0) {
			return car->getPos().x - (roadStart + roadLength);
		}
	}
};