/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 6
#define HIT 1000

// Prototype declarations of required functions
void PrintArray(int values[MAP_SIZE][MAP_SIZE]);
void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]);
void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]);
int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]);
int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]);
int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]);
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]);
void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col);
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]);


/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go below
*******************************************************************************
*******************************************************************************
******************************************************************************/

void PrintArray(int values[MAP_SIZE][MAP_SIZE])
{
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            printf("%d ", values[i][j]);
        }
        printf("\n");
    }
}

void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]) {
    int size[4] = {2, 3, 4, 5};
    int i, j, k, l;
    for (i = 0; i < 2; i++) {
        map[i][0] = size[0];
    }
    for (j = 1; j < 4; j++) {
        map[2][j] = size[1];
    }
    for (k = 0; k < 4; k++) {
        map[MAP_SIZE - 1][k] = size[2];
    }
    for (l = (MAP_SIZE-5); l < MAP_SIZE; l++) {
        map[l][MAP_SIZE-1] = size[3];
    }
}




void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]) {
    int direction = rand() % 2;
    int i;
    int x = 0;
    int y = 0;
    int unrestrained = (rand() % MAP_SIZE);
    int restrained = (rand() % (MAP_SIZE - size + 1));

    if (direction == 0) {
        y = unrestrained;
        x = restrained;

    } else if (direction == 1){
        y = restrained;
        x = unrestrained;
    }
    for (i = 0; i < size; i++) {
        if (direction == 0) {
            map[y][x + i] = size;
        } else if (direction == 1) {
            map[y + i][x] = size;
        }
    }
}


int CountValues(int value, int map[MAP_SIZE][MAP_SIZE])
{
    int count = 0;
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == value) {
                count++;
            }
        }
    }
    return count;
}

int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE])
{
    int direction = 0;
    int i, j;

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == size) {
                *row = i;
                *col = j;

                // Check the direction (horizontal or vertical)
                if (map[i][j + 1] == size) {
                    direction = 1; // Horizontal
                } else {
                    direction = 2; // Vertical
                }

                return direction;
            }
        }
    }
    return direction;
}

