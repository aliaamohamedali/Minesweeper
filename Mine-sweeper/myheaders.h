#ifndef MYHEADERS_H_INCLUDED
#define MYHEADERS_H_INCLUDED

typedef struct{
   char name[25];
   float score;
}player;

typedef struct{
   char display;
   int content;
   int state;
   int flagged;
   int question;
}cell;

cell object = {'X', 0, 0, 0, 0};

void loadSavedGames (char** savedGames, int* savedFiles, int* SIZE2);
void saveGame(char* fileName, cell** grid, int row, int col, player plyr, int moves, int openCells, int flags, int ques, time_t start);
void loadGame(char* fileName, cell** grid, int* row, int* col, player* plyr, int* moves, int* openCells, int* flags, int* ques, time_t *start);
void sortScores(player* list, int numPlayers);
void saveScores(player* list, int numPlayers);
void scoreBoard(/*char* fileName,*/player* list, int* playerNum, int* SIZE);
void printScores (player* list, int playerNum);
int takenName(player* list, int numPlayers, char* input);
int isInt(char* test);
int parseToInt(char* test);
float calcScore (float time, int moves, int row, int col);
void initGrid( cell** grid, int row, int col);
void fillMines(cell** grid, int row, int col, int mines);
void incrementContent(cell** grid, int row, int col);
void lose (cell** grid, int row, int col);
void win(cell** grid, int row, int col);
void flagCell(cell** grid,int x,int y);
int isFlagged(cell** grid, int row, int col, int x, int y);
void questionCell(cell** grid,int x,int y);
int isQuestioned(cell** grid, int row, int col, int x, int y);
void unMark(cell** grid, int x, int y);
int isMarked(cell** grid, int x, int y);
void displayGrid(cell** grid,int row,int col);
int validateInput(cell** grid, int row, int col, int x, int y);
void openCell(cell** grid, int row, int col, int x, int y, int* openCells, int* ques);


#endif // MYHEADERS_H_INCLUDED
