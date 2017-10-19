//#define RH_ASK_ARDUINO_USE_TIMER2
#define MY_DEBUG
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

#define EMPTY_STRING "                "
#define HELLO_STRING "HELLO"
#define RADIO_INIT_STRING "RADIO INIT"
#define GSM_POWERUP_STRING "GSM POWERUP"
#define GSM_INIT_STRING "GSM INIT"
#define CLEAN_SMS_MEMORY_STRING "CLEAN SMS MEMORY"
#define RADIO_INIT_ERROR_STRING "RADIO INIT ERROR"
#define GSM_INIT_ERROR_STRING "GSM INIT ERROR"
#define MESSAGE_MEMORY_WAS_EMPTY_STRING "MEMORY WAS EMPTY" 
#define MESSAGES_DELETED_STRING "MESSAGES DELETED" 

#define SEARCH_DEVICES_STATE_STRING "SEARCH STATE:   "
#define READY_FOR_SMS_STATE_STRING "READY FOR SMS:  "


//#include <GSM.h>
#include <sms.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RH_ASK.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

//GSM
SMSGSM gsm_sms;

//radio
RH_ASK driver(RADIO_FREG, RADIO_RX_PIN, RADIO_TX_PIN);
RHReliableDatagram manager(driver, BAZA_ADDRESS);

//buttons
int btn_Search_Remember = 0;
int btn_Gsm_Power = 0;
const int btn_Search_Remember_Pin = 5;
const int btn_Gsm_Power_Pin = 13;

// screen
LiquidCrystal_I2C lcd(0xbf, LCD_COL_NUM, LCD_ROW_NUM);

// ERRORS
boolean rf_error = false;
boolean gsm_error = false;

void setup() {
  Serial.begin(9600);
  lcd_init();
  lcd.clear();
  lcd_print_state(HELLO_STRING);
  digital_pin_init();
  delay(5000);
  lcd.clear();
  lcd_print_state(RADIO_INIT_STRING);
  rf_error = radio_init();
  delay(5000);
  if (!rf_error) {
    lcd.clear();
    lcd_print_state(GSM_POWERUP_STRING);
    gsm_power_on();
    delay(5000);
    lcd.clear();
    lcd_print_state(GSM_INIT_STRING);
    gsm_error = gsm_init();
    delay(3000);
#ifdef MY_DEBUG
    print_EEPROM();
#endif
    if (!gsm_error) {
      lcd.clear();
      lcd_print_state(CLEAN_SMS_MEMORY_STRING);
      clean_sms_memory();
      delay(5000);
      lcd_print_info(EMPTY_STRING, 0);
    } else {
      lcd.clear();
      lcd_print_state(GSM_INIT_ERROR_STRING);
    }
  } else {
    lcd.clear();
    lcd_print_state(RADIO_INIT_ERROR_STRING);
  }
}

void clean_sms_memory() {
#ifdef MY_DEBUG
  Serial.println("INSIDE of clean_sms_memory");
#endif
  char position;
  boolean empty_sms_memory = true;
  // Delete all messages from modem memory
  while (position = findSMS(SMS_ALL)) {
    char result_of_deleting = gsm_sms.DeleteSMS((byte) position);
    Serial.print("result_of_deleting = ");
    Serial.println((int) result_of_deleting);
#ifdef MY_DEBUG
    Serial.print("FOUND MESSAGE in position ");
    Serial.print((byte) position);
    if (result_of_deleting) {
      Serial.println(" was really DELETED");
      empty_sms_memory = false;
    } else {
      Serial.println(" was NOT really DELETED");
    }
#endif
  }
  if (empty_sms_memory) {
    lcd_print_info(MESSAGE_MEMORY_WAS_EMPTY_STRING, 0);
    Serial.println("MESSAGE MEMORY WAS EMPTY");
  } else {
    lcd_print_info(MESSAGES_DELETED_STRING, 0);
    Serial.println("MESSAGES HAVE BEEN DELETED");
  }
  gsm.SendATCmdWaitResp("AT+CPMS=\"SM\",\"SM\",\"SM\"", 1000, 1000, "+CPMS:", 10);
#ifdef MY_DEBUG
  Serial.println("OUT from clean_sms_memory");
#endif
}

