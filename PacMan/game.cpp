#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <string.h>
#include <time.h> 
#include "game.h"

int SDL_main(int, char**)
{
	char filename[80];
	char filenameBestScore[80] = "bestscore.txt";
	int threeBestScore[3] = {0,0,0};	//pole po uložení 3 nejlepších score hráèe
	int ghostCounter1 = 0;				//counter pro èásteènì náhodný pohyb ducha
	int ghostCounter2 = 50; 
	srand(time(0));						// init nahodného generátoru èísel
	Pacman pacmanPlayer;
	Vector vector;
	SDL_Rect* rectMaze;
	int lineNum, status;
	int directionGhost1 = UP;
	int directionGhost2 = DOWN;
	int directionGhost3 = DOWN;
	int directionGhost4 = UP;
	int lineNumBestScore = getLineNum(filenameBestScore);
	bestScoreArr = (int*)malloc(sizeof(int) * lineNumBestScore);
	readBestScore(filenameBestScore,lineNumBestScore, bestScoreArr);
	scoreSorting(lineNumBestScore, bestScoreArr, threeBestScore);
	SDL_bool doneMenu = SDL_FALSE;
	SDL_bool doneGame = SDL_FALSE;
	SDL_Event eventMenu;

	SDL_Window* windowMenu = SDL_CreateWindow("PacMan Menu",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VYSKA,
		SIRKA, 0);
	SDL_Window* windowGame = SDL_CreateWindow("PacMan",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VYSKA,
		SIRKA, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(windowGame, -1, 0);

	//menu loop
	while (!doneMenu)
	{
		while (SDL_PollEvent(&eventMenu))
		{
			menuDraw(renderer, threeBestScore);
			if (eventMenu.type == SDL_QUIT)
			{
				doneMenu = SDL_TRUE;
			}

			if (eventMenu.type == SDL_KEYDOWN)
			{
				if (eventMenu.key.keysym.sym == SDLK_a)
				{
					strcpy(filename, "Textak.txt");
					doneMenu = SDL_TRUE;
					break;
				}
				if (eventMenu.key.keysym.sym == SDLK_b)
				{
					strcpy(filename, "Textak2.txt");
					doneMenu = SDL_TRUE;
					break;
				}

			}

		}


	}
	SDL_Rect foodArr[FOODAMOUNT];
	lineNum = getLineNum(filename);
	mazeVector = (Vector*)malloc(sizeof(Vector) * lineNum);
	rectMaze = (SDL_Rect*)malloc(sizeof(SDL_Rect) * lineNum);
	status = loadMaze(filename, lineNum);
	
	//init skore a životù hráèe
	pacmanPlayer.score = 0;
	pacmanPlayer.lives = 4;


	//init pozice a velikosti rectu hrace
	SDL_Rect* playerRect = new SDL_Rect;
	playerRect->x = playerRect->y =
	playerRect->w = playerRect->h = 20;

	//create rect ghosta
	SDL_Rect* GhostRect1 = new SDL_Rect;
	SDL_Rect* GhostRect2 = new SDL_Rect;
	SDL_Rect* GhostRect3 = new SDL_Rect;
	SDL_Rect* GhostRect4 = new SDL_Rect;

	//init ghosta
	ghostInit(GhostRect1, 430, 20);
	ghostInit(GhostRect2, 200, 20);
	ghostInit(GhostRect3, 50, 450);
	ghostInit(GhostRect4, 400, 450);

	//init bludiste
	mazeInit(rectMaze, mazeVector, lineNum);

	//init žrádla
	foodInit(foodArr);

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{

		if (SDL_CreateWindowAndRenderer(SIRKA, VYSKA, 0, &windowGame, &renderer) == 0)
		{

			// Hlavní herní smyèka
			while (!doneGame)
			{
				SDL_Delay(20);
				ghostMove(GhostRect1, rectMaze, lineNum,&directionGhost1, &ghostCounter1);
				ghostMove(GhostRect2, rectMaze, lineNum, &directionGhost2, &ghostCounter1);
				ghostMove(GhostRect3, rectMaze, lineNum, &directionGhost3, &ghostCounter2);
				ghostMove(GhostRect4, rectMaze, lineNum, &directionGhost4, &ghostCounter2);
				ghostCounter1++;
				ghostCounter2++;
				SDL_Event event;
				graphicDraw(&vector, renderer, &lineNum, &pacmanPlayer, playerRect,
					GhostRect1, GhostRect2, GhostRect3, GhostRect4, foodArr, rectMaze);
				if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
					&lineNum, &pacmanPlayer, foodArr, rectMaze) == GHOSTCOLISION)
				{
					pacmanPlayer.lives -= 1;
					playerRect->x = playerRect->y = 20;
				}
				while (SDL_PollEvent(&event)) 
				{
					if (pacmanPlayer.lives <= 0)
					{
						loseGameDraw(renderer);
						SDL_Delay(200);
						doneGame = SDL_TRUE;
					}
					if (pacmanPlayer.score == FOODAMOUNT)
					{
						winGameDraw(renderer);
						SDL_Delay(200);
						doneGame = SDL_TRUE;
					}
					if (event.type == SDL_QUIT)
					{
						doneGame = SDL_TRUE;
					}
					if (event.type == SDL_KEYDOWN)
					{
						printf("x= %d, y= %d \n", playerRect->x, playerRect->y);

						// Logický pøepínaè zajištující pohyb hráèe na mapì
						switch (event.key.keysym.sym)
						{
						case SDLK_RIGHT:
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == NOCOLISION)
							{
								playerRect->x += 5;
							}
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == MAZECOLISION)
							{
								playerRect->x -= 5;
							}
							break;
						case SDLK_LEFT:
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == NOCOLISION)
							{
								playerRect->x -= 5;
							}
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == MAZECOLISION)
							{
								playerRect->x += 5;
							}
							
							break;
						case SDLK_UP:
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == NOCOLISION)
							{
								playerRect->y -= 5;
							}
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == MAZECOLISION)
							{
								playerRect->y += 5;
							}

							break;
						case SDLK_DOWN:
							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == NOCOLISION)
							{
								playerRect->y += 5;
							}

							if (isColision(playerRect, GhostRect1, GhostRect2, GhostRect3, GhostRect4, mazeVector,
								&lineNum, &pacmanPlayer, foodArr, rectMaze) == MAZECOLISION)
							{
								playerRect->y -= 5;
							}
							break;
						}
					}
				}

			}
		}
		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}
		if (windowGame) {
			SDL_DestroyWindow(windowGame);
		}
		if (windowMenu) {
			SDL_DestroyWindow(windowMenu);
		}
	}
	bestScoreWriteToTxt(filenameBestScore, pacmanPlayer);
	free(rectMaze);
	free(mazeVector);
	free(bestScoreArr);
	SDL_Quit();

	return 0;

}

