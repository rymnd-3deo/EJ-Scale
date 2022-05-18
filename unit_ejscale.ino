/*
@author: Raymond Ma
@created: 2021-07-22

copied functionality from Sly's BinderMakerMachine to read the EJ scales

2022-05-17: modified to use software serial so that it also works for Uno
*/

#include "src/ScaleEJSeries/ScaleEJSeries.h"
#include <SoftwareSerial.h>

float scaleWeight = 0;
float prevScaleWeight;
float stableScaleWeight = 0;

SoftwareSerial ser(3,8,false);	//cannot use default 0,1 on Uno since that's Serial1
Scale binderScale(&ser);  //STUPID SCALE RS-232 NEEDS RX AND TX PHYSICALLY SWAPPED FOR THIS TO WORK
//YOU ALSO NEED A MAX232 to convert RS-232 TO TTL!!

void setup() {
  Serial.begin(115200);   //debug
  ser.begin(9600);  //scale

  delay(3000);
  Serial.println("Initialized");
}

void loop() {
  readScale();
}

void readScale() {
  binderScale.receiveData();  //read byte one at a time
  
  if (binderScale.newData) {  //complete data received
    binderScale.newData = 0;  //reset to wait for next new data
    prevScaleWeight = scaleWeight;
    scaleWeight = binderScale.measureData;
	
	if (binderScale.stable){
		stableScaleWeight = scaleWeight;
	}
	Serial.print("<");
  Serial.print(scaleWeight);
	Serial.print(",");
	Serial.print(stableScaleWeight);
  Serial.println(">");
  }
}
