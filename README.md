## SNAKE

### Backstory

I chose to code the Snake game because the one from when I was little was too simple and too boring. I hope I managed to change that. Besides that, I wanted it to be a real challenge to the dynamics of the game, how well the snake moves on the matrix, how much I can decrease the delay of fast movements and restore the smoothness of the old game. And yes, these aspects really kept me in the grip during the entire development of the game.  

### Game description  

I tried to make the game keep its authenticity through the way the snake moves, and in addition I added 4 levels of difficulty and a time limit on each of these difficulties.

When you start the game, you will have to navigate through the menu. What does this menu contain? Well, once you start the game, you will be greeted by a welcome message. You will go to the game menu by simply pressing a button where you will find the following options:  
 - `play game` where you will start from the difficulty chosen in the next option, 'settings', and then the difficulty will increase progressively up to 4, each round of the game having 35 seconds. 
 - In `settings` you will find the option to *enter your name*, to *set the difficulty*, *the brightness of the LCD*, *the brightness of the matrix*, *the sound in two states (on/off)* and the *reset* option in which you return to the default settings (chosen by me :) ). 
 - In the game menu, next to 'play game' and 'settings', you will find a `highscore` section where the 5 best players will be kept, including the last one who played the game
 - `about` section where there is information about the name of the game, the name of the programmer and his github (that is, the one you are looking at now). 
 - Oh, you even have a `how to play` section in which the way the game should be played is presented succinctly.  
 
 You will be able to see the menu in the video at the bottom of the page. after you lose or go through all the rounds of the game without blinking, you will be notified if you beat the highscore and what score you have.
 
### How to play

But what does the game contain and how do you have to play to become the best?

It's just snake. It's simple to play. You have to use the joystick to control the snake to eat as much food as it can.  

Now let me tell you about the difficulties:

At `difficulty 1`, it's the original snake game where you have to eat as much food as possible to grow and score points. You'll see, you'll grow quickly and it won't seem so simple, that's why I fixed a time of 35 seconds in which you have to resist to advance to the next difficulty.  
On `difficulty 2`, the food on the matrix has a life time of 2 seconds, so be alert for another 35 seconds to eat as much as you can.  
There will be another 35 seconds of `difficulty 3` in which you will have 2 obstacles. Does it seem hard to get here? Let me tell you about the `difficulty 4` in which you have to have 2 obstacles and the life of the food is 2 seconds. You draw the conclusion whether it is difficult or not now.  

At difficulty 1 the score increases by 1 for each piece of food eaten, at difficulty 2 it will increase by 2, at difficulty 3 it will increase by 3 and at difficulty 4 by 4.  

You will only lose if you collide with yourself or obstacles, so you will have the option to go through walls.  

### Used components

- Arduino Uno - 1
- 8x8 LED Matrix - 1
- Matrix Driver - 1
- 16x2 LCD Display - 1
- 74HC595 Shift Register - 1
- Potentiometer - 1
- Joystick - 1
- Buzzer - 1
- Resistors - 3
- Capacitors - 1
- BreadBoards - 1
- Wires - all I had

### Pictures of the Setup

<div align="center">
<img src="https://github.com/Dani780-C/SNAKE/blob/main/images/Snake_image_1.jpeg" width="600" height="400"/>  
</div>  
<br>
<div align="center">
<img src="https://github.com/Dani780-C/SNAKE/blob/main/images/Snake_image_2.jpeg" width="600" height="400" />
</div>

### Video 

Watch a video on [YouTube]() with the presentation of the project.

### Code

[Code](https://github.com/Dani780-C/SNAKE/blob/main/snake.ino) is right here!
