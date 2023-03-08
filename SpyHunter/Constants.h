// A file for all global constants, structs and funtions

struct moveVector {
	int x;
	int y;
};

struct pos {
	int x;
	int y;
};

//screen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define LETTER_HEIGHT 8
#define LETTER_WIDTH 8
#define LETTER_VERTICAL_MARGIN 5
#define TEXT_MARGIN 5
const int BLACK[3] = { 0, 0, 0 };
const int DARK_GRAY[3] = { 40, 40, 40 };
const int GRAY[3] = { 100, 100, 100 };
const int WHITE[3] = { 255, 255, 255 };
const int GREEN[3] = { 0, 200, 0 };
const int BLUE[3] = { 0, 0, 255 };
const int MENU_BACKGROUND_COLOR[3] = { 108, 47, 240 };

// game
#define ROAD_LENGTH_CALCULATIONS (100 * 1.4)
#define OFF_THE_ROAD_DEATH_RADIUS 50
#define OUTSIDE_THE_ROAD_DEATH_RADIUS 150
#define MAX_AMOUNT_OF_CARS 8
enum Movement {
	VSTATIONAL = 0,
	VRight = 1,
	VLeft = -1,
	VUp = -1,
	VDown = 1
};

// makes it so there is no same game
// needs to be initialized at the beggining of the game
int _seed;

// returns a number from a range 1 - 3 rounded to 2 decimal points
	// if gets a negative number it makes is equal 0
double randomDeterministicNumber(double x) {
	if (x < 0) {
		x = 0;
	}
	x = x / 100;
	x += _seed;

	double firstPart = cos(log(pow(x + 1, 2)));
	double secondPart = sin(x / 5) * cos(log(cos(x) + 2));
	double number = (firstPart + secondPart) + 2;
	if (number < 1) {
		return 1;
	}
	else if (number > 3) {
		return 3;
	}
	// returns rounded number
	return double(int(number * 100 + 0.5)) / 100;
}

// converts a double speed to pixels to move by randomizing with what is after the comma
int speedToPixelsConverter(double speed) {
	int mainSpeed = speed;
	double fractionSpeed = speed - mainSpeed;
	if (rand() % 100 < fractionSpeed * 100) {
		return mainSpeed + 1;
	}
	return mainSpeed;
}