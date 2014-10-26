#include "pong.h"
#include "draw.h"
#include <3ds/types.h>
#include <3ds/services/hid.h>
#include <3ds/gfx.h>


//Flag for restarting the entire game.
int restart=1;

//Global variable for the location of the framebuffers. They switch back and forth from buffer1 and buffer2.
u8* screenTopLeft =0;
u8* screenTopRight =0;
u8* screenBottom = 0;

//Globals for ball location and movement dx/dy
int ballX = 200;
int ballX_old = 200;
int ballY = 120;
int ballY_old = 120;

//Globals for player1 location and movement dx/dy
int p1X = 40;
int p1X_old = 40;
int p1Y = 150;
int p1Y_old = 150;

//Globals for player2 location and movement dx/dy
int p2X = 340;
int p2X_old = 340;
int p2Y = 150;
int p2Y_old = 150;

//Game engine globals
int master3Doffset = 2;
int direction = 1;
u32 button = 0;
int colorR=0xFF;
int colorG=0xFF;
int colorB=0xFF;
int count = 0;

int score1 = 0;
int score2 = 0;

int flag = 0;

//Text
char* title = "PONG 3DS";
char* p1win = "Player 1 wins!";
char* p2win = "Player 2 wins!";

//Sets a collsion box.
int collisionBox(int box1x,int box1y,int width1,int height1,int box2x,int box2y,int width2,int height2){
	int corner1x=box1x+width1;
	int corner1y=box1y-height1;
	int corner2x=box2x+width2;
	int corner2y=box2y-height2;


	if( box1x==corner2x || corner1x==box2x ){
		if((box1y >= (corner2y-2) && box1y <= (box2y+2)) || (corner1y >= (corner2y-2) && corner1y <= (box2y+2)))return 1;

		return 0;

	}
	if( abs(box1y-corner2y)<5 || abs(corner1y-box2y)<5 ){
		if((box1x < corner2x && box1x > box2x) || (corner1x < corner2x && corner1x > box2x))return 2;
	}



	return 0;
}

//Updates player1 location
void p1Move() {

	if ((button & KEY_UP) && p1Y > 60) {
		p1render(1);
		p1Y--;

	}
	if ((button & KEY_DOWN) && p1Y < 238) {

		p1Y++;
		p1render(0);
	}

};

//Updates player2 location
void p2Move() {


	if ((button & KEY_X) && p2Y > 60) {
		p2render(1);
		p2Y--;


	}
	if ((button & KEY_B) && p2Y < 238) {

		p2Y++;
		p2render(0);
	}

};

//Physics for ball
void moveBall() {
	int jump = 1;

	if (flag)jump = 4;

	switch (direction) {
	case 1:
		ballX -= jump; //down left
		ballY += jump;
		break;
	case 3:
		ballX += jump; //down right
		ballY += jump;
		break;
	case 2:
		ballX += jump; //up right
		ballY -= jump;
		break;
	case 0:
		ballX -= jump; //up left
		ballY -= jump;
		break;
	default:
		break;

	}

	flag = 0;
};

//Checks for collsions
void checkCollision() {
	int old;
	old = direction;




	if (ballY < 18) {
		direction ^= 1;
	}
	if (ballY > 238) {
		direction ^= 1;
	}
	if (ballX > 59 && ballX < 333) {
		if (collisionBox(ballX, ballY, 8, 8, p1X, p1Y, 20, 60) == 1) {
			direction ^= 2;
			goto skip;
		}

		if (collisionBox(ballX, ballY, 8, 8, p2X, p2Y, 20, 60) == 1) {
			direction ^= 2;
			goto skip;
		}
	}

	if (collisionBox(ballX, ballY, 8, 8, p1X, p1Y, 20, 60) == 2) {
		direction ^= 1;
		if ((direction ^ old) != 0)flag = 1;


	}
	if (collisionBox(ballX, ballY, 8, 8, p2X, p2Y, 20, 60) == 2) {
		direction ^= 1;
		if ((direction ^ old) != 0)flag = 1;
	}

skip:

	if (ballX < 6) {

		score2++;
		checkWin();
	}

	if (ballX > 386) {
		score1++;
		checkWin();
	}

};

