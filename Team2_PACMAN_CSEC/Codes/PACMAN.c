#include "raylib.h"
#include <stdio.h>
#include <math.h>
#define ROWS 21

#define COLS 41
// Colors
Color loading[4]={YELLOW,PINK,RED,SKYBLUE};
Color ssnblue = (Color){13, 65, 190, 225};
Color pacmanColor = (Color){252, 234, 263, 255};
Color pellet = (Color){245,140,50,255};
// Screen and Cell sizes
int screen = 1;
const int screenWidth=1230;
const int screenHeight=730;
const int gameWidth = 1230;
const int gameHeight = 630;
const float cellWidth = (float)gameWidth / 41;
const float cellHeight = (float)gameHeight / 21;
//Highscore
int highscore;
//STRUCTURE DEFINITIONS
//Cell Structure
typedef struct {
    int row;
    int col;
} Cell;
//Pacman Structure
struct{
    float x;
    float y;
    int moveX;
    int moveY;
    float radius;
    float speed;
    int start;
    int end;
    int gridX;
    int gridY;
    int alive;
    int numGhost;
    Image anim[11];
    Texture2D anim_te[11];
}pacman={gameWidth/2, gameHeight/2, 0, 0, cellWidth*0.44f, 3.0, 30, 330, 20, 10, 1, 0};

typedef struct{
    Image gimage[8];
    Texture2D gtexture[8];
    int alive;
    int moveX;
    int moveY;
    float x;
    float y;
    float speed;
    Cell initial;
    Cell start;
    Cell end;
    Cell path[21*41];
    int distance;
    int sp;
    int spcounter;
    int start_time;
    int ghostFc;
    int mode;
    int modetime;
    int atcorner;
}ghost;
//Ghosts
    ghost ghosts[4];
//Maze
int maze[ROWS][COLS] = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
{1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1},
{1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1},
{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1},
{1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 3, 2, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 3, 1, 1, 1, 2, 1, 3, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1},
{1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
{0, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 0},
{1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 3, 1, 2, 1, 3, 1, 1, 1, 2, 1},
{1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1},
{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1},
{1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1},
{1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1},
{1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1},
{1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
//Declaring Game variables
float PacOldX,PacOldY,speedFactor;
int oldmoveX, oldmoveY;
int mainCounter, gameCounter, winCounter, retCounter, messCounter, x, sptime;
int startplayed = 0;
int update, is_sp, acceptInput;
int life, score, win, pelletcount = 0, level = 1;
//Cheat Code Variables
int cheatEnabled = 1, cheat = 0, aspirin = 0, freeze = 0, gohome = 0;

//Functions
//BFS
// Define queue structure for BFS
typedef struct {
    Cell items[ROWS * COLS];
    int front;
    int rear;
} Queue;

// Function to initialize queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

// Function to check if queue is empty
bool isEmpty(Queue* q) {
    return q->front == -1;
}

// Function to enqueue
void enqueue(Queue* q, Cell item) {
    if (q->rear == ROWS * COLS - 1) {
        printf("Queue is full\n");
        return;
    }
    if (isEmpty(q)) {
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = item;
}

// Function to dequeue
Cell dequeue(Queue* q) {
    Cell item;
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        item.row = -1;
        item.col = -1;
        return item;
    }
    item = q->items[q->front];
    if (q->front >= q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    else
        q->front++;
    return item;
}

// Function to check if a cell is valid and not a wall
bool isValid(int maze[][COLS], int visited[][COLS], int row, int col) {
    return (row >= 0) && (row < ROWS) &&
           (col >= 0) && (col < COLS) &&
           (maze[row][col] != 1) && (visited[row][col] == 0);
}

// Function to find the shortest distance using BFS and the actual path traversed
int shortestDistanceWithPath(int maze[][COLS], Cell start, Cell end, Cell path[]) {
    int visited[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = 0;
        }
    }
    Queue* q = createQueue();
    enqueue(q, start);
    visited[start.row][start.col] = 1;
   
    int distance = 0;
    // Array to store parent information
    Cell parent[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            parent[i][j] = (Cell){-1, -1}; // Initialize parent to (-1, -1)
        }
    }
    while (!isEmpty(q)) {
        int size = q->rear - q->front + 1;
        for (int i = 0; i < size; i++) {
            Cell current = dequeue(q);
            if (current.row == end.row && current.col == end.col) {
                // Reconstruct path
                Cell pathCell = current;
                int pathLength = 0;
                while (pathCell.row != start.row || pathCell.col != start.col) {
                    path[pathLength++] = pathCell;
                    pathCell = parent[pathCell.row][pathCell.col];
                }
                path[pathLength++] = start;
                // Reverse the path to get it in the correct order
                for (int j = 0; j < pathLength / 2; j++) {
                    Cell temp = path[j];
                    path[j] = path[pathLength - j - 1];
                    path[pathLength - j - 1] = temp;
                }
                return distance;
            }
            int row = current.row;
            int col = current.col;
            // Move to adjacent cells
            if (isValid(maze, visited, row - 1, col)) {
                enqueue(q, (Cell){row - 1, col});
                visited[row - 1][col] = 1;
                parent[row - 1][col] = current; // Set parent for the next cell
            }
            if (isValid(maze, visited, row + 1, col)) {
                enqueue(q, (Cell){row + 1, col});
                visited[row + 1][col] = 1;
                parent[row + 1][col] = current;
            }
            if (isValid(maze, visited, row, col - 1)) {
                enqueue(q, (Cell){row, col - 1});
                visited[row][col - 1] = 1;
                parent[row][col - 1] = current;
            }
            if (isValid(maze, visited, row, col + 1)) {
                enqueue(q, (Cell){row, col + 1});
                visited[row][col + 1] = 1;
                parent[row][col + 1] = current;
            }}
        distance++;
    }
    // No path found
    return -1;
}
// Draw pacman
void DrawPacman(Vector2 position, float radius, float angleStart, float angleEnd, Color color){
    DrawCircleSector(position, radius, angleStart, angleEnd, 0, color);
}

// Draw maze
void DrawMaze(int maze[][COLS], int rows, int cols, float cellWidth, float cellHeight, int sp){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (maze[i][j] == 2)
                DrawCircle(j * cellWidth + cellWidth / 2, i * cellHeight + cellHeight / 2, fminf(cellWidth, cellHeight) * 0.1f, pellet);
            else if (maze[i][j] == 3){
                if (sp == 0)
                    DrawCircle(j * cellWidth + cellWidth / 2, i * cellHeight + cellHeight / 2, fminf(cellWidth, cellHeight) * 0.2f, BLACK);
                else
                    DrawCircle(j * cellWidth + cellWidth / 2, i * cellHeight + cellHeight / 2, fminf(cellWidth, cellHeight) * 0.2f, pellet);
            }
        }
    }
}
Cell findgrid(int xstart,int xend,int ystart,int yend,int dis, int *found){
    Cell grid;
    for(int i = ystart; i<yend; i++){
        if(i>0 && i<20){
            for(int j = xstart; j<xend; j++){
                if (j>0 && j<40){
                    if(maze[i][j]!=1)
                        if (sqrt(pow(pacman.gridX-j,2)+pow(pacman.gridY-i,2)) == dis){
                            grid.row = i; grid.col = j;
                            *found = 1;
                            return grid;
                        }
                }
            }
        }
    }
    grid.row = 1; grid.col = 1; *found = 0;
    return grid;
}
void resetghost(){
    for(int i=0;i<4;i++){
        ghosts[i].x = ghosts[i].initial.col*cellWidth;
        ghosts[i].y = ghosts[i].initial.row*cellHeight;
        ghosts[i].moveX = 0;
        ghosts[i].moveY = 0;
        ghosts[i].alive = 1;
        ghosts[i].speed = pacman.speed*speedFactor;
        ghosts[i].sp = 0;
        ghosts[i].spcounter = 0;
        ghosts[i].start.row=(int)(ghosts[i].y/cellHeight);ghosts[i].start.col = (int)(ghosts[i].x/cellWidth);
        ghosts[i].mode = 1;
        ghosts[i].modetime = 1;
        ghosts[i].ghostFc = 0;
        ghosts[i].alive = 1;
        ghosts[i].atcorner = 0;
    }
}
void reset(){
    for(int row=0;row<ROWS;row++){
        for(int col=0;col<COLS;col++){
            maze[row][col] = abs(maze[row][col]);
        }
    }
}
void gamestart(int level){
    pacman.x = gameWidth/2; pacman.y = gameHeight/2;
    PacOldX = pacman.x; PacOldY = pacman.y;
    pacman.moveX = 0;pacman.moveY = 0;
    pacman.start = 30; pacman.end = 330;
    oldmoveX = 0; oldmoveY = 0;
    mainCounter = 1; gameCounter = 1; retCounter = 300; messCounter= 1, x = 1;
    int update=0, is_sp=0, acceptInput = 0;
    int cheat=0, pelletcount=0;
    reset();
    resetghost();
    if (level==1){
        life = 3;
        score = 0;speedFactor = 0.8;
        sptime = 360;startplayed=0;
        win = 0;
        pelletcount = 0;
    }
    else if(level==2){
        speedFactor = 0.85;
        sptime = 300;
    }
    else if(level==3){
        speedFactor = 0.9;
        sptime = 240;
    }
}
void initialise(){
    //pacman to start position
    pacman.moveX = 0;
    pacman.moveY = 0;
    pacman.start = 30;
    pacman.end = 330;
    //returning ghosts to starting state
    resetghost();
}
//Function to generate a cell which is not a wall
Cell checkwall(){
    Cell x;
    while(1){
        x.row = GetRandomValue(1,19);
        x.col = GetRandomValue(1,39);
        if(maze[x.row][x.col]!=1){
             return x;
        }
    }
}
//Function to pause
void standby(){
    gameCounter = 1;
    for(int i=0;i<4;i++){
        ghosts[i].moveX = 0; ghosts[i].moveY = 0;
        ghosts[i].ghostFc = 1;
    }
    pacman.x = PacOldX;pacman.y = PacOldY;
    pacman.moveX = 0; pacman.moveY = 0;
}
// CHEAT Functions
void processCHEAT(int key){
    if (key == KEY_C && cheat == 0)
        cheat = 1;
    else if (key == KEY_H && cheat == 1)
        cheat = 2;
    else if (key == KEY_E && cheat == 2)
        cheat = 3;
    else if (key == KEY_A && cheat == 3)
        cheat = 4;
    else if (key == KEY_T && cheat == 4){
        for (int i = 0; i < 21; i++){
            for (int j = 0; j < 41; j++){
                if (maze[i][j] == 2 || maze[i][j] == 3){
                    pelletcount += 1;
                    maze[i][j] = -maze[i][j];
                }
            }}
        cheat = 0;
    }
    else
        cheat = 0;
}

