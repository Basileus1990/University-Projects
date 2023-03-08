#define ENEMY_AGRO_RADIUS 100
#define AI_SPEED_CHANGE 5.0

void civilianControll(int distance, Car* currentCar, Car* cars[]);

void enemyControll(int distance, Car* currentCar, Car* cars[]);

// Called by the car. dispatech further control
void aiMoveCar(int distance, Car* currentCar, Car* cars[]) {
	// move the car vertically
	moveVector move = { VSTATIONAL, VUp * speedToPixelsConverter(currentCar->getSpeed() - cars[0]->getSpeed()) };
	currentCar->moveCar(move, cars, distance);

	if (currentCar->getStatus() == Civilian) {
		civilianControll(distance, currentCar, cars);
	}
	else {
		if (currentCar->getVerticalDistanceToACar(cars[0]) < ENEMY_AGRO_RADIUS) {
			enemyControll(distance, currentCar, cars);
		}
		else {
			civilianControll(distance, currentCar, cars);
		}
	}
}


void civilianControll(int distance, Car* currentCar, Car* cars[]) {
	// changes randomly speed

	if(distance % 1000 > rand() % 800 + 100) {
		currentCar->setSpeed(currentCar->getSpeed() + (rand() % 100) / 100 * AI_SPEED_CHANGE * -1);
		if (rand() % 100 > 40) {
			moveVector move = { VLeft, VSTATIONAL };
			currentCar->moveCar(move, cars, distance);
			// move away from the grass
			move.x = move.x * -1;
			if (currentCar->isOffRoad()) {
				currentCar->moveCar(move, cars, distance);
			}
		}
	}
	else {
		currentCar->setSpeed(currentCar->getSpeed() + (rand() % 100) / 100 * AI_SPEED_CHANGE);
		if (rand() % 100 > 40) {
			moveVector move = { VRight, VSTATIONAL };
			currentCar->moveCar(move, cars, distance);
			// move away from the grass
			move.x = move.x * -1;
			if (currentCar->isOffRoad()) {
				currentCar->moveCar(move, cars, distance);
			}
		}
	}
}

void enemyControll(int distance, Car* currentCar, Car* cars[]) {
	if (distance % 1000 > rand() % 800 + 100) {
		currentCar->setSpeed(currentCar->getSpeed() + (rand() % 100) / 100 * AI_SPEED_CHANGE * -1);
		// current on the  right or center
		if (currentCar->getPos().x - cars[0]->getPos().x >= 0) {
			// current on the bottom
			if (currentCar->getPos().y - cars[0]->getPos().y >= 0) {
				moveVector move = { VLeft, VUp };
				currentCar->moveCar(move, cars, distance);
			}
			// current on the top
			else {
				moveVector move = { VLeft, VDown };
				currentCar->moveCar(move, cars, distance);
			}
		}
		// current on the left
		else { 
			// current on the bottom
			if (currentCar->getPos().y - cars[0]->getPos().y >= 0) {
				moveVector move = { VRight, VUp };
				currentCar->moveCar(move, cars, distance);
			}
			// current on the top
			else {
				moveVector move = { VRight, VDown };
				currentCar->moveCar(move, cars, distance);
			}
		}
	}
}