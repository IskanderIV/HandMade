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
#define GSM_FREQ 115700
#define RADIO_TX_PIN 12
#define RADIO_RX_PIN 11
#define RADIO_FREG 2000

#include <GSM.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RH_ASK.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

//GSM
GSM gsmAccess;
GSM_SMS gsm(true);

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
    if (hasSMS()) {
      Serial.println("----------====<<<<< GET SMS >>>>>====----------");
      const uint8_t phone_length = 20;
      char phone[phone_length];
      getSMS(phone, phone_length);
      uint8_t device_states[MAX_DEVICES];
      memset(device_states, MAX_DEVICES, 0);
      int devicesCount = quiz_devices(device_states);
      char *text = prepare_sms_text(device_states, devicesCount);
      Serial.print("text sms = ");
      Serial.println(text);
      int errorSendSMS = sendSMS("I love you", phone);
      Serial.println("----------====<<<<< DEVICES ARE CHECKED >>>>>====----------");
    }
    delay(1000);
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

char* prepare_sms_text(char* device_states, int device_count) {
  String text_sms = "!";

  if (device_count != 0) {
    for (int i = 0; i < device_count; i++) {
      text_sms += "D";
      text_sms += (i + 1);
      text_sms += "=";
      text_sms += data[i];
      text_sms += "\n";
      text_sms.trim();
    }
    //    const int text_sms_length = text_sms.length();
    //    char text[text_sms_length];
    //    return text_sms.toCharArray(text, text_sms_length);
  } else {
    text_sms = "Error 0: No devices found";
  }
  const int text_sms_length = text_sms.length();
  char text[text_sms_length];
  text_sms.toCharArray(text, text_sms_length);
  return text;
}

int sendSMS(const String &text, char *phone) {
  Serial.println("SMS send started");
  uint8_t counter = 0;
  uint8_t num_of_effort = 3;
  uint8_t commandError;
  // 3 efforts for sending sms
  while (counter++ < num_of_effort && ((commandError = gsm.beginSMS(phone)) != 1));
  if (counter == num_of_effort + 1) return commandError;
  for (uint8_t i = 0; i < text.length(); i++) {
    gsm.write(text.charAt(i));
  }
  commandError = gsm.endSMS();
  Serial.println("SMS send finish");
  Serial.println("ErrorCommand = " + commandError);
  return commandError;
}

boolean hasSMS() {
  return gsm.available() == 1;
}

void getSMS(char *phone, uint8_t phone_length) {
  gsm.remoteNumber(phone, phone_length);
  Serial.println(phone);
  //  String phone = String(senderNumber);
  // Read message bytes and print them
  //  char next_symbol;
  //  while (next_symbol = sms.read()) {
  //    Serial.print(next_symbol);
  //  }
  // Delete message from modem memory
  gsm.flush();
  Serial.println("MESSAGE DELETED");
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
#ifdef __AVR_ATmega2560__
   Serial.println("__AVR_ATmega2560__");
   #else
    Serial.println("NO __AVR_ATmega2560__");
#endif
  //  Serial.print("__RXPIN__");
  //  Serial.println(__RXPIN__);
  //  Serial.print("__TXPIN__");
  //  Serial.println(__TXPIN__);
  boolean notConnected = true;
  while (notConnected) {
    Serial.println("SMS Messages Sender");
    if (gsmAccess.begin("", false, true) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
}

