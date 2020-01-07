#define MYEOF 4
#define OK 0
#define ERROR -1
#define GHOST 1
#define FOOD 2

//define pro obrazovku
#define VYSKA 600
#define SIRKA 600

#define blockSize 30

//define pro pohyby
#define UP		1
#define DOWN	2
#define RIGHT	3
#define LEFT	4

//define pro kolize
#define NOCOLISION		10
#define MAZECOLISION	11
#define FOODCOLISION	12
#define GHOSTCOLISION	13

//pocet jidlo
#define	FOODAMOUNT 40

typedef struct
{
	int x1;
	int y1;
	int x2;
	int y2;
} Vector;

typedef struct Pacman
{
	int lives;
	int score;
} Pacman;

typedef struct
{
	int x;
	int y;
} Ghost;

typedef struct
{
	int x;
	int y;
} Food;

Vector* mazeVector = NULL; // Pole usecek, tvoricich bludiste
SDL_Rect* rectMaze = NULL;
int* bestScoreArr  = NULL;

void printObjects(int);
int loadMaze(char*, int);
void mazeInit(SDL_Rect*, Vector*, int);
void mazeDraw(SDL_Renderer*, int, SDL_Rect*);
int getLineNum(char*);
void playerInit(SDL_Rect*);
void textDraw(SDL_Renderer*, Pacman*, SDL_Texture*);
void playerDraw(SDL_Renderer*, SDL_Rect*);
void ghostDraw(SDL_Renderer*, SDL_Rect*);
void foodDraw(SDL_Renderer*, SDL_Rect*);
void graphicDraw(Vector*, SDL_Renderer*, int*, Pacman*, SDL_Rect*, SDL_Rect*,SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*);
int isColision(SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*,Vector*, int*, Pacman*, SDL_Rect*, SDL_Rect*);
int ghostMove(SDL_Rect*, SDL_Rect*, int, int*, int*);
void ghostInit(SDL_Rect*, int, int);
void foodInit(SDL_Rect*);
void foodControlPrint(SDL_Rect*);
void menuDraw(SDL_Renderer* , int* );
void readBestScore(char*, int, int*);
int compareInt(const void*, const void*);
void scoreSorting(int, int*, int*);
void winGameDraw(SDL_Renderer*);
void loseGameDraw(SDL_Renderer*);
void bestScoreWriteToTxt(char*, Pacman);


