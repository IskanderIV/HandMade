//#define RH_ASK_ARDUINO_USE_TIMER2
#define BAZA_ADDRESS 0
#define MAX_DEVICES 255
#define LCD_ROW_1 0
#define LCD_ROW_2 1
#define LCD_ROW_NUM 2
#define LCD_COL_NUM 16
#define LCD_GSM_INFO_COL 0
#define LCD_RF_STATE_COL 12
#define LCD_RF_DATA_COL 6
#define GSM_FREQ 115200
#define RADIO_TX_PIN 12
#define RADIO_RX_PIN 11
#define RADIO_FREG 2000
#define GSM_RATE 2400
#define SMS_TX_PIN 3
#define SMS_RX_PIN 10
#define SMS_LENGTH 160
#define PHONE_LENGTH 20

//#include <GSM.h>
#include "sms.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RH_ASK.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

//GSM
//GSM gsmAccess;
GSM gsm;

//radio
RH_ASK driver(RADIO_FREG, RADIO_RX_PIN, RADIO_TX_PIN);
RHReliableDatagram manager(driver, BAZA_ADDRESS);

//button
int btn_Search_Remember = 0;
int btn_get_SMS = 0;
const int btn_Search_Remember_Pin = 5;
const int btn_get_SMS_Pin = 6;

// screen
LiquidCrystal_I2C lcd(0xbf, LCD_COL_NUM, LCD_ROW_NUM);

void setup() {
  Serial.begin(9600);
  lcd_init();
  digital_pin_init();
  radio_init();
  gsm_init();
  print_EEPROM();
  delay(300);
}

void print_EEPROM() {
  Serial.print("Remember devices: ");
  for (int i = 0; i < MAX_DEVICES; i++) {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
  }
  Serial.println();
}

//MESSAGE
const uint8_t msgLength = 1;
//output data
uint8_t data[msgLength];
//input buffer
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];

uint8_t numDevices = 0;

void loop() {
  readButtons();

  // FIND STATE // works like a server get request send response
  if (btn_Search_Remember) {
    // works like a server. Waits messages from devices and retries ack
    if (manager.available()) {
      Serial.println("got something!!");//TEST
      uint8_t from;
      uint8_t len = sizeof(buf);
      if (manager.recvfromAck(buf, &len, &from)) {
        printMessageInSerial(buf, from);
        uint8_t device_number = 0;
        for (uint8_t i = 0; i < MAX_DEVICES; i++) {
          if ((device_number = EEPROM.read(i)) == 0) {
            Serial.println("No such device found");//TEST
            device_number = i + 1;
            Serial.print("BAZA: new_device_number = ");//TEST
            Serial.println(device_number);//TEST
            break;
          } else if (device_number == from) {
            Serial.println("device_number == from");//TEST
            break;
          }
        }

        prepareData(device_number);
        print_RF_on_lcd(convertUint8ToString(data));

        // Send a reply back to the originator client
        serialPrint("BAZA: data = ", data[0]);//TEST
        serialPrint("BAZA: from = ", from);//TEST
        if (manager.sendtoWait(data, sizeof(data), from)) {
          EEPROM.write(device_number - 1, device_number);
        } else {
          Serial.println("sendtoWait failed");
        }
        print_EEPROM();//TEST
      } else {
        print_RF_on_lcd("Bad");
      }
    }
  }
  // WORK STATE // works like a client: send request get response
  else {
	  int position = hasSMS();
    if (position) {
      Serial.println("----------====<<<<< GET SMS >>>>>====----------");
	  
      char phone[PHONE_LENGTH];
      char in_sms_text[SMS_LENGTH];
      char out_sms_text[SMS_LENGTH];
      getSMS(position, phone, in_sms_text);
	  
      uint8_t device_states[MAX_DEVICES];
      memset(device_states, MAX_DEVICES, 0);
      int devicesCount = quiz_devices(device_states);
	  
      int num_symbols_in_sms = prepare_out_sms_text(out_sms_text, device_states, devicesCount);
      Serial.print("text sms = ");
      Serial.println(out_sms_text);
	  Serial.print("num of symbols in sms = ");
      Serial.println(num_symbols_in_sms);
	  
	  sendSMS(phone, out_sms_text);
      Serial.println("----------====<<<<< DEVICES ARE CHECKED >>>>>====----------");
    }
    delay(1000);
  }
}

int hasSMS() {
	return gsm.IsSMSPresent(SMS_UNREAD);		
}

void getSMS(int position, char *phone, char* sms_text) {
	if (position > 0 && position <= 20) {
		gsm.GetSMS((byte)position, phone, sms_text, SMS_LENGTH);
		Serial.println(phone);
		// Delete message from modem memory
		gsm.DeleteSMS(position);
		Serial.println("MESSAGE DELETED");
	}
}

