#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

// we can easily change these values during testing and if we want to customize the program
#define MAP_SIZE 31
#define CELL_SIZE 20
#define ROBOT_DELAY 2
#define DRAW_MAP_DELAY 5

enum directions {Right, Up, Left, Down};
enum cells {Path, Wall, EndPos, StartPos, CanBuild};

// MAZE GENERATION

// int row, int col - the current cell the maze generation algorithm is on
// int arr[MAP_SIZE][MAP_SIZE] - array used to store data of the cells in the maze
typedef struct _maze{
  int row;
  int col;
  enum cells arr[MAP_SIZE][MAP_SIZE];
} maze;

maze *initMaze(){
  maze *m = malloc(sizeof(maze));
  for (int i = 0; i < MAP_SIZE; i++){
    for (int j = 0; j< MAP_SIZE; j++){
      // if statement - set the four borders as walls first
      if (i % (MAP_SIZE - 1) == 0 || j % (MAP_SIZE - 1) == 0){
        m->arr[i][j] = Wall;
      }
      else if (i % 2 == 0 && j % 2 == 0){
        m->arr[i][j] = Wall;
      }
      else{
        m->arr[i][j] = CanBuild;
      }
    }
  }
  m->arr[1][0] = StartPos;
  m->arr[MAP_SIZE - 2][MAP_SIZE - 1] = EndPos;
  // we will start building our paths from the cell to the left of the exit
  m->row = MAP_SIZE - 2;
  m->col = MAP_SIZE - 2;
  m->arr[m->col][m->row] = Path;
  return m;
}

void drawMaze(maze *m){
  background();
  for (int i=0; i<MAP_SIZE; i++){
    for (int j=0; j<MAP_SIZE; j++){
      if (m->arr[i][j] == Path){
        setColour(white);
      }
      else if (m->arr[i][j] == Wall){
        setColour(black); 
      }
      else if (m->arr[i][j] == EndPos){
        setColour(gray);
      }
      else if (m->arr[i][j] == StartPos){
        setColour(blue);
      }
      else if (m->arr[i][j] == CanBuild){
        setColour(orange);
      }
      fillRect(j*CELL_SIZE ,i*CELL_SIZE ,CELL_SIZE ,CELL_SIZE);
    }
  }
}

