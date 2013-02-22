#Windows Programming

student gr. FAF-111 : Terman Sergiu

#Topic: Windows programming basics
-------------------------------
##Purpose
- Event-driven programming (message-oriented)
- API
- Windows application development

##Additional purpose

- Version control systems (git)
- Code style
- Debugging
- Virtual machine
- 3-tier architecture
- Testing

###What I've Done

In this laboratory work I created a basic window window with 2 buttons 2 edit boxes and 2 texts. I followed the mandatory points. And also I've added some interaction between the elements.

1. When the button "Add Food" is presed the text is inputed in the bigger Edit Box
2. At every adding the "Number" status is incremented with one so the user knows how many items he have added
3. When button "Funny" is pressed it checks how many items you've added, and depending on them pops a message box with a text. Check and you will see what does it shows.
4. There is a costumized horisontal resizing event. It streches the element depending on window size. Also there is a limit of window resize.

###Conclusion
After this laboratory work I understood the concept of message-oriented programming. Processing the messages and manipulating the parameters helped me to create a window and some basic elements. Also this experience gave me an image of Windows API, and how it is structured. The main problem for me was working with the given parameters from WindowProcedure. For example a tricky moment is when you process a message you should know that you can cast the parameters into needed type of variables.