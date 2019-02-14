/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

int timeElapsed = 0x0000;
int fastestWin = 0xffff;
int timeCounter = 0;

char textstring[] = "text, more text, and even more text!";

int winMargin = 0;
char *score;
int fastDifficulty;
int bigDifficulty;

int y;
int t;
int s;

int gamePhase = 0;

int menu_state;
int btnmutex;
int marked;
const int one_player_mode = 1;
const int two_player_mode = 2;
const int highscores = 3;

const int easy = 1;
const int medium = 2;
const int hard = 3;

int difficulty = 0;
int gameState;

int aidelay;
int PADDLE_HEIGHT = 10;

int paddleSpeed = 2;
int initDelay = 40;
int gameUpdateDelay;

int p1Score = 0;
int p2Score = 0;

int p1PrevY = 3;
int p2PrevY = 28;
int initPlayer = 1;

int ballX = 5;
int ballY = 17;

int ballDirectionXinit;
int ballDirectionYinit;

int ballDirectionX;
int ballDirectionY;

int paddle1Direction = 1;
int paddle2Direction = -1;
int p2mutex = 1;

int wait = 1;

int count;
int countMax;

int pauseMutex;

void updatePaddle1(){
	if((getbtns() & 0x2) && p1PrevY>1){
		p1PrevY -= paddleSpeed;
	}

	if((getbtns() & 0x1) && (p1PrevY+PADDLE_HEIGHT-1)<31){
		p1PrevY += paddleSpeed;
	}
}
//
void updatePaddle2(){
	if(getbtn2() && p2PrevY>1){
		p2PrevY -= paddleSpeed;
	}

	if(getbtn1() && (p2PrevY+PADDLE_HEIGHT-1)<31){
		p2PrevY += paddleSpeed;
	}
}
//written in collaboration and some parts modified slightly by Jonatan
void easyPaddle2(){
	if(((p2PrevY + PADDLE_HEIGHT-1) == 31) || (p2PrevY) == 0){
		paddle2Direction *= -1;
	}

	p2PrevY += paddle2Direction;
}
//written in collaboration and some parts modified slightly by Jonatan
void p2Harder(){

	if(ballX >= 32 && ballX <= 127 && ballY >= 2 && ballY <= 29/*p2mutex*/ && ballDirectionX > 0){
		if(((p2PrevY+(PADDLE_HEIGHT/2)-1) < ballY)){
			if((p2PrevY+PADDLE_HEIGHT-1)<=31){
				p2PrevY++;
			}
		} else if((p2PrevY+(PADDLE_HEIGHT/2)+1) > ballY){
			if(p2PrevY>=0){
				p2PrevY--;
			}
		}
	} /*else{
		if(p2PrevY==0 || p2PrevY+PADDLE_HEIGHT-1==31){
			paddle2Direction *= -1;
		}
		p2PrevY += paddle2Direction;
	}*/
	//}
}
//written in collaboration
void drawPaddles(){
	for(y=p1PrevY; y<(p1PrevY+PADDLE_HEIGHT-1); y++){
		setPixel(0, y);
	}

	for(y=p2PrevY; y<(p2PrevY+PADDLE_HEIGHT-1); y++){
		setPixel(127, y);
	}
}
//written in collaboration and some parts by Jonatan alone
void mainGameFunction(int ballSpeedX, int ballSpeedY, int modeOffset){
	if(ballX == 0+modeOffset){
		ballX = 0+modeOffset+1;
		//ballX = 1;
		if(ballY < p1PrevY || ballY>(p1PrevY+PADDLE_HEIGHT-1)){
			p2Score += 1;
			if(p2Score == 1){
				PORTE |= 0x1;
			} else if(p2Score == 2){
				PORTE |= 0x2;
			} else if(p2Score == 3){
				PORTE |= 0x4;
			} else if(p2Score == 4){
				PORTE |= 0x08;
			}
			ballX = 124;
			ballY = p2PrevY+(PADDLE_HEIGHT/2);
			ballDirectionX = 1;
		}
		if(ballY >= (p1PrevY+(PADDLE_HEIGHT/2)/*-1) && ballY <= (p1PrevY+(PADDLE_HEIGHT/2)*/)){
			ballDirectionY = 0;
		} else if(ballY>(p1PrevY+PADDLE_HEIGHT/2)&&ballY<=(p1PrevY+(PADDLE_HEIGHT-1))){
			ballDirectionY = ballSpeedY;
		} else{
			ballDirectionY = ballSpeedY*(-1);
		}
		ballDirectionX *= -1;
	} else if(ballX == 127-modeOffset){
		ballX = 127-modeOffset-1;
		if(ballY < p2PrevY || ballY>(p2PrevY+PADDLE_HEIGHT-1)){
			p1Score += 1;
			if(p1Score == 1){
				PORTE |= 0x080;
			} else if(p1Score == 2){
				PORTE |= 0x40;
			} else if(p1Score == 3){
				PORTE |= 0x20;
			} else if(p1Score == 4){
				PORTE |= 0x10;
			}
			ballX = 7;
			ballY = p1PrevY+(PADDLE_HEIGHT/2);
			ballDirectionX = -1;
		}
		if(ballY == (p2PrevY+(PADDLE_HEIGHT/2)/* && ballY <= (p2PrevY+(PADDLE_HEIGHT/2)+1)*/)){
			ballDirectionY = 0;
		} else if(ballY>(p2PrevY+(PADDLE_HEIGHT/2))
							&&ballY<=(p2PrevY+(PADDLE_HEIGHT-1))){
			ballDirectionY = ballSpeedY;
		} else{
			ballDirectionY = ballSpeedY*(-1);
		}
		ballDirectionX *= -1;
	}

	if(ballY <= 0){
		ballY = 1;
		ballDirectionY = 1;
	} else if(ballY >= 31){
		ballY = 30;
		ballDirectionY = -1;
	}
	// if(ballX<1 || ballX > 126){
	// 	ballDirectionX *= -1;
	// }

	ballX += ballDirectionX;
	ballY += ballDirectionY;
	delay(gameUpdateDelay);
	//while(s<gameUpdateDelay){}
 // labwork(); /* Do lab-specific things again and again */
	display_image(12, icon);
}