int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE])
{
    int i, j, k;
    int count = 0;
    int stored_row, stored_col;
    int direction = 0;

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == size) {
                count++;
            }
        }
    }

    if (count != size) {
        return 0;
    }

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (map[i][j] == size) {
                stored_row = i;
                stored_col = j;
                if (map[i][j + 1] == size) {
                    direction = 1; // Horizontal
                    goto escape;
                } else if (map[i + 1][j] == size) {
                    direction = 2; // Vertical
                    goto escape;
                }
            }
        }
    }

    escape:


    // Horizontal
    if (direction == 1) {
        for (k = stored_col; k < (stored_col + size); k++) {

            // Checks if there are any ships overlapping
            if (map[stored_row][k] != size) {
                return 0;
            }
            // Checks all around the ship as it is not at the very top or bottom.
            if ((stored_row < (MAP_SIZE - 1)) && (stored_row > 0)) {
                // Accounts for if the ships is at the left most or right most column
                if (stored_col == 0) {
                    if (map[stored_row + 1][k] > 0 || map[stored_row - 1][k] > 0 ||
                        map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                } else if ((stored_col + size - 1) == (MAP_SIZE - 1)) {
                    if (map[stored_row + 1][k] > 0 || map[stored_row - 1][k] > 0 ||
                        map[stored_row][stored_col - 1] > 0) {
                        return 0;
                    }
                } else {
                    if (map[stored_row + 1][k] > 0 || map[stored_row - 1][k] > 0 ||
                        map[stored_row][stored_col - 1] > 0 || map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                }

            } else if (stored_row == 0) { // Checks only to the bottom as it is at the very top.
                // Accounts for if the ships is at the left most or right most column
                if (stored_col == 0) {
                    if (map[stored_row + 1][k] > 0 || map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                } else if ((stored_col + size - 1) == (MAP_SIZE - 1)) {
                    if (map[stored_row + 1][k] > 0 || map[stored_row][stored_col - 1] > 0) {
                        return 0;
                    }
                } else {
                    if (map[stored_row + 1][k] > 0 || map[stored_row][stored_col - 1] > 0 || map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                }

                // Checks only to the top and right and left as it is at the very bottom.
            } else if (stored_row == MAP_SIZE - 1) {
                // Accounts for if the ships is at the left most or right most column
                if (stored_col == 0) {
                    if (map[stored_row - 1][k] > 0 || map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                } else if ((stored_col + size - 1) == (MAP_SIZE - 1)) {
                    if (map[stored_row - 1][k] > 0 || map[stored_row][stored_col - 1] > 0) {
                        return 0;
                    }
                } else {
                    if (map[stored_row - 1][k] > 0 || map[stored_row][stored_col - 1] > 0 || map[stored_row][stored_col + size] > 0) {
                        return 0;
                    }
                }
            }
        }
    } else if (direction == 2) { //Vertical
        for (k = stored_row; k < (stored_row + size); k++) {

            // Checks if there are any ships overlapping
            if (map[k][stored_col] != size) {
                return 0;
            }
            // Checks all around the ship as it is not at the edges.
            if ((stored_col < (MAP_SIZE - 1)) && (stored_col > 0)) {
                // Accounts for if the ships is at the top most or bottom most row
                if (stored_row == 0) {
                    if (map[k][stored_col + 1] > 0 || map[k][stored_col - 1] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                } else if ((stored_row + size - 1) == (MAP_SIZE - 1)) {
                    if (map[k][stored_col + 1] > 0 || map[k][stored_col - 1] > 0 || map[stored_row - 1][stored_col] > 0) {
                        return 0;
                    }
                } else {
                    if (map[k][stored_col + 1] > 0 || map[k][stored_col - 1] > 0 || map[stored_row - 1][stored_col] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                }

            } else if (stored_col == 0) { // Checks only to the right and top and bottom as it is at the very left.
                // Accounts for if the ships is at the top most or bottom most row
                if (stored_row == 0) {
                    if (map[k][stored_col + 1] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                } else if ((stored_row + size - 1) == (MAP_SIZE - 1)) {
                    if (map[k][stored_col + 1] > 0 || map[stored_row - 1][stored_col] > 0) {
                        return 0;
                    }
                } else {
                    if (map[k][stored_col + 1] > 0 || map[stored_row - 1][stored_col] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                }

            } else if (stored_col == MAP_SIZE - 1) { // Checks only to the left and top and bottom as it is at the very right.
                // Accounts for if the ships is at the top most or bottom most row
                if (stored_row == 0) {
                    if (map[k][stored_col - 1] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                } else if ((stored_row + size - 1) == (MAP_SIZE - 1)) {
                    if (map[k][stored_col - 1] > 0 || map[stored_row - 1][stored_col] > 0) {
                        return 0;
                    }
                } else {
                    if (map[k][stored_col - 1] > 0 || map[stored_row - 1][stored_col] > 0 || map[stored_row + size][stored_col] > 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]){
    int s2, s3, s4, s5;
    int size[4] = {2, 3, 4, 5};
    int invalid = 1;
    int randomMap[MAP_SIZE][MAP_SIZE] = {0};
    int i, j, k;

    while (invalid){
        for (k = 0; k < 4; k++) {
            AddRandomShip(size[k], randomMap);
        }
        s2 = IsShipValid(size[0], randomMap);
        s3 = IsShipValid(size[1], randomMap);
        s4 = IsShipValid(size[2], randomMap);
        s5 = IsShipValid(size[3], randomMap);

        if (s2 && s3 && s4 && s5){
            invalid = 0;
            for (i = 0; i < MAP_SIZE; i++){
                for (j = 0; j < MAP_SIZE; j++){
                    map[i][j] = randomMap[i][j];
                }
            }
        } else {
            for (i = 0; i < MAP_SIZE; i++){
                for (j = 0; j < MAP_SIZE; j++){
                    randomMap[i][j] = 0;
                }
            }
        }
    }
}


void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col) {
    int i, j;

    int s2Count = 0;
    int s3Count = 0;
    int s4Count = 0;
    int s5Count = 0;
    int s2x, s2y, s3x, s3y, s4x, s4y, s5x, s5y;
    int s2Direction, s3Direction, s4Direction, s5Direction;

    int tracker = 0;
    int count = 0;
    int changer = 0;

    if (shots[row][col] > 0) {
        return;
    }


    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            if (shots[i][j] != 0) {
                if (shots[i][j] % 1000 > count) {
                    count = shots[i][j] % 1000;
                }
            }
        }
    }

    s2Direction = TopLeftPosition(2, &s2y, &s2x, map);
    s3Direction = TopLeftPosition(3, &s3y, &s3x, map);
    s4Direction = TopLeftPosition(4, &s4y, &s4x, map);
    s5Direction = TopLeftPosition(5, &s5y, &s5x, map);


    if (map[row][col] == 0) {
        count++;
        shots[row][col] = count;
    } else {
        count++;
        shots[row][col] = 1000 + count;
    }
    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {
            // Checks how many shots has been made on a ship.
            if (shots[i][j] >= 1000 && shots[i][j] < 2000 && map[i][j] == 2) {
                s2Count++;
            } else if (shots[i][j] >= 1000 && shots[i][j] < 2000 && map[i][j] == 3) {
                s3Count++;
            } else if (shots[i][j] >= 1000 && shots[i][j] < 2000 && map[i][j] == 4) {
                s4Count++;
            } else if (shots[i][j] >= 1000 && shots[i][j] < 2000 && map[i][j] == 5) {
                s5Count++;
            }
        }
    }
    if (s2Count == 2) {
        if (s2Direction == 1) {
            for (i = s2x; i < (s2x + 2); i++) {
                changer = shots[s2y][i] % 1000;
                shots[s2y][i] = changer + 2000;
            }
        } else if (s2Direction == 2) {
            for (i = s2y; i < (s2y + 2); i++) {
                changer = shots[i][s2x] % 1000;
                shots[i][s2x] = changer + 2000;
            }
        }
    } else if (s3Count == 3) {
        if (s3Direction == 1) {
            for (i = s3x; i < (s3x + 3); i++) {
                changer = shots[s3y][i] % 1000;
                shots[s3y][i] = changer + 3000;
            }
        } else if (s3Direction == 2) {
            for (i = s3y; i < (s3y + 3); i++) {
                changer = shots[i][s3x] % 1000;
                shots[i][s3x] = changer + 3000;
            }
        }
    } else if (s4Count == 4) {
        if (s4Direction == 1) {
            for (i = s4x; i < (s4x + 4); i++) {
                changer = shots[s4y][i] % 1000;
                shots[s4y][i] = changer + 4000;
            }
        } else if (s4Direction == 2) {
            for (i = s4y; i < (s4y + 4); i++) {
                changer = shots[i][s4x] % 1000;
                shots[i][s4x] = changer + 4000;
            }
        }
    } else if (s5Count == 5) {
        if (s5Direction == 1) {
            for (i = s5x; i < (s5x + 5); i++) {
                changer = shots[s5y][i] % 1000;
                shots[s5y][i] = changer + 5000;
            }
        } else if (s5Direction == 2) {
            for (i = s5y; i < (s5y + 5); i++) {
                changer = shots[i][s5x] % 1000;
                shots[i][s5x] = changer + 5000;
            }
        }
    }
}
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]) {

    int i, j;

    for (i = 0; i < MAP_SIZE; i++) {
        for (j = 0; j < MAP_SIZE; j++) {

            if (map[i][j] > 0 && shots[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}
/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go above
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Provided functions needed for playing the Battleships game:

// First, prototype declarations for the bots:
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col);

// Gets the input for one move from the human player (an alphabetic row and a numeric column)
// This function converts both inputs to numeric values
void GetMoveHuman(int *row, int *col, int player)
{
	char a = ' ';
	int b = -1;
	printf("Player %d: enter move [row/col]: ", player);
	while (!(a >= 'A' && a <= 'Z')) {
		scanf("%c", &a);
	}
	while (!(b >= 0 && b <= 25)) {
		scanf("%d", &b);
	}
	*row = (int)(a - 'A');
	*col = b;
}

// Takes two "shots" arrays as input (which are the visible parts of the game shown to players) and formats them into a string for printing
// The player currently to move is highlighted
void GetDisplayMapString(int shots1[MAP_SIZE][MAP_SIZE], int shots2[MAP_SIZE][MAP_SIZE], int player, char *boardString)
{
	int i, j;
	char numbers[10];
	// Intialise board string to be empty:
	boardString[0] = '\0';
	strcat(boardString, "  ");

	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i%10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "\n  ");

	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "\n");

	for (i = 0; i < MAP_SIZE; i++) {
		int len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots1[i][j] / HIT > 0) {
				if (shots1[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots1[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots1[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots2[i][j] / HIT > 0) {
				if (shots2[i][j] / HIT == 1) {
					strcat(boardString, "X");
				} else {
					sprintf(numbers, "%d", shots2[i][j] / HIT);
					strcat(boardString, numbers);
				}
			} else if (shots2[i][j] > 0) {
				strcat(boardString, ".");
			} else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len+1] = '\0';
		strcat(boardString, "\n");
	}
	if (player == 1) {
		strcat(boardString, "  P1");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	} else {
		for (i = 0; i < MAP_SIZE; i++) {
			strcat(boardString, " ");
		}
		strcat(boardString, "   P2");
		for (i = 0; i < MAP_SIZE-2; i++) {
			strcat(boardString, "*");
		}
	}
	strcat(boardString, "\n");
}

// Plays one game of Battleships, beginning with the specified starting player
// Game type = 1 (human vs human) or 2 (human vs bot) or 3 (bot vs bot)
int PlayOneGame(int startingPlayer, int gameType)
{
	int row, col, player, gameOver;

	// String to display the boards
	char displayBoardString[(2*MAP_SIZE+5)*(MAP_SIZE+5)];

	// The maps containing the locations of the ships
	int mapPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int mapPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	// The locations of the shots
	int shotsPlayer1[MAP_SIZE][MAP_SIZE] = {0};
	int shotsPlayer2[MAP_SIZE][MAP_SIZE] = {0};

	player = startingPlayer;
	gameOver = 0;

	// Create random maps for each player
	InitialiseRandomMap(mapPlayer1);
	InitialiseRandomMap(mapPlayer2);

	// Display the board if a human is playing
	if (gameType != 3) {
		GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
		printf("%s", displayBoardString);
	}

	// Process one move for the current player
	while (!gameOver) {
		if (gameType == 1) {
			GetMoveHuman(&row, &col, player);
			if (player == 1) {
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else if (gameType == 2) {
			if (player == 1) {
				GetMoveHuman(&row, &col, player);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot1(shotsPlayer2, &row, &col);
				printf("Player 2 (bot) moves: %c%d\n", (char)(row+'A'), col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		} else {
			if (player == 1) {
				GetMoveBot1(shotsPlayer1, &row, &col);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			} else {
				GetMoveBot2(shotsPlayer2, &row, &col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}

		// Swap players
		if (!gameOver) {
			player = 3 - player;
		}
		if (gameType != 3) {
			GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
			printf("%s", displayBoardString);
		}
	}
	return player;
}

// Play a Battleships tournament
// If the tournament is between a human and a bot, only one game is played
// If the tournament is between two bots, the number of games is requested
void PlayBattleships(void)
{
	int gameType, numberOfGames, result;
	int i, wins1, wins2, player;

	// Get play options:
	printf("Options:\n");
	printf(" [1] = Human vs. Human\n");
	printf(" [2] = Human vs. Bot1\n");
	printf(" [3] = Bot1 vs. Bot2\n");
	printf("Choose game type: ");
	scanf("%d", &gameType);
	numberOfGames = 1;
	result = 0;

	// If two bots are playing a tournament, let the user choose how many games
	if (gameType == 3) {
		printf("Number of games: ");
		scanf("%d", &numberOfGames);
	}

	// Set win counts to zero
	wins1 = 0;
	wins2 = 0;

	// Player 1 will always start the first game
	// If a tournament is played (between two bots), the starting player alternates
	player = 1;

	for (i = 0; i < numberOfGames; i++) {
		result = PlayOneGame(player, gameType);
		if (result == 1) {
			wins1++;
		} else {
			wins2++;
		}
		// Switch the starting player for the next game
		player = 3 - player;
	}

	// Show the outcome (of a single game or tournament)
	if (numberOfGames == 1) {
		printf("\nGame over! Congratulations! Winner is Player %d\n\n", result);
	} else {
		printf("\nTournament over! Games played = %d\nPlayer 1 wins = %d / Player 2 wins = %d\n\n", numberOfGames, wins1, wins2);
	}
}

// The main function for the Battleships program
int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));

	printf("ENGGEN131 - C Project 2023\n");
	printf("                          ... presents ...\n");
	printf(" ___    __   _____ _____  _     ____  __   _     _   ___   __  \n");
	printf("| |_)  / /\\   | |   | |  | |   | |_  ( (` | |_| | | | |_) ( (` \n");
	printf("|_|_) /_/--\\  |_|   |_|  |_|__ |_|__ _)_) |_| | |_| |_|   _)_) \n");
	printf("\n                __/___            \n");
	printf("          _____/______|           \n");
	printf("  _______/_____\\_______\\_____     \n");
	printf("  \\              < < <       |    \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");

	PlayBattleships();

	return 0;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the two bots can be provided below
// The existing implementations are naive:
// - Bot 1 simply takes random shots (without checking for repeated shots)
// - Bot 2 also takes random shots (but makes a single test on each turn for a repeated shot)
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Strategy for Bot 1
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	*row = rand_row;
	*col = rand_col;
}

// Strategy for Bot 2
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int *row, int *col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	// If the shot has been tried before, try one more time
	if (shots[rand_row][rand_col] > 0) {
		rand_row = rand() % MAP_SIZE;
		rand_col = rand() % MAP_SIZE;
	}

	*row = rand_row;
	*col = rand_col;
}













