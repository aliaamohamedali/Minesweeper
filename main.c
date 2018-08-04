#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include "myheaders.h"

void loadSavedGames (char** savedGames, int* savedFiles, int* SIZE2){
    FILE* fptr = fopen("savedGames.txt", "r");
    if(fptr==NULL){
        printf("Error opening File");
        exit(1);
    }
    if((*savedFiles)==(*SIZE2)){
        (*SIZE2)*=2;
        savedGames=(char**)realloc(savedGames, (*SIZE2)*sizeof(char*));
    }
    int i=0;
    while(!feof(fptr)){
        fscanf(fptr, "%[^,]\n", *(savedGames+i));
        i++;
        (*savedFiles)++;
    }
    fclose(fptr);
}

void saveGame(char* fileName, cell** grid, int row, int col, player plyr, int moves, int openCells, int flags, int ques, time_t start){
    int i, k, j=0;
    strcat(fileName, ".bin");
    FILE *fptr=fopen(fileName, "wb");
    if(fptr==NULL){
        printf("Error saving game!\n");
        exit(1);
    }
    do{
       j = fwrite(plyr.name, strlen(plyr.name)+1, 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&moves, sizeof(moves), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&openCells, sizeof(openCells), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&flags, sizeof(flags), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&ques, sizeof(ques), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&row, sizeof(row), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&col, sizeof(col), 1, fptr);
    }while(j!=1);
    j=0;
    do{
       j = fwrite(&start, sizeof(start), 1, fptr);
    }while(j!=1);
    j=0;
    // Save grid
    for(i=0; i<row+2; i++)
       for(k=0; k<col+2; k++){
         do{
             j = fwrite((*(grid+i)+k), sizeof(cell), 1, fptr);
         }while(j!=1);
         j=0;
       }
    fclose(fptr);
    fptr = fopen("savedGames.txt", "a");
    if (fptr==NULL){
        printf("Error!");
    }
    fprintf(fptr,"%s\n", fileName);
    fclose(fptr);
}

