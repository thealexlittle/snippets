/*
 * File: Breakout.java
 * -------------------
 * Name:
 * Section Leader:
 * 
 * This file will eventually implement the game of Breakout.
 */

import acm.graphics.*;
import acm.program.*;
import acm.util.*;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;

public class Breakout extends GraphicsProgram {

	/** Width and height of application window in pixels.  IMPORTANT NOTE:
	 * ON SOME PLATFORMS THESE CONSTANTS MAY **NOT** ACTUALLY BE THE DIMENSIONS
	 * OF THE GRAPHICS CANVAS.  Use getWidth() and getHeight() to get the 
	 * dimensions of the graphics canvas. */
	public static final int APPLICATION_WIDTH = 400;
	public static final int APPLICATION_HEIGHT = 600;

	/** Dimensions of game board.  IMPORTANT NOTE: ON SOME PLATFORMS THESE 
	 * CONSTANTS MAY **NOT** ACTUALLY BE THE DIMENSIONS OF THE GRAPHICS
	 * CANVAS.  Use getWidth() and getHeight() to get the dimensions of
	 * the graphics canvas. */
	private static final int WIDTH = APPLICATION_WIDTH;
	private static final int HEIGHT = APPLICATION_HEIGHT;

	/** Dimensions of the paddle */
	private static final int PADDLE_WIDTH = 60;
	private static final int PADDLE_HEIGHT = 10;

	/** Offset of the paddle up from the bottom */
	private static final int PADDLE_Y_OFFSET = 30;

	/** Number of bricks per row */
	private static final int NBRICKS_PER_ROW = 10;

	/** Number of rows of bricks */
	private static final int NBRICK_ROWS = 10;

	/** Separation between bricks */
	private static final int BRICK_SEP = 4;

	/** Width of a brick */
	private static final int BRICK_WIDTH =
			(WIDTH - (NBRICKS_PER_ROW - 1) * BRICK_SEP) / NBRICKS_PER_ROW;

	/** Height of a brick */
	private static final int BRICK_HEIGHT = 8;

	/** Radius of the ball in pixels */
	private static final int BALL_RADIUS = 10;

	/** Offset of the top brick row from the top */
	private static final int BRICK_Y_OFFSET = 70;

	/** Number of turns */
	private static final int NTURNS = 3;

	/** Delay (game speed) */
	private static final int DELAY = 10;
	
	/** Scoreboard Cooridinates*/
	private static final int SCOREBOARD_X = 20;
	private static final int SCOREBOARD_Y = BRICK_Y_OFFSET/2;
	
	/* Method: run() */
	/** Runs the BreakOut program. */
	public void run() {
		drawBricks();
		paddle = drawPaddle();
		ball = drawBall();
		scoreBoard = updateScoreBoard(bricksLeft);
		GLabel welcome = printMessage("Click to Play");
		addMouseListeners();
		waitForClick();
		remove(welcome);

		while(gameOver() == false) {
			moveBall();
			wallCollision();
			collisionCheck();
			gameOver();
			pause(DELAY);
		}

	}

	/** draws bricks */
	private void drawBricks() {
		for (int row = 1; row <= NBRICK_ROWS; row++) {
			for (int b = 0; b < NBRICKS_PER_ROW; b++) {
				GRect brick = new GRect(BRICK_WIDTH,BRICK_HEIGHT);
				double x = ((getWidth()/2.0))-(BRICK_WIDTH/2.0)-((NBRICKS_PER_ROW*BRICK_WIDTH)/2.0)+(b*(BRICK_WIDTH+BRICK_SEP));
				double y = (BRICK_Y_OFFSET)+(BRICK_HEIGHT*row)+((row-1)*BRICK_SEP);
				setBrickColor(brick,row);
				add(brick,x,y);
			}
		}
	}

	/** set brick color */
	private void setBrickColor(GRect brick, int row) {
		if(row % 10 == 1 || row % 10 == 2) {
			brick.setColor(Color.RED);
			brick.setFillColor(Color.RED);
		} else if (row % 10 == 3 || row % 10 == 4) {
			brick.setColor(Color.ORANGE);
			brick.setFillColor(Color.ORANGE);
		} else if (row % 10 == 5 || row % 10 == 6) {
			brick.setColor(Color.YELLOW);
			brick.setFillColor(Color.YELLOW);
		} else if (row % 10 == 7 || row % 10 == 8) {
			brick.setColor(Color.GREEN);
			brick.setFillColor(Color.GREEN);
		} else if (row % 10 == 9 || row % 10 == 0) {
			brick.setColor(Color.CYAN);
			brick.setFillColor(Color.CYAN);
		}
		brick.setFilled(true);
	}

