/*
 Resposive LED Panels

 This sketch controls Interactive LED panels designed by Rodney Trusty
 Instagram: @Rodneytrusty
 
 Created September 18, 2019
 by Rodney Trusty
 */

#include <Adafruit_NeoPixel.h>

/*Set Defines for Row and Column control pins*/
#define ROW_1 8
#define ROW_2 10
#define ROW_3 11
#define ROW_4 12
#define NUM_ROWS 4       //Number of rows in IR array

#define COLUMN_1 2
#define COLUMN_2 3
#define COLUMN_3 4
#define COLUMN_4 5
#define NUM_COLUMNS 4    //Number of columns in IR array


/*Defines for Analog read pins*/
#define READ_1 A0
#define READ_2 A1
#define READ_3 A2
#define READ_4 A3
#define NUM_READ 4       //Number of analog reads in IR array

#define LED_SIGNAL 9     //Addressable LED Signal Pin

#define NUM_PIXELS (NUM_ROWS * NUM_COLUMNS)

#define LED_BRIGHTNESS 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_SIGNAL, NEO_GRB + NEO_KHZ800);

byte red = 80;

byte green = 255;

byte blue = 125;

byte rows[8] = {ROW_1, ROW_2, ROW_3, ROW_4};                

byte columns[8] = {COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4}; 

byte readVal[8] = {READ_1, READ_2, READ_3, READ_4};

short value_with_ir = 0;

short value_difference = 0;

short calibration_values[16] = {0,0,0,0,
                                0,0,0,0,
                                0,0,0,0,
                                0,0,0,0};
                               
unsigned short threshold = 60;         //Threshold to activate LEDs. Lower value = more sensitivity

byte pixel_num = 0;

void setup(){

Serial.begin(9600);                            //Set up Serial for debugging

pinMode(LED_SIGNAL, OUTPUT);                   //Set LED Signal pin to output. 
                                               //This pin controls the addressable LEDs

for(byte x = 0; x < NUM_ROWS; x++)             //Set Row pins to output
   {
    pinMode(rows[x], OUTPUT);
   }

for(byte x = 0; x < NUM_COLUMNS; x++)         //Set Column pins to output
   {
    pinMode(columns[x], OUTPUT);
   }

for(byte x = 0; x < NUM_READ; x++)               //Set Analog Read pins Input
   {
    pinMode(readVal[x], INPUT); 
   }

  strip.begin();                                 //Initialize Addressable LED library
  
  strip.setBrightness(LED_BRIGHTNESS);           //Set Addressable LED Brightness
  
  strip.show();                                  //Initialize all Pixels to off

  ir_calibrate();                                //Calibrate IR Sensors
                                                 //Make sure no objects are present during calibration
  
  }


  
void loop(){
            

for(byte x = 0; x < NUM_COLUMNS ; x++)             //Begin Loop through Columns
   {
    
       digitalWrite(columns[x], HIGH);             //Set current Column pin HIGH
       
       for(byte y = 0; y < NUM_ROWS; y++)          //Begin Loop through Rows     
          {

          digitalWrite(rows[y], HIGH);             //Set Current Row HIGH
          
          delayMicroseconds(100);                  //Mandatory Delay
          
          value_with_ir = analogRead(readVal[y]);  //Read analog value of IR Detector "WITH" IR Emitter
           
          digitalWrite(rows[y], LOW);              //Set Current Row LOW
          
          pixel_num = (x*NUM_COLUMNS)+(NUM_ROWS - (y+1));   //Calculate the corresponding LED
 
          if(value_with_ir > calibration_values[pixel_num]) 
            { 
              
              value_difference = value_with_ir - calibration_values[pixel_num];
              
            }         
            
        else{
          
              value_difference = 0;
              
            }

          if(value_difference > threshold)  
            {  
            //Change the values of red, green and blue as you wish anywhere in the program for cool effects
            strip.setPixelColor(pixel_num,red, green, blue);        //Turn ON the corresponding LED
             
            }
            
        else{                                   
          
            strip.setPixelColor(pixel_num,0,0,0);            //Turn OFF the corresponding LED
            
            }

          }
          
       digitalWrite(columns[x], LOW);        //Turn OFF Current ROW
       
    }
       
           strip.show();                     //Display Changes made to addressable LEDs
           
           }

  
void ir_calibrate(void)
                 {
                  
                 
    short ir_averages[NUM_PIXELS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

for(byte h = 0; h < 10; h++)
   {
       
    for(byte i = 0; i < NUM_COLUMNS; i++)
       {
        
        digitalWrite(columns[i], HIGH);                    //Set current Column pin HIGH

        for(byte j = 0; j < NUM_ROWS; j++)
           {
    
          digitalWrite(rows[j], HIGH);                      //Set current Row HIGH
          
          delayMicroseconds(100);                           //Mandatory Delay
          
          value_with_ir = analogRead(readVal[j]);           //Read analog value of IR Detector "WITH" IR Emitter
           
          digitalWrite(rows[j], LOW);                       //Set current Row LOW

          pixel_num = (i*NUM_COLUMNS)+(NUM_ROWS - (j+1));   //Calculate the corresponding LED number

          ir_averages[pixel_num] += value_with_ir;       //Add read value to total for position
           
           }
                  
       }

    for(byte m = 0; m < NUM_PIXELS; m++){
      
      calibration_values[m] = (ir_averages[m]/10);          //take average of accumulated detector reads
      
      } 

      
   }
                 }