void processASPIRIN(int key){
    if (key == KEY_A && aspirin == 0)
        aspirin = 1;
    else if (key == KEY_S && aspirin == 1)
        aspirin = 2;
    else if (key == KEY_P && aspirin == 2)
        aspirin = 3;
    else if (key == KEY_I && aspirin == 3)
        aspirin = 4;
    else if (key == KEY_R && aspirin == 4)
        aspirin = 5;
    else if (key == KEY_I && aspirin == 5)
        aspirin = 6;
    else if (key == KEY_N && aspirin == 6){
        if(life<5)
            life=life+1;
        aspirin = 0;}
    else
        aspirin = 0;
}
int processFREEZE(int key){
    if (key == KEY_F && freeze == 0)
        freeze = 1;
    else if (key == KEY_R && freeze == 1)
        freeze = 2;
    else if (key == KEY_E && freeze == 2)
        freeze = 3;
    else if (key == KEY_E && freeze == 3)
        freeze = 4;
    else if (key == KEY_Z && freeze == 4)
        freeze = 5;
    else if (key == KEY_E && freeze == 5){
        freeze = 0;
        standby();
        gameCounter = 240;
    }
    else
        freeze = 0;
    return 0;
}

int processGOHOME(int key){
    if (key == KEY_G && gohome == 0)
        gohome = 1;
    else if (key == KEY_O && gohome == 1)
        gohome = 2;
    else if (key == KEY_H && gohome == 2)
        gohome = 3;
    else if (key == KEY_O && gohome == 3)
        gohome = 4;
    else if (key == KEY_M && gohome == 4)
        gohome = 5;
    else if (key == KEY_E && gohome == 5){
        gohome = 0;
        resetghost();
    }
    else
        gohome = 0;
    return 0;
}
void cheatcodes(int key){
    processCHEAT(key);
    processASPIRIN(key);
    processFREEZE(key);
    processGOHOME(key);
}


