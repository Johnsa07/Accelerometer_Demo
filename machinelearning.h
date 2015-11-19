#define bool int
#define true 1
#define false 0

void initialize();
int calcQ(int action);
int directReward(int action);
int maxNextQ(int state);
void printQ();
void printRoom();
void testAllActions(int state);
int correctAction(int state);
void goToState(int state);
void moveCar(int action);
bool collisionCheck(int action);