/// <summary>
/// Funkce naète seznam nejlepších score do pamìti pro následné zobrazení v meny
/// </summary>
void readBestScore(char* filenameBestScore, int lineNumBestScore, int* bestScoreArr)
{
	FILE* f;
	int i = 0;
	f = fopen(filenameBestScore, "r");
	if (f)
	{
		while (fscanf(f, "%d", &bestScoreArr[i]) != EOF)
		{
			printf("score in file = %d \n", bestScoreArr[i]); //kontrolní výpis score do konzloze
			i++;
		}
		fclose(f);
	}
}

/// <summary>
/// Funkce porovnávající integer pro následné použití ve funkci "qsort"
/// </summary>
int compareInt(const void* a, const void* b)
{
	if (*(int*)a > * (int*)b) return -1;
	if (*(int*)a == *(int*)b) return 0;
	if (*(int*)a < *(int*)b) return 1;
}

/// <summary>
/// Setøídìní naèteného score hráèe pro následné zobrazení
/// </summary>
void scoreSorting(int lineNumBestScore, int* bestScoreArr, int* threeBestScore)
{
	qsort(bestScoreArr, lineNumBestScore, sizeof(int), compareInt);
	for (int i = 0; i < 3; i++)
	{
		threeBestScore[i] = bestScoreArr[i];
	}
}

