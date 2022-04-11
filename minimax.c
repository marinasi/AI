#include <stdio.h>
#include <stdlib.h>

#define M1 5  // White nodes
#define M2 6  // Black nodes

// Final results:
#define WON 10
#define LOST -10
#define TIE 0

// Define the users of the game:
#define PROG 0
#define USER 1

struct node{
    int blacks, whites, pathWorth; // Number of black, white nodes and the worth of the path
    struct node * move1, // Taking out one black
                * move2, // Taking out two blacks and one white 
                * move3, // Taking out one white 
                * move4, // Taking out two whites and one black
                * nextMove; // The move it will make next
};

int minimax(struct node * root, int player){
    int res1, res2, res3, res4;
    int max_value = -100000; // something really small
    int min_value = 100000;  // something really big
                               // Results of each path and the maximun and the minimun value of all the paths

    if(root->blacks == 1 && root->whites == 0){ // Only scenario where we can have a tie
        root->pathWorth = TIE;
        return TIE;
    }
    else if(root->blacks == 0 && root->whites == 0){
        if(player == PROG){
            root->pathWorth = WON;
            return WON;
        }else{
            root->pathWorth = LOST;
            return LOST;
        }
    }
    else if(player == PROG){  // If the program is playing
        // Create the tree that we'll need:
        if(root->blacks >= 1){
            root->move1 = (struct node *)malloc(sizeof(struct node)); // Create the new node
            root->move1->blacks = root->blacks - 1; // make the adjustments for the first move
            root->move1->whites = root->whites;
            res1 = minimax(root->move1,USER); // hold the value of the tree that is going to be created
        }else{
            root->move1 = NULL;
        }
        if(root->blacks >= 2 && root->whites >= 1){ // Same with the above for the second move
            root->move2 = (struct node *)malloc(sizeof(struct node));
            root->move2->blacks = root->blacks - 2;
            root->move2->whites = root->whites - 1;
            res2 = minimax(root->move2,USER);
        }else{
            root->move2 = NULL;
        }
        if(root->whites >= 1){ // Same with the above for the third move
            root->move3 = (struct node *)malloc(sizeof(struct node)); 
            root->move3->blacks = root->blacks; 
            root->move3->whites = root->whites - 1;
            res3 = minimax(root->move3,USER); 
        }else{
            root->move3 = NULL;
        }
        if(root->whites >= 2 && root->blacks >= 1){ // Same with the above for the fourth move
            root->move4 = (struct node *)malloc(sizeof(struct node)); 
            root->move4->blacks = root->blacks - 1; 
            root->move4->whites = root->whites - 2;
            res4 = minimax(root->move4,USER); 
        }else{
            root->move4 = NULL;
        }
        // Now find the best move to make:
        if(root->move1 != NULL){
            max_value = res1;
            root->nextMove = root->move1;
        }if(root->move2 != NULL && res2 > max_value){
            max_value = res2;
            root->nextMove = root->move2;
        }if(root->move3 != NULL && res3 > max_value){
            max_value = res3;
            root->nextMove = root->move3;
        }if(root->move4 != NULL && res4 > max_value){
            max_value = res4;
            root->nextMove = root->move4;
        }
        root->pathWorth = max_value;
        return max_value;
    }else{ // If the user is playing
        // Create the tree that we'll need:
        if(root->blacks >= 1){
            root->move1 = (struct node *)malloc(sizeof(struct node)); // Create the new node
            root->move1->blacks = root->blacks - 1; // make the adjustments for the first move
            root->move1->whites = root->whites;
            res1 = minimax(root->move1,PROG); // hold the value of the tree that is going to be created
        }else{
            root->move1 = NULL;
        }
        if(root->blacks >= 2 && root->whites >= 1){ // Same with the above for the second move
            root->move2 = (struct node *)malloc(sizeof(struct node));
            root->move2->blacks = root->blacks - 2;
            root->move2->whites = root->whites - 1;
            res2 = minimax(root->move2,PROG);
        }else{
            root->move2 = NULL;
        }
        if(root->whites >= 1){ // Same with the above for the third move
            root->move3 = (struct node *)malloc(sizeof(struct node)); 
            root->move3->blacks = root->blacks; 
            root->move3->whites = root->whites - 1;
            res3 = minimax(root->move3,PROG); 
        }else{
            root->move3 = NULL;
        }
        if(root->whites >= 2 && root->blacks >= 1){ // Same with the above for the fourth move
            root->move4 = (struct node *)malloc(sizeof(struct node)); 
            root->move4->blacks = root->blacks - 1; 
            root->move4->whites = root->whites - 2;
            res4 = minimax(root->move4,PROG); 
        }else{
            root->move4 = NULL;
        }
        // Now find the best move the user can make:
        if(root->move1 != NULL){
            min_value = res1;
            root->nextMove = root->move1;
        }if(root->move2 != NULL && res2 < min_value){
            min_value = res2;
            root->nextMove = root->move2;
        }if(root->move3 != NULL && res3 < min_value){
            min_value = res3;
            root->nextMove = root->move3;
        }if(root->move4 != NULL && res4 < min_value){
            min_value = res4;
            root->nextMove = root->move4;
        }
        root->pathWorth = min_value;
        return min_value;
    }
}