//Checks to see if we have meet the requirements for entering the win state
void checkWin() {
	if (score1 > 9) {
		score1 = 0;
		score2 = 0;
		win(1);
	}
	if (score2 > 9) {
		score1 = 0;
		score2 = 0;
		win(2);
	}

	reset();
	gfxFlushBuffers();
	gfxSwapBuffers();
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	reset();
	gfxFlushBuffers();
	gfxSwapBuffers();
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
};

//Render function for ball and updates player and balls new locations.
void render() {


	//drawBox(ballX_old, ballY_old, 8, 8, -master3Doffset,color, screenTopLeft);
	drawFillRect(ballX_old-master3Doffset, ballY_old-8, ballX_old-master3Doffset+8,ballY_old,0,0,0, screenTopLeft);
	//drawBox(ballX_old, ballY_old, 8, 8, master3Doffset,color, screenTopRight);
	drawFillRect(ballX_old+master3Doffset, ballY_old-8, ballX_old+master3Doffset+8,ballY_old,0,0,0, screenTopRight);
	//drawBox(ballX, ballY, 8, 8, -master3Doffset,color, screenTopLeft);
	drawFillRect(ballX-master3Doffset, ballY-8, ballX-master3Doffset+8,ballY,colorR,colorG,colorB, screenTopLeft);
	//drawBox(ballX, ballY, 8, 8, master3Doffset,color, screenTopRight);
	drawFillRect(ballX+master3Doffset, ballY-8, ballX+master3Doffset+8,ballY,colorR,colorG,colorB, screenTopRight);

	gfxFlushBuffers();
	gfxSwapBuffers();
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	//drawBox(ballX_old, ballY_old, 8, 8, -master3Doffset,color, screenTopLeft);
	drawFillRect(ballX_old-master3Doffset, ballY_old-8, ballX_old-master3Doffset+8,ballY_old,0,0,0, screenTopLeft);
	//drawBox(ballX_old, ballY_old, 8, 8, master3Doffset,color, screenTopRight);
	drawFillRect(ballX_old+master3Doffset, ballY_old-8, ballX_old+master3Doffset+8,ballY_old,0,0,0, screenTopRight);
	//drawBox(ballX, ballY, 8, 8, -master3Doffset,color, screenTopLeft);
	drawFillRect(ballX-master3Doffset, ballY-8, ballX-master3Doffset+8,ballY,colorR,colorG,colorB, screenTopLeft);
	//drawBox(ballX, ballY, 8, 8, master3Doffset,color, screenTopRight);
	drawFillRect(ballX+master3Doffset, ballY-8, ballX+master3Doffset+8,ballY,colorR,colorG,colorB, screenTopRight);
	gfxFlushBuffers();
	gfxSwapBuffers();
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);

	p1X_old = p1X;
	p1Y_old = p1Y;
	p2X_old = p2X;
	p2Y_old = p2Y;
	ballX_old = ballX;
	ballY_old = ballY;
	wait(200);
};

//Reset the game
void reset() {

	ballX = 200;
	ballY = 120;
	p1X = 40;
	p1Y = 150;
	p2X = 340;
	p2Y = 150;
	p1X_old = p1X;
	p1Y_old = p1Y;
	p2X_old = p2X;
	p2Y_old = p2Y;
	ballX_old = ballX;
	ballY_old = ballY;
	direction = (count & 3);
	button = 0;
	wait(2000);
	clearScreen(screenTopLeft,GFX_TOP);

	clearScreen(screenTopRight,GFX_TOP);

	clearScreen(screenBottom,GFX_BOTTOM);
	drawString(title, 125, 120, 255, 255, 255, screenBottom,GFX_BOTTOM);

	drawChar((score1 + 48), 100 + master3Doffset, 2, 255, 255, 255, screenTopLeft);
	drawChar((score2 + 48), 300 + master3Doffset, 2, 255, 255, 255, screenTopLeft);
	drawChar((score1 + 48), 100 - master3Doffset, 2, 255, 255, 255, screenTopRight);
	drawChar((score2 + 48), 300 - master3Doffset, 2, 255, 255, 255, screenTopRight);
	wait(2000);
	//drawBox(198, 239, 4, 240, 0,color, screenTopLeft);
	drawFillRect(198, 239, 198+4, 239+240,colorR,colorG,colorB, screenTopLeft);
	//drawBox(198, 239, 4, 240, 0,color, screenTopRight);
	drawFillRect(198, 239, 198+4, 239+240,colorR,colorG,colorB, screenTopRight);
	initial();
	wait(2000);
};