int quiz_devices(uint8_t *device_states) {
  prepareData(1);
  int counter = 0;

  for (uint8_t i = 0; i < MAX_DEVICES; i++) {
    uint8_t device_number = EEPROM.read(i);
    if (device_number == 0) {
      break;
    }
    counter++;
    uint8_t from;
    if (manager.sendtoWait(data, sizeof(data), device_number)) {
      // Now wait for a reply from Device
      uint8_t len = sizeof(buf);
      if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
        serialPrint("Get data from Device 0x", from);
        if (from == device_number) {
          device_states[i] = buf[0];
          Serial.print("Device #");
          Serial.print(device_number);
          Serial.println(" is SWITCH ON = " + device_states[i]);
        }
      }
    } else {
      Serial.print("Device #");
      Serial.print(device_number);
      Serial.println(" is SWITCH OFF = " + device_states[i]);
    }// end if sendtoWait
  } // end for

  return counter;
}

int prepare_out_sms_text(char* sms_text, char* device_states, int device_count) {
  if (device_count != 0) {
	int symbol_position = 0;
	for (int i = 0; i < device_count; i++) {
	  sms_text[symbol_position] = 'D';
	  symbol_position++;
	  int divide = 100;
	  int elder_position_number = 0;
	  for(int j=0; j<3; j++) {
		  elder_position_number = (int) (i / divide) - (elder_position_number * divide);
		  sms_text[symbol_position] = (char) elder_position_number;		  
		  symbol_position++;
		  divide /=10; 
	  }
	  sms_text[symbol_position] = '=';
	  symbol_position++;
	  sms_text[symbol_position] = device_states[i];
	  symbol_position++;
	  sms_text[symbol_position] = '\n';
	  symbol_position++;
	}
  } else {
    sms_text = "Error 0: No devices found";
  }
  return symbol_position - 1;
}

void sendSMS(char* phone, char* sms_text) {
  Serial.println("SMS send has been started");
  if(gsm.SendSMS(phone, sms_text)) {
	  Serial.println("SMS send has been finished");
  } else {
	  Serial.println("There is an ERROR while sending sms");
  }
}

void serialPrint(String msg, int data) {
  Serial.print(msg);
  Serial.println(data);
}

String convertUint8ToString(uint8_t *data) {
  String radioMessage = "   ";
  radioMessage += data[0];
  radioMessage.trim();
  return radioMessage;
}

void lcd_init() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, LCD_ROW_1);
  lcd.print("Hello, My Lord!");
}

void digital_pin_init() {
  pinMode(btn_Search_Remember_Pin, INPUT);
  pinMode(btn_get_SMS_Pin, INPUT);
  //  pinMode(pwr_Pin, OUTPUT);
  //  digitalWrite(pwr_Pin, true);
}

void radio_init() {
  lcd.setCursor(LCD_RF_STATE_COL, LCD_ROW_2);
  if (!manager.init()) {
    lcd.print("RF=0");
    return;
  }
  lcd.print("RF=1");
}

void readButtons() {
  btn_Search_Remember = digitalRead(btn_Search_Remember_Pin);
  btn_get_SMS = digitalRead(btn_get_SMS_Pin);
}

void print_RF_on_lcd(String str) {
  if (str.length() > 3) {
    str = str.substring(0, 2);
  }
  lcd.setCursor(LCD_RF_DATA_COL, LCD_ROW_2);
  lcd.print(str);
}

void clear_lcd_scr() {
  String clear_text = " ";
  for (int i = 0; i < LCD_COL_NUM - 1; i++) {
    clear_text += clear_text;
  }
  lcd.setCursor(0, LCD_ROW_1);
  lcd.print(clear_text);
  lcd.setCursor(0, LCD_ROW_2);
  lcd.print(clear_text);
}

void clear_lcd_scr_region(int row, int col1, int col2) {
  if (row < 0 || row > LCD_ROW_NUM - 1) return;
  if (col1 > col2) return;
  if (col1 < 0 || col1 > LCD_COL_NUM - 1) return;
  if (col2 < 0 || col2 > LCD_COL_NUM - 1) return;
  String clear_text = "";
  for (int i = 0; i < col2 - col1 + 1; i++) {
    clear_text += (char)0x20;
  }
  lcd.setCursor(col1, row);
  lcd.print(clear_text);
}

void printMessageInSerial(uint8_t* buf, uint8_t from) {
  Serial.print("Request from : 0x");
  Serial.print(from, HEX);
  Serial.print(" txBuf = ");
  Serial.print(buf[0]);
  Serial.println();
}

void prepareData(uint8_t device_number) {
  data[0] = device_number;
}

void gsm_init() {
  boolean notConnected = true;
  while (notConnected) {
    Serial.println("SMS Messages Sender");
    if (gsm.begin(GSM_RATE 2400)) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
}

