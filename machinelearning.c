#include "includes.h"



int currentState;
char desiredState;
int nextState;
double gammah = 0.7;
bool actionsTested[7] = {false, false, false, false, false, false, false};
int numOfStates = 7;
int roomSize = 15;
int currentPlace;
int carY;
int carX;

int Q[7][7];
int R[7][7] =
{{100, 0, 0, 0, 0, 0, 0}, //Forward
{0, 100, 0, 0, 0, 0, 0},
{0, 0, 100, 0, 0, 0, 0},
{0, 0, 0, 100, 0, 0, 0},  //Still
{0, 0, 0, 0, 100, 0, 0},
{0, 0, 0, 0, 0, 100, 0},
{0, 0, 0, 0, 0, 0, 100}}; //Backward

int QCollision[7][7];
int RCollision[7][7] =
{{0, 0, 0, 0, 0, 0, 0},
{100, 0, 0, 0, 0, 0, 0},
{100, 0, 0, 0, 0, 0, 0},
{0, 100, 100, 100, 100, 100, 0},
{0, 0, 0, 0, 0, 0, 100},
{0, 0, 0, 0, 0, 0, 100},
{0, 0, 0, 0, 0, 0, 0}};

int room[15][15];

void initialize();
int calcQ(int action);
int directReward(int action);
int maxNextQ(int state);
//void printQ();
//void printRoom();
void testAllActions(int state);
int correctAction(int state);
void goToState(int state);
//void moveCar(int action);
//bool collisionCheck(int action);

/* int main()
{
	cout << "Starting!" << endl;
	initialize();
	cout << "IniDone" << endl;

	while (1)
	{
	cout << "Enter desired state: " << endl;
	cin >> desiredState; 

		if (desiredState == 'f'){ //forward
			nextState = 0;
		}
		else if (desiredState == 'd'){ //forward left
			nextState = 1;
		}
		else if (desiredState == 'g'){ //forward right
			nextState = 2;
		}
		else if (desiredState == 'v'){ //back left
			nextState = 4;
		}
		else if (desiredState == 'n'){ //back right
			nextState = 5;
		}
		else if (desiredState == 'b'){ //back
			nextState = 6;
		}
		else if (desiredState == 'Q'){ //PrintQ
			printQ();
		}
		else{ //stopped
			nextState = 3;
		}
		goToState(nextState);
		//printQ();
		printRoom();
//	}

	return 0;
} */


void testAllActions(int state){
	for (int i = 0; i < numOfStates; i++){
		Q[state][i] = calcQ(i);
	}
}

void goToState(int state){

	if (actionsTested[state] == false)
	{
		testAllActions(state);
		actionsTested[state] = true;
	}
	currentState = correctAction(state);
	moveCar(currentState);
}

int correctAction(int state){
	int bestAction = 0;
	int max = 0;
	for (int i = 0; i < numOfStates; i++){
		if (Q[state][i] > max)
		{
			max = Q[state][i];
			bestAction = i;
		}
	}
	return bestAction;
}

int directReward(int action){
	return R[nextState][action];
}

int maxNextQ(int action){
	int currentMax = 0;
	for (int i = 0; i < numOfStates; i++){
		if (Q[nextState][i] > currentMax)
		{
			currentMax = Q[nextState][i];
		}
	}
	if (currentMax <= directReward(action))
	{
		currentMax = 0;
	}
	return currentMax;
}

int calcQ(int action){
	return directReward(action) + gammah*maxNextQ(action);
}

/* void moveCar(int action){
	if (action == 0){
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY--;
			room[carY][carX] = 1;
		}
	}
	else if (action == 1){
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY--;
			carX--;
			room[carY][carX] = 1;
		}
	}
	else if (action == 2){
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY--;
			carX++;
			room[carY][carX] = 1;
		}
	}
	else if (action == 4){
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY++;
			carX--;
			room[carY][carX] = 1;
		}
	}
	else if (action == 5) {
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY++;
			carX++;
			room[carY][carX] = 1;
		}
	}
	else if(action == 6){
		if (collisionCheck(action) == false){
			room[carY][carX] = 0;
			carY++;
			room[carY][carX] = 1;
		}
	}
	else{
		//Do nothing
	}
} */

/* bool collisionCheck(int action){
	bool collision = false;

	if (action == 0){
		carY--;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY++;
	}
	else if (action == 1){
		carY--;
		carX--;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY++;
		carX++;
	}
	else if (action == 2){
		carY--;
		carX++;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY++;
		carX--;
	}
	else if (action == 4){
		carY++;
		carX--;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY--;
		carX++;
	}
	else if (action == 5){
		carY++;
		carX++;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY--;
		carX--;
	}
	else if (action == 6){
		carY++;
		if (room[carY][carX] == 2){
			collision = true;
			cout << "Crash!" << endl;
		}
		carY--;
	}
	else {
		collision = false;
	}
	return collision;
} */

void initialize(){
	srand (time(NULL));
	for (int i = 0; i < numOfStates; i++){
		for (int j = 0; j < numOfStates; j++){
			Q[i][j] = 0;
			//QCollision[i][j] = 0;
		}
	}

	/*for (int i = 0; i < roomSize; i++){
		for (int j = 0; j < roomSize; j++){
			room[i][j] = 0;
		}
	} */

	/*int blockY = 10;
	int blockX = 7; */

	room[carY][carX] = 1; //Initializing starting position for car
	//room[blockY][blockX] = 2;
	//currentState = 3;
}
