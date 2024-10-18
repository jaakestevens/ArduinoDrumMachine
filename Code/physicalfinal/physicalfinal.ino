/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/
#include <LiquidCrystal.h>

//initialises pins for lcd
LiquidCrystal lcd(7,8,9,10,11,12);

//joystick pins
const int X_pin = A0;
const int Y_pin = A1;
const int switchPin = 4;


//X Joystick variables
int Xstick;
int Ystick;
int Xval = 0;
int Yval = 0;
int switchVal;

//selects step
int select;

//current step
int currentStep = 0;

//plays buzzer

//starting position of emptyblock
int emptyBlockPos = 0;

//step indicator (star)
byte steps[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

//full block(selected)
byte block[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

//select block (cursor)
byte emptyBlock[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};

//array of selected steps. Initialised as a simple house drum pattern
int stepArray[3][16] = {{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0},
                        {0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0},
                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1}};


// the setup routine runs once when you press reset:
void setup() {

  // initialise lcd screen
  lcd.begin(16,2); 
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  //creates characters
  lcd.createChar(0, steps);
  lcd.createChar(1, block);
  lcd.createChar(2, emptyBlock);

  //switch pin
  pinMode(4,INPUT);
  digitalWrite(4,HIGH);


//Opening loading screen and instructions
  for(int i = 0; i < 8; i++)
  {
    lcd.setCursor(0, 0);
    lcd.write("Mini Drum");
    lcd.setCursor(0, 1);
    lcd.write("Sequencer");
    delay(100);
  }

  lcd.clear();

  for(int i = 0; i < 15; i++)
  {
    lcd.setCursor(0, 0);
    lcd.write("Move left/right");
    lcd.setCursor(0, 1);
    lcd.write("To choose steps");
    delay(100);
  }

  lcd.clear();

  for(int i = 0; i < 15; i++)
  {
    lcd.setCursor(0, 0);
    lcd.write("Click to add");
    lcd.setCursor(0, 1);
    lcd.write("or remove steps");
    delay(100);
  }

  //initialises lcd cursor
  lcd.setCursor(1, 1);
}
void loop() {
  
  //clears the screen
  lcd.clear();

  //reads the switch value
  switchVal = digitalRead(4);

  //Serial.println(switchVal);

  //reads the x input of joystick
  Xstick = analogRead(X_pin);

  //this is so that you can only move left and right or up and down seperately(not at the same time)
  if(Ystick > 300 && Ystick < 700)
  {
  if(Xstick > 800 && Xval != 15)
  {
    Xval += 1; //change this to -= 1 if you wish to change alternated control
  }

  if(Xstick < 300 && Xval != 0)
  {
    Xval -= 1; //change this to += 1 if you wish to change alternated control
  }
  }

//reads y input of joystick
  Ystick = analogRead(Y_pin);

//Same logic implemented for the x axis
if(Xstick > 300 && Xstick < 700)
{
  if(Ystick > 800 && Yval != 2)
  {
    Yval += 1;

    if(Yval == 0)
  {
  lcd.setCursor(0,0);
    lcd.write("Kick"); //this will write the name of the banks once if a change in bank has been made
  }
  if(Yval == 1)
  {
  lcd.setCursor(0,0);
    lcd.write("Snare");
  }
  if(Yval == 2)
  {
  lcd.setCursor(0,0);
    lcd.write("Hat");
  }

  }

  if(Ystick < 300 && Yval != 0)
  {
    Yval -= 1;
    if(Yval == 0)
  {
  lcd.setCursor(0,0);
    lcd.write("Kick");
  }
  if(Yval == 1)
  {
  lcd.setCursor(0,0);
    lcd.write("Snare");
  }
  if(Yval == 2)
  {
  lcd.setCursor(0,0);
    lcd.write("Hat");
  }
  }
  
}

  //increments current step in sequence
  currentStep++;


  //make sure pattern runs from 0 to 15
  if(currentStep == 16)
  {
    currentStep = 0;
  }


  //moving step indicator(star)
  lcd.setCursor(currentStep, 0);
  lcd.write(byte(0));

  //cursor indicator
  lcd.setCursor(Xval, 1);
 lcd.write(byte(2)); //full block


//draws steps
  for(int i = 0; i < 16; i++)
  {
    if(Yval == 0)
    {
      if(stepArray[0][i] == 1)
      {
        lcd.setCursor(i, 1);
        lcd.write(byte(1));
      }
    }
    if(Yval == 1)
    {
       if(stepArray[1][i] == 1)
      {
        lcd.setCursor(i, 1);
        lcd.write(byte(1));
      }
    }
    if(Yval == 2)
    {
       if(stepArray[2][i] == 1)
      {
        lcd.setCursor(i, 1);
        lcd.write(byte(1));
      }
    }
  }

 if(switchVal == 0) // adds/removes steps from the array 
 {
    if(stepArray[Yval][Xval] == 0)
    {
      stepArray[Yval][Xval] = 1;
    }
    else
    {
      stepArray[Yval][Xval] = 0;
    }
 }

//outputs the array of values from steps
Serial.print(stepArray[0][currentStep]);
Serial.print(" ");
Serial.print(stepArray[1][currentStep]);
Serial.print(" ");
Serial.println(stepArray[2][currentStep]);


//delay time sets the speed of the sequencer ( a higher delay time will mean that the pattern is less stable and the timing will become more skewed)
  delay(100);
}