void print_EEPROM() {
  // Before using EEPROM should be filled by zeros
  Serial.print("Remember devices: ");
  for (int i = 0; i < MAX_DEVICES; i++) {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
  }
  Serial.println();
}

//RADIO MESSAGE DATA STRUCTURES
const uint8_t msgLength = 1;
//output data
uint8_t data[msgLength];
//input buffer
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];

uint8_t numDevices = 0;

void loop() {
  if (!rf_error && !gsm_error) {
    Serial.println("-----------LOOP-------------");
    readButtons();

    // FIND DEVICES STATE // works like a server get request send response
    if (btn_Search_Remember) {
      // works like a server. Waits messages from devices and retries ack
      lcd_print_state(SEARCH_DEVICES_STATE_STRING);
      if (manager.available()) {
        Serial.println("BAZA: got something by RF!!");//TEST
        uint8_t from;
        uint8_t len = sizeof(buf);
        if (manager.recvfromAck(buf, &len, &from)) {
#ifdef MY_DEBUG
          printMessageInSerial(buf, from);
#endif
          uint8_t device_number = 0;
          for (int i = 0; i < MAX_DEVICES; i++) {
            if ((device_number = EEPROM.read(i)) == 0) {
              Serial.println("BAZA: No such device found");//TEST
              device_number = i + 1;
              Serial.print("BAZA: new device number = ");//TEST
              Serial.println(device_number);//TEST
              break;
            } else if (device_number == from) {
              Serial.println("BAZA: device was identified");//TEST
              break;
            }
          }

          prepareData(device_number);
          // Send a reply back to the originator client
          serialPrint("BAZA: data = ", data[0]);//TEST
          serialPrint("BAZA: from = ", from);//TEST
          if (manager.sendtoWait(data, sizeof(data), from)) {
            EEPROM.write(device_number - 1, device_number);
          } else {
            Serial.println("BAZA: sendtoWait failed");
          }
#ifdef MY_DEBUG
          print_EEPROM();//TEST
#endif
        } else {

        }
      }
    }
    // WORK STATE // works like a client: send request get response
    else {
      lcd_print_state(READY_FOR_SMS_STATE_STRING);
      Serial.println("WORK STATE");
      char position;
      findSMS(SMS_UNREAD);
      position = findSMS(SMS_READ);
#ifdef MY_DEBUG
      Serial.print("position when SMS_UNREAD = ");
      Serial.println((int)position);
#endif
      if (position) {
        Serial.println("----------====<<<<< GET SMS >>>>>====----------");
        char phone[PHONE_LENGTH];
        char in_sms_text[SMS_LENGTH];
        char out_sms_text[SMS_LENGTH];
        getSMS(position, phone, in_sms_text);
#ifdef MY_DEBUG
        Serial.print("phone = ");
        Serial.println(phone);
        Serial.print("in_sms_text = ");
        Serial.println(in_sms_text);
#endif
        uint8_t device_states[MAX_DEVICES];
        memset(device_states, MAX_DEVICES, 0);
        int devicesCount = quiz_devices(device_states);
        Serial.println("after quiz_devices");
        int num_symbols_in_sms = prepare_out_sms_text(out_sms_text, device_states, devicesCount);
#ifdef MY_DEBUG
        Serial.println("after prepare_out_sms_text");
        Serial.print("text sms = ");
        Serial.println(out_sms_text);
        Serial.print("num of symbols in sms = ");
        Serial.println(num_symbols_in_sms);
#endif
        sendSMS(phone, out_sms_text);
        Serial.println("----------====<<<<< DEVICES ARE CHECKED >>>>>====----------");
      }
      delay(1500);
    }
  } else {
    if (rf_error) {
      Serial.println("Radio init error!");
    }
    if (gsm_error) {
      Serial.println("GSM init error!");
    }
    delay(5000);
  }
}

