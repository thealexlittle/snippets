/*
 * File: HangmanCanvas.java
 * ------------------------
 * This file keeps track of the Hangman display.
 */

import acm.graphics.*;

public class HangmanCanvas extends GCanvas {
	
	/** Resets the display so that only the scaffold appears */
	public void reset() {
		removeAll();
		incorrectGuesses = "";
		double x1 = getWidth()/2 - BEAM_LENGTH;
		double y1 = Y_OFFSET;
		double x2 = getWidth()/2;
		double y2 = Y_OFFSET + SCAFFOLD_HEIGHT;
		double y3 = Y_OFFSET + ROPE_LENGTH;
		GLine scaffoldBase = new GLine(x1, y1, x1, y2);
		GLine beam = new GLine(x1, y1, x2, y1);
		GLine rope = new GLine(x2, y1, x2, y3);
		add(scaffoldBase);
		add(beam);
		add(rope);
	}

/**
 * Updates the word on the screen to correspond to the current
 * state of the game.  The argument string shows what letters have
 * been guessed so far; unguessed letters are indicated by hyphens.
 */
	public void displayWord(String word) {
		GObject oldLabel = getElementAt(getWidth()/2, getHeight() - 2*Y_OFFSET);
		if (oldLabel != null) {
			remove(oldLabel);
		}
		GLabel newLabel = new GLabel(word);
		newLabel.setFont("Serif-18-bold");
		add(newLabel, (getWidth() - newLabel.getWidth())/2, getHeight() - 2*Y_OFFSET);
	}

/**
 * Updates the display to correspond to an incorrect guess by the
 * user.  Calling this method causes the next body part to appear
 * on the scaffold and adds the letter to the list of incorrect
 * guesses that appears at the bottom of the window.
 */
	public void noteIncorrectGuess(char letter) {
		incorrectGuesses += letter;
		GObject oldLabel = getElementAt(getWidth()/2, getHeight() - Y_OFFSET);
		if (oldLabel != null) {
			remove(oldLabel);
		}
		GLabel newLabel = new GLabel(incorrectGuesses);
		newLabel.setFont("Serif-14-bold");
		add(newLabel, (getWidth() - newLabel.getWidth())/2, getHeight() - Y_OFFSET);
		addBodyPart(incorrectGuesses.length());
	}
	
	/** Adds a body part depending on what turn the user is on in the game. */
	private void addBodyPart(int index) {
		switch(index) {
			case 0: break;
			case 1: addHead(); break;
			case 2: addBody(); break;
			case 3: addLeftArm(); break;
			case 4: addRightArm(); break;
			case 5: addLeftLeg(); break;
			case 6: addRightLeg(); break;
			case 7: addLeftFoot(); break;
			case 8: addRightFoot(); break;
			default: break;
		}
	}

	private void addHead() {
		double x = getWidth()/2 - HEAD_RADIUS;
		double y = Y_OFFSET + ROPE_LENGTH;
		GOval head = new GOval(2*HEAD_RADIUS, 2*HEAD_RADIUS);
		add(head, x, y);
	}

	private void addBody() {
		double x1 = getWidth()/2;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS;
		double x2 = x1;
		double y2 = y1 + BODY_LENGTH;
		GLine body = new GLine(x1, y1, x2, y2);
		add(body);
	}

	private void addLeftArm() {
		double x1 = getWidth()/2;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + ARM_OFFSET_FROM_HEAD;
		double x2 = x1 - UPPER_ARM_LENGTH;
		double y2 = y1 + LOWER_ARM_LENGTH;
		GLine upperArm = new GLine(x1, y1, x2, y1);
		GLine lowerArm = new GLine(x2, y1, x2, y2);
		add(upperArm);
		add(lowerArm);
	}

	private void addRightArm() {
		double x1 = getWidth()/2;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + ARM_OFFSET_FROM_HEAD;
		double x2 = x1 + UPPER_ARM_LENGTH;
		double y2 = y1 + LOWER_ARM_LENGTH;
		GLine upperArm = new GLine(x1, y1, x2, y1);
		GLine lowerArm = new GLine(x2, y1, x2, y2);
		add(upperArm);
		add(lowerArm);
	}
	
	private void addLeftLeg() {
		double x1 = getWidth()/2;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + BODY_LENGTH;
		double x2 = x1 - HIP_WIDTH;
		double y2 = y1 + LEG_LENGTH;
		GLine leftHip = new GLine(x1, y1, x2, y1);
		GLine leg = new GLine(x2, y1, x2, y2);
		add(leftHip);
		add(leg);
	}
	
	private void addRightLeg() {
		double x1 = getWidth()/2;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + BODY_LENGTH;
		double x2 = x1 + HIP_WIDTH;
		double y2 = y1 + LEG_LENGTH;
		GLine rightHip = new GLine(x1, y1, x2, y1);
		GLine leg = new GLine(x2, y1, x2, y2);
		add(rightHip);
		add(leg);
	}
	
	private void addLeftFoot() {
		double x1 = getWidth()/2 - HIP_WIDTH;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + BODY_LENGTH + LEG_LENGTH;
		double x2 = x1 - FOOT_LENGTH;
		double y2 = y1;
		GLine foot = new GLine(x1, y1, x2, y2);
		add(foot);
	}

	private void addRightFoot() {
		double x1 = getWidth()/2 + HIP_WIDTH;
		double y1 = Y_OFFSET + ROPE_LENGTH + 2*HEAD_RADIUS + BODY_LENGTH + LEG_LENGTH;
		double x2 = x1 + FOOT_LENGTH;
		double y2 = y1;
		GLine foot = new GLine(x1, y1, x2, y2);
		add(foot);
	}
	
/* Constants for the simple version of the picture (in pixels) */
	private static final int SCAFFOLD_HEIGHT = 360;
	private static final int BEAM_LENGTH = 144;
	private static final int ROPE_LENGTH = 18;
	private static final int HEAD_RADIUS = 36;
	private static final int BODY_LENGTH = 144;
	private static final int ARM_OFFSET_FROM_HEAD = 28;
	private static final int UPPER_ARM_LENGTH = 72;
	private static final int LOWER_ARM_LENGTH = 44;
	private static final int HIP_WIDTH = 36;
	private static final int LEG_LENGTH = 108;
	private static final int FOOT_LENGTH = 28;
	private static final int Y_OFFSET = 30;

	/** The string that contains the incorrect guesses the user has input */
	private String incorrectGuesses = ""; 
}
