# Windows Programming Laboratory Work #4

## Title
Windows Timer. Animation.

## Contents
- Windows timer

## Mandatory Objectives
- Create an animation based on Windows timer which involves at least 5 different drawn objects

## Objectives With Points (completed)
- Increase and decrease animation speed using mouse wheel (2 pt)
- Solve flicking problem (2 pt)
- Add animated objects which interact with each other (2-6 pt) :
- Few balls which have different velocity, moving angles, and colors add balls with mouse,
- Animate a Nyan Cat that leaves a rainbow tail (Math.floor(+30% for task with interacting objects))

## Application Creation Steps
First I've sketched my whole application. After that I designed a Class for the floating circles. When users cliks hew circles are generated with random speed, size, velocity and colour. When they hit the borders the color and direction is changed. Also there is a Rasta nyan leaving a rainbow trail (btw the applications also has a Rasta nyan theme song). When the user scrolls then the velocity is changed.

![Result images](https://github.com/TUM-FAF/WP-FAF-111-Terman-Sergiu/blob/master/Lab%234/Capture.PNG)

## What I've used
- I've used the brushes
- Ellipse function
- Rectangle function
- PolyBezier function
- Timer
- Bitmaps

The process was really simple. The main problem was to design the class that will do all the stuff. Then combining simple functions I was able to create something more comlex.

## Conclusion
In this lab I first used OOP in winapi. It is pretty cool, because you put all funtcionalities with the class. Combining simple functions in a class is awesome. In main file you only use obejcts. Drawing the Rasta nyan was not included in my class. It was drawn manual.
(The lab is under time destorsion)