/// <summary>
/// Funkce pro zobrazení obrazovky kdy hráè vyhraje hru
/// </summary>
void winGameDraw(SDL_Renderer* renderer)
{
	SDL_Surface* winSurface = SDL_LoadBMP("youwon.bmp");
	SDL_Texture* winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
	SDL_RenderCopy(renderer, winTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
	if (winTexture)
	SDL_DestroyTexture(winTexture);
}

/// <summary>
/// Funkce pro zobrazení obrazovky, kdy hráè prohraje hru
/// </summary>
void loseGameDraw(SDL_Renderer* renderer)
{
	SDL_Surface* loseSurface = SDL_LoadBMP("youlose.bmp");
	SDL_Texture* loseTexture = SDL_CreateTextureFromSurface(renderer, loseSurface);
	SDL_RenderCopy(renderer, loseTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
	if (loseTexture)
	SDL_DestroyTexture(loseTexture);
}

/// <summary>
/// Zápis posledního score kterého dosáhl hráè ve høe do souboru
/// </summary>
void bestScoreWriteToTxt(char* filenameBestScore, Pacman pacmanPlayer)
{
	FILE* f;
	int i = 0;
	f = fopen(filenameBestScore, "a");
	if (f)
	{
		fprintf(f, "%d\n", pacmanPlayer.score);
		fclose(f);
	}
}
/// <summary>
/// Kontrolni vypis kritickych pametovych objektu
/// </summary>
void printObjects(int numMaze)
{
	printf("Prehled objektu v pameti:\n\n");
	printf("Bludiste:\n");
	for (int i = 0; i <= numMaze - 1; i++)
	{
		printf("x1:%u, y1:%u, x2:%u, y2:%u\n", mazeVector[i].x1, mazeVector[i].y1, mazeVector[i].x2, mazeVector[i].y2);
	}

}

/// <summary>
/// Naètení vectorù bludištì ze souboru
/// </summary>
int loadMaze(char* filename, int lineNum)
{

	int x1, y1, x2, y2;  // Souøadnice usecky z textoveho souboru 
	FILE* f1;			 //Ukazatel na stream otevøeného souboru     
	int i = 0;           //Poèet radku textaku pro ktere je nutno realokovat pamet

	f1 = fopen(filename, "r");
	for (int i = 0; i < lineNum; i++)
	{
		mazeVector[i].x1 = 0;
		mazeVector[i].y1 = 0;
		mazeVector[i].x2 = 0;
		mazeVector[i].y2 = 0;
	}

	if (f1)
	{

		printf("Necetl jsem ze souboru:\n");

		while (fscanf(f1, "%d,%d,%d,%d", &x1, &y1, &x2, &y2) != EOF)
		{
			printf("x1=%d y1=%d x2=%d y2=%d \n", x1, y1, x2, y2);
			mazeVector[i].x1 = x1;
			mazeVector[i].y1 = y1;
			mazeVector[i].x2 = x2;
			mazeVector[i].y2 = y2;
			i++;
		}
	}

	else
	{
		printf("Reading file error \n");
		return ERROR;
	}
	return OK;
}

/// <summary>
/// Funkce pøevádí vectory naètené ze souboru a následnì z nich tvoøí recty
/// </summary>
void mazeInit(SDL_Rect* mazeRect, Vector* mazeVector, int linenum)
{
	for (int i = 0; i < linenum; i++)
	{
		if (mazeVector[i].y1 == mazeVector[i].y2) // vodorovna
		{

			mazeRect[i].w = abs((mazeVector[i].x2) - (mazeVector[i].x1));
			mazeRect[i].h = 5;
			mazeRect[i].x = mazeVector[i].x1;
			mazeRect[i].y = mazeVector[i].y1;
		}
		else
		{ //svisla
			mazeRect[i].w = 5;
			mazeRect[i].h = abs((mazeVector[i].y2) - (mazeVector[i].y1));
			mazeRect[i].x = mazeVector[i].x1;
			mazeRect[i].y = mazeVector[i].y1;
		}

	}
}

/// <summary>
/// Funkce vykreslující jednotlivé recty ze kterých se skládá bludištì
/// </summary>
void mazeDraw(SDL_Renderer* renderer, int lineNum, SDL_Rect* mazeRect)
{
	for (int i = 0; i < lineNum; i++)
	{
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(renderer, &mazeRect[i]);
	}
	return;
}

/// <summary>
/// Zjisti kolik radek popisu bludiste je v textovem souboru
/// </summary>
int getLineNum(char* filename)
{
	FILE* f1;
	int countLines = 0;
	char filechar[40], chr;
	f1 = fopen(filename, "r");
	if (f1)
	{
		chr = getc(f1);
		while (chr != EOF)
		{
			if (chr == '\n')
			{
				countLines = countLines + 1;
			}
			chr = getc(f1);
		}
		fclose(f1);
	}
	else
	{
		return (ERROR);
	}
	countLines++;
	return(countLines);
}

/// <summary>
/// inicializuje hráèe na urèené pozici na mapì
/// </summary>
void playerInit(SDL_Rect* a)
{
	a->x = 0;
	a->y = 10;
	a->w = 20;
	a->h = 20;
}

/// <summary>
/// Funkce zajištuje vykreslení textu score a životy hráèe bìhem hry na obrazovce
/// </summary>
void textDraw(SDL_Renderer* renderer, Pacman* pacmanPlayer, SDL_Texture* textureScore)
{
	TTF_Init();
	char buffer[200];
	sprintf(buffer, "Score: %d    Lives: %d", pacmanPlayer->score, pacmanPlayer->lives);
	TTF_Font* font = TTF_OpenFont("arial.ttf", 50);
	SDL_Color color = { 255, 255, 0 };
	SDL_Surface* surfaceScore = TTF_RenderText_Solid(font, buffer, color);
	textureScore = SDL_CreateTextureFromSurface(renderer, surfaceScore);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(textureScore, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectScore = { 10, 550, texW, texH };
	SDL_RenderCopy(renderer, textureScore, NULL, &dstrectScore);
}

/// <summary>
/// Funkce vykreslující hráèe
/// </summary>
void playerDraw(SDL_Renderer* renderer, SDL_Rect* playerRect)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, playerRect);
}

/// <summary>
/// Funkce vykreslující ducha
/// </summary>
void ghostDraw(SDL_Renderer* renderer, SDL_Rect* ghost)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, ghost);
}

