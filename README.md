# Live_Foosball
IOT Project that tracks the scores of an ongoing foosball game and displays them on a website

2 hcsr04 proximity sensors are used (1 for each goal post) to increment the score of the scoring team.
1 seven segment display is used to monitor a certain team's score (The output pins that light the seven segment display are named as capital letters from A to G in the arduino file).
The Arduino Yun connects to the IBM Watson IOT platform on IBM Cloud and sends the score information.
The information is sent to a web page template using the Node-Red platform.
When the web page template -which is also included in the repository- receives the score information, a javascript code updates the contents of the webpage.
The web page refreshes itself every 1 second so that it keeps the score almost in real time.
