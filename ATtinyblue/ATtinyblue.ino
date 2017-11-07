#include <SoftwareSerial.h>  //Software Serial Port
#define RxD 6
#define TxD 5

const int FSR_PIN = A2; // Pin connected to FSR/resistor divider

const float VCC = 3.3; // Measured voltage of Ardunio 5V line
const float R_DIV = 100.0; // Measured resistance of 3.3k resistor
 
SoftwareSerial blueToothSerial(RxD,TxD);
 
void setup() 
{ 
  pinMode(FSR_PIN, INPUT);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection(); 
} 
 
void loop() 
{
  int fsrADC = analogRead(FSR_PIN);
  char recvChar;  

  if (fsrADC != 0)  {
    float fsrV = fsrADC * VCC / 1023.0;
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance

    if (fsrR <= 600) 
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.000000642857;

  byte B[8] = {0,0,0,0,0,0,0,0};
  for(int i =0; i<8; i++){
    B[7-i]=(int)force % 10;
  force = force / 10;
}

    if(blueToothSerial.available()){
      for(int i=0; i<8; i++){
        //전송포멧
      blueToothSerial.println(B[i]);
      }      
    }
    delay(500);
  }else{
    if(blueToothSerial.available()){
      blueToothSerial.println("0");
    }
    delay(500);
  }
}

  void setupBlueToothConnection(){
  blueToothSerial.begin(115200); //Set BluetoothBee BaudRate to default baud rate 38400
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}