/// <summary>
/// Funkce vykreslující žrádlo
/// </summary>
void foodDraw(SDL_Renderer* renderer, SDL_Rect* foodArr)
{
	for (int i = 0; i < FOODAMOUNT; i++)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &foodArr[i]);
	}
	return;
}
/// <summary>
/// Funkce zajištující renderování všech objektù a jejich naslednému postupnému vykreslení v obrazovce
/// </summary>
void graphicDraw(Vector* vector, SDL_Renderer* renderer, int* lineNum,
	Pacman* pacmanPlayer, SDL_Rect* playerRect, SDL_Rect* GhostRect1,
	SDL_Rect* GhostRect2, SDL_Rect* GhostRect3, SDL_Rect* GhostRect4, SDL_Rect* foodArr, SDL_Rect* rectMaze)
{
	SDL_Texture* textureScore = NULL;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	textDraw(renderer, pacmanPlayer, textureScore);
	mazeDraw(renderer, *lineNum, rectMaze);
	playerDraw(renderer, playerRect);
	ghostDraw(renderer, GhostRect1);
	ghostDraw(renderer, GhostRect2);
	ghostDraw(renderer, GhostRect3);
	ghostDraw(renderer, GhostRect4);
	foodDraw(renderer, foodArr);
	SDL_RenderPresent(renderer);
	if (textureScore)
	SDL_DestroyTexture(textureScore);
}

