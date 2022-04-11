#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#ifndef max     // The max method we will need in h
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min     // The min method we will need in h
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#define N 6     // The size of the maze
#define P 0.3   // The probability of a block being unavailable

struct block{    
    int available;  // Indicates if the block is available or not
    struct block * next,
                 * previous,
                 * parent;
    int g, h, h1, h2, x, y, isFinalState, visited, inList, isStartingNode;
}maze[N][N];

struct block * takeoutMin(struct block * head, int algorithm);
void putinChildren(struct block * parent);
int addChild(struct block * currentBlock, struct block * child, struct block * parent);
void printMaze(){
    int i,j;
    for(i = 0; i< N; i++){
        for(j = 0; j < N; j++){
            if(maze[i][j].available == 1){
                if(maze[i][j].isFinalState == 1){
                printf("X   ");
                }else{
                    printf("O   ");
                }
            }
            else{
                printf("#   ");
            }
        }   
        printf("\n");
    }
    printf("\n");
    return;
}

void printFinalMaze(){
    int i,j;
    
    for(i = 0; i< N; i++){
        for(j = 0; j < N; j++){
            if(maze[i][j].isFinalState == 1){
                printf("X   ");
            }else if(maze[i][j].isStartingNode){
                printf("S   ");
            }else if(maze[i][j].available == 1){
                printf("O   ");
            }else if(maze[i][j].visited == 1){
                printf("*   ");
            }else if(maze[i][j].inList == 1){
                printf("+   ");
            }
            else{
                printf("#   ");
            }
        }   
        printf("\n");
    }
    printf("\n");
    return;
}



int main(){
    int i,j;
    int Sx, Sy, G1x, G1y, G2x, G2y; // The coordinates of each block
    int algorithm = 1; // If the user wants to choose the UCS OR THE A* ALGORITHM

// Input that we will use for our program from the user:

    printf("-------------WELCOME-------------\n\n\n");
    printf("Please enter the coordinates x and y for the starting block(S) and the two final blocks(G1, G2) \n\n");
    printf("\nSx: ");
    scanf("%d", &Sx);
    printf("\nSy: ");
    scanf("%d", &Sy);
    printf("\nG1x: ");
    scanf("%d", &G1x);
    printf("\nG1y: ");
    scanf("%d", &G1y);
    printf("\nG2x: ");
    scanf("%d", &G2x);
    printf("\nG2y: ");
    scanf("%d", &G2y);
    printf("\n\n");
    // printf("\nDo you want the UCS (Choice: 1) or the A-star (Choice: 2) algorithm?\n");
    // printf("Choice: ");
    // scanf("%d", &algorithm);
    printf("\n\n");


    for(i = 0; i < N; i++){     // Make the blocks unavailable with a probability equal to P
        for(j = 0; j < N; j++){
            double r = ((double) rand() / (RAND_MAX));
            // adding the coordinates in the struct to use later
            maze[i][j].x = i;
            maze[i][j].y = j;
            maze[i][j].isFinalState = 0;
            if(r <= P){
                maze[i][j].available = 0;
            }else{
                maze[i][j].available = 1;
            }
            maze[i][j].visited = 0;   
            maze[i][j].inList = 0;
            maze[i][j].h = min(max((abs(i - G1x)),abs(j - G1y)),max((abs(i - G2x)),abs(j - G2y)));
        }
    }
    
    // Making sure that each starting and ending point is available and marking them as Final States:
    maze[Sx][Sy].available = 1;
    maze[Sx][Sy].isStartingNode = 1;
    maze[G1x][G1y].available = 1;
    maze[G1x][G1y].isFinalState = 1;
    maze[G2x][G2y].available = 1;
    maze[G2x][G2y].isFinalState = 1;
    printMaze();

    int ret = fork();   // In order to have both algorithms in the program
    if(ret == 0){      // The child
        printf("UCS ALGORITHM \n");
        algorithm = 1;
    }else{             // The parent
        printf("A-STAR ALGORITHM\n");
        algorithm = 2;  
    }
    // THE MAIN PROGRAM:
    int movesCounter = 0;
    struct block head ; 
    head.next = &maze[Sx][Sy]; // The starting block
    head.parent = NULL;
    maze[Sx][Sy].parent = NULL;
    maze[Sx][Sy].previous = &head;
    maze[Sx][Sy].g = 0;
    maze[Sx][Sy].next = NULL;
    putinChildren(head.next);
    struct block * nowBlock = takeoutMin(head.next,algorithm);
    putinChildren(head.next);
    
    
    while(head.next != NULL){
        
        nowBlock = takeoutMin(head.next,algorithm);
        putinChildren(nowBlock);
        movesCounter++;

        if(nowBlock->isFinalState == 1){ // FOUND FINAL STATE
            printf("GOT TO THE END FOUND THE FINAL BLOCK IN %d MOVES\n", movesCounter);
            printf("Cost: %d\n",nowBlock->g);
            // printf("The reversed path is: \n-row|column\n");    // Is right only with A*
            // while(nowBlock->parent != NULL){
            //     printf("- %d, %d\n", nowBlock->x, nowBlock->y);
            //     nowBlock = nowBlock->parent;
            // }
            printFinalMaze();
            return 0;
        }
    }
    printf("THERE IS NO PATH TO THE END\n Moves: %d\n", movesCounter);
    printFinalMaze();
    return 0;
}


