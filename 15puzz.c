#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

const int size = 4;

void clearScreen() {
    system("cls");
}

void insertionSort(double array[],int size);

void drawBoard(int board[size][size]);

void scrambleBoard(int board[size][size]);
													
void displayBoard(int board[size][size], int key);

int isValidMove(int i, int j);

void moveTile(char direction, int board[size][size]);

int isGameComplete(int board[size][size]);

int countInversions(int array[], int size);

int getEmptyRow(int board[size][size]);

int isSolvable(int board[size][size]);

int getKey();

void displayTime(clock_t start, clock_t end, int moves, double *speed, double *time);

void saveTime(double time);

void checkPB(double time);

void displayAvgs();

void displaySession(int *count);

void resetSession();

int main() {
    srand(time(NULL));

    int board[size][size];
    int key = 0;
    drawBoard(board);
	do{
	    do{
	    	scrambleBoard(board);
	    } while(!isSolvable(board));
		
	    int timerStart = 0;
	    int moves = 0;
	    int timeCount = 0;
	    double TPS = 0;
	    double time = 0;
	    key = 0;
	    clock_t start_time, end_time;
	    
	    while (!isGameComplete(board)) {
	    	displayBoard(board,key);
	        key = getKey();
	
	        if (!timerStart && key != 9) {
	            start_time = clock();
	            timerStart = 1;
	        }
	
	        if (key == 'Q' || key == 'q') {
	            printf("\nQuitting the game.");
	            break;
	        }
	        
	        if (!timerStart && key == 9) {
	        	displaySession(&timeCount);
	        	key = getKey();
	        	if(key == 114 && timeCount != 0) {
	        		clearScreen();
					printf("Do you want to clear your session? Press ENTER to confirm.\nPress any key if you want to cancel.\n");
					key = getKey();
					if(key == 13){
						resetSession();
						getKey();
						key = 0;
					}
					else{
						printf("\nProcess has been cancelled.\nPress any key to return to the menu.");
						getKey();
						key = 0;
					}		
				}
				else key = 0;
	        }
	
	        moveTile(key,board);
	        moves++;
	    }
	    end_time = clock();
	
	    if (isGameComplete(board)) {
	        displayBoard(board,key);
	        displayTime(start_time, end_time, moves, &TPS, &time);
	        printf("\nCongratulations! You solved the puzzle in %d moves!\n",moves);
	        printf("Time taken: %.2lf seconds\n",time);
	        printf("TPS: %.2lf\n",TPS);
	    	checkPB(time);
	    	saveTime(time);
	    	displayAvgs();
	    }
	    
	    if (key == 'Q' || key == 'q') break;
	    printf("\nPress any key to continue playing. Press 'Q' to quit.");
	    key = getKey();
	    if (key == 'Q' || key == 'q') {
	        printf("\nQuitting the game.\nThanks for playing!");
	        break;
	    }
	} while(key != 'Q' || key != 'q');
    return 0;
}


void insertionSort(double array[],int size){
	int i,j;
	for(i = 1; i < size; i++){
		double temp = array[i];
		j = i - 1;
		while(j >= 0 && array[j] > temp){
			array[j+1] = array[j];
			j--;
		}
		array[j+1] = temp;
	}
}

void drawBoard(int board[size][size]) {
    int i, j, count = 1;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            board[i][j] = count++;
        }
    }
    board[size - 1][size - 1] = 0;
}

void scrambleBoard(int board[size][size]) {
    int n, i, j, k, l, temp;
    for(n = 0; n < size * size * size; n++) {
        i = rand() % size;
        j = rand() % size;
        k = rand() % size;
        l = rand() % size;
        temp = board[i][j];
        board[i][j] = board[k][l];
        board[k][l] = temp;
    }
}

void displayBoard(int board[size][size], int key) {
	int i,j;
    clearScreen();
    if(key == 0){
    	printf("Welcome to the 15 Puzzle Game!\n");
    	printf("Use WASD or arrow keys to move tiles.\n");
		printf("Press TAB to view your session. Press 'Q' to quit.\n\n");
	}
	else printf("\n\n\n\n");
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if (board[i][j] == 0) printf("   ");
            else printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
}

int isValidMove(int i, int j) {
    return (i >= 0 && i < size && j >= 0 && j < size) ? 1 : 0;
}

void moveTile(char direction, int board[size][size]) {
    int i,j,empty_x,empty_y;

    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if (board[i][j] == 0){
                empty_y = i;
                empty_x = j;
            }
        }
    }

    int new_y = empty_y;
	int new_x = empty_x;

    switch(direction) {
        case 'U':
            new_y++;
            break;
        case 'D':
            new_y--;
            break;
        case 'L':
            new_x++;
            break;
        case 'R':
            new_x--;
            break;
    }

    if(isValidMove(new_y, new_x)) {
        board[empty_y][empty_x] = board[new_y][new_x];
        board[new_y][new_x] = 0;
    }
}

int isGameComplete(int board[size][size]) {
	int i,j;
    int checkOrder[size*size];
    int tile = 0;
    int num = 1;

    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            checkOrder[tile] = num;
            tile++;
            num++;
        }
    }
    checkOrder[(size*size)-1] = 0;

    int array[size*size];
    int index = 0;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            array[index++] = board[i][j];
        }
    }

    if (array[(size*size)-1] != 0) {
        return 0;
    }

    for(int i = 0; i < size * size; i++) {
        if (array[i] != checkOrder[i]) {
            return 0;
        }
    }

    return 1;
}