// seekPath - the recursive backtracking function
void seekPath(maze *m);
void seekRight(maze *m){
  if (m->arr[m->row][m->col + 2] == CanBuild){
    m->arr[m->row][m->col + 1] = Path;
    m->arr[m->row][m->col + 2] = Path;
    setColour(white);
    fillRect((m->col + 1)*CELL_SIZE ,m->row*CELL_SIZE ,2*CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
    // the recursion
    m->col += 2;
    seekPath(m);
    // the backtracking
    m->col -= 2;
  }
  else if (m->arr[m->row][m->col + 2] == Path && m->arr[m->row][m->col + 1] == CanBuild){
    m->arr[m->row][m->col + 1] = Wall;
    setColour(black);
    fillRect((m->col + 1)*CELL_SIZE ,m->row*CELL_SIZE ,CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
  }
}

// seekUp, seekLeft, seekDown - same logic as seekRight is applied to the remaining 3 directions
void seekUp(maze *m){
  if (m->arr[m->row - 2][m->col] == CanBuild){
    m->arr[m->row - 1][m->col] = Path;
    m->arr[m->row - 2][m->col] = Path;
    setColour(white);
    fillRect(m->col*CELL_SIZE,(m->row - 2)*CELL_SIZE,CELL_SIZE,2*CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
    m->row -= 2;
    seekPath(m);
    m->row += 2;
  }
  else if (m->arr[m->row - 2][m->col] == Path && m->arr[m->row - 1][m->col] == CanBuild){
    m->arr[m->row - 1][m->col] = Wall;
    setColour(black);
    fillRect(m->col*CELL_SIZE ,(m->row - 1)*CELL_SIZE ,CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
  }
}

void seekLeft(maze *m){
  if (m->arr[m->row][m->col - 2] == CanBuild){
    m->arr[m->row][m->col - 1] = Path;
    m->arr[m->row][m->col - 2] = Path;
    setColour(white);
    fillRect((m->col - 2)*CELL_SIZE ,m->row*CELL_SIZE ,2*CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
    m->col -= 2;
    seekPath(m);
    m->col += 2;
  }
  else if (m->arr[m->row][m->col - 2] == Path && m->arr[m->row][m->col - 1] == CanBuild){
    m->arr[m->row][m->col - 1] = Wall;
    setColour(black);
    fillRect((m->col - 1)*CELL_SIZE ,m->row*CELL_SIZE ,CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
  }
}

void seekDown(maze *m){
  if (m->arr[m->row + 2][m->col] == CanBuild){
    m->arr[m->row + 1][m->col] = Path;
    m->arr[m->row + 2][m->col] = Path;
    setColour(white);
    fillRect(m->col*CELL_SIZE ,(m->row + 1)*CELL_SIZE ,CELL_SIZE ,2*CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
    m->row += 2;
    seekPath(m);
    m->row -= 2;
  }
  else if (m->arr[m->row + 2][m->col] == Path && m->arr[m->row + 1][m->col] == CanBuild){
    m->arr[m->row + 1][m->col] = Wall;
    setColour(black);
    fillRect(m->col*CELL_SIZE ,(m->row + 1)*CELL_SIZE ,CELL_SIZE ,CELL_SIZE);
    sleep(DRAW_MAP_DELAY);
  }
}

void seekPath(maze *m){
  // the function exits if the 4 adjacent cells can't be built on
  while (m->arr[m->row + 1][m->col] == CanBuild || m->arr[m->row - 1][m->col] == CanBuild ||
      m->arr[m->row][m->col + 1] == CanBuild || m->arr[m->row][m->col - 1] == CanBuild){
    int dir = rand() % 4;
    if (dir == Right){
      seekRight(m);
    }
    else if (dir == Up){
      seekUp(m);
    }
    else if (dir == Left){
      seekLeft(m);
    }
    else if (dir == Down){
      seekDown(m);
    }
  }
}

void generateMaze(maze *m){
  drawMaze(m);
  seekPath(m);
  sleep(500);
}

// ROBOT PATHFINDING

// int x, int y - position of the robot in the MAP array (for calculation only)
// int dir - direction the robot is facing (for calculation and graphics)
// float realX, realY - actual coordinates of the robot displayed on drawapp-2.0.jar (for graphics only)
typedef struct _robot{
  int x;
  int y;
  enum directions dir;
  int realX;
  int realY;
} robot;

robot *initBot(){
  robot *r = malloc(sizeof(robot));
  r->x = 0;
  r->y = 1;
  r->dir = Left;
  r->realX = r->x*CELL_SIZE;
  r->realY = r->y*CELL_SIZE;
  return r;
}

// int arr[3] - array length will always be 3 (3 points in a triangle)
void putCoordsToArr(int arr[3], int a, int b, int c){
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
}

void drawBot(robot *r){
  setColour(green);
  int xCoords[3];
  int yCoords[3];
  if (r->dir == Right){
    putCoordsToArr(xCoords, r->realX, r->realX, r->realX+CELL_SIZE);
    putCoordsToArr(yCoords, r->realY, r->realY+CELL_SIZE, r->realY+CELL_SIZE/2);
  }
  else if (r->dir == Up){
    putCoordsToArr(xCoords, r->realX, r->realX+CELL_SIZE, r->realX+CELL_SIZE/2);
    putCoordsToArr(yCoords, r->realY+CELL_SIZE, r->realY+CELL_SIZE, r->realY);
  }
  else if (r->dir == Left){
    putCoordsToArr(xCoords, r->realX+CELL_SIZE, r->realX+CELL_SIZE, r->realX);
    putCoordsToArr(yCoords, r->realY, r->realY+CELL_SIZE, r->realY+CELL_SIZE/2);
  }
  else if (r->dir == Down){
    putCoordsToArr(xCoords, r->realX, r->realX+CELL_SIZE, r->realX+CELL_SIZE/2);
    putCoordsToArr(yCoords, r->realY, r->realY, r->realY+CELL_SIZE);
  }
  fillPolygon(3, xCoords, yCoords);
}

void update(robot *r){
  clear();
  drawBot(r);
  sleep(ROBOT_DELAY);
}

void turnLeft(robot *r){
  r->dir = (r->dir+1)%4;
  update(r);
}

void turnRight(robot *r){
  r->dir = (r->dir+3)%4;
  update(r);
}

// int axis - move along which axis? 0: x-axis, 1: y-axis
// int sgn - move along the positive or negative direction of the axis
void smoothForward(robot *r, int axis, int sgn){
  for (int i = 0; i < CELL_SIZE; i++){
    if (axis == 0){
      r->realX += sgn;
    }
    else{
      r->realY += sgn;
    }
    update(r);
  }
}

// moveForward - only changes the position of the robot for calculations, the graphics are done by smoothForward
void moveForward(robot *r){
  if (r->dir == Right){
    r->x++;
    smoothForward(r, 0, 1);
  }
  else if (r->dir == Up){
    r->y--;
    smoothForward(r, 1, -1);
  }
  else if (r->dir == Left){
    r->x--;
    smoothForward(r, 0, -1);
  }
  else if (r->dir == Down){
    r->y++;
    smoothForward(r, 1, 1);
  }
}

// int type - 0: check forward cell, 1: check left cell relative to robot
// return statements - only even numbered cells (0 and 2) in m->arr can be stepped onto 
int canMove(robot *r, maze *m, int type){
  if (r->dir == Right && type == 0 || r->dir == Down && type == 1){
    return m->arr[r->y][r->x+1]%2 == 0;
  }
  else if (r->dir == Up && type == 0 || r->dir == Right && type == 1){
    return m->arr[r->y-1][r->x]%2 == 0;
  }
  else if (r->dir == Left && type == 0 || r->dir == Up && type == 1){
    return m->arr[r->y][r->x-1]%2 == 0;
  }
  else if (r->dir == Down && type == 0 || r->dir == Left && type == 1){
    return m->arr[r->y+1][r->x]%2 == 0;
  }
  return 0;
}

int atMarker(robot *r, maze *m){
  return m->arr[r->y][r->x] == EndPos;
}

// drawBotZoomed, danceBot - celebratory dance for completing the maze (just for fun)
void drawBotZoomed(int x, int y, int i){
  int xCoords[3];
  int yCoords[3];
  clear();
  putCoordsToArr(xCoords, x-i, x-i, x+CELL_SIZE+i);
  putCoordsToArr(yCoords, y-i, y+CELL_SIZE+i, y+CELL_SIZE/2);
  fillPolygon(3, xCoords, yCoords);
  sleep(25);
}

void danceBot(robot *r){
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < CELL_SIZE/2; j++){
      drawBotZoomed(r->realX, r->realY, j);
    }
    for (int j = CELL_SIZE/2; j > 0; j--){
      drawBotZoomed(r->realX, r->realY, j);
    }
  }
}

void runBot(robot *r, maze *m){
  foreground();
  while (!atMarker(r, m)){
    if (canMove(r, m, 1)){
      turnLeft(r);
      moveForward(r);
    }
    else if (canMove(r, m, 0)){
      moveForward(r);
    }
    else{
      turnRight(r);
    }
  }
  danceBot(r);
}

int main(){
  if (MAP_SIZE < 3 || MAP_SIZE > 1499 || MAP_SIZE % 2 != 1){
    printf("Invalid map size, please put a odd number between 3 and 1499 inclusive.\n");
  }
  else{
    setWindowSize(MAP_SIZE*CELL_SIZE,MAP_SIZE*CELL_SIZE);
    srand(time(NULL));
    while(1){
      maze *myMap = initMaze();
      generateMaze(myMap);
      robot *myBot = initBot();
      runBot(myBot, myMap);
      free(myMap);
      free(myBot);
      clear();
    }
  }
  return 0;
}