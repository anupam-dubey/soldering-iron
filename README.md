# Temperature Cnntrolled soldering iron
<img src="images\2.jpg"  width=200/></br> 
<img src="images\1.jpg" width=150/>
<img src="images\diagram.jpg" width=150/>


## Description
USES popular nano mega328P ,The device controlles Temperature of  soldering iron based on hakko 907 models with inbuilt temperature sensor
Four Wires used in soldering iron contains heater and thermocouple sensor, heater wire is connected to arduino pin 11 for pwm via popular IRF44 mosfet
sensor wire goes to LM358 opamp for gain of 45 first stage and 2 for second stage gives a total gain of 90. It is adviced that before makeing this project be sure about thermocouple and heater wires and check the thermocouple using multimeter for determination of gain value. 
To calibrate the soldering station I use an external thermocouple with a multimeter that can measure temperatures. 
(Although other devices could be used)
The procedure consists of the following steps.
1. Rotate the soldering station's encoder to set a temperature value. Wait until the actual temperature stabilizes (the actual temperature = the set temperature). Let Xi be the set temperature.
2. Using an external thermocouple measure the temperature of the soldering iron's tip. Let Yi be the temperature measured by an external device.
3. Using different set temperatures repeat 1-2 steps a few times . i=1,2,...n
4. Now we have the set of points (X1,Y1),(X2,Y2),....,(Xn,Yn)
5. Find a function that maps Xi to Yi for all i=1,..,n.
If we have only 2 points (X1,Y1),(X2,Y2), then we can draw the line (Y=a*X+b) between the points. This line will be the solution to the problem.  If we have more than 2 points, we should use the method of least squares.
6. Use the function to convert ADC's numbers to actual temperatures.

|No |	Xi set temperature	|Yi temperature measured by an external device|
|---|	------------------	|---------|
|1	|40                 	|52|
|2	|80	                    |95|
|3	|100                    |125|
|4	|160	                |195|

We can use an online service to find the function.
Let's open wolframalpha.com. Then we should type the string "linear fit {40,52},{80,95},{100,125},{160,195}" in the edit box.
The solution for our problem is y= 1.202 x+2.56.
I used this function in the arduino's sketch to convert ADC's numbers to actual
temperatures.