void loadGame(char* fileName, cell** grid, int* row, int* col, player* plyr, int* moves, int* openCells, int* flags, int* ques, time_t *start){
    int i, k, j=0;
    FILE *fptr=fopen(fileName, "rb");
    if(fptr==NULL){
        printf("Error Loading!\n");
    }
    else
        while(!feof(fptr)){
           do{
              j = fread(plyr->name, strlen(plyr->name)+1, 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(moves, sizeof(moves), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(openCells, sizeof(openCells), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(flags, sizeof(flags), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(ques, sizeof(ques), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(row, sizeof(row), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(col, sizeof(col), 1, fptr);
           }while(j!=1);
           j=0;
           do{
              j = fread(start, sizeof(start), 1, fptr);
           }while(j!=1);
           j=0;
           // Initialize grid
           grid=(cell**)malloc(((*row)+2)*sizeof(cell*));
           for(i=0; i<(*row)+2; i++)
              *(grid+i)=(cell*)malloc(((*col)+2)*sizeof(cell));
           // load grid
           for(i=0; i<(*row)+2; i++)
             for(k=0; k<(*col)+2; k++){
                do{
                   j = fread((*(grid+i)+k), sizeof(cell), 1, fptr);
                }while(j!=1);
                j=0;
             }
           }
           fclose(fptr);
}

void sortScores(player* list, int numPlayers){
    int i, k;
    player temp;
    for(i=numPlayers-1; i>0; i--)
        for(k=i; k>0; k--)
            if(list[k].score > list[k-1].score){
            temp = list[k];
            list[k] = list[k-1];
            list[k-1] = temp;
        }
}

void saveScores(player* list, int numPlayers){
    FILE* fptr = fopen("scores.txt", "w");
    int i;
    for(i=0; i<numPlayers; i++){
        fprintf(fptr, "%s,%f\n", list[i].name, list[i].score);
    }
    fclose(fptr);
}

void scoreBoard(player* list, int* playerNum, int* SIZE){
    int i=0;
    FILE* fptr = fopen("scores.txt", "r");
    if(fptr==NULL){
        exit(1);
    }
    else
        while(!feof(fptr)){
            if(i==(*SIZE)){
                (*SIZE)*=2;
                list= (player*)realloc(list, (*SIZE)*sizeof(player));
            }
            fscanf(fptr, "%[^,],%f\n",(list+i)->name, &((list+i)->score));
            (*playerNum)++;
            i++;
        }
    fclose(fptr);
}

void printScores (player* list, int playerNum){
    int i;
    printf("Rank\tName\t\tScore\n");
    for(i=0; i<playerNum; i++){
        printf("%d:\t%s\t\t%f\n", i+1, (list+i)->name, (list+i)->score);
    }
    printf("%d %d\n", i, playerNum);
    // At this point playerNum is the position of the first "empty" element in the array
}

int takenName(player* list, int numPlayers, char* input){
  int i;
  for(i=0; i<numPlayers; i++)
     if(strcmp(list[i].name, input)==0){
        printf("Username already taken! Please try another!\n");
        return 1;
        }
  return 0;
}

int isInt(char* test){
    int i=0, l=strlen(test);
    if((test[0]<'3' && l==1)){
        printf("please enter valid numbers!\n");
        return 0;
    }
    while(i<l){
        if(test[i]<'0' || test[i]>'9'){
            printf("please enter valid numbers!\n");
            return 0;
        }
        i++;
    }
    if(atoi(test)>1000){
       printf("Please enter valid numbers!\n");
        return 0;
    }
    return atoi(test);
}

int parseToInt(char* test){
    int i=0, l=strlen(test);
    while(i<l){
        if(test[i]<'0' || test[i]>'9'){
            printf("please enter valid numbers!\n");
            return 0;
        }
        i++;
    }
    return atoi(test);
}

float calcScore (float time, int moves, int row, int col){
    return (pow(row,4)*pow(col,4)/(moves*time));
}

void initGrid( cell** grid, int row, int col){
    int i, k;
    for(i=1; i<=row; i++)
        for(k=0; k<=col; k++){
            *(*(grid+i)+k)= object;
        }
}

void fillMines(cell** grid, int row, int col, int mines){
    time_t t;
    srand(time(&t));
    int i, k;
    while(mines!=0){
        i= ((int)ceil(rand() / 13.0 * 7 + rand()) % row) +1;
        k= ((int)ceil(rand() / 13.0 * 7 + rand()) % col) +1;
        //printf("\n>>> i: %d k: %d\n", i, k);
        if(i>=1 && k>=1 && i<=row && k<=col && (*(grid+i)+k)->content!=9){
            (*(grid+i)+k)->content=9;
            mines--;
        }
    }
}

void incrementContent(cell** grid, int row, int col){
    int i, k, l, m;
    for(i=1; i<=row; i++)
        for(k=1; k<=col; k++)
            if((*(grid+i)+k)->content==9)
                for(l=i-1; l<=i+1; l++)
                    for(m=k-1; m<=k+1; m++)
                        if(l>0 && l<=row)
                            if(m>0 && m<=col)
                                if((*(grid+l)+m)->content!=9)
                                    ((*(grid+l)+m)->content)++;
}

void lose (cell** grid, int row, int col){
    printf("Sorry you lose\n");
    int i, k;
    for(i=1; i<=row; i++)
        for(k=1; k<=col; k++){
            if ((*(grid+i)+k)->content!=9)
                (*(grid+i)+k)->display ='M';
            else if ((*(grid+i)+k)->content==9)/*Has mine*/
                (*(grid+i)+k)->display ='*';
            if ((*(grid+i)+k)->content==9 && (*(grid+i)+k)->state==1)/*Has mine && opened*/
                (*(grid+i)+k)->display = '!';
            if ((*(grid+i)+k)->flagged==1 && (*(grid+i)+k)->content!=9)/*flagged && has no mine*/
                (*(grid+i)+k)->display = '-';
        }
    displayGrid(grid, row, col);
}

void win(cell** grid, int row, int col){
    /*pass by grid*/
    int i=1, k=1;
    for(i=1; i<=row; i++)
        for(k=1; k<=col; k++){
            if (grid[i][k].content==9)/*has mine*/
                (*(grid+i)+k)->display ='F';
            else
                (*(grid+i)+k)->display ='-';
        }
    printf("***YOU WIN***\n");
    /*
    displayGrid(grid, row, col);
    plyr->score=calcScore(seconds, moves, row, col);
    printf("%s\n", plyr->name);
    printf("%f\n", plyr->score);
    */
}

void flagCell(cell** grid,int x,int y){
    (*(grid+x)+y)->flagged=1;
    (*(grid+x)+y)->display='F';
}

int isFlagged(cell** grid, int row, int col, int x, int y){
    if(x<1 || y<1 || x>row || y>col){ /*Out of bound input*/
        printf("Out of bound input!");
        return 1;
    }
    if((*(grid+x)+y)->state==1){ /*Already open*/
        printf("Cell already open!");
        return 1;
    }
    if((*(grid+x)+y)->flagged==0) /* Not flagged*/
        return 0;
    if((*(grid+x)+y)->flagged==1)
        printf("Cell already flagged!\n");
    return 1;
}

void questionCell(cell** grid,int x,int y){
    (*(grid+x)+y)->question=1;
    (*(grid+x)+y)->display='?';
}

int isQuestioned(cell** grid, int row, int col, int x, int y){
    if(x<1 || y<1 || x>row || y>col){ /*Out of bound input*/
        printf("Out of bound input!");
        return 1;
    }
    if((*(grid+x)+y)->state==1){ /*Already open*/
        printf("Cell already open!");
        return 1;
    }
    if((*(grid+x)+y)->question==0) /*Not Questioned*/
        return 0;
    if((*(grid+x)+y)->question==1)
        printf("Cell already questioned!\n");
    return 1;
}

void unMark(cell** grid, int x, int y){
    (*(grid+x)+y)->flagged=0;
    (*(grid+x)+y)->question=0;
    (*(grid+x)+y)->display='X';
}

int isMarked(cell** grid, int x, int y){
    if((*(grid+x)+y)->flagged==0 && (*(grid+x)+y)->question==0){
        printf("Cell not flagged or questioned.\n");
        return 0;
    }
    return 1;
}

void displayGrid(cell** grid,int row,int col){
    int i=1, k=1;
    printf(" ");
    for(k=1; k<=col; k++){
        printf(" %d", k);
    }
    printf("\n");
    for(i=1; i<=row; i++){
        printf("%d ", i);
        for(k=1; k<=col; k++)
            printf("%c ", (*(grid+i)+k)->display);
        printf("\n");
    }
}

int validateInput(cell** grid, int row, int col, int x, int y){
    if(x<1 || y<1 || x>row || y>col){ /*Out of bound input*/
        printf("Out of bound input!\n");
        return 0;
    }
    if((*(grid+x)+y)->state==1){ /*Already open*/
        printf("Cell already open!\n");
        return 0;
    }
    if((*(grid+x)+y)->flagged==1){ /*Is flagged*/
        printf("Flagged cell! Cannot open!\n");
        return 0;
    }
    return 1;
}

void openCell(cell** grid, int row, int col, int x, int y, int* openCells, int* ques){
    if (x > row || y > col) // Out of bound
        return;
    if (x < 1 || y < 1) // Out of bound
        return;
    if((*(grid+x)+y)->state==1)  // Already Open
        return;
    if((*(grid+x)+y)->flagged==1) // Flagged cell
        return;
    (*(grid+x)+y)->state=1; /*Mark as open*/
    (*(grid+x)+y)->display=(*(grid+x)+y)->content +'0'; /* change Display*/
    (*openCells)++;
    if((*(grid+x)+y)->question==1){  /*Cell had a '?' on it*/
        (*(grid+x)+y)->question=0;
        (*ques)--;
    }
    if ((*(grid+x)+y)->content==0){ /*No mines around*/
        (*(grid+x)+y)->display=' ';
            openCell(grid, row, col, x-1, y, openCells, ques);
            openCell(grid, row, col, x-1, y+1, openCells, ques);
            openCell(grid, row, col, x, y+1, openCells, ques);
            openCell(grid, row, col, x+1, y+1, openCells, ques);
            openCell(grid, row, col, x+1, y, openCells, ques);
            openCell(grid, row, col,  x+1, y-1, openCells, ques);
            openCell(grid, row, col, x, y-1, openCells, ques);
            openCell(grid, row, col, x-1, y-1, openCells, ques);
    }
}

int main(){
    int row=0, col=0, mines=0, moves=0, flags=0, ques=0, openCells=0, players=0, savedFiles=0, x, y; int i, loseOrWin=0;
    int SIZE=100, SIZE2=20;
    char action;
    char fileName[20], buff[10];
    time_t start, during, end;
    float seconds;
    player plyr;
    cell** grid;
    char** savedGames=(char**)malloc(SIZE2*sizeof(char*));
    for(i=0; i<SIZE2; i++){
        savedGames[i]=malloc(20*sizeof(char));
    }
    loadSavedGames(savedGames, &savedFiles, &SIZE2);
    player* playerList=(player*)malloc(SIZE*sizeof(player));
    scoreBoard(playerList, &players, &SIZE);
    // printScores(playerList, players);
    printf("Welcome to Minesweeper!\n");
 while (row==0 || col==0){
 while (row==0 || col==0){
    printf("Please choose next action:\n1)New Game.\n2)Load Game.\n3)Show Scoreboard.\n4)Exit.\n");
    action = getchar();
    // scanf("%c", &action);
    while(action!='1' && action !='2' && action !='3' && action != '4'){
        printf("Choice must be 1, 2, 3 or 4:\n");
        scanf(" %c", &action);
    }
    switch (action){
        case '1':
           printf("please Enter Name (b/w 1 & 14 charachters):");
           do{
              scanf("%s",plyr.name);
              if(strlen(plyr.name)>14)
                printf("Username too large! Try again:\n");
           }while(takenName(playerList, players, plyr.name)|| strlen(plyr.name)>14);
           getchar();
           do{
              printf("Please enter rows, (3<=row<=1000):\n");
              scanf("%s", buff);
           }while(!isInt(buff));
           row = isInt(buff);
           do{
              printf("Please enter columns, (3<=column<=1000):\n");
              scanf("%s", buff);
           }while(!isInt(buff));
           col = isInt(buff);
           //printf("%d %d", row, col);
           grid=(cell**)malloc((row+2)*sizeof(cell*));
           for(i=0; i<row+2; i++)
              *(grid+i)=(cell*)malloc((col+2)*sizeof(cell));
           mines = 1+ (row*col)/10;
           initGrid(grid, row, col);
           fillMines(grid, row, col, mines);
           incrementContent(grid, row, col);
           time(&start);
           break;
        case '2':
           if (savedFiles==0){
             printf("No saved Games\n");
             break;
           }
           printf("List of saved games: (%d game(s))\nEnter number of game you want to load.\n", savedFiles);
           for(i=0; i<savedFiles; i++){
               printf("%d: %s\n", i+1, (*savedGames+i));
           }
           do{
              scanf("%s", buff);
           }while(!parseToInt(buff) || (parseToInt(buff)<1) || (parseToInt(buff)>savedFiles));
           strcpy(fileName, (*savedGames+parseToInt(buff)-1));
           printf("\n%s\n", fileName);
           loadGame(fileName, grid, &row, &col, &plyr, &moves, &openCells, &flags, &ques, &start);
           break;
        case '3':
           printScores(playerList, players);
           break;
        default:
           return 1;
    }
  }
    while(!loseOrWin){
          if (openCells==(row*col-mines)){  /* Player has won*/
              time(&end);
              seconds = (float)difftime(end, start);
              win(grid, row, col);
              if(players==SIZE){
                  SIZE*=2;
                  playerList=(player*)realloc(playerList, SIZE*sizeof(player));
              }
              plyr.score=calcScore(seconds, moves, row, col);
              printf("%s\n", plyr.name);  // Print Name
              printf("%f\n", plyr.score); // Print score
              strcpy(playerList[players].name, plyr.name); // Add to score list
              playerList[players].score=plyr.score; // Add
              players++;
              displayGrid(grid, row, col);
              sortScores(playerList, players);
              printScores(playerList, players);
              saveScores(playerList, players);
              printf("\n%d\n", players);
              loseOrWin=1;
              row=0; col=0;
          }
          displayGrid(grid, row, col);
          printf("moves=%d\tflags=%d\tQues=%d\tTime=%f\n", moves, flags, ques, seconds);
          printf("Choose action.\nOpen cell (o)\nFlag cell (f).\nQuestion cell. (q)\nUnmark cell. (m)\nSave Game. (s)\nExit w/o save. (e)\n");
          getchar();
          scanf("%c", &action);
          while(action!='o' && action!='f' && action!='q' && action!='m' && action!='s' && action!='e'){
              printf("Please enter 'o', 'f', 'q', 'm', 's' or 'e'\n");
              getch();
              scanf("%c", &action);
          }
          switch(action){
               case 'o':
                     do{
                        do{
                          printf("Enter valid position (row) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        x=parseToInt(buff);
                        do{
                          printf("Enter valid position (column) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        y=parseToInt(buff);
                     } while(!validateInput(grid, row, col, x, y));
                     openCell(grid, row, col, x, y, &openCells, &ques);
                     if ((*(grid+x)+y)->content==9){/*Has mine*/
                        lose(grid, row, col);
                        loseOrWin=1;
                        row=0; col=0;
                     }
                     moves++;
                     time(&during);
                     seconds=(float)difftime(during, start);
                     break;
               case 'f':
                     do{
                        do{
                          printf("Enter valid position (row) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        x=parseToInt(buff);
                        do{
                          printf("Enter valid position (column) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        y=parseToInt(buff);
                     }while(isFlagged(grid, row, col, x, y));
                     if (grid[x][y].question==1){
                        grid[x][y].question=0;
                        ques--;
                     }
                     flagCell(grid, x, y);
                     moves++;
                     flags++;
                     time(&during);
                     seconds=(float)difftime(start, during);
                     break;
               case 'q':
                     do{
                        do{
                          printf("Enter valid position (row) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        x=parseToInt(buff);
                        do{
                          printf("Enter valid position (column) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        y=parseToInt(buff);
                     }while(isQuestioned(grid, row, col, x, y));
                     if (grid[x][y].flagged==1){
                        grid[x][y].flagged=0;
                        flags--;
                     }
                     questionCell(grid, x, y);
                     moves++;
                     ques++;
                     time(&during);
                     seconds=(float)difftime(start, during);
                     break;
               case 'm':
                     do{
                        do{
                          printf("Enter valid position (row) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        x=parseToInt(buff);
                        do{
                          printf("Enter valid position (column) of cell:");
                          scanf("%s", buff);
                        }while(!parseToInt(buff));
                        y=parseToInt(buff);
                     }while(!isMarked(grid, x, y));
                     if((*(grid+x)+y)->flagged==1)
                       flags--;
                     else
                       ques--;
                     unMark(grid, x, y);
                     moves++;
                     time(&during);
                     seconds=(float)difftime(start, during);
                     break;
               case 's':
                     printf("File will be saved to your name\n");
                     strcpy(fileName, plyr.name);
                     saveGame(fileName, grid, row, col, plyr, moves, openCells, flags, ques, start);
                     loseOrWin=1;
                     row=0; col=0;
                     break;
               case 'e':
                     printf("Warning! All progress will be lost");
                     return 0;
           }
    }
 }
}
