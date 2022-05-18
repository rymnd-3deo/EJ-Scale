#include "ScaleEJSeries.h"

Scale::Scale(Stream* serialport){
	_serial = serialport;
}

void Scale::receiveData() {
  static char packet[18];
  static int byteCount = 0;
  if (_serial->available()) {
    packet[byteCount] = char(_serial->read());
    if (byteCount == 0) {
      if ((packet[0] == 'S') || (packet[0] == 'Q') || (packet[0] == 'U') || (packet[0] == 'O')) {
        byteCount++;
      }
    }
    else if (byteCount == 1) {
      if ((packet[1] == 'T') || (packet[1] == 'S') || (packet[1] == 'L') || (packet[1] == 'V')) {
        byteCount++;
      }
      else byteCount = 0;
    }
    else if (byteCount == 16) {

      packet[17] = '\0';
      byteCount = 0;
	  parseData(packet);	//parse the incoming bytes and convert to value
    }
    else byteCount++;
  }
}

void Scale::parseData( char* frame) {
  char packetHeader[3];
  char packetData[8];
  char packetUnits[4];
  char packetDataSign;
  char packetTerminator[3];

  packetHeader[0] = frame[0];
  packetHeader[1] = frame[1];
  packetHeader[2] = '\0';
  packetDataSign = frame[3];
  packetData[0] = frame[4];
  packetData[1] = frame[5];
  packetData[2] = frame[6];
  packetData[3] = frame[7];
  packetData[4] = frame[8];
  packetData[5] = frame[9];
  packetData[6] = frame[10];
  packetData[7] = frame[11];
  packetData[8] = '\0';
  packetUnits[0] = frame[12];
  packetUnits[1] = frame[13];
  packetUnits[2] = frame[14];
  packetUnits[3] = '\0';
  packetTerminator[0] = frame[15];
  packetTerminator[1] = frame[16];
  packetTerminator[2] = '\0';

  if ((packetTerminator[0] == '\r') && (packetTerminator[1] == '\n')) {
    measureData = atof(packetData);
    if (packetDataSign == '-') measureData = measureData * -1.0;

    receivedHeader = HEADER_ERROR;
    for (int i = 0; i < 4; i++) {
      if (strcmp(packetHeader, headerStrings[i]) == 0) {
        receivedHeader = (Header)i;
      }
    }
    receivedUnits = UNIT_ERROR;
    for (int i = 0; i < 14; i++) {
      if (strcmp(packetUnits, unitStrings[i]) == 0) {
        receivedUnits = (Units)i;
      }
    }
	if (frame[0]=='U'){
		stable = 0;
	} else if (frame[0]=='S'){
		stable = 1;
	}
	newData = 1;
  } 
}

void Scale::sendCommand(Command type){
  _serial.print(commandStrings[type]);
}
