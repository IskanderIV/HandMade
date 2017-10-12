//#define RH_ASK_ARDUINO_USE_TIMER2
#define ERROR_RADIO 2
#define BAZA_ADDRESS 0
#define MSG_LENGTH 3

#include <RHReliableDatagram.h>
#include <EEPROM.h>
#include <RH_ASK.h>
#include <SPI.h>
//error
uint8_t error = 0;

//button
boolean WORK = false;
boolean INFO = false;
uint8_t work_switch = 0;
uint8_t info_switch = 0;
const int work_switch_Pin = 5;
const int info_switch_Pin = 6;

uint8_t device_number;

//radio
int radio_freq = 4000;
int tx_Pin = 12;
int rx_Pin = 11;
RH_ASK driver(radio_freq, rx_Pin, tx_Pin);
RHReliableDatagram manager(driver, 1);

boolean getDataFromBaza = false;

void setup() {
  Serial.begin(19200);
  digital_pin_init();
  radio_init();
  Serial.print("EEPROM: "); //TEST
  Serial.println(EEPROM.read(0)); //TEST
  uint8_t mem_byte = EEPROM.read(0);
  if (mem_byte < 0 || mem_byte >= 0xff) {
    mem_byte = 0;
    EEPROM.write(0, mem_byte);
  }
  manager.setThisAddress(mem_byte);
  delay(300);
}

//MESSAGE
const uint8_t msgLength = 1;
//output data
uint8_t data[msgLength]; //number of device
//input buffer
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];

void loop() {
//  memset(buf, 0, msgLength);
  readButtons();
  if (work_switch) info_switch = 0;
  
  // FIND STATE // works like a client: send request get response
  if (info_switch) {
    prepareData("client");
    if (manager.sendtoWait(data, sizeof(data), BAZA_ADDRESS)) {
      // Now wait for a reply from Baza
      uint8_t len = sizeof(buf);
      if (manager.recvfromAckTimeout(buf, &len, 2000)) {
        Serial.println("Get data from Baza");
        getDataFromBaza = true;
        setDeviceNumber(buf[0]);
        serialPrint("My new address: ", buf[0]);      
        
        // switch ON the VD1
      } else { 
        Serial.println("aaaaaaaaa, is Baza running?");
      }
    } else {
      Serial.println("Bad send!");
    }
    delay(200);
  } else {
    // WORK STATE // works like a server get request send response
    if (manager.available()) {
      Serial.println("got from Baza!!");//TEST
      uint8_t from;
      uint8_t len = sizeof(buf);
      if (manager.recvfromAck(buf, &len, &from)) {
        serialPrint("Request from : 0x", from);
        prepareData("server");        
        // Send a reply back to the Baza
        if (manager.sendtoWait(data, sizeof(data), from)) {
          Serial.println("sendtoWait SUCCESS");
        } else {
          Serial.println("sendtoWait failed");
        }
      } else {
        Serial.println("recvfromAck failed");
      }
    }
  }  
}
void serialPrint(String msg, int data) {
  Serial.print(msg);
  Serial.println(data);
}

void setDeviceNumber(uint8_t number) {
  EEPROM.write(0, number);
  manager.setThisAddress(number);
  manager.setHeaderFrom(number);
}

void digital_pin_init() {
  pinMode(work_switch, INPUT);
  pinMode(info_switch, INPUT);
}

void radio_init() {
  if (!manager.init()) {
    error = ERROR_RADIO;
  }
}

void readButtons() {
  work_switch = digitalRead(work_switch_Pin);
  info_switch = digitalRead(info_switch_Pin);
}

void prepareData(String workingType) {
  if (workingType == "client"){
    data[0] = EEPROM.read(0);
  }
  if (workingType == "server"){
    data[0] = (uint8_t) EEPROM.read(0);
  }
}