	/** Draws the paddle */
	private GRect drawPaddle() {
		GRect paddle = new GRect(PADDLE_WIDTH,PADDLE_HEIGHT);
		double paddleX = (getWidth()/2)-(PADDLE_WIDTH/2);
		double paddleY = getHeight()- PADDLE_Y_OFFSET;
		paddle.setFilled(true);
		add(paddle,paddleX,paddleY);

		return paddle;
	}

	/** Allows the paddle to move with the mouse */
	public void mouseMoved(MouseEvent e){
		paddle.setLocation(e.getX(), getHeight()- PADDLE_Y_OFFSET);
		if (e.getX() > getWidth()-PADDLE_WIDTH){
			paddle.setLocation(getWidth()-PADDLE_WIDTH,getHeight() - PADDLE_Y_OFFSET);
		}
	}

	/** Draws the ball and initializes its path*/
	private GOval drawBall() {
		GOval ball = new GOval(BALL_RADIUS*2,BALL_RADIUS*2);
		double ballX = (getWidth()/2);
		double ballY = (getHeight()/2);
		ball.setFilled(true);
		add(ball,ballX,ballY);

		vx = rgen.nextDouble(1.0,3.0);
		if  (rgen.nextBoolean(.5)){ 
			vx =-vx;
		}
		vy = 3;

		return ball;
	}
	
	/** Moves Ball */ 
	private void moveBall() {
		ball.move(vx,vy);
	}

	/** Checks for  wall collisions */
	private void wallCollision() {
		if (ball.getX() < 0 || ball.getX()+2*BALL_RADIUS > getWidth()){
			vx=-vx;
		}
		if (ball.getY() < 0){
			vy=-vy;
		}
	}
	
	/** Checks to see if the ball has collided with an object */
	private GObject getCollidingObject() {
		GObject collision = null;

		if (getElementAt(ball.getX(),ball.getY()) != null) {
			collision = getElementAt(ball.getX(),ball.getY());

		} else if (getElementAt(ball.getX()+(2*BALL_RADIUS),ball.getY()) != null) {
			collision = getElementAt(ball.getX()+(2*BALL_RADIUS),ball.getY());

		} else if (getElementAt(ball.getX(),ball.getY()+(2*BALL_RADIUS)) != null) {
			collision = getElementAt(ball.getX(),ball.getY()+(2*BALL_RADIUS));

		} else if (getElementAt(ball.getX()+(2*BALL_RADIUS),ball.getY()+(2*BALL_RADIUS)) !=null){
			collision = getElementAt(ball.getX()+(2*BALL_RADIUS),ball.getY()+(2*BALL_RADIUS));
		}

		return collision;
	}

	/** Checks to see if the ball collided with a paddle or a ball */ 
	private void collisionCheck() {
		GObject collider = getCollidingObject();
		if (collider == paddle) {
			vy = Math.abs(vy)*-1;
		} else if (collider != null) {
			vy = -vy;
			remove(collider);
			remove(scoreBoard);
			bricksLeft--;
			scoreBoard = updateScoreBoard(bricksLeft);
		}
	}
	
	private GLabel printMessage(String text) {
		GLabel message = new GLabel(text);
		message.setFont("SansSerif-28");
		message.setColor(Color.RED);
		add(message,(getWidth()/2)-(message.getWidth()/2),(getHeight()/2)-(message.getAscent()/2));
		add(message);
		return message;
	}
	
	private GLabel updateScoreBoard( int bricksLeft) {
		GLabel score = new GLabel("Bricks Left: "+bricksLeft);
		score.setFont("SansSerif-14");
		add(score,SCOREBOARD_X,SCOREBOARD_Y);
		return score;
	}
	
	/** Checks to see if the user has reached the game over end conditions */
	private boolean gameOver() {
		boolean gameOver = false;
		
		if (ball.getY() > getHeight()) {
			deaths++;
			remove(ball);
			add(ball,getWidth()/2,getHeight()/2);
	
			GLabel message = printMessage("Lives Remaining: "+(NTURNS-deaths));
			waitForClick();
			remove(message);
		}
		if (deaths == NTURNS) {
			printMessage("Game Over!");
			gameOver = true; 
		}else if (bricksLeft == 0){
			printMessage("You Win!!");
			gameOver = true;
		}
		return gameOver;
	}
	/** Instance Label of the scoreBoard */
	private GLabel scoreBoard;
	
	/** Instance object of the paddle */
	private GRect paddle;

	/** Instance object of the ball */
	private GOval ball;

	/** Instance variables for the ball's velocity */
	private double vx, vy;
	
	/** Death Counter */
	private int deaths = 0;
	
	 /** Bricks left counter */
	private int bricksLeft = NBRICKS_PER_ROW*NBRICK_ROWS;
	
	/** Random generator */
	private RandomGenerator rgen = RandomGenerator.getInstance();
	
}
