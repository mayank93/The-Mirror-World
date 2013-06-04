About:
This Program takes the dimensions of object from user though a file( file format specified in a file named input.txt ) and draws them. This program shows the reflection of light through one side reflecting mirror.There are blocks , back side of mirror, and world boundary which absorbs light. 
At the end of the program it stores the coordinates of the objects in a file specified by you as second argument in command line.

You can resize a object by selecting it then pressing 'a' to enter resize mode and using left , right arrow keys.
You can add/delete objects.
You can select a projector and increase or decrease number of pixel by pressing 'i' or 'u' respectively
You can decrease or increase the speed of updation by pressin 'j' or 'k' respectively.

How to run:
-> The makefile compiles the code. (Type 'make' on the terminal)
-> Now type './assignment1 "inputfile name"  "outfile name" ' (on terminal) to run the executable. {linux}

object= (mirror,projector,block)

Controls:

-> Press 'c' to select a object or left-click on the object you want to select.

-> You can use the arrow keys to move the object.

-> Press 'l' to rotate anti-clockwise and 'r' to rotate clockwise.

-> Press 'f' to increase speed of translation/rotation and s to decrease.

-> Press 'a' to enter/exit into resizing mode.

-> Click Right mouse button to deselect selected object.

-> In reszing mode press right,left arrows keys to increase or decrease length of projector respectively.

-> Press 'g' to enter Gaze at cursor mode.

-> Press 't' to enter light transport mode.

-> Press 'z' to enter random walk mode.

-> Press 'q' or 'esc' to exit the program.

-> Press 'm' to add a Mirror.

-> Press 'p' to add a Projector.

-> Press 'b' to add a block.

-> Press 'del' to delete selected objected (if number of type of selected object is greator the one ). 

-> you can enter any number of modes simutaneously.

-> you can select a projector and increase or decrease number of pixel by pressing 'i' or 'u' respectively

-> Press 'j' or 'k' to decrease or increase the speed of updation respectively.
