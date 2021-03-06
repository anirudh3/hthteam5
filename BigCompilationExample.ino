#include "NAxisMotion.h"        //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>

NAxisMotion mySensor;                 //Object that for the sensor
unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 40;          //To stream at 25Hz without using additional timers (time period(ms) =1000/frequency(Hz))
bool updateSensorData = true;         //Flag to update the sensor data. Default is true to perform the first read before the first stream

int ledPin = 5;      // LED connected to digital pin 9
//int analogPin = 5;   // potentiometer connected to analog pin 3
//int val = 100;         // variable to store the read value

float val = 0;

void setup() //This code is executed once
{
  //Peripheral Initialization
  Serial.begin(115200);           //Initialize the Serial Port to view information on the Serial Monitor
  I2C.begin();                    //Initialize I2C communication to the let the library communicate with the sensor. 
  //Sensor Initialization
  mySensor.initSensor();          //The I2C Address can be changed here inside this function in the library
  mySensor.setOperationMode(OPERATION_MODE_NDOF);   //Can be configured to other operation modes as desired
  mySensor.setUpdateMode(MANUAL);	//The default is AUTO. Changing to manual requires calling the relevant update functions prior to calling the read functions
  //Setting to MANUAL requires lesser reads to the sensor
  mySensor.updateAccelConfig();
  updateSensorData = true;
  Serial.println();
  Serial.println("Default accelerometer configuration settings...");
  Serial.print("Range: ");
  Serial.println(mySensor.readAccelRange());
  Serial.print("Bandwidth: ");
  Serial.println(mySensor.readAccelBandwidth());
  Serial.print("Power Mode: ");
  Serial.println(mySensor.readAccelPowerMode());
  Serial.println("Streaming in ...");	//Countdown
  Serial.print("3...");
  delay(1000);	//Wait for a second
  Serial.print("2...");
  delay(1000);	//Wait for a second
  Serial.println("1...");
  delay(1000);	//Wait for a second

  pinMode(ledPin, OUTPUT);   // sets the pin as output  

}

void loop() //This code is looped forever
{
  if (updateSensorData)  //Keep the updating of data as a separate task
  {
    mySensor.updateAccel();        //Update the Accelerometer data
    mySensor.updateLinearAccel();  //Update the Linear Acceleration data
    mySensor.updateGravAccel();    //Update the Gravity Acceleration data
    mySensor.updateCalibStatus();  //Update the Calibration Status
    updateSensorData = false;
  }
  if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();

    Serial.print("Time: ");
    Serial.print(lastStreamTime);
    Serial.print("ms ");

//    Serial.print("aX: ");
//    Serial.print(mySensor.readAccelX()); //Accelerometer X-Axis data

//    Serial.print(" aY: ");
//    Serial.print(mySensor.readAccelY());  //Accelerometer Y-Axis data

//    Serial.print(" aZ: ");
//    Serial.print(mySensor.readAccelZ());  //Accelerometer Z-Axis data

//    Serial.print(" lX: ");
//    Serial.print(mySensor.readLinearAccelX()); //Linear Acceleration X-Axis data

//    Serial.print(" lY: ");
//    Serial.print(mySensor.readLinearAccelY());  //Linear Acceleration Y-Axis data

//    Serial.print(" lZ: ");
//    Serial.print(mySensor.readLinearAccelZ());  //Linear Acceleration Z-Axis data


    
    Serial.print(" gX: ");
    Serial.print(mySensor.readGravAccelX()); //Gravity Acceleration X-Axis data

    Serial.print(" gY: ");
    Serial.print(mySensor.readGravAccelY());  //Gravity Acceleration Y-Axis data

    Serial.print(" gZ: ");
    Serial.print(mySensor.readGravAccelZ());  //Gravity Acceleration Z-Axis data

    Serial.print("      C: ");
    Serial.print(mySensor.readAccelCalibStatus());  //Accelerometer Calibration Status (0 - 3)

    Serial.println();

    updateSensorData = true;

    // For 1st PWM output
    val = mySensor.readGravAccelX() + 10;
    Serial.print("val:");
    Serial.print(val);
    if(val > 0)
    {
      val = 255 * val/20;
    }
    else
    {
      val = 0.0;
    }

//    val = analogRead(analogPin);   // read the input pin

    analogWrite(ledPin, val );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255


    
  }
}