char findSMS(byte required_status) {
#ifdef MY_DEBUG
  Serial.println("INSIDE of findSMS");
  Serial.print("findSMS type = ");
  switch (required_status) {
    case SMS_UNREAD:
      Serial.println("AT+CMGL=\"REC UNREAD\"");
      break;
    case SMS_READ:
      Serial.println("AT+CMGL=\"REC READ\"");
      break;
    case SMS_ALL:
      Serial.println("AT+CMGL=\"ALL\"");
      break;
  }
  Serial.println("OUT from findSMS");
#endif

  return gsm_sms.IsSMSPresent(required_status);
}

void getSMS(char position, char *phone, char* sms_text) {
#ifdef MY_DEBUG
  Serial.println("INSIDE of getSMS");
#endif
  if (position > 0 && position <= 20) {
#ifdef MY_DEBUG
    gsm.SendATCmdWaitResp("AT+CPMS=\"SM\",\"SM\",\"SM\"", 1000, 1000, "+CPMS:", 10);
#endif
    gsm_sms.GetSMS((byte)position, phone, sms_text, SMS_LENGTH);
#ifdef MY_DEBUG
    gsm.SendATCmdWaitResp("AT+CPMS=\"SM\",\"SM\",\"SM\"", 1000, 1000, "+CPMS:", 10);
#endif
    clean_sms_memory();
  }
#ifdef MY_DEBUG
  Serial.println("OUT from getSMS");
#endif
}

int quiz_devices(uint8_t *device_states) {
  prepareData(1);
  int counter = 0;

  for (int i = 0; i < MAX_DEVICES; i++) {
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

int prepare_out_sms_text(char* sms_text, uint8_t* device_states, int device_count) {
  String str;

#ifdef MY_DEBUG
  Serial.println("INSIDE  prepare_out_sms_text() method");
#endif
  if (device_count != 0) {
    char number[3];
    int symbol_position = 0;
    for (int i = 0; i < device_count; i++) {
      str += 'D';
      String device_number = String(i + 1, DEC);
      str += device_number;
      str += '=';
      String device_state = String((int)device_states[i], DEC);
      str += device_state;
      str += ';';
    }
  } else {
    str = "Error 0: No devices found";
  }
  strcpy(sms_text, str.c_str());
#ifdef MY_DEBUG
  Serial.println("OUT from  prepare_out_sms_text() method");
#endif
  return str.length();
}

void sendSMS(char* phone, char* sms_text) {
  Serial.println("SMS sending has been started");
  if (gsm_sms.SendSMS(phone, sms_text)) {
    Serial.println("SMS sending has been finished");
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
}

void digital_pin_init() {
  pinMode(btn_Search_Remember_Pin, INPUT);
}

boolean radio_init() {
  lcd.setCursor(LCD_RF_STATE_COL, LCD_ROW_2);
  if (manager.init()) {
    return false;
  }
  return true;
}

void readButtons() {
  btn_Search_Remember = digitalRead(btn_Search_Remember_Pin);
}

void lcd_print_state(String str) {
  lcd.setCursor(0, LCD_ROW_1);
  lcd.print(str);
}

void lcd_print_info(String str, uint8_t col) {
  lcd.setCursor(col, LCD_ROW_2);
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

void gsm_power_on()
{
#ifdef MY_DEBUG
  Serial.println("INSIDE of gsm_power_on");
#endif
  pinMode(btn_Gsm_Power_Pin, OUTPUT);
  digitalWrite(btn_Gsm_Power_Pin, LOW);
  delay(1000);
  digitalWrite(btn_Gsm_Power_Pin, HIGH);
  delay(2000);
  digitalWrite(btn_Gsm_Power_Pin, LOW);
  delay(3000);
#ifdef MY_DEBUG
  Serial.println("OUT from gsm_power_on");
#endif
}

boolean gsm_init() {
  Serial.println("Start of GSM initialisation");
  if (gsm.begin(GSM_RATE)) {
    Serial.println("GSM is initialized");
    return false;
  }
  Serial.println("GSM is not initialized");
  return true;
}
