
int outputValue = 0;                               //the motor is initiated at zero

unsigned long time = 0;
const int sampleWindow = 50;                       // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;                               // 'sample' = analog reading of microphone

#include <AccelStepper.h>                          // Library used to drive the stepper motor
 
AccelStepper stepper;                              // Calling the library

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);                       // Maximum possible speed of stepper
}

void loop() {
 
  unsigned long startMillis= millis();             // Start of sample window
  unsigned int peakToPeak = 0;                     // Peak-to-peak of signal
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
   
                                                   // Collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
  sample = analogRead(0);                          // Obtaining analog values from microphone
  if (sample < 1024)                               // Toss out spurious readings
  {
  if (sample > signalMax)
  {
  signalMax = sample;                              // Save just the max levels
  }
  else if (sample < signalMin)
  {
  signalMin = sample;                              // Save just the min levels
  }
  }
  }
  peakToPeak = signalMax - signalMin;              // Max - min = peak-peak amplitude
  
   
  Serial.println(peakToPeak);                      // This was used to read values in a certain room and calibrate to the noise levels
  
                                                   // Noise levels in room were usually under 80;  
  if ((100 < peakToPeak) && (peakToPeak < 400))    // Noise level between 100-400 trigger motor to turn
    {outputValue = 100;                            // Speed of 100rpm
    Serial.println(outputValue);
    stepper.setSpeed(outputValue);
    time = millis();
    while (millis() - time < 5000)                 // Motor runs for 5 seconds then stops until mic value is > 100
    {stepper.runSpeed();}
    stepper.stop();
    }

  if (peakToPeak > 400)                            // Motor runs at different speed if mic value > 400
    {outputValue = 300;                            // Speed of 300rpm
    Serial.println(outputValue);
    stepper.setSpeed(outputValue);
    time = millis();
    while (millis() - time < 5000)                 // Motor runs for 5 seconds then stops until mic value is > 100
    {stepper.runSpeed();}
    stepper.stop();
    }
}
    

  
