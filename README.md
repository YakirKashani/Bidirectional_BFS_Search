# Bidirectional BFS Search

This project implements a Breadth-First Search (BFS) algorithm for solving a maze. It visualizes the maze and the BFS process in real-time using OpenGL and GLUT.

## Features
1. Random Maze Generation:
  The maze is generated with random wall and spaces, enssuring a mix of paths and obstacles.
  The strarting point is at the center of the maze, and the target point is randomly placed.
2. Bidirectional BFS:
  BFS is executed simultaneously from the start and target points to speed up the pathfinding process.
3. Visualization:
  The maze and BFS progress are displayed graphically.
  Colors represent different states:
    - White: Free space
    - BLACK: Wall
    - Light Blue: Starting point
    - Red: Target point
    - Green: visited nodes (gray)
    - Magneta: Final path
    - Gray: Processed nodes (black)
4. Interactive Menu:
  Right-click to open the menu and start the BFS algorithm.

## Pictures
![Maze_Base](https://github.com/user-attachments/assets/442956a2-965c-49eb-90cf-3a22a2929465)

![Maze_Running](https://github.com/user-attachments/assets/c51b7d70-720b-450f-8e05-a5251e1fbdae)

![Maze_Final](https://github.com/user-attachments/assets/d6611af8-08c3-4512-af5b-ac7fbb094c67)

