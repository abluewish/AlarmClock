//The code is modified from code from https://create.arduino.cc/projecthub/plouc68000/simplest-uno-digital-clock-ever-4613aa

#include "LiquidCrystal.h"

// This defines the LCD wiring to the DIGITALpins
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Digital LCD Constrast setting
int cs=9;// pin 9 for contrast PWM
const int contrast = 100;// default contrast

// set the initial Time to be 01:05:29
int h=1;
int m=5;
int s=29;


// Time Set Buttons
int button1;
int button2;
int button3;

// Pins definition for Time Set Buttons
int hs=11;// pin 0 for Hours Setting
int ms=12;// pin 1 for Minutes Setting
int reset=13;

// Backlight Time Out 
const int Time_light=600; // Let the light keep on for 60 seconds
int bl_TO=Time_light;// Backlight Time-Out
int bl=10; // Backlight pin
const int backlight=120; 

// For accurate Time reading, use Arduino Real Time Clock and not just delay()
static uint32_t last_time, now = 0; // RTC


void setup()
{
  lcd.begin(16,2);
  pinMode(hs,INPUT_PULLUP);// avoid external Pullup resistors for Button 1
  pinMode(ms,INPUT_PULLUP);// and Button 2
  pinMode(reset,INPUT_PULLUP); //Button 3
  analogWrite(cs,contrast);// Adjust Contrast VO
  analogWrite(bl,backlight);// Turn on Backlight
  now=millis(); // read RTC initial value  
  pinMode(8,OUTPUT);
}


void loop()
{ 
  lcd.begin(16,2);// every second
// Update LCD Display
// Print TIME in Hour, Min, Sec 
 lcd.setCursor(0,0);
 lcd.print("Time ");
 if(h<10)lcd.print("0");// always 2 digits
 lcd.print(h);
 lcd.print(":");
 if(m<10)lcd.print("0");
 lcd.print(m);
 lcd.print(":");
 if(s<10)lcd.print("0");
 lcd.print(s);

 
 lcd.setCursor(0,1);// for Line 2
 lcd.print("Alarm clock");
  for ( int i=0 ;i<5 ;i++)// make 5 time 200ms loop, for faster Button response
{

  while ((now-last_time)<200) //delay200ms
  { 
    now=millis();
  }
 // inner 200ms loop
 last_time=now; // prepare for next loop 

 // read Setting Buttons
 button1=digitalRead(hs);// Read Buttons
 button2=digitalRead(ms);

 //Backlight time out 
 bl_TO--;
 if(bl_TO==0)
 {
  analogWrite(bl,0);// Backlight OFF
  bl_TO++;
 }
 
 // Hit any to activate Backlight 
 if(  ((button1==0)|(button2==0)) & (bl_TO==1)  )
 {
  bl_TO=Time_light;
  analogWrite(bl,backlight);
  }
  else
 // Process Button 1 or Button 2 when hit while Backlight on 
 {
  if(button1==0){
   h=h-1;
   bl_TO=Time_light;
   analogWrite(bl,backlight);
  }

 if(button2==0){
  s=29;
  m=m-1;
  bl_TO=Time_light;
  analogWrite(bl,backlight);
  }

 if((button1==0)|(button2==0))// Update display if time set button pressed
 {
  // Update LCD Display
  // Print TIME in Hour, Min, Sec
  lcd.setCursor(0,0);
  lcd.print("Time ");
  if(h<10)lcd.print("0");// always 2 digits
  lcd.print(h);
  lcd.print(":");
  if(m<10)lcd.print("0");
  lcd.print(m);
  lcd.print(":");
  if(s<10)lcd.print("0");
  lcd.print(s);

 
  lcd.setCursor(0,1);// for Line 2
  lcd.print("Alarm clock");
 }


 } // end if else
}// end for



// outer 1000ms loop

 s=s-1; //increment sec. counting
    
   
// ---- manage seconds, minutes, hours am/pm overflow ----
 if(s==-1){
  s=59;
  m=m-1;
 }
 if(m==-1)
 {
  m=59;
  h=h-1;
 }
 if(h==-1)
 {
  h=24;
 } 
 if(s==0 & m==0 & h==0){
  h=24;
  m=59;
  s=58;
  digitalWrite(8,HIGH);
  tone(8,880);
  delay(200);
  tone(8,880);
  delay(200);
  button3=digitalRead(reset);
  while(button3==1){
    tone(8,880);
    delay(200);
    tone(8,880);
    delay(200);
    noTone(8);
    delay(500);
    button3=digitalRead(reset);
  }
 }
 else{
 noTone(8);
 }

 
// Loop end
}
