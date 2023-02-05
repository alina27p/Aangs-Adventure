//prototypes
void initialize();

void goToSplash();
void splash();
void goToInstructions1();
void instructions1();
void goToInstructions2();
void instructions2();
void goToInstructions3();
void instructions3();
void goToLevel1();
void level1();
void goToLevel2();
void level2();
void goToLevel3();
void level3();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();

void initLevel1();
void updateLevel1();
void initLevel2();
void updateLevel2();
void initLevel3();
void updateLevel3();

// states
enum {SPLASH, INSTRUCTIONS1, INSTRUCTIONS2, INSTRUCTIONS3, LEVEL1, LEVEL2, LEVEL3, PAUSE, WIN, LOSE};
int state;

//buttons variables
unsigned short oldButtons;
unsigned short buttons;

int hOff;
int vOff;
OBJ_ATTR shadowOAM[128];

//variables
int time;
int level;
int lives;
int avatarState;

//level1
int collisionTime;
ANISPRITE appa;
#define WARBALLOONCOUNT 6
#define ATTACKSCOUNT 8
ANISPRITE warBalloon[WARBALLOONCOUNT];
ANISPRITE attacks[ATTACKSCOUNT];

//level2
ANISPRITE aang;
#define MAPWIDTH 512
#define MAPHEIGHT 512
#define SCROLLCOUNT 10
int collisionTime2;
ANISPRITE scrolls[SCROLLCOUNT];
int scrollsCollected;
int second;
int hasKey;
ANISPRITE key;
enum {SPRITERIGHT, SPRITELEFT, SPRITEFRONT, SPRITEBACK, SPRITEIDLE};
int prevVoff;
int prevHoff;

int collisionCheck(unsigned char *collisionMap, int col, int row, int mapWidth);

//level3
int numOfSoldiers;
int seed;
ANISPRITE aang2;
#define SOLDIERCOUNT 1
ANISPRITE soldiers[SOLDIERCOUNT];
enum {SOLDIERSTAND, SOLDIERIGNITE, SOLDIERAIM, SOLDIERFIRE, SOLDIERIDLE};
#define FIREBALLCOUNT 3
ANISPRITE fireball[FIREBALLCOUNT];
int aangSSRow;
int aangSSCol;
#define AIRBALLCOUNT 3
ANISPRITE airball[AIRBALLCOUNT];
int collisionTime3;
int collisionTime4;