int countInversions(int array[], int size) {
	int i,j;
    int inversions = 0;
    for(i = 0; i < size - 1; i++) {
        for(j = i + 1; j < size; j++) {
            if (array[i] > array[j] && array[i] != 0 && array[j] != 0) {
                inversions++;
            }
        }
    }
    return inversions;
}

int getEmptyRow(int board[size][size]) {
	int i,j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                return i;
            }
        }
    }
    return 0;
}

int isSolvable(int board[size][size]) {
	int i,j;
    int array[size*size];
    int index = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            array[index++] = board[i][j];
        }
    }

    int inversions = countInversions(array, (size*size));
    int blankRow = size - getEmptyRow(board);

    return ((blankRow % 2 == 0 && inversions % 2 == 1) || (blankRow % 2 == 1 && inversions % 2 == 0)) ? 1 : 0;
}

int getKey() {
    int ch = 0;
    ch = getch();
    if (ch == 224) {
        ch = getch();
        switch (ch) {
            case 72:
                return 'U';
            case 80:
                return 'D';
            case 75:
                return 'L';
            case 77:
                return 'R';
        }
    }
    switch(ch){
    	case 119:
    		return 'U';
    	case 115:
    		return 'D';
    	case 97:
    		return 'L';
    	case 100:
    		return 'R';
	}
    return ch;
}

void displayTime(clock_t start, clock_t end, int moves, double *speed, double *time) {
	double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    *time = timeTaken;
    *speed = moves/timeTaken;
}

void saveTime(double time){
	char timeStr[100];
	FILE *fp;
	fp = fopen("times.txt","a+");
	if(fp == NULL){
		printf("File times.txt can't be opened.\n");
		exit(1);
	}
	sprintf(timeStr,"%.2lf",time);
    fprintf(fp,"%s\n",timeStr);
	fclose(fp);
}

void checkPB(double time){
	double PB = 999999.99;
	char *eptr;
	char buffer[10];
	FILE *fp;
	fp = fopen("times.txt","a+");
	if(fp == NULL){
		printf("File times.txt can't be opened.\n");
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	while(fgets(buffer,sizeof(buffer),fp)) {
        double result = strtod(buffer, &eptr);
        if(PB > result) PB = result;
    }
	if(PB > time) printf("You've set a new personal best of %.2lf!\n",time);
	fclose(fp);
}

void displayAvgs(){
	int i,j;
	int timeCount = 0;
	int index = 0;
	double total5 = 0;
	double max5 = 0;
	double total12 = 0;
	double max12 = 0;
	double min5 = 999999.99;
	double min12 = 999999.99;
	char *eptr;
	char buffer[10];
	FILE *fp;
	fp = fopen("times.txt","a+");
	if(fp == NULL){
		printf("File times.txt can't be opened.\n");
		exit(1);
	}
	while(fgets(buffer,sizeof(buffer),fp)){
		timeCount++;
    }
	rewind(fp);
	double arrTemp[timeCount];
	
	while(fgets(buffer,sizeof(buffer),fp)){
    	double result = strtod(buffer, &eptr);
    	arrTemp[index] = result;
    	index++;
    }
    
    for(i = index - 5; i < index; i++){
    	if(max5 < arrTemp[i]) max5 = arrTemp[i];
    	if(min5 > arrTemp[i]) min5 = arrTemp[i];
    	total5 += arrTemp[i];
	}
	
	for(j = index - 12; j < index; j++){
    	if(max12 < arrTemp[j]) max12 = arrTemp[j];
    	if(min12 > arrTemp[j]) min12 = arrTemp[j];
    	total12 += arrTemp[j];
	}
	fclose(fp);
	if(timeCount >= 5) printf("Average of 5: %.2lf\n",(total5-(min5+max5))/3);
	if(timeCount >= 12) printf("Average of 12: %.2lf\n",(total12-(min12+max12))/10);
}

void displaySession(int *count){
	int i;
	int timeCount = 0;
	int index = 0;
	double total = 0;
	char *eptr;
	char buffer[10];
	FILE *fp;
	clearScreen();
	fp = fopen("times.txt","a+");
	if(fp == NULL){
		printf("File times.txt can't be opened.\n");
		exit(1);
	}
	
	while(fgets(buffer,sizeof(buffer),fp)){
		timeCount++;
    }
    *count = timeCount;
    rewind(fp);
    double arrTemp[timeCount];
    
    while(fgets(buffer,sizeof(buffer),fp)){
    	double result = strtod(buffer, &eptr);
    	arrTemp[index] = result;
    	index++;
    }
    insertionSort(arrTemp,timeCount);
    
	if(timeCount == 0) printf("Session is empty.\nDo some solves!\nPress any key to go back.");
	else {
		printf("Here is the list of times in your session.\n(Sorted from fastest to slowest):\n\n");
    	for(i = 0; i < timeCount; i++) {
    		printf("%.2lf ",arrTemp[i]);
    		if((i+1) % 10 == 0) printf("\n");
    		total += arrTemp[i];
		}
		printf("\n\nSession Mean is %.2lf out of %d solves.\n",total/timeCount,timeCount);
		printf("If you wish to clear your session, press 'R'.\n\nPress any key to return to the menu.");
	}
	fclose(fp);
}

void resetSession(){
	FILE *fp;
	fp = fopen("times.txt","w");
	if(fp == NULL){
		printf("File times.txt can't be opened.\n");
		exit(1);
	}
	fclose(fp);
	clearScreen();
	printf("Session has been cleared.\n");
	printf("Press any key to return to the menu.");
}
