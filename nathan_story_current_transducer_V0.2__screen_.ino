//Nathan Story
//University of Missouri Ag 
//2022SP-AG_S_TCH-3225-01 (Sensors)

//include libraries for I2C and screen graphics
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include <splash.h>
 
//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) with address 0x3C
#define OLED_RESET -1
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDR 0x3C

//Create screen object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Set analog pin to be used for input.
//Instantiate global decimal variable for analog input
//Instantiate global int variable for holding digital value
const byte analogIn_hall = A0;                                
float analogVal; 
int digitalVal;                           

//Setup 
void setup() {
  //Begin serial connection
  Serial.begin(115200);

  //Start the display and clear it if anything is on it
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  //Set the analog pin as an input to start reading in values
  pinMode(analogIn_hall,INPUT);                                
}

//loop
void loop() {
  //----hall-sensor--------------------------------------------------------------
  //Get analog value from sensor
  analogVal = analogRead(analogIn_hall);    
  //Call function to convert from analog to digital direction              
  digitalVal = convertAnalogToDirection(analogVal); 
  //Convert ints to strings & concatanate for print output on serial
  //output includes digital (field detected) value, analo value, and direction of field.      
  Serial.println("Detected: " + String(abs(digitalVal))      
  + " Strength: " + String(analogVal) 
  + " Direction: " + String(digitalVal)); 
  //-----------------------------------------------------------------------------      


  //--Graphics-------------------------------------------------------------------
  //Updates display with real time values
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Analog: " + String(analogVal)); //Displays analog value
  display.setCursor(0,20);
  display.print("Amps: " + String(convertAmps(analogVal))); //Displays Amps
  display.setCursor(0,40);
  display.print("Volts(at hall): " + String(analogVal*4.61/1023)); // Converts & Displays analog to volts at the hall sensor
  display.display();
  //-----------------------------------------------------------------------------
}


//convert analog to direction based on value < 527 or > 528. My setup bounces between 527 and 528 for base analog value (0 amps).
//usually stayed on 527 once I secured all connections
//returns direction of 0 if no amps are detected
//returns -1 for negative amps and 1 for positive amps
float convertAnalogToDirection(float analogInput)             
{
  if(analogInput > 528) return 1;
  else if (analogInput < 527) return -1;
  else return 0;
}

//Converts the analog value to amps based on linear regression equation calculated in excel
//This equation may change based on what kind of micro controller is being used and the inherent circuit properties like resistance
//Pull up or pull down resistors may help but I did not need them in my instance because the arduino has them built in
float convertAmps (float analogInput)
{
  if (analogInput == 527) return 0.00; 
  else return ((abs(analogInput-527) - 0.7278)/3.0409);
}
