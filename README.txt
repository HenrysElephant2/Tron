CSCI4229 Final Project - Tron
Jason Cummings and Alex Burnley

To run the program, just run run “make” and “./main”.

Controls:
    Any time:
	1/2 - Bloom on/off
    Menu:
	Mouse only
    In Game:
	Space - Start the game
	A/D - Top player left and right
	J/L - Bottom player left and right
	R - Quick reset after game end

Features:
One of our most prominent features is bloom. Adding bloom to the program gives the impression that the colored strips are actually lit and glowing and contributes greatly to the overall effect of the image. We also added particle explosions on player death to give a basic impression of the bikes shattering into thousands of fragments as in the movie. Both of these were accomplished using shaders. We also made sure that we rendered the trails in a way that makes them appear as one continuous segment, which is a vast improvement over the unconnected segments we had at one point in time. Finally, we used transparency to give the impression of reflections in the floor and to give the trails the glassy look they have in the movie.

Notes:
There are some occasions where the explosions do not render when the game is run for the first time. They should show up in the next round. Also, we both use Mac and we were only able to test the program on Linux through the school VM, which yields some graphical differences. We’re not sure if this is something to do with what the VM has installed or if it’s a general difference on Linux, so we’ve included a couple of reference screenshots of how the game looks on our machines in the folder “Screenshots.” The game has never been tested on Windows.

Contributions:
Jason built most of the game mechanics and did the graphics for the map and skybox, the trails and other transparent objects, and built the menus and their textures. Alex created all of the models and their textures for the players, as well as coded all of the shaders to color them correctly and display the particle explosions. He also implemented bloom. Both helped the other at various point throughout the project and contributed to the overall functionality of the game.