int main(){
    struct node * gameNode;
    int player, choice, stopFlag, a;

    // Opening statements
    printf("\n\n=============\n   WELCOME\n=============\n\n");
    printf("Prepare to be beaten by a computer\n\n");
    printf("Instructions:\n---------------------------------------------------------------------------------------------------------\nThere are %d black boxes and %d white ones.\n", M1, M2);
    printf("The one that takes out the last box wins the game.\n");
    printf("If the last move doesn't involve taking out at least one white box, the game is considered to be a tie.\n");
    printf("The moves that one can make are the following:\n");
    printf("Take out one black box, one white, two black and one white or two white and one black.\n");
    printf("Let's begin, the computer plays first\n---------------------------------------------------------------------------------------------------------\n\n");

    player = PROG;
    stopFlag = 0;
    // Initialize:
    gameNode->blacks = M1;
    gameNode->whites = M2;

    while(stopFlag == 0){
        if(gameNode->blacks == 0 && gameNode->whites == 0){
            if(player == PROG){
                printf("The user won. \nYou are slowly turnign into a computer like me.\n");
                stopFlag = 1;
            }else{
                printf("The computer won. \nWe might learn through our failures but the only thing that you will learn by playing again is that you can't win me.\n");
                stopFlag = 1;
            }
        }else if(gameNode->blacks == 1 && gameNode->whites == 0){
            printf("The game ended as a tie.\n");
            stopFlag = 1;
        }
        else{
            if(player == PROG){
                minimax(gameNode,PROG);
                if(gameNode->nextMove == gameNode->move1){
                    gameNode = gameNode->nextMove;
                    player = 2;
                    printf("The computer took one black box out.\n\n");
                    printf("There are %d black boxes and %d white ones.\n\n", gameNode->blacks, gameNode->whites);
                }else if(gameNode->nextMove == gameNode->move2){
                    gameNode = gameNode->nextMove;
                    player = 2;
                    printf("The computer took two black boxes and a white one out.\n");
                    printf("There are %d black boxes and %d white ones.\n", gameNode->blacks, gameNode->whites);
                }else if(gameNode->nextMove == gameNode->move3){
                    gameNode = gameNode->nextMove;
                    player = 2;
                    printf("The computer took one white box out.\n");
                    printf("There are %d black boxes and %d white ones.\n", gameNode->blacks, gameNode->whites);
                }else{
                    gameNode = gameNode->nextMove;
                    player = 2;
                    printf("The computer took two white boxes and a black one out.\n");
                    printf("There are %d black boxes and %d white ones.\n\n", gameNode->blacks, gameNode->whites);
                }
            }else{
                int validChoice;
                do{
                    validChoice = 1;
                    if(gameNode->blacks >= 1){
                        printf("Press 1 to take 1 black box out. \n");
                    }
                    if(gameNode->blacks >= 2 && gameNode->whites >= 1){
                        printf("Press 2 to take 2 black and 1 white boxes out. \n");
                    }
                    if(gameNode->whites >= 1){
                        printf("Press 3 to take 1 white box out. \n");
                    }
                    if(gameNode->whites >= 2 && gameNode->blacks >= 1){
                        printf("Press 4 to take 2 white and 1 black boxes out. \n");
                    }
                    scanf(" %d",&choice);

                    if(choice == 1 && gameNode->blacks >= 1){
                        printf("The user took 1 black box out. \n\n");
                        gameNode->blacks--;
                        player = PROG;
                    }
                    else if(choice == 2 && gameNode->blacks >= 2 && gameNode->whites >= 1){
                        printf("The user took 2 black and 1 white boxes out. \n\n");
                        gameNode->blacks--;
                        gameNode->blacks--;
                        gameNode->whites--;
                        player = PROG;
                    }
                    else if(choice == 3 && gameNode->whites >= 1){
                        printf("The user took 1 white box out. \n\n");
                        gameNode->whites--;
                        player = PROG;
                    }
                    else if(choice == 4 && gameNode->whites >= 2 && gameNode->blacks >= 1){
                        printf("The user took 2 white and 1 black boxes out. \n\n");
                        gameNode->blacks--;
                        gameNode->whites--;
                        gameNode->whites--;
                        player = PROG;
                    }else{
                        printf("WRONG INPUT... TRY AGAIN (DUMPASS)\n\n");
                        validChoice = 0;
                    }
                }while(validChoice == 0);
            }
        }
        
    }
    return 0;
}