#include "HX711.h"
#include <SPI.h>
#include <Ethernet.h>

#include <Servo.h>
 
int servopin = 4;
int sensorpin=13;
Servo servo;
int angle = 0;


///Ethernet part

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
// Enter the IP address for Arduino, as mentioned we will use 192.168.0.16
// Be careful to use , insetead of . when you enter the address here
IPAddress ip(192,168,2,102);

//
int photocellPin = 0;  // Analog input pin on Arduino we connected the SIG pin from sensor
int photocellReading=0;  // Here we will place our reading

char server[] = "192.168.2.103"; 

// Initialize the Ethernet server library
EthernetClient client;

////////////////////////////////////////


HX711 cell(3 ,2);

int brifgeopen=0;
//int sensorpin=13;
int ObjHeight=0;
int maxHeight=0;
long val = 0;
int count = 0;                                          // count = 0
char input[13];                                         // character array of size 12 
float weight;
float maxweight;
float height;
float amount;
boolean flag = 0;                                       // flag =0


int iopin1=5;
int iopin2=6;
int iopin3=7;
int iopin4=8;
int iopin5=9;
int iopin6=10;
int iopin7=11;
int iopin8=12;


void calWeight()
{
  val = cell.read();
  weight=((val - 8434056) / 34246.0f * 45 )+10656;
}

void calheight()
{
     ObjHeight=0;
  //delay(30);
  if(digitalRead(iopin1)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin2)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin3)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin4)==HIGH)
  {
      ObjHeight=ObjHeight+1;

  }
  //delay(30);
  if(digitalRead(iopin5)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin6)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin7)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  if(digitalRead(iopin8)==HIGH)
  {
      ObjHeight=ObjHeight+1;
  }
  //delay(30);
  height=(float)ObjHeight;
  //Serial.println("Height of the Object is : ");
  //Serial.println(ObjHeight);  
}

void setup() {
  Serial.begin(9600);
  servo.attach(servopin);
  pinMode(13,INPUT);
  
  pinMode(iopin1, INPUT);
  pinMode(iopin2, INPUT);
  pinMode(iopin3, INPUT);
  pinMode(iopin4, INPUT);
  pinMode(iopin5, INPUT);
  pinMode(iopin6, INPUT);
  pinMode(iopin7, INPUT);
  pinMode(iopin8, INPUT);
 


  // start the Ethernet connection
  Ethernet.begin(mac, ip);
}

 
void loop() {

if(Serial.available())
   {
      count = 0;
      while(Serial.available() && count < 12)          // Read 12 characters and store them in input array
      {
         input[count] = Serial.read();
         count++;
         delay(5);
      }
     Serial.print(input);                             // Print RFID tag number 
      
      //////////////////////////////////////////////////////////////////////////////////


    calheight();
    maxHeight=ObjHeight;
    for(int i=0;i<50;i++)
    {
        calheight();
        if(ObjHeight>maxHeight)
          {
            maxHeight=ObjHeight;
          }
         else if(ObjHeight<maxHeight)
         {
         // break;
         }
         delay(30);
      
    }
    Serial.println(" ");
    Serial.println(" Height is ");
    Serial.println(maxHeight);
    height=maxHeight;
    
    ///////////////////////////////////////////////////////////////////////////////
    
  
          //Serial.println();
         calWeight();
         for(int i=0;i<50;i++)
         {
          maxweight=weight;
          calWeight();
          if(weight>10)
          {
            if(weight>maxweight)
            {
              maxweight=weight;
            }
            if(weight<maxweight)
            {
              break;
            }
          }
          delay(30);
         }
         weight=maxweight;
        Serial.println(weight);
        brifgeopen=1;
        

  amount=123.45;

  /////Sending data to database done here


   if (client.connect(server, 80)) {
    client.print("GET /New Folder/writedata_ALL.php?"); // This
    client.print("RFID="); // This
    client.print(input); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.print("&Weight="); // This
    client.print(weight);
    client.print("&Height="); // This
    client.print(height);
    client.print("&AmountCharged="); // This
    client.print(amount);
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.2.103") ; // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server

  }

  else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }


 
  // Give the server some time to recieve the data and store it. I used 10 seconds here. Be advised when delaying. If u use a short delay, the server might not capture data because of Arduino transmitting new data too soon.
  delay(1000);

  /////End of database module */

  
  
   }
  
   
 
}

