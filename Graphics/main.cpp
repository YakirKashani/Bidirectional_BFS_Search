
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "Cell.h"
#include <iostream>

using namespace std;

const int WIDTH = 600;
const int HEIGHT = 600;
const int MSZ = 200; // maze-size
const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int PATH = 4;
const int GRAY = 5;
const int BLACK = 6;

const int START_GRAY = 7;
const int TARGET_GRAY = 8;

int maze[MSZ][MSZ] = {0};

bool runBFS = false;

int targetRow, targetCol;


queue <Cell*> graysStart;
queue <Cell*> graysTarget;

void SetupMaze()
{
	int i, j;
	for (i = 0;i < MSZ;i++) { // outer margins
		maze[0][i] = WALL; // first line
		maze[MSZ - 1][i] = WALL; // last line
		maze[i][0] = WALL; // left column
		maze[i][MSZ - 1] = WALL; // right column
	}
	for (i = 1;i < MSZ - 1;i++) { //inner space
		for (j = 1;j < MSZ - 1;j++) {
			if (i % 2 == 1) { //mostly spaces
				if (rand() % 10 > 1) // 80%
					maze[i][j] = SPACE;
				else // 20%
					maze[i][j] = WALL;
			}
			else { //mostly walls
				if (rand() % 10 >= 3) //70%
					maze[i][j] = WALL;
				else // 30%
					maze[i][j] = SPACE;
			}
		}
	}
	maze[MSZ / 2][MSZ / 2] = START; // Middle poinmt

	targetRow = rand() % MSZ;
	targetCol = rand() % MSZ;
	maze[targetRow][targetCol] = TARGET; // target point - random
}

void init()
{
	glClearColor(0.5,0.5,0.5,0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	SetupMaze();
}

void ShowMaze() {
	int i, j;
	for (i = 0;i < MSZ;i++) {
		for (j = 0;j < MSZ;j++) {
			// 1. set color of cell
			switch (maze[i][j])
			{
			case SPACE:
				glColor3d(1, 1, 1); //white
				break;
			case WALL:
				glColor3d(0, 0, 0);//black
				break;
			case START:
				glColor3d(0.6, 0.6, 1);//light-blue
				break;
			case TARGET:
				glColor3d(1, 0, 0);//red
				break;
			case PATH:
				glColor3d(1, 0, 1);//magenta
				break;
			case GRAY:
				glColor3d(0, 1, 0);//green
				break;
			case START_GRAY:
				glColor3d(0, 1, 0);//green
				break;
			case TARGET_GRAY:
				glColor3d(0, 1, 0);//green
				break;
			case BLACK:
				glColor3d(0.6, 0.6, 0.6);//gray
				break;
			}
			//show cell
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	ShowMaze();
	glutSwapBuffers(); // show all
}

void RestorePath(Cell* pc) {
	while (pc != nullptr) {
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

bool CheckNeighbor(char root, int row, int col, Cell* pCurrent) {
	Cell* cell;
	switch (root) {
	case 's':
		if (maze[row][col] == TARGET_GRAY) { //success
			runBFS = false;
			cout << "The solution has been found.\n";
			RestorePath(pCurrent);
			while (!graysTarget.empty()) {
				if (graysTarget.front()->getRow() == row && graysTarget.front()->getCol() == col) {
					RestorePath(graysTarget.front());
					break;
				}
				else
					graysTarget.pop();
			}
			return false;
		}
		else {
			cell = new Cell(row, col, pCurrent);
			maze[row][col] = START_GRAY;
			graysStart.push(cell);
			return true;
		}
		break;
	case 't':
		if (maze[row][col] == START_GRAY) { //success
			runBFS = false;
			cout << "The solution has been found.\n";
			RestorePath(pCurrent);
			while (!graysStart.empty()) {
				if (graysStart.front()->getRow() == row && graysStart.front()->getCol() == col) {
					RestorePath(graysStart.front());
					break;
				}
				else
					graysStart.pop();
			}
			return false;
		}
		else {
			cell = new Cell(row, col, pCurrent);
			maze[row][col] = TARGET_GRAY;
			graysTarget.push(cell);
			return true;
		}
		break;
	}
}

void RunBFSIteration(char root) {
	queue <Cell*>& grays = (root == 's') ? graysStart : graysTarget;
	bool go_on = true;

	if (grays.empty())
	{
		runBFS = false;
		cout << "There is no solution. Grays is empty\n";
		return;
	}
	else { // grays is not empty
		Cell* pCurrent = grays.front();
		grays.pop(); // extract the first elemnt from grays
		// 1. paint pCurrent black
		int row = pCurrent->getRow();
		int col = pCurrent->getCol();
		if ((maze[row][col] != START) && (maze[row][col] != TARGET))
			maze[row][col] = BLACK;
		// 2. check all the neighbors of pCurrent
		// go up
		if  ((root == 's' && (maze[row + 1][col] == SPACE || maze[row + 1][col] == TARGET_GRAY)) 
			|| (root == 't' && (maze[row + 1][col] == SPACE || maze[row + 1][col] == START_GRAY)))
		{
			go_on = CheckNeighbor(root, row + 1, col, pCurrent);
		}
		//down
		if ((root == 's' && go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == TARGET_GRAY))
			|| (root == 't' && go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == START_GRAY)))
		{
			go_on = CheckNeighbor(root, row - 1, col, pCurrent);
		}
		//left
		if ((root == 's' && go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == TARGET_GRAY))
			|| (root == 't' && go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == START_GRAY)))
		{
			go_on = CheckNeighbor(root, row, col - 1, pCurrent);
		}
		//right
		if ((root == 's' && go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == TARGET_GRAY))
			||	(root == 't' && go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == START_GRAY)))
		{
			go_on = CheckNeighbor(root, row, col + 1, pCurrent);
		}
	}
}

void idle() // always running
{
	if (runBFS)
		RunBFSIteration('s'); // BFS iteration from start
	if (runBFS)
		RunBFSIteration('t'); // BFS iteration from target
	glutPostRedisplay(); // indirect call to display
}

void menu(int choice) {
	switch (choice) 
	{
	case 1: //BFS
		Cell * ps = new Cell(MSZ / 2, MSZ / 2, nullptr);
		graysStart.push(ps);
		Cell* pt = new Cell(targetRow, targetCol, nullptr);
		graysTarget.push(pt);
		runBFS = true;
		break;
	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("BFS");
	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);

	//add menu
	glutCreateMenu(menu);
	glutAddMenuEntry("BFS", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}