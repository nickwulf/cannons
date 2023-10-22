=======
Cannons
=======

Cannons is a 2-player game where you try to shoot your opponent by choosing the correct firing angle and power.

How To Play
-----------
Take turns shooting at each other. On your turn, you can choose the firing angle and power. Then fire. If you miss, see where the cannon ball landed so you can improve your aim on your next turn. When a player successfully hits the other, they get a point and the arena resets.

Controls
________
* A, D         ➜ Adjust the angle counter-clockwise/clockwise, respectively.
* W, S         ➜ Increase/decrease the power, respectively.
* Space        ➜ Fire.
* P            ➜ Pause the game and see instructions

Create The Executable
-----------
To run the makefile, you'll need g++, which you can easily get by installing MinGW.

This game's graphics are built using freeglut, a modern replacement for the original OpenGL GLUT library. You can see how to install freeglut to work with MinGW at https://www.transmissionzero.co.uk/computing/using-glut-with-mingw/.

Once you have g++ and freeglut working, just run the makefile to create the Windows executable