//Sleeping function to kill time. Should probably update this to use svc tick.
void wait(int t) {
	int i = 0;
	int j = 0;
	int z = 0;

	for (i = 0; i < t; i++) {
		for (j = 0; j < t; j++) {
			z = i / 33;
		}
	}

};


//Draws the win screen.
void win(int player) {
	if (player == 1) {
		drawString(p1win, 70, 50, 255, 255, 255, screenTopLeft,GFX_TOP);
	}
	if (player == 2) {
		drawString(p2win, 220, 50, 255, 255, 255, screenTopRight,GFX_TOP);
	}
	wait(3000);
};

//Draws the inital player paddles and ball.
void initial() {
	//drawBox(p1X_old, p1Y_old, 20, 60, -master3Doffset,color, screenTopLeft);
	drawFillRect(p1X_old-master3Doffset, p1Y_old-60, p1X_old-master3Doffset+20, p1Y_old,colorR,colorG,colorB, screenTopLeft);
	//drawBox(p2X_old, p2Y_old, 20, 60, -master3Doffset,color, screenTopLeft);
	drawFillRect(p2X_old-master3Doffset, p2Y_old-60, p2X_old-master3Doffset+20, p2Y_old,colorR,colorG,colorB, screenTopLeft);
	//drawBox(ballX_old, ballY_old, 8, 8, -master3Doffset,color, screenTopLeft);
	drawFillRect(ballX_old-master3Doffset, ballY_old-8, ballX_old-master3Doffset+8,ballY_old,colorR,colorG,colorB, screenTopLeft);
	//drawBox(p1X_old, p1Y_old, 20, 60, master3Doffset,color, screenTopRight);
	drawFillRect(p1X_old+master3Doffset, p1Y_old-60, p1X_old+master3Doffset+20, p1Y_old,colorR,colorG,colorB, screenTopRight);
	//drawBox(p2X_old, p2Y_old, 20, 60, master3Doffset,color, screenTopRight);
	drawFillRect(p2X_old+master3Doffset, p2Y_old-60, p2X_old+master3Doffset+20, p2Y_old,colorR,colorG,colorB, screenTopRight);
	//drawBox(ballX_old, ballY_old, 8, 8, master3Doffset,color, screenTopRight);
	drawFillRect(ballX_old+master3Doffset, ballY_old-8, ballX_old+master3Doffset+8,ballY_old,colorR,colorG,colorB, screenTopRight);
};

