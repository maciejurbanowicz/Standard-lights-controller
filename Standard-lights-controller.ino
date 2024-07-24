//***** GLOBAL VARIABLES *****

//Relay board
#define LightPinLeft 2 //left channel ID
#define LightPinRight 3 //right channel ID

//Switch
#define switch1 8 //the main switch connected to D8
#define switch2 9 //the secondary switch connected to D9

int switch1statustemp = 1;
int switch2statustemp = 1;

volatile int switch1status = 1; //default value for the switch: 1 = OFF, 0 = ON
volatile int switch2status = 1; //default value for the switch: 1 = OFF, 0 = ON

//Lights
int lightLstatus = 0;
int lightRstatus = 0;

#define led1 13 //on-board LED for status preview

//END ***** GLOBAL VARIABLES *****

/***** serialInit *****
serialInit is a function that initializes the serial output. This function is required for printing telemtry readings in the terminal.
*/

void serialInit(int serialinit) {
  Serial.begin(serialinit);
  while (!Serial); {
    ;
  }
  Serial.println("Serial initialized");
}

/***** SwitcherReading *****
SwitchesReading is a function that reads the statuses of the switches 1 and 2.
When riding a motorcycle, lots of vibrations occur and they may impack the readings.
Thus, this function reads 1000 times the statuses and counts how many times one value occured.
Hence, the sum is compared with the threshold and then the status is set.
*/

void SwitchesReading() {
  switch1statustemp = 0;
  switch2statustemp = 0;
  int iterations = 1000;
  int threshold = 500;
  {
    for (int i = 0 ; i<iterations; i++) {
      switch1statustemp = switch1statustemp + digitalRead(switch1);
      switch2statustemp = switch2statustemp + digitalRead(switch2);
    }
    if (switch1statustemp > threshold) {
      switch1status = 1; //OFF
    }
    else {
      switch1status = 0; //ON
    }
    if (switch2statustemp > threshold) {
      switch2status = 1; //OFF
    }
    else {
      switch2status = 0; //ON
    }
  }
  LightsController();
}

/***** LightsController *****
LightsController is a function that turns on/off left and right lights.
It also turns on/off the built-in LED - it's helpful for troubleshooting.
Furthermore, it saves lights status in the global variables, so the telemetry function can read and print them.
*/

void LightsController() {
  {
  if (switch1status == 1 && switch2status == 1) {
    digitalWrite(LightPinLeft,LOW); //Left light off
    digitalWrite(LightPinRight,LOW); //Right light off
    digitalWrite(led1,LOW); //LED off
    lightLstatus = 0; //telemtry status
    lightRstatus = 0; //telemtry status
    digitalWrite(LEDR,HIGH);
    digitalWrite(LEDG,HIGH);
    digitalWrite(LEDB,HIGH);
    }
    if ((switch1status == 1 && switch2status == 0) || (switch1status == 0 && switch2status == 1) ){
      digitalWrite(LightPinLeft,HIGH);  //Left light on
      digitalWrite(LightPinRight,HIGH);  //Right light on
      digitalWrite(led1,HIGH); //LED on
      lightLstatus = 1; //telemtry status
      lightRstatus = 1; //telemtry status
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,HIGH);
      digitalWrite(LEDB,HIGH);
      }
    if (switch1status == 0 && switch2status == 0) {
      //If such case happens, then the main switch (switch 1 and 2) are broken
      digitalWrite(LightPinLeft,LOW);  //Left light off
      digitalWrite(LightPinRight,LOW); //Right light off
      digitalWrite(led1,HIGH); //LED off
      lightLstatus = 0; //telemtry status
      lightRstatus = 0; //telemtry status
      digitalWrite(LEDR,LOW);
      digitalWrite(LEDG,LOW);
      digitalWrite(LEDB,LOW);
    }
  }  
}

/***** telemetry *****
telemetry function prints the key information about the status of the system.
*/

void telemetry() {
  //for (;;) {
    Serial.print("Switch 1 = ");Serial.print(switch1status);
    Serial.print(" | Switch 2 = ");Serial.print(switch2status);
    Serial.print(" | Switch 1 RAW = ");Serial.print(digitalRead(switch1));
    Serial.print(" | Switch 2 RAW = ");Serial.print(digitalRead(switch2));
    Serial.print(" | Light L = ");Serial.print(lightLstatus);
    Serial.print(" | Light R = ");Serial.print(lightRstatus);
    Serial.print(" | switch1statustemp = ");Serial.print(switch1statustemp);
    Serial.print(" | switch2statustemp = ");Serial.println(switch2statustemp);
  //}
}

void setup() {
  serialInit(9600);

  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);

  pinMode(LightPinLeft,OUTPUT);
  pinMode(LightPinRight,OUTPUT);

  pinMode(led1,OUTPUT);
  pinMode(LEDR,OUTPUT);
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
  
  //default settings for LEDs
  {
    digitalWrite(LEDR,HIGH);
    digitalWrite(LEDG,HIGH);
    digitalWrite(LEDB,HIGH);
    digitalWrite(led1,LOW);
  }
  
  //SwitchesReading(); //Initial reading of the statuses of the switches.

  attachInterrupt(digitalPinToInterrupt(switch1),SwitchesReading,CHANGE);
  attachInterrupt(digitalPinToInterrupt(switch2),SwitchesReading,CHANGE);

  //telemetry();
}

void loop() {
  telemetry();
  //LightsController();
}