// The method we will use in our connected list to find the block with the minimum value of g or (g + h) and take it out of the list
struct block * takeoutMin(struct block * head, int algorithm){
    struct block * minBlock = head;
    struct block * currentBlock = head;
    
    while(currentBlock->next != NULL){ // While not the tail of the list
        currentBlock = currentBlock->next;
        if(algorithm == 1){
            if(currentBlock->g < minBlock->g){
                minBlock = currentBlock;
            }
        }else{
            if((currentBlock->g + currentBlock->h) < (minBlock->g + minBlock->h)){
                minBlock = currentBlock;
            }
        }
    }
    // Take it out of the list
    minBlock->previous->next = minBlock->next;

    if(minBlock->next != NULL){ // If it is not the tail
        (minBlock->next)->previous = minBlock->previous;
    }
    minBlock->available = 0;
    minBlock->visited = 1;
    return minBlock;
}
// Method that adds the child to the list if the child can be added(available == 1)
int addChild(struct block * currentBlock1, struct block * child, struct block * parent){
    if(child->available == 0)
        return 0;
    child->g = parent->g + 1;
    currentBlock1->next = child;
    child->previous = currentBlock1;
    child->parent = parent;
    child->available = 0;
    child->inList = 1;

    return 1;
}
// The method that adds the children of the block we took out in the end of the list
void putinChildren(struct block * parent){
    parent->available = 0;
    struct block * currentBlock2 = parent; // Set a new block because we are going to need the parent later on
    while(currentBlock2->next != NULL){    // In order to reach the end of the tail
        currentBlock2 = currentBlock2->next;
    }
    // Now we will check for all the possible children
    if(parent->x == N){
        if(parent->y == N){
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
        }else if(parent->y == 0){
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
        }else{
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x - 1][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
        }
    }else if(parent->x == 0){
        if(parent->y == N){
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y], parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y - 1], parent) != 0)
                currentBlock2 = currentBlock2->next;
        }else if(parent->y == 0){
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y], parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y + 1], parent) != 0)
                currentBlock2 = currentBlock2->next;
                
        }else{
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y], parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y - 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y - 1], parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x][parent->y + 1],parent) != 0)
                currentBlock2 = currentBlock2->next;
            if(addChild(currentBlock2, &maze[parent->x + 1][parent->y + 1], parent) != 0)
                currentBlock2 = currentBlock2->next;
        }
    }else{  // IF IT IS A RANDOM NODE IN THE MIDDLE
        if(addChild(currentBlock2, &maze[parent->x + 1][parent->y - 1],parent) != 0)      // down left
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x + 1][parent->y], parent) != 0)         // down middle
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x + 1][parent->y + 1], parent) != 0)     // down right
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x][parent->y - 1], parent) != 0)         // same row left
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x][parent->y + 1],parent) != 0)          // same row right
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x - 1][parent->y - 1],parent) != 0)      // up left
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x - 1][parent->y],parent) != 0)          // up middle
            currentBlock2 = currentBlock2->next;
        if(addChild(currentBlock2, &maze[parent->x - 1][parent->y + 1],parent) != 0)      // up right
            currentBlock2 = currentBlock2->next;
    }

    return;
}