//Draws player2's paddle
void p1render(int up) {
	if(up==1)
	{
		//paint_line(40 - master3Doffset, p1Y, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y, 40 - master3Doffset+20, p1Y, 0,0,0,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y, 40 + master3Doffset+20, p1Y, 0,0,0,screenTopRight);
		//paint_line(40 - master3Doffset, p1Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y-60, 40 - master3Doffset+20, p1Y-60, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y - 60, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y-60, 40 + master3Doffset+20, p1Y-60, 0xFF,0xFF,0xFF,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		//paint_line(40 - master3Doffset, p1Y, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y, 40 - master3Doffset+20, p1Y, 0,0,0,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y, 40 + master3Doffset+20, p1Y, 0,0,0,screenTopRight);
		//paint_line(40 - master3Doffset, p1Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y-60, 40 - master3Doffset+20, p1Y-60, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y - 60, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y-60, 40 + master3Doffset+20, p1Y-60, 0xFF,0xFF,0xFF,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	}
	else
	{
		//paint_line(40 - master3Doffset, p1Y, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y, 40 - master3Doffset+20, p1Y, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y, 40 + master3Doffset+20, p1Y, 0xFF,0xFF,0xFF,screenTopRight);
		//paint_line(40 - master3Doffset, p1Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y-60, 40 - master3Doffset+20, p1Y-60, 0,0,0,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y - 60, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y-60, 40 + master3Doffset+20, p1Y-60, 0,0,0,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		//paint_line(40 - master3Doffset, p1Y, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y, 40 - master3Doffset+20, p1Y, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y, 40 + master3Doffset+20, p1Y, 0xFF,0xFF,0xFF,screenTopRight);
		//paint_line(40 - master3Doffset, p1Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(40 - master3Doffset, p1Y-60, 40 - master3Doffset+20, p1Y-60, 0,0,0,screenTopLeft);
		//paint_line(40 + master3Doffset, p1Y - 60, 20, 0, 42, screenTopRight);
		drawLine(40 + master3Doffset, p1Y-60, 40 + master3Doffset+20, p1Y-60, 0,0,0,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	}

}

//Draws player2s paddle
void p2render(int up) {

	if(up==1)
	{
		//paint_line(340 - master3Doffset, p2Y, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y, 340 - master3Doffset+20, p2Y, 0,0,0,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y, 340 + master3Doffset+20, p2Y, 0,0,0,screenTopRight);
		//paint_line(340 - master3Doffset, p2Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y-60, 340 - master3Doffset+20, p2Y-60, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y - 60, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y-60, 340 + master3Doffset+20, p2Y-60, 0xFF,0xFF,0xFF,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		//paint_line(340 - master3Doffset, p2Y, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y, 340 - master3Doffset+20, p2Y, 0,0,0,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y, 340 + master3Doffset+20, p2Y, 0,0,0,screenTopRight);
		//paint_line(340 - master3Doffset, p2Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y-60, 340 - master3Doffset+20, p2Y-60, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y - 60, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y-60, 340 + master3Doffset+20, p2Y-60, 0xFF,0xFF,0xFF,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	}
	else
	{
		//paint_line(340 - master3Doffset, p2Y, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y, 340 - master3Doffset+20, p2Y, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y, 340 + master3Doffset+20, p2Y, 0xFF,0xFF,0xFF,screenTopRight);
		//paint_line(340 - master3Doffset, p2Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y-60, 340 - master3Doffset+20, p2Y-60, 0,0,0,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y - 60, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y-60, 340 + master3Doffset+20, p2Y-60, 0,0,0,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		//paint_line(340 - master3Doffset, p2Y, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y, 340 - master3Doffset+20, p2Y, 0xFF,0xFF,0xFF,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y, 340 + master3Doffset+20, p2Y, 0xFF,0xFF,0xFF,screenTopRight);
		//paint_line(340 - master3Doffset, p2Y - 60, 20, 0, 42, screenTopLeft);
		drawLine(340 - master3Doffset, p2Y-60, 340 - master3Doffset+20, p2Y-60, 0,0,0,screenTopLeft);
		//paint_line(340 + master3Doffset, p2Y - 60, 20, 0, 42, screenTopRight);
		drawLine(340 + master3Doffset, p2Y-60, 340 + master3Doffset+20, p2Y-60, 0,0,0,screenTopRight);
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
	}
}

void game()
{
	hidScanInput();
	//Grab the pointers to the framebuffers
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);

	//If the game has been restarted, reset the game (we do this one time in the beginning to set everything up)
	if(restart==1)
	{
		gfxSet3D(true);
		score1 = 0;
		score2 = 0;

		reset();
		//Swap buffers for vsync
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		reset();
		gfxFlushBuffers();
		gfxSwapBuffers();
		screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_BOTTOM, NULL, NULL);
		restart=0;
	}

	//Get the status of the gamepad
	button=hidKeysHeld();

	//Check if their are any collisions between the ball and the paddles.
	checkCollision();

	//Update location of player1 and 2 paddles
	p1Move();
	p2Move();

	//Update location of the ball
	moveBall();

	//Render the scene
	render();

	//Swap buffers for vsync
	gfxFlushBuffers();
	gfxSwapBuffers();

	//Increment the counter (used for physicals calcuations)
	count++;
}
