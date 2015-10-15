# tcc (Teensy Command Controller)

Last update: Thu Oct 15 15:32:51 JST 2015

![tcc_image](https://raw.githubusercontent.com/uobikiemukot/tcc/master/img/tcc_small.jpg)

## description

-	teensy firmware for command controller
-	parse MacroLua like input from serial and output joystick command
-	based on teensy cores (https://github.com/PaulStoffregen/cores) and teensy-template (https://github.com/apmorton/teensy-template)

## hardware

-	teensy3.1 * 1
-	serial usb converter * 1
-	tactile switch * 2
-	jumper wire * n
-	breadboard * 1

## macro

### buttons

press button on first frame and release on next frame

- 	S: start
-	1: button1
-	2: button2
-	3: button3
-	4: button4
-	5: button5
-	6: button6
-	7: button7
-	8: button8
-	9: button9

### directions

-	U: upper
-	L: left
-	R: right
-	D: down
-	RD: lower right
-	LD: lower left
-	RU: upper right
-	LU: upper left

### only press/release

-	_n: hold n button or direction
-	^n: release n button or direction

### wait

-	Wn: wait n frame

###  others

-	.: 1 frame advance
-	,: simultaneous input

## macro samples

-	R.D.RD.6.: shoryuken command
-	D.RD.R.4.: hadouken command
-	D.LD.L.1.: tatsumaki command
-	R.W2.R.W16.4.W18.RU.W1.8.L,1,2.W2.4.: JS > JDFC > JS
-	U.W4.R,8.L,1,2.W2.R.W5.1.W16.4.: JDFC > dash > JK > JS
-	_1,_2.W20.D.RU.W3.8.L.W1.^1,^2.: 2 buttons hold JDFC

## how to use

build

~~~
 $ make
 $ make upload # press teensy's reboot button
~~~

send macro command to teensy like this

~~~
 $ echo "R.W2.R.W16.4.W18.RU.W1.8.L,1,2.W2.4." > /dev/ttyUSB0
~~~

then, push tactile switch (connected to 15 pin)!

## license

The MIT License (MIT)

Copyright (c) 2015 haru (uobikiemukot at gmail dot com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