/// <summary>
/// Funkce vyšetøující kolize hráèe vùèi duchùm, žrádlu a bludišti
/// </summary>
int isColision(SDL_Rect* playerRect, SDL_Rect* ghostRect1, SDL_Rect* ghostRect2, SDL_Rect* ghostRect3, SDL_Rect* ghostRect4,
	Vector* mazeVector, int* linenum, Pacman* pacmanPlayer, SDL_Rect* foodArr, SDL_Rect* rectMaze)
{
	for (int i = 0; i < *linenum; i++)
	{
		SDL_bool collisionPlayerMaze = SDL_HasIntersection(playerRect, &rectMaze[i]);
		if (collisionPlayerMaze == SDL_TRUE)
		{
			printf("Hrac kolize\n");
			return MAZECOLISION;
		}

	}

	for (int i = 0; i < FOODAMOUNT; i++)
	{
		SDL_bool collisionPlayerFood = SDL_HasIntersection(playerRect, &foodArr[i]);
		if (collisionPlayerFood == SDL_TRUE)
		{
			pacmanPlayer->score += 1;
			foodArr[i].x = foodArr[i].y = -10;
		}
	}

	//kolize hráè bludištì
	SDL_bool collisionPlayerGhost1 = SDL_HasIntersection(playerRect, ghostRect1);
	SDL_bool collisionPlayerGhost2 = SDL_HasIntersection(playerRect, ghostRect2);
	SDL_bool collisionPlayerGhost3 = SDL_HasIntersection(playerRect, ghostRect3);
	SDL_bool collisionPlayerGhost4 = SDL_HasIntersection(playerRect, ghostRect4);

	if (collisionPlayerGhost1 == SDL_TRUE || collisionPlayerGhost2 == SDL_TRUE || collisionPlayerGhost3 == SDL_TRUE || collisionPlayerGhost4 == SDL_TRUE)
	{
		return GHOSTCOLISION;
	}

	return NOCOLISION;
}

/// <summary>
/// Funkce zajištující pohyb duchù po mapì
/// </summary>
int ghostMove(SDL_Rect* ghostRect, SDL_Rect* mazeRect, int linenum, int* direction, int* ghostCounter)
{
	//logická podmínka zajištující èásteènou náhodnost pohybu
	if (*ghostCounter % 150 == 0)
	{
		*direction = rand() % (LEFT + 1 - UP) + UP;
	}

	if (*direction == UP)
	{
		ghostRect->y--;
	}

	if (*direction == RIGHT)
	{
		ghostRect->x++;
	}
	if (*direction == LEFT)
	{
		ghostRect->x--;
	}
	if (*direction == DOWN)
	{
		ghostRect->y++;
	}

	//cyklus pro zjištìní kolize ducha s bludištìm
	for (int i = 0; i < linenum; i++)
	{
		SDL_bool collisionMazeGhost1 = SDL_HasIntersection(&mazeRect[i], ghostRect);
		SDL_bool collisionMazeGhost2 = SDL_HasIntersection(&mazeRect[i], ghostRect);
		SDL_bool collisionMazeGhost3 = SDL_HasIntersection(&mazeRect[i], ghostRect);
		SDL_bool collisionMazeGhost4 = SDL_HasIntersection(&mazeRect[i], ghostRect);
		if (collisionMazeGhost1 == SDL_TRUE)
		{
			if (*direction == UP)
			{
				ghostRect->y++;
			}

			if (*direction == RIGHT)
			{
				ghostRect->x--;
			}
			if (*direction == LEFT)
			{
				ghostRect->x++;
			}
			if (*direction == DOWN)
			{
				ghostRect->y--;
			}
			switch (*direction)
			{
			case UP:
			{
				*direction = RIGHT; break;
			}
			case RIGHT:
			{
				*direction = DOWN; break;
			}
			case DOWN:
			{
				*direction = LEFT; break;
			}
			case LEFT:
			{
				*direction = UP; break;
			}
			}
		}
	}
	return *direction;

}

