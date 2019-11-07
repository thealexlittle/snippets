/*
 * File: Hangman.jomputerava
 * ------------------
 * This program will eventually play the Hangman game from
 * Assignment #4.
 * Link: 
 */

import acm.graphics.*;
import acm.program.*;
import acm.util.*;
import java.awt.*;

public class Hangman extends ConsoleProgram {

	private static final int NGUESSES = 8;

	public void init() {
		canvas = new HangmanCanvas();
		add(canvas);
	}

	public void run() {
		while(!isGameOver()) {
			setUp();
			playGame(solution);
			endGame();
		}
	}
	
	/** Assigns a word to solution, then hides that solution, and resets the display of the game */
	private void setUp() {
		solution = lexicon.getWord();
		setFont("Serif-18-bold");
		println("Welcome to Hangman!");
		hideWord();
		canvas.reset();
		canvas.displayWord(hiddenWord);
	}

	/** Turns every character in the solution string into a dash */
	private void hideWord() {
		String result = "";
		for (int i = 0;  i < solution.length(); i++) {
				result = result + "-";
		}
		hiddenWord = result;
	}

	/** Allows the user to play the game as long as the win or lose conditions have not been met */
	private void playGame(String solution) {
		while (!isGameOver()) {
			println("The word now looks like this: " + hiddenWord);
			println("You have " + numGuesses + " guesses left.");
			String letter = readLine("Your guess: ");
			char guess = toUpperCase(letter.charAt(0));
			if (letter.length() == 1 && isLetter(guess)) {
				guess = toUpperCase(letter.charAt(0));
				checkGuess(guess);
			} else {
				println("This is not a valid guess, Chief.");
			}
		canvas.displayWord(hiddenWord);
		}

	}

	/** Updates the hidden word revealing correct guesses, otherwise detracts a turn signaling an incorrect guess */
	private void checkGuess(char ch) {
		for(int i = 0; i < solution.length(); i++) {
			if(isCorrectGuess(ch) && !isAlreadyGuessed(ch)) {
				println("You guessed correctly, Boss!");
				updateHiddenWord(ch);
				break;
			}
		}
		if(!isCorrectGuess(ch)) {
			println("This isn't a correct guess, my guy.");
			canvas.noteIncorrectGuess(ch);
			numGuesses--;
		}
	}

	/** Changes hiddenWord dashes to the correctly guessed letter wherever it appears in the solution */
	private void updateHiddenWord(char ch) {
		String result = "";
		for (int i = 0; i < solution.length(); i++) {
			if (!isLetter(hiddenWord.charAt(i))) {
				if (solution.charAt(i) != ch) {
					result = result + "-";
				} else {
					result = result + ch;
				}
			} else {
				result += solution.charAt(i);
			}
		}
		hiddenWord = result;
	}
	
	/** Checks whether the letter the user typed matches a letter in the solution */
	private boolean isCorrectGuess(char ch) {
		for(int i = 0; i < solution.length(); i++) {
			if(ch == solution.charAt(i)) {
				return true;
			}
		}
		return false;
	}

	/** Checks whether the letter the user type has already been correctly guessed */
	private boolean isAlreadyGuessed(char ch) {
		for (int i = 0; i < solution.length(); i++) {
			if(ch == hiddenWord.charAt(i)){
				return true;
			}
		}
		return false;
	}
	
	/** Returns whether the game is over, either all turns have been used or all letters have been guessed */
	private boolean isGameOver() {
		if (numGuesses == 0 || hiddenWord.equals(solution)) {
			return true;
		} else {
			return false;
		}
	}
	
	/** 
	 * Displays whether or not the game was won, and offers to play again; if the user inputs "y," 
	 * then the guesses are reset and hiddenWord is defaulted again, which makes isGameOver false.
	 */
	private void endGame() {
		if(hiddenWord.equals(solution)) {
			println("You guessed the word: " + solution);
			println("You win, mate!");
		} else {
			println("You Lose, GG.");
			println("The word was: " + solution);
		}
		String input = readLine("Play again? [y]es or [n]o: ");
		if(input.equals("y")) {
			numGuesses = NGUESSES;
			hiddenWord = "hiddenWord";
		} else {
			println("Goodbye!");
		}
	}
	
	/** For some reason, these methods were not available to us, so we wrote them for our program */
	private char toUpperCase(char ch){
		if(ch >= 'a' && ch <= 'z') {
			return (char) (ch + 'A' - 'a');
		} else {
			return ch;
		}
	}

	/** Only need to check whether the input is an upper-case letter since every input is made upper-case */
	private boolean isLetter(char ch){
		if (ch >= 'A' && ch <= 'Z') {
			return true;
		}
		return false;
	}
	
	/** Initializes the lexicon */
	private HangmanLexicon lexicon = new HangmanLexicon();
	
	/** Solution string, defaulted so that the game doesn't make isGameOver to true. */
	private String solution = "solution";
	
	/** Hidden word string, defaulted so that the game doesn't make isGameOver true. */
	private String hiddenWord = "hiddenWord";

	/** Number of Guesses */
	public int numGuesses = NGUESSES;

	/** Instance of the canvas */
	private HangmanCanvas canvas;
}

	