//To check if alphabet key is pressed
bool isAnyAlphabetKeyPressed(){
    for (int key = KEY_A; key <= KEY_Z; key++){
        if (IsKeyPressed(key))
            return true;
    }
    return false;
}
int main(){
    //Intialising Window and FPS
    InitWindow(screenWidth,screenHeight,"PACMAN");
    SetTargetFPS(60);
    //File for storing High Score and reading highscore
    FILE *scoreReadFile,*scoreWriteFile;
    scoreReadFile = fopen("C:/Users/VISHAL/Desktop/My Code Files/C/Team2_PACMAN_CSEC/Codes/highscore.dat","r");
    if(scoreReadFile!=NULL){
        fscanf(scoreReadFile,"%d",&highscore);
    }
    else{
        highscore = 0;
    }
    fclose(scoreReadFile);
    //Loading Images

    Image heart_im=LoadImage("../Resources/sprites/heart.png");
    ImageResize(&heart_im,cellWidth*1.5,cellHeight*1.5);
    Texture2D heart_te=LoadTextureFromImage(heart_im);
    Image maze_im=LoadImage("../Resources/mazes/orgfmaze.png");
    ImageResize(&maze_im,1230,630);
    Texture2D maze_te=LoadTextureFromImage(maze_im);
    Image mazew_im=LoadImage("../Resources/mazes/pwfmaze.png");
    ImageResize(&mazew_im,1230,630);
    Texture2D mazew_te=LoadTextureFromImage(mazew_im);
    Image gameover_im=LoadImage("../Resources/images/endscr.png");
    ImageResize(&gameover_im,screenWidth,screenHeight);
    Texture2D gameover_te=LoadTextureFromImage(gameover_im);
    Image ssn_logo= LoadImage("../Resources/images/SSN_logo.png");
    Image ssn_logo_small=ImageCopy(ssn_logo);
    ImageResize(&ssn_logo_small,100,50);
    Texture2D ssn_texture2=LoadTextureFromImage(ssn_logo_small);
    Texture2D ssn_texture=LoadTextureFromImage(ssn_logo);
    Image pacman_logo=LoadImage("../Resources/images/pacman_logo.png");
    Texture2D loadscreen_pacman=LoadTextureFromImage(pacman_logo);
    Image pacman_small= ImageCopy(pacman_logo);
    ImageResize(&pacman_small,200,100);
    Texture2D pac_small_te = LoadTextureFromImage(pacman_small);
    Image ghosts_image=LoadImage("../Resources/images/pac-man-wallpaper-preview.png");
    Texture2D loadscreen_background=LoadTextureFromImage(ghosts_image);
    Image inky_cyan=LoadImage("../Resources/images/inkybig(ss2).png");
    Texture2D inky=LoadTextureFromImage(inky_cyan);
    Image rules_im=LoadImage("../Resources/images/rules2.png");
    ImageResize(&rules_im,1100,650);
    Texture2D rules_te = LoadTextureFromImage(rules_im);
    Image gfrightBlue_im1 = LoadImage("../Resources/sprites/ghosts/vulnerable/blue1.png");
    Texture2D gfrightBlue_te1 = LoadTextureFromImage(gfrightBlue_im1);
    Image gfrightBlue_im2 = LoadImage("../Resources/sprites/ghosts/vulnerable/blue2.png");
    Texture2D gfrightBlue_te2 = LoadTextureFromImage(gfrightBlue_im2);
    Image gfrightWhite_im1 = LoadImage("../Resources/sprites/ghosts/vulnerable/white1.png");
    Texture2D gfrightWhite_te1 = LoadTextureFromImage(gfrightWhite_im1);
    Image gfrightWhite_im2 = LoadImage("../Resources/sprites/ghosts/vulnerable/white2.png");
    Texture2D gfrightWhite_te2 = LoadTextureFromImage(gfrightWhite_im2);
    Image geyesL_im = LoadImage("../Resources/sprites/ghosts/eyes/leftEyes.png");
    Texture2D geyesL_te = LoadTextureFromImage(geyesL_im);
    Image geyesR_im = LoadImage("../Resources/sprites/ghosts/eyes/rightEyes.png");
    Texture2D geyesR_te = LoadTextureFromImage(geyesR_im);
    Image geyesU_im = LoadImage("../Resources/sprites/ghosts/eyes/upEyes.png");
    Texture2D geyesU_te = LoadTextureFromImage(geyesU_im);
    Image geyesD_im = LoadImage("../Resources/sprites/ghosts/eyes/downEyes.png");
    Texture2D geyesD_te = LoadTextureFromImage(geyesD_im);
    Image info_im = LoadImage("../Resources/images/info.png");
    ImageResize(&info_im,screenWidth,screenHeight);
    Texture2D info_te = LoadTextureFromImage(info_im);
    //Loading Audio
    InitAudioDevice();
    Sound ssn_start=LoadSound("../Resources/audio/startup_ssn3_gta(edited).mp3");
    Sound pellet=LoadSound("../Resources/audio/munch_1.wav");
    SetSoundVolume(pellet,0.55);
    Sound n_siren=LoadSound("../Resources/audio/siren_1.wav");
    SetSoundVolume(n_siren,0.45);
    Sound s_siren=LoadSound("../Resources/audio/siren_3.wav");
    SetSoundVolume(s_siren,0.45);
    Sound start_s = LoadSound("../Resources/audio/start.mp3");
    Sound pacdead_s = LoadSound("../Resources/audio/pacdead.mp3");
    SetSoundVolume(pacdead_s,0.5);
    //Loading Fonts
    Font loadscreen_font=LoadFont("../Resources/fonts/f1/Pacfont-ZEBZ.ttf");
    Font texts1=LoadFont("../Resources/fonts/f3/Run-6YeWA.ttf");
    Font texts2=LoadFont("../Resources/fonts/f4/DebugFreeTrial-MVdYB.otf");
    Font texts3=LoadFont("../Resources/fonts/f2/BalAstaralChrome-mLdW5.ttf");
    Font texts4=LoadFont("../Resources/fonts/f6/Robus-BWqOd.otf");
    Font mainFont=LoadFont("../Resources/fonts/PressStart2P-Regular.ttf");
    //Load Screen Variables
    int posx=0;
    int posy=screenHeight/2;
    int frame_counter=0;
    int temp_c=0;
    float alpha=0.0;
    Vector2 mousepos;
   
    //Loading Pacman death animation images 
    char address[100];
    for(int i=0;i<11;i++){
        sprintf(address,"../Resources/sprites/pac/deathAnim/death%d.png",i+1);
        pacman.anim[i] = LoadImage(address);
        pacman.anim_te[i] = LoadTextureFromImage(pacman.anim[i]);
    }
    //Initialising unique ghost values
    //Blinky - ghosts[0]
    ghosts[0].gimage[0] = LoadImage("../Resources/sprites/ghosts/red/blinkyL1.png");
    ghosts[0].gimage[1] = LoadImage("../Resources/sprites/ghosts/red/blinkyL2.png");
    ghosts[0].gimage[2] = LoadImage("../Resources/sprites/ghosts/red/blinkyR1.png");
    ghosts[0].gimage[3] = LoadImage("../Resources/sprites/ghosts/red/blinkyR2.png");
    ghosts[0].gimage[4] = LoadImage("../Resources/sprites/ghosts/red/blinkyU1.png");
    ghosts[0].gimage[5] = LoadImage("../Resources/sprites/ghosts/red/blinkyU2.png");
    ghosts[0].gimage[6] = LoadImage("../Resources/sprites/ghosts/red/blinkyD1.png");
    ghosts[0].gimage[7] = LoadImage("../Resources/sprites/ghosts/red/blinkyD2.png");
    ghosts[0].x = 24*cellWidth;
    ghosts[0].y = 9*cellHeight;
    ghosts[0].initial.row = 9;ghosts[0].initial.col = 24;
    ghosts[0].start_time=240;
    //Clyde - ghosts[1]
    ghosts[1].gimage[0] = LoadImage("../Resources/sprites/ghosts/orange/inkyL1.png");
    ghosts[1].gimage[1] = LoadImage("../Resources/sprites/ghosts/orange/inkyL2.png");
    ghosts[1].gimage[2] = LoadImage("../Resources/sprites/ghosts/orange/inkyR1.png");
    ghosts[1].gimage[3] = LoadImage("../Resources/sprites/ghosts/orange/inkyR2.png");
    ghosts[1].gimage[4] = LoadImage("../Resources/sprites/ghosts/orange/inkyU1.png");
    ghosts[1].gimage[5] = LoadImage("../Resources/sprites/ghosts/orange/inkyU2.png");
    ghosts[1].gimage[6] = LoadImage("../Resources/sprites/ghosts/orange/inkyD1.png");
    ghosts[1].gimage[7] = LoadImage("../Resources/sprites/ghosts/orange/inkyD2.png");
    ghosts[1].x = 24*cellWidth;
    ghosts[1].y = 8*cellHeight;
    ghosts[1].initial.row = 8;ghosts[1].initial.col = 24;
    ghosts[1].start_time=480;
    //Pinky - ghosts[2]
    ghosts[2].gimage[0] = LoadImage("../Resources/sprites/ghosts/pink/pinkyL1.png");
    ghosts[2].gimage[1] = LoadImage("../Resources/sprites/ghosts/pink/pinkyL2.png");
    ghosts[2].gimage[2] = LoadImage("../Resources/sprites/ghosts/pink/pinkyR1.png");
    ghosts[2].gimage[3] = LoadImage("../Resources/sprites/ghosts/pink/pinkyR2.png");
    ghosts[2].gimage[4] = LoadImage("../Resources/sprites/ghosts/pink/pinkyU1.png");
    ghosts[2].gimage[5] = LoadImage("../Resources/sprites/ghosts/pink/pinkyU2.png");
    ghosts[2].gimage[6] = LoadImage("../Resources/sprites/ghosts/pink/pinkyD1.png");
    ghosts[2].gimage[7] = LoadImage("../Resources/sprites/ghosts/pink/pinkyD2.png");
    ghosts[2].x = 24*cellWidth;
    ghosts[2].y = 11*cellHeight;
    ghosts[2].initial.row = 11;ghosts[2].initial.col = 24;
    ghosts[2].start_time=540;
    //Inky - ghosts[3]
    ghosts[3].gimage[0] = LoadImage("../Resources/sprites/ghosts/blue/clydeL1.png");
    ghosts[3].gimage[1] = LoadImage("../Resources/sprites/ghosts/blue/clydeL2.png");
    ghosts[3].gimage[2] = LoadImage("../Resources/sprites/ghosts/blue/clydeR1.png");
    ghosts[3].gimage[3] = LoadImage("../Resources/sprites/ghosts/blue/clydeR2.png");
    ghosts[3].gimage[4] = LoadImage("../Resources/sprites/ghosts/blue/clydeU1.png");
    ghosts[3].gimage[5] = LoadImage("../Resources/sprites/ghosts/blue/clydeU2.png");
    ghosts[3].gimage[6] = LoadImage("../Resources/sprites/ghosts/blue/clydeD1.png");
    ghosts[3].gimage[7] = LoadImage("../Resources/sprites/ghosts/blue/clydeD2.png");
    ghosts[3].x = 24*cellWidth;
    ghosts[3].y = 12*cellHeight;
    ghosts[3].initial.row = 12;ghosts[3].initial.col = 24;
    ghosts[3].start_time=600;
   
    for(int i=0;i<4;i++){
        for(int j=0; j<8; j++){
            ImageResize(&ghosts[i].gimage[j],25,25);
            ghosts[i].gtexture[j] = LoadTextureFromImage(ghosts[i].gimage[j]);
        }
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        if(!IsSoundPlaying(ssn_start) && screen!=5)
                PlaySound(ssn_start);
        if(screen==1){
             if(IsKeyPressed(KEY_S))
                screen = 2;
            static int color_index=0;
            frame_counter+=1;
            if(frame_counter<180){
                DrawTexturePro(ssn_texture,(Rectangle){ 0, 0, ssn_texture.width, ssn_texture.height }, (Rectangle){screenWidth/2-ssn_texture.width/2,screenHeight/2-ssn_texture.height/2,ssn_texture.width,ssn_texture.height}, (Vector2){ 0, 0 }, 0.0f, Fade(WHITE, alpha));
                alpha+=0.01;
            }
            else if(frame_counter<360){
                DrawTexturePro(ssn_texture,(Rectangle){ 0, 0, ssn_texture.width, ssn_texture.height }, (Rectangle){screenWidth/2-ssn_texture.width/2,screenHeight/2-ssn_texture.height/2,ssn_texture.width,ssn_texture.height}, (Vector2){ 0, 0 }, 0.0f, Fade(WHITE, alpha));
                alpha-=0.01;
            }
            if (frame_counter%15==0)
                temp_c+=1;
           
           
            if (frame_counter>380){        
                if (temp_c%2==0){
                    DrawCircle(screenWidth/2,600,20,loading[color_index]);
                    DrawPacman((Vector2){posx,posy},95,40,320,YELLOW);
                    if(frame_counter%15==0){
                    color_index+=1;
                    if(color_index==4)
                        color_index=0;
                    }
                }
                else
                    DrawPacman((Vector2){posx,posy},90,0,360,YELLOW);
             
                DrawTexture(inky,posx-350,posy-100,WHITE);
                posx+=6;
                if(posx-350>screenWidth+ssn_texture.width/2){
                    screen+=1;
                }
            }
        }
        if(screen==2){
           Rectangle ssn = {50,625,100,50};
           static int click = 0;
           char* space ="press space to play";
           static float text_size=35, text_step=0.4;
           float max_size=50, min_size=35;
           text_size+=text_step;
           Vector2 textSize = MeasureTextEx(texts4, space, text_size, 10);
           float text_width=textSize.x;
           float text_height=textSize.y;
           if(text_size>max_size||text_size<min_size)
               text_step=-text_step;
           Vector2 info = {225, 650};
           Vector2 dev = {315, 650};
           float butradius = 30;
           mousepos=GetMousePosition();
           DrawTexture(ssn_texture2,50,625,BLUE);
           DrawTexture(loadscreen_background,screenWidth/2-125,screenHeight/2-35,WHITE);
           DrawTexture(loadscreen_pacman,screenWidth/5+35,0,WHITE);
           DrawTextEx(texts2,"A game recreated by SSN CSE Department",(Vector2){330,270},25,5,WHITE);
           DrawTextEx(texts4, space,(Vector2){(screenWidth+20 - text_width) / 2, (screenHeight+230 - text_height) / 2},text_size,10,YELLOW);
           if (CheckCollisionPointCircle(mousepos,info,butradius)){
                DrawCircleLinesV(info, butradius, ssnblue);
                DrawText("i",222,633,40,ssnblue);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    screen=4;
                }
            }
            else{
                DrawCircleV(info,butradius,ssnblue);
                DrawText("i",222,633,40,BLACK);
            }
            if (CheckCollisionPointCircle(mousepos,dev,butradius)){
                DrawCircleLinesV(dev, butradius, ssnblue);
                DrawText("?",305,633,35,ssnblue);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    ClearBackground(BLACK);
                    screen=3;
                }
            }
            else{
                DrawCircleV(dev,butradius,ssnblue);
                DrawText("?",305,633,35,BLACK);
            }
            if(IsKeyPressed(KEY_SPACE)){
                screen=5;
                StopSound(ssn_start);
            }
            if(CheckCollisionPointRec(mousepos,ssn)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                click+=1;
            }
            if(click==3||click==4){
                DrawText("WE MADE THE BEST PAC-MAN! PERIOD!",320,400,30,WHITE);
            }
            if(click==7){
                click=0;
            }
            DrawTextEx(mainFont,TextFormat("HIGH SCORE %d",highscore),(Vector2){475,325},20,1,RED);
        }
        if(screen==3){
            mousepos=GetMousePosition();
            Rectangle back = {50, 630, 120, 50};
            DrawTexture(rules_te,50,10,WHITE);
            DrawTexture(pac_small_te,975,600,WHITE);
            Rectangle toggle = {750, 630, 180, 40};
            Rectangle on = {755, 635, 80, 30};
            Rectangle off = {845,635,80,30};
            DrawRectangleRoundedLines(toggle, 0.5, 2, 3, ssnblue);
            DrawTextEx(mainFont, "CHEAT", (Vector2){620,642},22,0,ssnblue);
            if(cheatEnabled){
                DrawRectangleRounded(on, 0.4, 1, ssnblue);
                DrawTextEx(mainFont,"ON",(Vector2){770,642},20,1,RAYWHITE);
                DrawTextEx(mainFont,"OFF",(Vector2){857,642},20,1,ssnblue);
            }
            else{
                DrawRectangleRounded(off, 0.4, 1, ssnblue);
                DrawTextEx(mainFont,"ON",(Vector2){770,642},20,1,ssnblue);
                DrawTextEx(mainFont,"OFF",(Vector2){857,642},20,1,RAYWHITE);
            }
            if (CheckCollisionPointRec(mousepos,off)){
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    cheatEnabled = 0;
            }
            if (CheckCollisionPointRec(mousepos,on)){
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    cheatEnabled = 1;
            }
            if (CheckCollisionPointRec(mousepos,back)){
                DrawRectangleLines(50,630,120,50,ssnblue);
                DrawTextEx(texts2, "Back", (Vector2){60,635},42,5,ssnblue);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    screen=2;
                }
            }
            else{
                DrawRectangle(50,630,120,50,ssnblue);
                DrawTextEx(texts2, "Back",(Vector2){60,635},42,5,BLACK);
            }
        }
        if(screen==4){
            mousepos=GetMousePosition();
            DrawTexture(info_te,0,0,WHITE);
            Rectangle back = {50, 630, 120, 50};
            DrawTexture(pac_small_te,975,600,WHITE);
            if (CheckCollisionPointRec(mousepos,back)){
                DrawRectangleLines(50,630,120,50,ssnblue);
                DrawTextEx(texts2, "Back", (Vector2){60,635},42,5,ssnblue);
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    screen=2;
                }
            }
            else{
                DrawRectangle(50,630,120,50,ssnblue);
                DrawTextEx(texts2, "Back",(Vector2){60,635},42,5,BLACK);
            }
           
        }
        if(screen==5){
            static int updateLife = 1, isLvl1 = 1;
            if(score==10000 && updateLife){
                life++;
                updateLife = 0;
            }
            if(level==1 && isLvl1){
                isLvl1 = 0;
                gamestart(level);
            }
            else if(pelletcount==360){  
                winCounter++;
                standby();
                if(winCounter>120){
                    level++;
                    winCounter = 1;
                    pelletcount = 0;
                    gamestart(level);
                }
            }
            if(level==4){
                win = 1;
            }
            else if (life==0 && gameCounter>70){
                win = -1;
            }
            if(win==0){
        //Check for cheat code
         if (isAnyAlphabetKeyPressed() && cheatEnabled)
            cheatcodes(GetKeyPressed());

        PacOldX = pacman.x, PacOldY = pacman.y;

        //Altering variables for intended motion
        gameCounter++;
        if(gameCounter<240){
            if(!startplayed){
                PlaySound(start_s);
                startplayed=1;
            }
        }
        else if (gameCounter>240){
             if(!IsSoundPlaying(n_siren) && win==0 && ghosts[0].sp==0 && ghosts[1].sp==0 && ghosts[2].sp==0 && ghosts[3].sp==0){
                PlaySound(n_siren);
                StopSound(s_siren);
            }
            else if(!IsSoundPlaying(s_siren) && win==0 && !IsSoundPlaying(n_siren)){
                PlaySound(s_siren);
                StopSound(n_siren);
            }
            //GETTING AND VALIDATING INPUT
            //Setting conditions to accept input
            if ((pacman.x<=-1)||(pacman.x>screenWidth+1))
                acceptInput=0;
            else if(IsKeyPressed(KEY_RIGHT)||IsKeyPressed(KEY_LEFT)||IsKeyPressed(KEY_UP)||IsKeyPressed(KEY_DOWN))
                acceptInput = 1;
            if(acceptInput){
             if (IsKeyPressed(KEY_RIGHT)){
                update=0;
                oldmoveX = 1;
                cheat = 0;
                    if (maze[(int)((pacman.y - pacman.radius) / cellHeight)][(int)((pacman.x + pacman.radius) / cellWidth) + 1] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight)][(int)((pacman.x + pacman.radius) / cellWidth) + 1] != 1){                
                        pacman.moveX = 1;
                        pacman.moveY = 0;
                        pacman.start = 30;
                        pacman.end = 330;}
                }
                else if (IsKeyPressed(KEY_LEFT)){
                    update=0;
                    oldmoveX = -1;
                    cheat = 0;
                    if(pacman.gridX!=27 || abs(pacman.gridY-10)>0){
                    if (maze[(int)((pacman.y - pacman.radius) / cellHeight)][(int)((pacman.x - pacman.radius) / cellWidth) - 1] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight)][(int)((pacman.x - pacman.radius) / cellWidth) - 1] != 1){
                        pacman.moveX = -1;
                        pacman.moveY = 0;
                        pacman.start = 210;
                        pacman.end = 510;}
                    }
                }
                else if (IsKeyPressed(KEY_DOWN)){
                    update=1;
                    oldmoveY = 1;
                    cheat = 0;
                    if (maze[(int)((pacman.y + pacman.radius) / cellHeight) + 1][(int)((pacman.x - pacman.radius) / cellWidth)] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight) + 1][(int)((pacman.x + pacman.radius) / cellWidth)] != 1){
                    pacman.moveX = 0;
                    pacman.moveY = 1;
                    pacman.start = 135;
                    pacman.end = 405;}
                }
                else if (IsKeyPressed(KEY_UP)){
                    update=1;
                    oldmoveY = -1;
                    cheat = 0;
                    if (maze[(int)((pacman.y - pacman.radius) / cellHeight) - 1][(int)((pacman.x - pacman.radius) / cellWidth)] != 1 && maze[(int)((pacman.y - pacman.radius) / cellHeight) - 1][(int)((pacman.x + pacman.radius) / cellWidth)] != 1){
                        pacman.moveX = 0;
                        pacman.moveY = -1;
                        pacman.start = 385;
                        pacman.end = 585;}
                    }
                    else if(update==0){
                        if(oldmoveX==1){
                            if (maze[(int)((pacman.y - pacman.radius) / cellHeight)][(int)((pacman.x + pacman.radius) / cellWidth) + 1] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight)][(int)((pacman.x + pacman.radius) / cellWidth) + 1] != 1){                
                                pacman.moveX = 1;
                                pacman.moveY = 0;
                                pacman.start = 30;
                                pacman.end = 330;}
                        }
                        else if(oldmoveX==-1){
                            if((int)(pacman.x/cellWidth)!=27 || abs(pacman.gridY-10)>0){
                            if (maze[(int)((pacman.y - pacman.radius) / cellHeight)][(int)((pacman.x - pacman.radius) / cellWidth) - 1] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight)][(int)((pacman.x - pacman.radius) / cellWidth) - 1] != 1){
                                pacman.moveX = -1;
                                pacman.moveY = 0;
                                pacman.start = 210;
                            pacman.end = 510;}
                            }
                        }
                    }
                    else if(update==1){
                        if(oldmoveY==1){
                            if (maze[(int)((pacman.y + pacman.radius) / cellHeight) + 1][(int)((pacman.x - pacman.radius ) / cellWidth)] != 1 && maze[(int)((pacman.y + pacman.radius) / cellHeight) + 1][(int)((pacman.x + pacman.radius) / cellWidth)] != 1){
                                pacman.moveX = 0;
                                pacman.moveY = 1;
                                pacman.start = 135;
                                pacman.end = 405;
                        }}
                        else if(oldmoveY==-1){
                            if (maze[(int)((pacman.y - pacman.radius) / cellHeight) - 1][(int)((pacman.x - pacman.radius) / cellWidth)] != 1 && maze[(int)((pacman.y - pacman.radius) / cellHeight) - 1][(int)((pacman.x + pacman.radius) / cellWidth)] != 1){
                                pacman.moveX = 0;
                                pacman.moveY = -1;
                                pacman.start = 320;
                                pacman.end = 585;
                            }
                        }
            }}
        }
        //Check wall collision
        int nextPosX = pacman.x + pacman.speed * pacman.moveX;
        int nextPosY = pacman.y + pacman.speed * pacman.moveY;
        float leftEdge = nextPosX - pacman.radius;
        float rightEdge = nextPosX + pacman.radius;
        float topEdge = pacman.y - pacman.radius;
        float bottomEdge = pacman.y + pacman.radius;
        bool collideX = false;
        if (pacman.moveX != 0){
            if (maze[(int)(topEdge / cellHeight)][(int)(leftEdge / cellWidth)] != 1 && maze[(int)(topEdge / cellHeight)][(int)(rightEdge / cellWidth)] != 1 && maze[(int)(bottomEdge / cellHeight)][(int)(leftEdge / cellWidth)] != 1 && maze[(int)(bottomEdge / cellHeight)][(int)(rightEdge / cellWidth)] != 1)
                collideX = true;
        }
        leftEdge = pacman.x - pacman.radius;
        rightEdge = pacman.x + pacman.radius;
        topEdge = nextPosY - pacman.radius;
        bottomEdge = nextPosY + pacman.radius;
        bool collideY = false;
        if (pacman.moveY != 0){
            if (maze[(int)(topEdge / cellHeight)][(int)(leftEdge / cellWidth)] != 1 && maze[(int)(topEdge / cellHeight)][(int)(rightEdge / cellWidth)] != 1 && maze[(int)(bottomEdge / cellHeight)][(int)(leftEdge / cellWidth)] != 1 && maze[(int)(bottomEdge / cellHeight)][(int)(rightEdge / cellWidth)] != 1)
                collideY = true;
        }
        if (collideX)
            pacman.x = nextPosX;
        if (collideY)
            pacman.y = nextPosY;
        else if(pacman.x<-pacman.radius)
            pacman.x=screenWidth+pacman.radius;
        else if(pacman.x>screenWidth+pacman.radius)
            pacman.x=-pacman.radius;
        else if(pacman.x>=cellWidth*40.4 && pacman.moveX!=0)
            pacman.x = pacman.x+pacman.moveX*pacman.speed;
        else if(pacman.x>=screenWidth+pacman.radius+1 && pacman.moveX==-1)
            pacman.x = pacman.x-pacman.speed;
        pacman.gridX = pacman.x / cellWidth;
        pacman.gridY = pacman.y / cellHeight;
        // Check collision with pellets and Updating Game Status
        
        if (maze[pacman.gridY][pacman.gridX] == 2){
            PlaySound(pellet);
            maze[pacman.gridY][pacman.gridX] = -2;
            score = score + 10;  
            pelletcount+=1;
        }
        else if (maze[pacman.gridY][pacman.gridX] == 3){
            PlaySound(pellet);
            StopSound(n_siren);
            maze[pacman.gridY][pacman.gridX] = -3;
            score = score + 50;    
            is_sp=1;
            pelletcount+=1;
        }
        if (is_sp==1){
            for(int i=0;i<4;i++){
                if((int)ghosts[i].x/cellWidth < 22 || (int)ghosts[i].x/cellWidth > 26 || (int)ghosts[i].y/cellHeight > 13 ||         (int)ghosts[i].y/cellHeight < 7){
                    ghosts[i].sp = 1;
                    ghosts[i].spcounter = 0;
                }
            }
            is_sp = 0;
        }
        for(int i=0; i<4;i++){
            ghosts[i].start.row=(int)(ghosts[i].y/cellHeight);ghosts[i].start.col = (int)(ghosts[i].x/cellWidth);
            ghosts[i].ghostFc++;
            //Checking for collision
            if((int)(ghosts[i].x/cellWidth)==pacman.gridX && (int)(ghosts[i].y/cellHeight)==pacman.gridY){
                if(ghosts[i].sp==1){
                   score+=200*pow(2,pacman.numGhost);
                   pacman.numGhost++;
                   ghosts[i].alive = 0;
                   ghosts[i].sp = 0;
                   ghosts[i].spcounter = 0;
                   ghosts[i].speed = 4*pacman.speed;
                   ghosts[i].end.row = ghosts[i].initial.row;
                   ghosts[i].end.col = ghosts[i].initial.col;
                }
                else if(ghosts[i].alive){
                    life-=1;
                    gameCounter=1;
                    acceptInput=0;
                    pacman.alive = 0;
                    initialise();
                }
            }
            if(ghosts[i].alive==0&&(int)(ghosts[i].x/cellWidth)==ghosts[i].initial.col&&(int)(ghosts[i].y/cellHeight)==ghosts[i].initial.row){
                ghosts[i].ghostFc = 420;
                ghosts[i].alive = 1;
            }
            if(ghosts[i].alive){
            if(ghosts[i].spcounter>sptime || ghosts[i].sp==0){
                ghosts[i].speed = speedFactor*pacman.speed;
                ghosts[i].sp = 0;
                ghosts[i].spcounter = 0;
                ghosts[i].atcorner = 0;
                ghosts[i].modetime++;
                //Algorithm for Blinky
                int blinkyX = (int)(ghosts[0].x/cellWidth); int blinkyY = (int)(ghosts[0].y/cellHeight);
                if(i==0){
                    static int blinkyCell = 0;
                    Cell blinkyPath[]={{5,33},{5,39},{3,39},{3,33},{1,33},{1,39}};
                    if(ghosts[0].modetime<660){
                        ghosts[0].end.row = blinkyPath[blinkyCell].row;
                        ghosts[0].end.col = blinkyPath[blinkyCell].col;
                    }
                    else{
                        ghosts[0].end.row = pacman.gridY;
                        ghosts[0].end.col = pacman.gridX;
                    }
                    if (blinkyCell==6)
                        blinkyCell = 0;
                    else if((int)(ghosts[0].y/cellHeight) == blinkyPath[blinkyCell].row && (int)(ghosts[0].x/cellWidth) == blinkyPath[blinkyCell].col)
                        blinkyCell += 1;
                }
                //Algorithm for Clyde
                else if(i==1){
                    int clydePacDist = abs((int)(ghosts[1].y/cellHeight)-(int)(pacman.y/cellHeight)) +
                                       abs((int)(ghosts[1].x/cellWidth)-(int)(pacman.x/cellWidth));
                    if (ghosts[i].modetime%120==0){
                    ghosts[i].modetime = 0;
                    if(clydePacDist<=8 || (pacman.x==PacOldX && pacman.y==PacOldY))
                        ghosts[i].mode = 1;
                    else
                        ghosts[i].mode = 2;
                    }
                    if(ghosts[i].mode==1){
                    static int clydeCell = 0;
                    Cell clydePath[]={{15,1},{15,13},{17,13},{17,19},{19,19},{19,1},{17,1},{17,19},{15,19},{15,13},{17,13},{17,1}};
                    ghosts[1].end.row = clydePath[clydeCell].row;
                    ghosts[1].end.col = clydePath[clydeCell].col;
                    if (clydeCell==11)
                        clydeCell = 0;
                    else if((int)(ghosts[1].y/cellHeight) == clydePath[clydeCell].row && (int)(ghosts[1].x/cellWidth) == clydePath[clydeCell].col)
                        clydeCell += 1;
                    }
                    else if (ghosts[i].mode==2){
                        ghosts[1].end.row = (int)(pacman.y/cellHeight);
                        ghosts[1].end.col = (int)(pacman.x/cellWidth);
                    }
                }
                //Algorithm for Pinky
                else if(i==2){
                    if(ghosts[i].modetime>1200){
                        ghosts[i].mode = 2;
                    }
                    if(ghosts[i].modetime>1920){
                        ghosts[i].modetime = 0;
                        ghosts[i].mode = 1;
                    }
                    if(ghosts[i].mode==1){
                    int pinkyRow = (int)pacman.y/cellHeight+pacman.moveY*4;
                    int pinkyCol = (int)pacman.x/cellWidth+pacman.moveX*4;
                    if((pacman.x == PacOldX && pacman.y == PacOldY) ||(maze[pinkyRow][pinkyCol]==1||(pinkyRow<0 || pinkyRow>20)||    (pinkyCol<0 || pinkyCol>40))||(pinkyRow==blinkyY&&pinkyCol==blinkyX)){
                        ghosts[2].end.row=(int)pacman.y/cellHeight;
                        ghosts[2].end.col=(int)pacman.x/cellWidth;
                    }
                    else{
                        ghosts[2].end.row = pinkyRow;
                        ghosts[2].end.col = pinkyCol;
                    }}
                    else{
                        static int pinkyCell = 0;
                        Cell pinkyPath[]={{1,13},{3,13},{3,3},{1,1},{5,1},{5,15},{1,15}};
                        ghosts[i].end.row = pinkyPath[pinkyCell].row;
                        ghosts[i].end.col = pinkyPath[pinkyCell].col;
                    if (pinkyCell==6)
                        pinkyCell = 0;
                    else if((int)(ghosts[i].y/cellHeight) == pinkyPath[pinkyCell].row && (int)(ghosts[i].x/cellWidth) == pinkyPath[pinkyCell].col)
                        pinkyCell += 1;
                    }
                }
                //Algorithm for inky
                else if(i==3){
                    Cell inkyCell;
                    static int inkyPathFound = 0, inkyFc = 0;
                    int pacBlinkydis = sqrt(pow(pacman.gridX-blinkyX,2)+pow(pacman.gridY-blinkyY,2));
                    if(pacman.gridY<=blinkyY && pacman.gridX<=blinkyX){
                        inkyCell = findgrid(blinkyX,40,blinkyY,20,2*pacBlinkydis,&inkyPathFound);
                    }
                    else if(pacman.gridY<=blinkyY && pacman.gridX>blinkyX){
                        inkyCell = findgrid(0,blinkyX,blinkyY,20,2*pacBlinkydis,&inkyPathFound);
                    }
                    else if(pacman.gridY>blinkyY && pacman.gridX<blinkyX){
                        inkyCell = findgrid(blinkyX,40,0,blinkyY,2*pacBlinkydis,&inkyPathFound);
                    }
                    else if(pacman.gridY>blinkyY && pacman.gridX>blinkyX){
                        inkyCell = findgrid(0,blinkyX,0,blinkyY,2*pacBlinkydis,&inkyPathFound);
                    }
                    if(inkyPathFound == 1){
                        ghosts[3].end.row = inkyCell.row;
                        ghosts[3].end.col = inkyCell.col;
                    }
                    else{
                        static int inkyIndex = 0;
                        Cell inkyPath[]={{15,21},{15,39},{19,39},{19,35},{15,35},{15,29},{17,29},{17,21}};
                        ghosts[i].end.row = inkyPath[inkyIndex].row;
                        ghosts[i].end.col = inkyPath[inkyIndex].col;
                        if (inkyIndex==7)
                            inkyIndex = 0;
                        else if((int)(ghosts[i].y/cellHeight) == inkyPath[inkyIndex].row && (int)(ghosts[i].x/cellWidth) == inkyPath[inkyIndex].col)
                            inkyIndex += 1;
                    }
                   
                }
            }
            if(ghosts[i].sp==1){
                ghosts[i].speed = 0.4*pacman.speed;
                ghosts[i].spcounter++;
                if(ghosts[i].moveX==0&&ghosts[i].moveY==0){
                    ghosts[i].atcorner = 1;
                    ghosts[i].end = checkwall();
                }
                else if(!ghosts[i].atcorner){
                if(pacman.x<=ghosts[i].x && pacman.y<=ghosts[i].y){
                    ghosts[i].end.row=19;
                    ghosts[i].end.col=39;
                }
                if(pacman.x<=ghosts[i].x && pacman.y>=ghosts[i].y){
                    ghosts[i].end.row=1;
                    ghosts[i].end.col=39;
                }
                if(pacman.x>=ghosts[i].x && pacman.y<=ghosts[i].y){
                    ghosts[i].end.row=19 ;
                    ghosts[i].end.col=1;
                }
                if(pacman.x>=ghosts[i].x && pacman.y>=ghosts[i].y){
                    ghosts[i].end.row=1;
                    ghosts[i].end.col=1;
                }}
            }}
            //Find shortest path using BFS
            ghosts[i].distance = shortestDistanceWithPath(maze,ghosts[i].end,ghosts[i].start,ghosts[i].path);
            if(ghosts[i].distance>0){
                ghosts[i].moveX = ghosts[i].path[ghosts[i].distance-1].col-ghosts[i].path[ghosts[i].distance].col;
                ghosts[i].moveY = ghosts[i].path[ghosts[i].distance-1].row-ghosts[i].path[ghosts[i].distance].row;
            }
            else{
                ghosts[i].moveX = 0;ghosts[i].moveY = 0;
            }
            if(ghosts[i].ghostFc>ghosts[i].start_time && pelletcount<360){
            ghosts[i].x += ghosts[i].speed*ghosts[i].moveX;
            ghosts[i].y += ghosts[i].speed*ghosts[i].moveY;
            }
        }
        if(ghosts[0].sp==0&&ghosts[1].sp==0&&ghosts[2].sp==0&&ghosts[3].sp==0)
            pacman.numGhost = 0;
        ClearBackground(BLACK);
        //Displaying Status to the Player
        DrawTextEx(mainFont,TextFormat("%d",score),(Vector2){15,screenHeight*0.88},18,0,WHITE);
        DrawTextEx(mainFont,TextFormat("Level %d",level),(Vector2){15,screenHeight*0.92},18,0,GRAY);
        DrawTextEx(mainFont,TextFormat("Highscore %d",highscore),(Vector2){15,screenHeight*0.96},18,1,GRAY);
        for(int i=1;i<=life;i++)
            DrawTexture(heart_te,screenWidth/1.6+2*(i+1)*cellWidth,screenHeight*0.88,WHITE);
        //Drawing Maze Image
        DrawTexture(maze_te,0,0,WHITE);
        if(winCounter<120 && pelletcount==360){
            StopSound(n_siren);
            StopSound(s_siren);
            if(x%2==0)
                DrawTexture(mazew_te,0,0,WHITE);
        }
        //Drawing Pellets
        if (x % 2 == 0){
            DrawMaze(maze, 21, 41, cellWidth, cellHeight, 0);
            mainCounter++;
        }
        else
            DrawMaze(maze, 21, 41, cellWidth, cellHeight, 1);
        if(gameCounter<240&&life==3&&level==1&&pelletcount==0)
             DrawTextEx(mainFont,"READY!",(Vector2){685,457},16,1,YELLOW);
        //Drawing Pacman and Ghosts
        if(pacman.alive){
        if((pacman.moveX==0&&pacman.moveY==0)||(PacOldX==pacman.x&&PacOldY==pacman.y))
            DrawPacman((Vector2){pacman.x, pacman.y}, cellWidth*0.35f, pacman.start, pacman.end, pacmanColor);
        else if (x%2==0)
            DrawPacman((Vector2){pacman.x, pacman.y}, cellWidth * 0.35f, 0, 360, pacmanColor);
        else
            DrawPacman((Vector2){pacman.x, pacman.y}, cellWidth*0.35f, pacman.start, pacman.end, pacmanColor);
        for(int i=0;i<4;i++){
            static Texture2D ghost_te;
            if(ghosts[i].alive==0){
               if(ghosts[i].moveX==1)
                   ghost_te = geyesR_te;
               else if(ghosts[i].moveX==-1)
                   ghost_te = geyesL_te;
               else if(ghosts[i].moveY==1)
                   ghost_te = geyesD_te;
               else if(ghosts[i].moveY==-1)
                   ghost_te = geyesU_te;
            }
            else if(ghosts[i].sp==1){
                if(sptime-ghosts[i].spcounter>150 && x%2==0)
                    ghost_te = gfrightBlue_te1;
                else if(sptime-ghosts[i].spcounter<150 && x%2==0)
                    ghost_te = gfrightWhite_te1;
                else
                    ghost_te = gfrightBlue_te2;
            }
            else{
                if(ghosts[i].moveX==-1 && x%2==0)
                    ghost_te = ghosts[i].gtexture[0];
                else if(ghosts[i].moveX==-1)
                    ghost_te = ghosts[i].gtexture[1];
                else if(ghosts[i].moveX==1 && x%2==0)
                    ghost_te = ghosts[i].gtexture[2];
                else if(ghosts[i].moveX==1)
                    ghost_te = ghosts[i].gtexture[3];
                else if(ghosts[i].moveY==-1 && x%2==0)
                    ghost_te = ghosts[i].gtexture[4];
                else if(ghosts[i].moveY==-1)
                    ghost_te = ghosts[i].gtexture[5];
                else if(ghosts[i].moveY==1 && x%2==0)
                    ghost_te = ghosts[i].gtexture[6];
                else
                    ghost_te = ghosts[i].gtexture[7];
            }
           DrawTexture(ghost_te,ghosts[i].path[ghosts[i].distance].col * cellWidth, ghosts[i].path[ghosts[i].distance].row * cellHeight, WHITE);
        }
        }
        else{
            static int animFc = -1;
            static Texture2D pacim;
            animFc++;
            if(!IsSoundPlaying(pacdead_s)){
                StopSound(s_siren);
                StopSound(n_siren);
                PlaySound(pacdead_s);
            }
            if(animFc%4==0&&animFc<41)
                pacim = pacman.anim_te[animFc/4];
            DrawTexture(pacim,pacman.x-cellWidth/2,pacman.y-cellHeight/2,WHITE);
            if(animFc>80){
                gameCounter = 1;
                initialise();
                animFc = -1;
                pacman.x = gameWidth/2;
                pacman.y = gameHeight/2;
                pacman.alive = 1;
            }
        }
        //Altering mainCounter
        if (mainCounter < 20)
            mainCounter++;
        else{
            mainCounter = 0;
            x++;
        }
    }
   
        else if(win==-1){
            if(messCounter<120){
                DrawTexture(maze_te,0,0,WHITE);
                DrawTextEx(mainFont,"YOU LOSE!",(Vector2){675,457},16,1,RED);
                messCounter+=1;
            }
            else{
                messCounter = 1;
                screen = 6;
                isLvl1 = 1;
            }
        }
        else if(win == 1){
            if(messCounter<120){
                DrawTexture(maze_te,0,0,WHITE);
                DrawTextEx(mainFont,"YOU WIN!",(Vector2){678,457},16,1,YELLOW);
                messCounter+=1;
                
            }
            else{
                messCounter = 1;
                screen = 6;
                isLvl1 = 1;
            }
        }
        if(score>highscore){
            highscore = score;
            scoreWriteFile = fopen("C:/Users/VISHAL/Desktop/My Code Files/C/Team2_PACMAN_CSEC/Codes/highscore.dat","w");
            fprintf(scoreWriteFile,"%d",highscore);
            fclose(scoreWriteFile);
        }
    }
    else if(screen == 6){
        retCounter-=1;
        DrawTexture(gameover_te,0,0,WHITE);
        DrawTextEx(mainFont,TextFormat("SCORE %d",score),(Vector2){500,200},20,1,WHITE);
        DrawTextEx(mainFont,TextFormat("Home Screen in %d seconds",retCounter/60+1),(Vector2){330,600},22,1,WHITE);
        if(retCounter==0){
            win = 0; score = 0; level = 1;pacman.alive=1;pelletcount=0;
            screen = 2;
            retCounter = 300;
        }
    }
       
        EndDrawing();
    }
    return 0;
}
