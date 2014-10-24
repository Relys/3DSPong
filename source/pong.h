#ifndef PONG_H
#define PONG_H

void p1Move();
void p2Move();
void moveBall();
void checkCollision();
void checkWin();
void render();
void reset();
void wait(int t);
void win(int player);
void initial();
void p1render(int up);
void p2render(int up);
int collisionBox(int box1x,int box1y,int width1,int height1,int box2x,int box2y,int width2,int height2);
void game();
void switchBuffer();

#endif