/// <summary>
/// Funkce inicializující pozici a velikost ghosta na dodaných souøadnicích
/// </summary>
void ghostInit(SDL_Rect* ghost, int x, int y)
{
	ghost->h = 20;
	ghost->w = 20;
	ghost->x = x;
	ghost->y = y;
}

/// <summary>
/// Funkce inicializující žrádlo po obvodu hlavního herního ètverce mapy
/// </summary>
void foodInit(SDL_Rect* foodArr)
{
	int count1 = 1;
	int count2 = 1;
	int count3 = 1;
	int count4 = 1;
	for (int i = 0; i < 10; i++)
	{
		foodArr[i].h = 5;
		foodArr[i].w = 5;
		foodArr[i].x = 20;
		foodArr[i].y = count1 * 45;
		count1++;
	}
	for (int i = 10; i < 20; i++)
	{
		foodArr[i].h = 5;
		foodArr[i].w = 5;
		foodArr[i].x = 550;
		foodArr[i].y = count2 * 45;
		count2++;
	}
	for (int i = 20; i < 30; i++)
	{
		foodArr[i].h = 5;
		foodArr[i].w = 5;
		foodArr[i].x = count3 * 50;
		foodArr[i].y = 20;
		count3++;
	}
	for (int i = 30; i < FOODAMOUNT; i++)
	{
		foodArr[i].h = 5;
		foodArr[i].w = 5;
		foodArr[i].x = count4 * 50;
		foodArr[i].y = 450;
		count4++;
	}
}

/// <summary>
/// Debug funkce pro vypis souøadnic žrádla na mapì
/// </summary>
void foodControlPrint(SDL_Rect* foodArr)
{
	for (int i = 0; i < FOODAMOUNT; i++)
	{
		printf("Zradlo na pozicich:\n");
		printf("h= %d, w= %d, x= %d, y= %d", foodArr[i].h, foodArr[i].w, foodArr[i].x, foodArr[i].y);
		printf("\n");
	}
}

/// <summary>
/// Funkce vykreslující grafické menu na zaèátku hry
/// </summary>
void menuDraw(SDL_Renderer* renderer, int* threeBestScore)
{
	TTF_Init();
	char buffer[200];
	sprintf(buffer, "Tvoje 3 best score: %d, %d, %d", threeBestScore[0], threeBestScore[1], threeBestScore[2]);
	TTF_Font* fontScore = TTF_OpenFont("arial.ttf", 20);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* surfaceBestScore = TTF_RenderText_Blended_Wrapped(fontScore, buffer, color, 500);
	SDL_Texture* textureBestScore = SDL_CreateTextureFromSurface(renderer, surfaceBestScore);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(textureBestScore, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectScore = { 300,150, texW, texH };

	//naètení menu ze souboru
	SDL_Surface* menuSurface = SDL_LoadBMP("Menu.bmp");
	SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
	SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
	SDL_RenderCopy(renderer, textureBestScore, NULL, &dstrectScore);
	SDL_RenderPresent(renderer);
	if (textureBestScore)
	SDL_DestroyTexture(textureBestScore);
	if(menuTexture)
	SDL_DestroyTexture(menuTexture);
}