int unpaused = 0;
int paused = 1;

//gamePhase = 0;

void main(void) {
	int gamePhase = 0;
	while(1){
		if(gamePhase == 0){
			SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
			SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
			while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
			OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
			while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
			SYSKEY = 0x0;  /* Lock OSCCON */

			/* Set up output pins */
			AD1PCFG = 0xFFFF;
			ODCE = 0x0;
			TRISECLR = 0xFF;
			PORTE = 0x0;

			/* Output pins for display signals */
			PORTF = 0xFFFF;
			PORTG = (1 << 9);
			ODCF = 0x0;
			ODCG = 0x0;
			TRISFCLR = 0x70;
			TRISGCLR = 0x200;

			/* Set up input pins */
			TRISDSET = (1 << 8);
			TRISFSET = (1 << 1);

			/* Set up SPI as master */
			SPI2CON = 0;
			SPI2BRG = 4;
			/* SPI2STAT bit SPIROV = 0; */
			SPI2STATCLR = 0x40;
			/* SPI2CON bit CKP = 1; */
		        SPI2CONSET = 0x40;
			/* SPI2CON bit MSTEN = 1; */
			SPI2CONSET = 0x20;
			/* SPI2CON bit ON = 1; */
			SPI2CONSET = 0x8000;

			display_init();
			//display_string(0, "KTH/ICT lab");
			// display_string(1, "in Computer");
			// display_string(2, "Engineering");
			// display_string(3, "Welcome!");
			initialization();


			gamePhase++;
		} else if(gamePhase == 1){
			clearTextBuffer();
			//written in collaboration
			int px = 10;
			int oy = 0;
			int ny = 22;
			int gx = 101;

			//written by Jonatan Holmbäck, mainly
			count = 0;
			countMax = 12000;
			while(count<countMax){ //written in collaboration
				drawP(px, 11);
				drawO(44, oy);
				drawN(67, ny);
				drawG(gx, 11);
				if(px<21){
					px++;
				}
				if(oy<11){
					oy++;
				}
				if(ny>11){
					ny--;
				}

				if(gx>90){
					gx--;
				}
				display_image(12, icon);
				if(px>=21 && oy>=11 && ny<=11 && gx<=90){
					T2CONSET = 0x8000;
				}

				delay(100);
				clearScreen();
			}
			gamePhase++;
		} else if(gamePhase == 2){
			clearScreen();
			clearTextBuffer();
			display_string(0, "   Main Menu");
			display_string(2, "   [1 Player]");
			display_string(3, "   2 Players");
			display_update();



			gameState = unpaused;
			marked = one_player_mode;

			gameUpdateDelay = initDelay;
			btnmutex = 1;
			 /* Do any lab-specific initialization */
			menu_state = 0;
		 	difficulty = 0;
			//int stop = 0;
			//Written by Jonatan Holmbäck
			while(!menu_state){
				if(!getbtns() && !getbtn2()){
					btnmutex = 1;
				}
				if(marked == one_player_mode){
					if(getbtn2() && btnmutex){
						btnmutex = 0;
						marked = two_player_mode;
						clearTextBuffer();
						display_string(1, "   1 Player");
						display_string(2, "  [2 Players]");
						display_string(3, "   Highscores");
						display_update();
					} else if(getbtns() & 0x1){
						btnmutex = 0;
						menu_state = one_player_mode;
					}
				}

				if(marked == two_player_mode){
					if(getbtns() & 0x2 && btnmutex){
						btnmutex = 0;
						marked = one_player_mode;
						clearTextBuffer();
						display_string(0, "   Main Menu");
						display_string(2, "  [1 Player]");
						display_string(3, "   2 Players");
						display_update();
					} else if(getbtns() & 0x1 && btnmutex){
						btnmutex = 0;
						menu_state = two_player_mode;
					} else if(getbtn2() && btnmutex){
						btnmutex = 0;
						marked = highscores;
						display_string(1, "   1 Player");
						display_string(2, "   2 Players");
						display_string(3, "  [Highscores]");
						display_update();
					}
				}

				if(marked == highscores){
					if(getbtns() & 0x2 && btnmutex){
						btnmutex = 0;
						marked = two_player_mode;
						clearTextBuffer();
						display_string(1, "   1 Player");
						display_string(2, "  [2 Players]");
						display_string(3, "   Highscores");
						display_update();
					} else if(getbtns() & 0x1 && btnmutex){
						btnmutex = 0;
						menu_state = highscores;
						gamePhase = 5;
					}
				}

				if(((p1PrevY + PADDLE_HEIGHT-1) >= 31)){
					p1PrevY = 31-PADDLE_HEIGHT-1;
					paddle1Direction *= -1;
				} else if(p1PrevY <= 0){
					p1PrevY = 1;
					paddle1Direction *= -1;
				}


				if(((p2PrevY + PADDLE_HEIGHT-1) >= 31)){
					p2PrevY = 31-PADDLE_HEIGHT-1;
					paddle2Direction *= -1;
				} else if(p2PrevY <= 0){
					p2PrevY = 1;
					paddle2Direction *= -1;
				}



				p1PrevY += paddle1Direction;
				p2PrevY += paddle2Direction;

				if(initPlayer == 1){
					initPlayer = 2;
				} else{
					initPlayer = 1;
				}
				 //written in collaboration
				// if(getbtns() & 0x2){
				// 	menu_state = one_player_mode;
				// 	//stop = 1;
				// } else if(getbtns() & 0x1){
				// 	menu_state = two_player_mode;
				// 	//stop = 1;
				// }
			}
			gamePhase++;
		} else if(gamePhase == 3){
			clearTextBuffer();


			while(!difficulty){ //written in collaboration
					display_string(0, "    Difficulty");
					display_string(1, "      Easy");
					display_string(2, "      Medium");
					display_string(3, "      Hard");
					display_update();

					if(getbtns() & 0x2){ //goes back to menu (phase 2) where difficulty will be reset to 0
						gamePhase = 1;
						difficulty = 1;
					}
					// if(getsw() & 0x8){
					// 	difficulty = easy;
					// } else if(getsw() & 0x4){
					// 	difficulty = medium;
					// } else if(getsw() & 0x2){
					// 	difficulty = hard;
					// }
			}
			gamePhase++;
		} else if(gamePhase == 4){
			clearTextBuffer();

			display_update();
			//display_image(0, bg);
			//display_image(12, bg);

			//written in collaboration
			//initializing the locations of the paddles at the start of the game
			// for(y=3; y<3+PADDLE_HEIGHT-1; y++){
			// 	setPixel(0, y);
			// }
			//
			// for(y=5; y<5+PADDLE_HEIGHT-1; y++){
			// 	setPixel(127, y);
			// }


			int error;
			display_image(12, icon);
			if(difficulty == easy){ //written in colaboration
				PADDLE_HEIGHT = 10;
				// paddleSpeed = 2;
				//
				// p1Score = 0;
				// p2Score = 0;
				//
				// // p1PrevY = 3;
				// // p2PrevY = 16;
				//
				// // ballX = 5;
				// // ballY = 17;
				//
				// ballDirectionXinit = 1;
				// ballDirectionYinit = 1;
				// ballDirectionX = ballDirectionXinit;
				// ballDirectionY = ballDirectionYinit;
				//
				// paddle1Direction = 1;
				// paddle2Direction = -1;
			} else if(difficulty == medium){ //written in collaboration
				PADDLE_HEIGHT = 8;
				error = 2;
				gameUpdateDelay /= 2;
				aidelay = 213;
				// ballDirectionXinit = 1;
				// ballDirectionYinit = 1;
				//
				// ballDirectionX = ballDirectionXinit;
				// ballDirectionY = ballDirectionYinit;
				//
				// paddleSpeed = 2;
				//
				// p1Score = 0;
				// p2Score = 0;
				//
				// // p1PrevY = 3;
				// // p2PrevY = 16;
				//
				// // ballX = 5;
				// // ballY = 17;
				//
				// paddle1Direction = 1;
				// paddle2Direction = -1;
			} else if(difficulty == hard){//written in collaboration
				error = 2;
				gameUpdateDelay /= 2;
				aidelay = 291;
				PADDLE_HEIGHT = 6;

				// ballDirectionXinit = 1;
				// ballDirectionYinit = 1;
				//
				// ballDirectionX = ballDirectionXinit;
				// ballDirectionY = ballDirectionYinit;
				//
				// paddleSpeed = 2;
				//
				// p1Score = 0;
				// p2Score = 0;
				//
				// // p1PrevY = 3;
				// // p2PrevY = 16;
				//
				// // ballX = 5;
				// // ballY = 17;
				//
				// paddle1Direction = 1;
				// paddle2Direction = -1;
			}

			ballDirectionXinit = 1;
			ballDirectionYinit = 1;

			ballDirectionX = ballDirectionXinit;
			ballDirectionY = ballDirectionYinit;

			paddleSpeed = 2;

			p1Score = 0;
			p2Score = 0;

			// p1PrevY = 3;
			// p2PrevY = 16;

			// ballX = 5;
			// ballY = 17;

			paddle1Direction = 1;
			paddle2Direction = -1;

			// PADDLE_HEIGHT = 8;
			//
			// paddleSpeed = 2;
			// gameUpdateDelay = 35;
			//
			// p1Score = 0;
			// p2Score = 0;
			//
			// p1PrevY = 3;
			// p2PrevY = 16;
			//
			// ballX = 122;
			// ballY = 17;
			//
			// ballDirectionX = -1;
			// ballDirectionY = -1;
			//
			// paddle1Direction = 1;
			// paddle2Direction = -1;

			if(initPlayer == 1){
				ballX = 7;
				ballY = p1PrevY+(PADDLE_HEIGHT/2);
				ballDirectionX = 1;
			} else{
				ballX = 124;
				ballY = p2PrevY+(PADDLE_HEIGHT/2);
				ballDirectionX = -1;
			}

			t = 0;
			s = 0;
			int mutex = 1; //written by Jonatan Holmbäck
			pauseMutex = 1;
			p2mutex = 1;

			timeElapsed = 0x0000;
			while(p1Score < 5 && p2Score < 5){
				if(gameState == paused){ //written in collaboration
					display_string(0, "  Game paused");
					display_string(2, " sw3 to restart");
					display_string(3, " sw4 to resume");
					display_update();
					// IECSET(0) = 0x08000; //enable interrupts for switch 3
				  // IPCSET(3) = 04000000; //set priority for switch 3 to 1
					if(!(getsw()>>2) & 0x1){
						pauseMutex = 1;
					}
					if(((getsw() >> 2) & 0x1) && pauseMutex){
						main();
					}
				} else if(gameState == unpaused){
					// IECCLR(0) = 0x8000; //disable interrupts for switch 3
					// IPCCLR(3) = 0x04000000; //set priority for switch 3 to 0
					clearTextBuffer();
					//one player mode

					if(menu_state == one_player_mode)
					{
						clearScreen();
						//written by Jonatan Holmbäck
						if(difficulty != easy && mutex){
							updatePaddle1();
							mutex--;
						} else if(difficulty == easy){
							easyPaddle2();
							updatePaddle1();
						}else{
							mutex++;
						}
						//written in collaboration
						if(difficulty == easy){
							//drawBall(ballX, ballY);
							setPixel(ballX, ballY);
						}
						if(difficulty == medium || difficulty == hard){
							setPixel(ballX, ballY);
							if(t>=aidelay && TMR2 % error == 0){
								p2Harder();
								t=0;
							}

						}

						drawPaddles();

						//written by Jonatan Holmbäck
						//draw colon
						setPixel(64, 2);
						setPixel(64, 4);

						//written by Jonatan Holmbäck
						//draw score for player 1
						if(p1Score == 0){
							drawZero(62, 0);
						} else if(p1Score == 1){
							drawOne(58, 0);
						} else if(p1Score == 2){
							drawTwo(62, 0);
						} else if(p1Score == 3){
							drawThree(62, 0);
						} else{
							drawFour(62, 0);
						}
						//draw score for player 2
						//written by Jonatan Holmbäck
						if(p2Score == 0){
							drawZero(70, 0);
						} else if(p2Score == 1){
							drawOne(70, 0);
						} else if(p2Score == 2){
							drawTwo(70, 0);
						} else if(p2Score == 3){
							drawThree(70, 0);
						} else{
							drawFour(70, 0);
						}
						// void drawZero(){
						//
						// }
						//written by Jonatan Holmbäck
						if(difficulty == easy){
							mainGameFunction(ballDirectionXinit, ballDirectionYinit, 0);
						} else{
							mainGameFunction(ballDirectionXinit, ballDirectionYinit, 0);
						}

					}
					//Two player mode
					if(menu_state == two_player_mode)
					{

						//clearing the screen between frames
						clearScreen();
						//controlling paddle 1
						updatePaddle1();
						//controlling paddle 2
						updatePaddle2();
						//drawing the paddles
						drawPaddles();

						if(difficulty == easy){
							drawBall(ballX, ballY);
							//setPixel(ballX, ballY);
						} else if(difficulty == medium || difficulty == hard){
							setPixel(ballX, ballY);
						}
						//written by Jonatan Holmbäck
						//draw colon
						setPixel(64, 2);
						setPixel(64, 4);

						//draw score for player 1
						//written by Jonatan Holmbäck
						if(p1Score == 0){
							drawZero(62, 0);
						} else if(p1Score == 1){
							drawOne(58, 0);
						} else if(p1Score == 2){
							drawTwo(62, 0);
						} else if(p1Score == 3){
							drawThree(62, 0);
						} else{
							drawFour(62, 0);
						}
						//draw score for player 2
						//written by Jonatan Holmbäck
						if(p2Score == 0){
							drawZero(70, 0);
						} else if(p2Score == 1){
							drawOne(70, 0);
						} else if(p2Score == 2){
							drawTwo(70, 0);
						} else if(p2Score == 3){
							drawThree(70, 0);
						} else{
							drawFour(70, 0);
						}
						//give point to player 2 or bounce of paddle 1
						//written by Jonatan Holmbäck
						if(difficulty == easy){
							mainGameFunction(ballDirectionXinit, ballDirectionYinit, 2);
						} else{
							mainGameFunction(ballDirectionXinit, ballDirectionYinit, 0);
						}
						// //give point to player 1 or bounce of paddle 2

						// //ball bounce of top or bottom wall of the screen

						// //ball movement up and down

						// //drawing the ball on the screen

						// //speed of the ball and updating of the screen
						// //basically the speed of the game

						//display the content of the screen image
					}
				}
			}

			//bigDifficulty = difficulty;

			if(timeElapsed < fastestWin){
				fastestWin = timeElapsed;
				fastDifficulty = difficulty;
			}
			int diff;
			if(p1Score-p2Score < 0){
				diff = p2Score-p1Score;
			} else{
				diff = p1Score-p2Score;
			}
			if(diff>winMargin || (diff >= winMargin && difficulty>bigDifficulty)){
				winMargin = diff;
				bigDifficulty = difficulty;
				if(winMargin == 1 || winMargin == -1){
					score = "5:4";
				} else if(winMargin == 2 || winMargin == -2){
					score = "5:3";
				} else if(winMargin == 3 || winMargin == -3){
					score = "5:2";
				} else if(winMargin == 4 || winMargin == -4){
					score = "5:1";
				} else{
					score = "5:0";
				}
			}
			difficulty = 0;

			gamePhase++;
		} else if(gamePhase == 5){
			//clearing the screen before displaying who won
			clearScreen();
			// int i;
			// for(i=0; i<512; i++){
			// 	icon[i] = 0;
			// }
			clearTextBuffer();
			//displaying which player who won the game
			wait = 1;
			//written in collaboration
			while(wait){

				if(p1Score == 5){
					display_string(1, " Player 1 wins!");
				} else {
					display_string(1, " Player 2 wins!");
				}
				display_string(3, "Btn4 to restart");

				display_update();

				if(getbtns() & 0x2){
					wait = 0;
				}
			}

			gamePhase = 0;
		} else if(gamePhase == 6){
			//Written by Jonatan Holmbäck
			clearTextBuffer();
			// const int fastest = 0;
			// const int biggest = 1;
			// int sel = fastest;
			while(menu_state == highscores){
				//if(sel == fastest){
					clearTextBuffer();
					display_string(0 ,"Fastest win:");
					if(fastDifficulty == easy){
						//clearTextBuffer();
						display_string(0 ,"Fastest win(Low)");
					} else if(fastDifficulty == medium){
						//clearTextBuffer();
						display_string(0 ,"Fastest win(Med)");
					} else if(fastDifficulty == hard){
						//clearTextBuffer();
						display_string(0 ,"Fastest win(Hi)");
					}
					time2string(textstring, fastestWin);
					if(fastestWin == 0xffff){
						display_string(1, "00:00");
					} else{
						display_string(1, textstring);
					}
					display_string(2, "Biggest win:");
					if(bigDifficulty == easy){
						//clearTextBuffer();
						display_string(2, "Biggest win(Low)");
					} else if(bigDifficulty == medium){
						//clearTextBuffer();
						display_string(2, "Biggest win(Med)");
					} else if(bigDifficulty == hard){
						//clearTextBuffer();
						display_string(2, "Biggest win(Hi)");
					}
					if(winMargin == 0){
						display_string(3, "0:0");
					} else{
						display_string(3, score);
					}
					// if(getbtn2()){
					// 	sel = biggest;
					// }
				/*} /*else{
					display_string(0 ,"Fastest win");
					display_string(1, "[Biggest win]");
					if(winMargin == 0){
						display_string(3, "0:0");
					} else{
						display_string(3, score);
					}

				}*/

				display_update();
				if(getbtns() & 0x2 && btnmutex){
					menu_state = 0;
					gamePhase = 2;
				}
				if(!getbtns() && !getbtn2() && !getbtn1()){
					btnmutex = 1;
				}
				clearTextBuffer();
			}
		}
	}

        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/


	// display_image(96, paddle2);
	//written in collaboration







}
/* Interrupt Service Routine */
void user_isr_main( void )
{
	//written by Jonatan Holmbäck
	//handles interrupts for INT1
	// if(IFS(0) & 0x80){
	// 	if(gameState == paused){
	// 		gamePhase = 1;
	// 	}
	// }

	//handles interrupt for INT2
	//written in mainly in collaboration (some by Jonatan alone)
	if(IFS(0) & 0x800){
		IFS(0) &= 0xfffff7ff;
		if(!difficulty && menu_state){
			difficulty = hard;
		}
	}
	//handles interrupt for INT3
	//written in collaboration
	if(IFS(0) & 0x8000){
		IFS(0) &= 0xffff7fff;
		if(!difficulty && menu_state){
			difficulty = medium;
		}
	}
	//handles interrupt for INT4
	//written in collaboration
  if(IFS(0) & 0x80000){
		IFS(0) &= 0xfff7ffff;
		if(gameState == unpaused && difficulty){
			//T2CON &= ~0x00008000;
			if((getsw()>>2) && 0x1){
				pauseMutex = 0;
			}
			gameState = paused;
		} else if(gameState == paused && difficulty){
			//T2CON |= 0x8000;
			gameState = unpaused;
		} else if(!difficulty && menu_state){
			difficulty = easy;
		}
		//written by Jonatan Holmbäck
		if(count<countMax){
			count = countMax;
		}
  }
	//handles interrupts for Timer2
	if(IFS(0) & 0x100){
		IFS(0) &= 0xfffffeff;
		if(count < countMax){
			count++;
		} else if(difficulty && menu_state == one_player_mode && gameState == unpaused){
			t++;
		}

		if(difficulty && gameState == unpaused){
			s++;
		}

		if(gameState == unpaused && difficulty){
			if(timeCounter<10000){
				timeCounter++;
			} else{
				tick(&timeElapsed);
				timeCounter = 0;
			}
		}

	}


	/*if((IFS(0) & 0x8000)){
		IFS(0) &= 0xffff7fff;
		if(gameState == paused){
			main();
		}
	}*/

}
