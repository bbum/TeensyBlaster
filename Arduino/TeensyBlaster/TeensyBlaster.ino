// https://github.com/kroimon/Arduino-SerialCommand
// Which is an updated fork of the link below with some nice additions:
// http://husks.wordpress.com/2011/05/23/a-minimal-arduino-library-for-processing-serial-commands/
#include <SerialCommand.h>

// http://arcfn.com/ir
#include <IRremote.h>

/*
 Global Configuration
 */
 
#define arduinoLED 11
IRsend irsend;
SerialCommand SCmd;

/*
 Convenience
 */

void serialprintf(char *format,...)
{
  char buff[128];
  va_list args;
  va_start (args,format);
  vsnprintf(buff,sizeof(buff),format,args);
  va_end (args);
  buff[sizeof(buff)/sizeof(buff[0])-1]='\0';
  Serial.print(buff);
}

void OK()
{
  Serial.println("OK"); 
}

void unrecognized()
{
  Serial.println("Unrecognized command."); 
}

/*
 Commands
 */
 
void testCommand()
{
  /* Write some sequenced NEC goop -- useful for testing devices that can receive arbitrary IR */
  digitalWrite(arduinoLED, HIGH);  
  irsend.sendNEC(0x56781234, 32);
  irsend.sendNEC(0x12345678, 32);  
  digitalWrite(arduinoLED, LOW);
  OK();
}

void sonyTVPower()
{
  digitalWrite(arduinoLED, HIGH);
  /* Sony commands are sent 3 times.  Just because. */
  irsend.sendSony(0xa90, 12);
  delay(100);
  irsend.sendSony(0xa90, 12);
  delay(100);
  irsend.sendSony(0xa90, 12);
  delay(100);
  digitalWrite(arduinoLED, LOW);  
}

void led0()
{
  digitalWrite(arduinoLED, LOW);
  OK();
}

void led1()
{
  digitalWrite(arduinoLED, HIGH);
  OK();
}

/*
 Setup & Event Handling
 */

void setup()
{
  pinMode(arduinoLED,OUTPUT);
  digitalWrite(arduinoLED,LOW);
  Serial.begin(9600); 

  SCmd.addCommand("led1",led1);
  SCmd.addCommand("led0",led0);
  SCmd.addCommand("sp",sonyTVPower);

  SCmd.addCommand("TEST",testCommand);
  SCmd.addDefaultHandler(unrecognized); 
}

void loop() {
  SCmd.readSerial();
}

