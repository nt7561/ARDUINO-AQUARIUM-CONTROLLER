// ΑΥΤΟ ΤΟ ΠΡΟΓΡΑΜΜΑ ΓΡΑΦΤΗΚΕ ΓΙΑ ΤΗ ΔΙΑΧΕΙΡΙΣΗ ΤΟΥ ΕΝΥΔΡΕΙΟΥ
// ΑΡΧΙΚΑ ΣΕΤΑΡΕΙ ΤΟ ΧΡΟΝΟ ΧΡΗΣΙΜΟΠΟΙΩΝΤΑΣ ΤΟ RTC, ΣΤΗ ΣΥΝΕΧΕΙΑ ΔΗΜΙΟΥΡΓΕΙ ΚΑΠΟΙΑ ΑΛΑΡΜΣ, ΤΑ ΟΠΟΊΑ ΧΡΗΣΙΜΕΥΟΥΝ ΓΙΑ ΤΟ ΑΝΑΜΑ-ΣΒΗΣΙΜΟ ΤΩΝ ΦΩΤΩΝ, ΤΟ ΑΝΑΜΑ ΣΒΗΣΙΜΟ ΤΟΥ C02
// ΚΑΙ ΓΙΑ ΤΗΝ ΑΥΤΟΜΑΤΗ ΧΟΡΗΓΗΣΗ ΛΙΠΑΣΜΑΤΩΝ ΣΤΟ ΕΝΥΔΡΕΙΟ
// ΤΟ ΕΠΟΜΕΝΟ ΣΤΑΔΙΟ ΘΑ ΕΙΝΑΙ Η ΔΗΜΙΟΥΡΓΕΙΑ ΤΗΣ ΛΕΙΤΟΥΡΓΕΙΑΣ ΚΑΙ ΤΟΥ ΑΥΤΟΜΑΤΟΥ ΤΑΙΣΜΑΤΟΣ ΤΩΝ ΨΑΡΙΩΝ  ΚΑΘΩΣ ΚΑΙ ΕΝΑ WEB BASED LOG ΠΡΟΚΕΙΜΕΝΟΥ ΝΑ ΜΠΟΡΕΙ ΚΑΠΟΙΟΣ ΝΑ ΔΕΙ ΑΠΟΜΑΚΡΥΣΜΕΝΑ ΤΗΝ ΚΑΤΑΣΤΑΣΗ ΤΟΥ ΕΝΥΔΡΕΙΟΥ



#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>   //gia to RTC
#include "RTClib.h" //RTC Library
#include <Time.h>
//#include <TimeAlarms.h>
//#include <DateTime.h>
//#include <DateTimeStrings.h>



//========================================================================================================================
//ΟΡΙΣΜΟΣ ΧΡΟΝΟΥ ΓΙΑ ΤΟ ΑΝΑΜΑ-ΣΒΗΣΙΜΟ Τ5 ΦΩΤΙΣΜΟΥ
//
int T5_ON_HOUR = 12,
    T5_ON_MINUTE = 0,
    T5_OFF_HOUR = 21,
    T5_OFF_MINUTE = 0;

//========================================================================================================================
//ΟΡΙΣΜΟΣ ΧΡΟΝΟΥ ΓΙΑ ΤΟ ΑΝΑΜΑ-ΣΒΗΣΙΜΟ Τ8 ΦΩΤΙΣΜΟΥ
//
int T8_ON_HOUR = 13,
    T8_ON_MINUTE = 30,
    T8_OFF_HOUR = 22,
    T8_OFF_MINUTE = 30;

//========================================================================================================================
//ΟΡΙΣΜΟΣ ΧΡΟΝΟΥ ΓΙΑ ΤΟ ΑΝΑΜΑ-ΣΒΗΣΙΜΟ CO2
//
int CO2_ON_HOUR = 12,
    CO2_ON_MINUTE = 30,
    CO2_OFF_HOUR = 21,
    CO2_OFF_MINUTE = 0;
//========================================================================================================================
//ΟΡΙΣΜΟΣ ΜΕΤΑΒΛΗΤΩΝ ΓΙΑ ΤΗ ΛΙΠΑΝΣΗ
//
//ΙΧΝΟΣΤΟΙΧΕΙΑ
int pump_count_1 = 0;        //Number of times pump ran
int pump_delay_1 = 2160; //1666
int pump_month_1;           //for LCD to Print the Month Pump ran
int pump_day_1;             //for LCD to Print the Date Pump ran
int time1_H;               //for LCD to Print the Time Pump ran
int time1_M;               //for LCD to Print the Minute Pump ran
int time1_S;                 //for LCD to Print the Seconds Pump ran


//ΚΑΛΙΟ
int pump_count_2 = 0;        //Number of times pump ran
int pump_delay_2 = 6200;
int pump_month_2;           //for LCD to Print the Month Pump ran
int pump_day_2;             //for LCD to Print the Date Pump ran
int time2_H;               //for LCD to Print the Time Pump ran
int time2_M;                 //for LCD to Print the Minute Pump ran
int time2_S;                 //for LCD to Print the Seconds Pump ran

//EXELL
int pump_count_3 = 0;        //Number of times pump ran
int pump_delay_3 = 3100;
int pump_month_3;           //for LCD to Print the Month Pump ran
int pump_day_3;             //for LCD to Print the Date Pump ran
int time3_H;               //for LCD to Print the Time Pump ran
int time3_M;              //for LCD to Print the Minute Pump ran
int time3_S;                 //for LCD to Print the Seconds Pump ran

//========================================================================================================================
//ΟΡΙΣΜΟΣ ΜΕΤΑΒΛΗΤΩΝ ΓΙΑ ΤΟ ΤΑΙΣΜΑ
//
// ΠΡΩΤΟ ΤΑΙΣΜΑ
int Feed1_count = 0;
int Feed1_delay = 2160; //1666
int Feed1_month;           //for LCD to Print the Month Pump ran
int Feed1_day;             //for LCD to Print the Date Pump ran
int Feedtime1_H;               //for LCD to Print the Time Pump ran
int Feedtime1_M;               //for LCD to Print the Minute Pump ran
int Feedtime1_S;


// ΔΕΥΤΕΡΟ ΤΑΙΣΜΑ
int Feed2_count = 0;
int Feed2_delay = 2160; //1666
int Feed2_month;           //for LCD to Print the Month Pump ran
int Feed2_day;             //for LCD to Print the Date Pump ran
int Feedtime2_H;               //for LCD to Print the Time Pump ran
int Feedtime2_M;               //for LCD to Print the Minute Pump ran
int Feedtime2_S;                 //for LCD to Print the Seconds Pump ran


//========================================================================================================================
//ΟΡΙΣΜΟΣ PIN ΟΘΟΝΗΣ
//
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//========================================================================================================================
//ΟΡΙΣΜΟΣ ΧΡΩΜΑΤΩΝ
//
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

RTC_DS1307 RTC;

// Data wire is plugged into pin 19 on the Arduino (TEMP SENSOR)
#define ONE_WIRE_BUS 19

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//temp sensor unique addresses
DeviceAddress WaterThermometer = {0x28, 0xFF, 0xE0, 0xAD, 0x62, 0x15, 0x03, 0xBD};

//========================================================================================================================
// ΟΡΙΣΜΟΣ PIN Relay Board
//
int RelayT5Lights = 22;
int RelayT8Lights = 23;
int RelayCO2 = 24;
int RelayFuns = 25;
int RelayHeater = 26;
int RelayMicroPump = 27;
int RelayKalioPump = 28;
int RelayAlgiPump = 29;
int RelayFeeder = 30;
//========================================================================================================================
// OΡΙΣΜΟΣ ΜΕΓΙΣΤΗΣ, ΤΡΕΧΟΥΣΑΣ, ΕΛΑΧΙΣΤΗΣ ΘΕΡΜΟΚΡΑΣΙΑΣ
//
float HighestTemp = 26.5;
float CurrentTemp = 0.0;
float LowestTemp  = 25.5;

//========================================================================================================================
//ΕΚΚΙΝΗΣΗ ΤΟΥ SETUP
//

DateTime prevRefresh;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  // Start up the library for temp sensor
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  sensors.setResolution(WaterThermometer, 9);
  tft.begin(0x9341); // SDFP5408
  tft.fillScreen(BLACK); // Clear screen
  tft.setRotation(1); // screen orientation
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  tft.setTextSize(2);
  tft.println("    Waiting 10 seconds ");
  tft.print("for RTC  to  Sync.... ");
  delay(10000);//wait for RTC to Respond, if not set time
  tft.print("DONE");
  delay(2000);
  tft.fillScreen(BLACK); // Clear screen

  tft.setCursor(60, 60);
  tft.setTextSize(3);
  tft.println("WELCOME TO");
  tft.setCursor(60, 90);
  tft.setTextSize(3);
  tft.println(" AQUARIUM ");
  tft.setCursor(60, 120);
  tft.setTextSize(3);
  tft.println("CONTROLLER");

  delay(3000);
  tft.fillScreen(BLACK); // Clear screen

  //-------( Initialize Pins so relays are inactive at reset)----
  digitalWrite(RelayT8Lights, 0 );
  digitalWrite(RelayT5Lights, 0 );
  digitalWrite(RelayCO2, 0 );
  digitalWrite(RelayMicroPump, 0 );
  digitalWrite(RelayKalioPump, 0 );
  digitalWrite(RelayAlgiPump, 0 );
  digitalWrite(RelayFuns, 0 );
  digitalWrite(RelayHeater, 0 );
  digitalWrite(RelayFeeder, 0);

  //---( THEN set pins as outputs )----
  pinMode(RelayT8Lights,  OUTPUT);
  pinMode(RelayT5Lights,  OUTPUT);
  pinMode(RelayCO2,       OUTPUT);
  pinMode(RelayMicroPump, OUTPUT);
  pinMode(RelayKalioPump, OUTPUT);
  pinMode(RelayAlgiPump,  OUTPUT);
  pinMode(RelayFuns,      OUTPUT);
  pinMode(RelayHeater,    OUTPUT);
  pinMode(RelayFeeder,    OUTPUT);
  delay(2000); //Check that all relay


  prevRefresh = RTC.now();

}// end setup

int loopCounter = 0;

bool isFirst = true;
int nextSecond = -1;
int nextMinute = -1;
bool blackDot = true;
//========================================================================================================================
//ΕΚΚΙΝΗΣΗ ΤΟΥ LOOP
//
void  loop() {

  DateTime now =  RTC.now();    //Get the date and time from RTC
  sensors.requestTemperatures(); // Send the command to get temperatures
  CurrentTemp  = sensors.getTempCByIndex(0);


  if ((nextMinute == now.minute() && nextSecond <= now.second())
      || isFirst == true)
  {
    Serial.println(" [ Will refresh screen ] ");
    tft.fillScreen(BLACK);
    isFirst = false;

    digitalDateTimeDisplay (); //ΕΜΦΑΝΙΣΗ ΗΜΕΡΟΜΗΝΙΑΣ-ΩΡΑΣ
    if (loopCounter == 0)
    {
      digitalTempDisplay (); // ΕΜΦΑΝΙΣΗ ΘΕΡΜΟΚΡΑΣΙΑΣ
    }
    else if (loopCounter == 1)
    {
      T5_T8_CO2Status(); // ΕΛΕΓΧΟΣ ΚΑΤΑΣΤΑΣΗΣ ΦΩΤΩΝ-CO2;
    }
    else if (loopCounter == 2)
    {
      DosingStatus (); // ΕΛΕΓΧΟΣ ΚΑΤΑΣΤΑΣΗΣ ΛΙΠΑΝΣΗΣ
    }
    else if (loopCounter == 3)
    {
      FeedingStatus (); // ΕΛΕΓΧΟΣ ΚΑΤΑΣΤΑΣΗΣ ΛΙΠΑΝΣΗΣ
    }

    if (loopCounter >= 3)  loopCounter = 0;
    else                  loopCounter++;

    if (now.second() + 7 >= 60)
      nextMinute = (now.minute() + 1) % 60;
    else
      nextMinute = now.minute();

    nextSecond = (now.second() + 7) % 60;
    Serial.print("Will refresh @ ");
    Serial.print(nextMinute);
    Serial.print(":");
    if (nextMinute < 10)
    {
      Serial.print("0");
    }
    Serial.print(nextSecond);

  }
  //========================================================================================================================
  //ΟΡΙΣΜΟΣ ΕΠΑΝΑΦΟΡΑΣ ΚΑΤΑΣΤΑΣΗΣ ΔΟΣΟΜΕΤΡΙΚΩΝ
  //
  if (now.hour() == 0 && now.minute() == 0)
  {
    ResetDosingBooleans();
  }

}
//========================================================================================================================
// ΕΜΦΑΝΙΣΗ ΗΜΕΡΟΜΗΝΙΑΣ - ΩΡΑΣ
//
void digitalDateTimeDisplay () {
  DateTime now = RTC.now(); //GRAB DATE AND TIME FROM RTC
  tft.setTextColor(YELLOW, BLACK); //set text color & size for  DATE coming from TinyRTC
  tft.setTextSize(4);
  tft.setCursor(50, 5); //tft.setCursor(0, 35);
  tft.print(now.day(), DEC);
  tft.print('/');
  tft.print(now.month(), DEC);
  tft.print('/');
  tft.print(now.year(), DEC);
  tft.println(' ');
  tft.setCursor(95, 50);
  tft.setTextColor(YELLOW, BLACK); //set color for TIME
  tft.setTextSize(4);//set text  size for  TIME coming from TinyRTC
  if (now.hour() < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(now.hour(), DEC);
  }
  else {
    tft.print(now.hour(), DEC);
  }
  tft.print(':');
  if (now.minute() < 10) { //PRINT A 0 IN FRONT OF THE MINUTE IF LESS THAN 10
    tft.print('0');
    tft.print(now.minute(), DEC);
  }
  else {
    tft.println(now.minute(), DEC);
    tft.setTextColor(YELLOW, BLACK);
    tft.setTextSize(2);
    tft.print("__________________________");
  }

}
//========================================================================================================================
// ΕΜΦΑΝΙΣΗ ΘΕΡΜΟΚΡΑΣΙΑΣ (ΛΕΙΤΟΥΡΓΙΑ ΘΕΡΜΟΣΤΑΤΗ-ΑΝΕΜΙΣΤΗΡΩΝ ΑΝΑΛΟΓΑ ΜΕ ΤΗ ΘΕΡΜΟΚΡΑΣΙΑ)
//
void digitalTempDisplay ()
{
  if (CurrentTemp > HighestTemp) {  //ΣΥΝΑΡΤΗΣΗ ΓΙΑ ΝΑ ΦΑΙΝΕΤΑΙ ΤΟ ΧΡΩΜΑ ΤΗΣ ΘΕΡΜΟΚΡΑΣΙΑΣ ΔΙΑΦΟΡΕΤΙΚΟ ΑΝΑΛΟΓΑ ΜΕ ΤΟ ΕΥΡΟΣ ΤΗΣ
    tft.setCursor(15, 110); //tft.setCursor(5, 70);
    tft.setTextColor(RED, BLACK);
    tft.setTextSize(3);
    tft.print("Water Temp:");
    tft.setTextSize(3);
    tft.println(sensors.getTempCByIndex(0));
    HeaterOFF ();
    FunsON ();
  }
  else if (CurrentTemp < LowestTemp)
  {
    tft.setCursor(15, 110); //tft.setCursor(5, 70);
    tft.setTextColor(CYAN, BLACK); //set color for TEMP
    tft.setTextSize(3);
    tft.print("Water Temp:");
    tft.setTextSize(3);
    tft.println(sensors.getTempCByIndex(0));
    HeaterON();
    FunsOFF();
  }
  else {
    tft.setCursor(15, 110); //tft.setCursor(5, 70);
    tft.setTextColor(GREEN, BLACK); //set color for TEMP
    tft.setTextSize(3);
    tft.print("Water Temp:");
    tft.setTextSize(3);
    tft.println(sensors.getTempCByIndex(0));
    HeaterOFF ();
    FunsOFF ();
  }
}
//========================================================================================================================
//ΑΝΑΜΑ ΘΕΡΜΟΣΤΑΤΗ
//
void HeaterON()
{
  digitalWrite(RelayHeater, 1);
  tft.setCursor(50, 150); //tft.setCursor(85, 170);
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(3);
  tft.print ("Heater is ON!");
}
//========================================================================================================================
//ΣΒΗΣΙΜΟ ΘΕΡΜΟΣΤΑΤΗ
//
void HeaterOFF()
{
  digitalWrite(RelayHeater, 0);
  tft.setCursor(50, 150); //tft.setCursor(85, 170);
  tft.setTextColor(RED, BLACK); //set color for DOSING
  tft.setTextSize(3);
  tft.print ("Heater is OFF");
}
//========================================================================================================================
//ΑΝΑΜΑ ΑΝΕΜΙΣΤΗΡΩΝ
//
void FunsON ()
{
  digitalWrite(RelayFuns, 1);
  tft.setCursor(50, 190); //tft.setCursor(85, 190);
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(3);
  tft.print ("Funs are ON!");
}
//========================================================================================================================
//ΣΒΥΣΙΜΟ ΑΝΕΜΙΣΤΗΡΩΝ
//
void FunsOFF()
{
  digitalWrite(RelayFuns, 0);
  tft.setCursor(50, 190); //tft.setCursor(85, 190);
  tft.setTextColor(RED, BLACK); //set color for DOSING
  tft.setTextSize(3);
  tft.print ("Funs are OFF");
}
void T5_T8_CO2Status() {
  DateTime now = RTC.now();

  //========================================================================================================================
  //ΑΝΑΜΑ-ΣΒΗΣΙΜΟ Τ5 ΛΑΜΠΩΝ
  //
  if (now.hour() < T5_ON_HOUR || now.hour () >= T5_OFF_HOUR)// || (now.hour() == 12 && now.minute() < 30) || (now.hour () ==22 && now.minute() > 30))
  {
    T5LightsOFF();
  }
  else //if (now.hour() >= T5OffHour && now.minute()>= T5OffMinute) || (now.hour() < T5OnHour && now.minute() < T5OffMinute)
  {
    T5LightsON();
  }
  //========================================================================================================================
  //ΑΝΑΜΑ-ΣΒΗΣΙΜΟ Τ8 ΛΑΜΠΩΝ
  //
  if (now.hour() < T8_ON_HOUR  || now.hour () >= T8_OFF_HOUR)   //&& now.minute() >= T8OnMinute) && (now.hour() < T8OffHour && now.minute()< T8OffMinute )
  {
    T8LightsOFF();
  }  else                                     //if (now.hour() >= T8OffHour && now.minute() >= T8OffMinute)|| (now.hour() < T8OnHour && now.minute() < T8OffMinute)
  {
    T8LightsON();
  }
  //========================================================================================================================
  //ΑΝΑΜΑ-ΣΒΗΣΙΜΟ CO2
  //
  if (now.hour() < CO2_ON_HOUR || now.hour() >= CO2_OFF_HOUR) //&& (now.hour() < 11 && now.minute() == 30) || (now.hour ()==21 && now.minute ()>30))
  {
    CO2_OFF(); //ANOIGMA CO2
  }
  else                                         //if now.hour() >= CO2OffHour && now.minute() >= CO2OffMinute || (now.hour() < CO2OnHour && now.minute() < CO2OffMinute) && now.minute() >= CO2OffMinute
  {
    CO2_ON();
  }
}

//========================================================================================================================
// T5-T8 LAMPES
//

void T5LightsON()
{
  digitalWrite(RelayT5Lights, 1);
  tft.setCursor(10, 100);
  tft.setTextColor(GREEN, BLACK); //set color for TIME ON
  tft.setTextSize(3);//set text  size
  tft.print ("T5 lights are ON");
  tft.setCursor(10, 125);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn off at ");
  tft.print (T5_OFF_HOUR);
  tft.print (":00");
  Serial.println ("T5 lights are ON");
}

void T5LightsOFF() //SBHSIMO T5 LAMPES
{
  digitalWrite(RelayT5Lights, 0);
  tft.setCursor(10, 100);
  tft.setTextColor(RED, BLACK); //set color for TIME OFF
  tft.setTextSize(3);
  tft.print("T5 lights are OFF");
  tft.setCursor(10, 125);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn on at ");
  tft.print (T5_ON_HOUR);
  tft.print (":00");
  Serial.println ("T5 lights are OFF");
}

void T8LightsON() //ANAMA T8 LAMPES
{
  digitalWrite(RelayT8Lights, 1);
  tft.setCursor(10, 145);
  tft.setTextColor(GREEN, BLACK); //set color for TIME ON
  tft.setTextSize(3);//set text  size
  tft.print ("T8 lights are ON");
  tft.setCursor(10, 175);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn off at ");
  tft.print (T8_OFF_HOUR);
  tft.print (":00");
  Serial.println ("T8 lights are ON");
}

void T8LightsOFF() //SBHSIMO T5 LAMPES
{
  digitalWrite(RelayT8Lights, 0);
  tft.setCursor(10, 145);
  tft.setTextColor(RED, BLACK); //set color for TIME OFF
  tft.setTextSize(3);
  tft.print ("T8 lights are OFF");
  tft.setCursor(10, 175);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn on at ");
  tft.print (T8_ON_HOUR);
  tft.print (":00");
  Serial.println ("T8 lights are OFF");
}

//========================================================================================================================
//CO2
//

void CO2_ON() //ANAMA CO2
{
  digitalWrite(RelayCO2, 1);
  tft.setCursor(10, 195);
  tft.setTextColor(GREEN, BLACK); //set color for TIME ON
  tft.setTextSize(3);//set text  size
  tft.print ("CO2 is ON");
  tft.setCursor(10, 220);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn off at ");
  tft.print (CO2_OFF_HOUR);
  tft.print (":00");
  Serial.println ("CO2 in ON");
}

void CO2_OFF() //SBHSIMO CO2
{
  digitalWrite(RelayCO2, 0);
  tft.setCursor(10, 195);
  tft.setTextColor(RED, BLACK); //set color for TIME OFF
  tft.setTextSize(3);
  tft.print ("CO2 is OFF");
  tft.setCursor(10, 220);
  tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.print ("will turn on at ");
  tft.print (CO2_ON_HOUR);
  tft.print (":00");
  Serial.println("CO2 is OFF");
}
//========================================================================================================================
// ΩΡΕΣ ΧΟΡΗΓΗΣΗΣ ΛΙΠΑΣΜΑΤΩΝ
//
int pump1_count = 0, //ΙΧΝΟΣΤΟΙΧΕΙΑ
    MICRO_HOUR = 13,
    MICRO_MINUTE = 10,
    MICRO_SEC = 0;

int pump2_count = 0,  //ΚΑΛΙΟ
    KALIO_HOUR = 13,
    KALIO_MINUTE = 20,
    KALIO_SEC = 0;

int pump3_count = 0,  //EXELL
    EXELL_HOUR = 13,
    EXELL_MINUTE = 25,
    EXELL_SEC = 0;



//========================================================================================================================
// ΟΡΙΣΜΟΣ ΑΡΧΙΚΗΣ ΚΑΤΑΣΤΑΣΗΣ ΧΟΡΗΓΗΣΗΣ ΛΙΠΑΣΜΑΤΩΝ - ΤΡΟΦΗΣ
//
bool hasDosedMicros = false;
bool hasDosedKalio = false;
bool hasDosedEXELL = false;
bool hasDosedFood1  = false;
bool hasDosedFood2  = false;

void ResetDosingBooleans()
{
  hasDosedMicros = false;
  hasDosedKalio = false;
  hasDosedEXELL = false;
  hasDosedFood1 = false;
  hasDosedFood2  = false;
}
//========================================================================================================================
// ΧΟΡΗΓΗΣΗ ΛΙΠΑΣΜΑΤΩΝ
//

void DosingStatus() {
  DateTime now = RTC.now();
  if (hasDosedMicros == false) {

    tft.setTextColor(YELLOW, BLACK); //set color for DOSING
    tft.setTextSize(2);
    tft.setCursor(0, 100);
    tft.println("System will dose");
    tft.print("Micros at ");
    if (MICRO_HOUR < 0) {
      tft.print ("0");
      tft.print(MICRO_HOUR);
    } else tft.print(MICRO_HOUR);
    tft.print(":");
    if (MICRO_MINUTE < 0) {
      tft.print ("0");
      tft.print(MICRO_MINUTE);
    } else
      tft.print(MICRO_MINUTE);
  } else if (hasDosedMicros == true ) {
    MicroDosingComplete();
  }

  if (hasDosedKalio == false) {
    tft.setTextColor(YELLOW, BLACK); //set color for DOSING
    tft.setTextSize(2);
    tft.setCursor(0, 150);
    tft.println("System will dose");
    tft.print("Kalio at ");
    if (KALIO_HOUR < 0) {
      tft.print ("0");
      tft.print(KALIO_HOUR);
    } else tft.print(KALIO_HOUR);
    tft.print(":");
    if (KALIO_MINUTE < 0) {
      tft.print ("0");
      tft.print(KALIO_HOUR);
    } else
      tft.print(KALIO_MINUTE);
  } else if (hasDosedKalio == true) {
    KalioDosingComplete();
  }
  if (hasDosedEXELL == false) {
    tft.setTextColor(YELLOW, BLACK); //set color for DOSING
    tft.setTextSize(2);
    tft.setCursor(0, 195);
    tft.println("System will dose");
    tft.print("EXELL at ");
    if (EXELL_HOUR < 0) {
      tft.print ("0");
      tft.print(EXELL_HOUR);
    } else tft.print(EXELL_HOUR);
    tft.print(":");
    if (EXELL_MINUTE < 0) {
      tft.print ("0");
      tft.print(EXELL_HOUR);
    } else
      tft.print(EXELL_MINUTE);
  } else if (hasDosedEXELL == true) {
    ExellDosingComplete();
  }
  //========================================================================================================================
  //ΙΧΝΟΣΤΟΙΧΕΙΑ
  //
  pump1_count ;
  pump_day_1 = now.day();
  pump_month_1 = now.month();
  //========================================================================================================================
  //ΚΑΛΙΟ
  //
  pump2_count ;
  pump_day_2 = now.day();
  pump_month_2 = now.month();

  //========================================================================================================================
  //ΕXELL
  //
  pump3_count ;
  pump_day_3 = now.day();
  pump_month_3 = now.month();
  //========================================================================================================================
  //ΧΟΡΗΓΗΣΗ ΙΧΝΟΣΤΟΙΧΕΙΩΝ
  //
  if (now.hour() == MICRO_HOUR && now.minute() == MICRO_MINUTE && hasDosedMicros == false) {

    hasDosedMicros = true;
    MicroDosing_ON();
    Serial.println("Microdosing ON");

  }  else {
    MicroDosing_OFF();
  }

  //========================================================================================================================
  //ΧΟΡΗΓΗΣΗ ΚΑΛΙΟΥ
  //

  if (now.hour() == KALIO_HOUR && now.minute() == KALIO_MINUTE && hasDosedKalio == false) {

    hasDosedKalio = true;
    KalioDosing_ON();
    Serial.println("Kaliodosing ON");

  }  else {
    KalioDosing_OFF();
  }
  //========================================================================================================================
  //ΧΟΡΗΓΗΣΗ EXELL
  //
  if (now.hour() == EXELL_HOUR && now.minute() == EXELL_MINUTE && hasDosedEXELL == false) {

    hasDosedEXELL = true;
    EXELLDosing_ON();
    Serial.println("EXELLdosing ON");

  }  else {
    EXELLDosing_OFF();
  }
}

void MicroDosing_ON() {
  DateTime now = RTC.now();
  time1_H = now.hour();
  time1_M = now.minute();
  time1_S = now.second();
  digitalWrite(RelayMicroPump, 1);
  pump1_count ++;
  delay(pump_delay_1);
  digitalWrite(RelayMicroPump, 0);
  MicroDosingComplete();
}
void MicroDosingComplete() {
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(2);
  tft.setCursor(0, 100);
  tft.print("Dosed Micro ");
  tft.print(pump1_count);
  tft.println(" time(s) on ");
  tft.print(pump_day_1);
  tft.print(" / ");
  tft.print(pump_month_1);
  tft.print(" ");
  if (time1_H < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(time1_H);
  } else
    tft.print(time1_H);
  tft.print(": ");
  if (time1_M < 10) { //PRINT A 0 IN FRONT OF MINUTES IF LESS THAN 10
    tft.print ('0');
    tft.print(time1_M);
  } else
    tft.print(time1_M);
  tft.print(": ");
  if (time1_S < 10) {//PRINT A 0 IN FRONT OF SECONDS IF LESS THAN 10
    tft.print ('0');
    tft.print(time1_S);
  } else  tft.println(time1_S);
}
void MicroDosing_OFF() {
  digitalWrite(RelayMicroPump, 0);

}

//ΚΑΛΙΟ
void KalioDosing_ON() {
  DateTime now = RTC.now();
  time2_H = now.hour();
  time2_M = now.minute();
  time2_S = now.second();
  digitalWrite(RelayKalioPump, 1);
  pump2_count ++;
  delay(pump_delay_2);
  digitalWrite(RelayKalioPump, 0);
  KalioDosingComplete();
}
void KalioDosingComplete() {
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(2);
  tft.setCursor(0, 150);
  tft.print("Dosed Kalio ");
  tft.print(pump2_count);
  tft.println(" time(s) on ");
  tft.print(pump_day_2);
  tft.print(" / ");
  tft.print(pump_month_2);
  tft.print(" ");
  if (time2_H < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(time2_H);
  } else tft.print(time2_H);
  tft.print(": ");
  if (time2_M < 10) { //PRINT A 0 IN FRONT OF MINUTES IF LESS THAN 10
    tft.print ('0');
    tft.print(time2_M);
  } else  tft.print(time2_M);
  tft.print(": ");
  if (time2_S < 10) {//PRINT A 0 IN FRONT OF SECONDS IF LESS THAN 10
    tft.print ('0');
    tft.print(time2_S);
  } else tft.println(time2_S);
}

void KalioDosing_OFF() {
  digitalWrite(RelayKalioPump, 0);
}

//EXELL
void EXELLDosing_ON() {
  DateTime now = RTC.now();
  time3_H = now.hour();
  time3_M = now.minute();
  time3_S = now.second();
  digitalWrite(RelayAlgiPump, 1);
  pump3_count ++;
  delay(pump_delay_3);
  digitalWrite(RelayAlgiPump, 0);
  ExellDosingComplete();
}
void ExellDosingComplete() {
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(2);
  tft.setCursor(0, 195);
  tft.print("Dosed EXELL ");
  tft.print(pump3_count);
  tft.println(" time(s) on ");
  tft.print(pump_day_3);
  tft.print(" / ");
  tft.print(pump_month_3);
  tft.print(" ");
  if (time3_H < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(time3_H);
  } else tft.print(time3_H);
  tft.print(": ");
  if (time3_M < 10) { //PRINT A 0 IN FRONT OF MINUTES IF LESS THAN 10
    tft.print ('0');
    tft.print(time3_M);
  } else tft.print(time3_M);
  tft.print(": ");
  if (time3_S < 10) {//PRINT A 0 IN FRONT OF SECONDS IF LESS THAN 10
    tft.print ('0');
    tft.println(time3_S);
  } else tft.println(time2_S);
}

void EXELLDosing_OFF() {
  digitalWrite(RelayAlgiPump, 0);

}

//========================================================================================================================
// ΩΡΕΣ ΧΟΡΗΓΗΣΗΣ ΤΡΟΦΗΣ
//

int FEED1_HOUR = 14,
    FEED1_MINUTE = 15,
    FEED1_SEC = 0;

int FEED2_HOUR = 14,
    FEED2_MINUTE = 20,
    FEED2_SEC = 0;

void FeedingStatus() {
  DateTime now = RTC.now();
  if (hasDosedFood1 == false && hasDosedFood2 == false) {

    tft.setTextColor(YELLOW, BLACK); //set color for DOSING
    tft.setTextSize(2);
    tft.setCursor(0, 100);
    tft.println("System will feed at ");
    if (FEED1_HOUR < 0) {
      tft.print ("0");
      tft.print(FEED1_HOUR);
    } else tft.print(FEED1_HOUR);
    tft.print(":");
    if (FEED1_MINUTE < 0) {
      tft.print ("0");
      tft.print(FEED1_MINUTE);
    } else
      tft.print(FEED1_MINUTE);
    tft.print(" & ");
    if (FEED2_HOUR < 0) {
      tft.print ("0");
      tft.print(FEED2_HOUR);
    } else tft.print(FEED2_HOUR);
    tft.print(":");
    if (FEED2_MINUTE < 0) {
      tft.print ("0");
      tft.print(FEED2_MINUTE);
    } else
      tft.print(FEED2_MINUTE);

  } else if (hasDosedFood1 == true && hasDosedFood2 == true ) {
    Feed1Complete();
    Feed2Complete();
  }
  else if (hasDosedFood1 == true && hasDosedFood2 == false) {
    Feed1Complete ();
    tft.setCursor(0, 150);
    tft.println("System will feed at ");
    if (FEED2_HOUR < 0) {
      tft.print ("0");
      tft.print(FEED2_HOUR);
    } else tft.print(FEED2_HOUR);
    tft.print(":");
    if (FEED2_MINUTE < 0) {
      tft.print ("0");
      tft.print(FEED2_MINUTE);
    } else
      tft.print(FEED2_MINUTE);

  }
  //ΠΡΩΤΟ ΤΑΙΣΜΑ
  Feed1_count ;
  Feed1_day = now.day();
  Feed1_month = now.month();
  
  //ΔΕΥΤΕΡΟ ΤΑΙΣΜΑ
  Feed2_count ;
  Feed2_day = now.day();
  Feed2_month = now.month();
  
  //ΕΝΕΡΓΟΠΟΙΗΣΗ ΠΡΩΤΟΥ ΤΑΙΣΜΑΤΟΣ
  
  if (now.hour() == FEED1_HOUR && now.minute() == FEED1_MINUTE && hasDosedFood1 == false) {
  
    hasDosedFood1 = true;
    Food1_ON();
    Serial.println("Feeding1 ON");
  
  }  else  {
    Food1_OFF();
  }
  
  //ΕΝΕΡΓΟΠΟΙΗΣΗ ΔΕΥΤΕΡΟΥ ΤΑΙΣΜΑΤΟΣ
  
  if (now.hour() == FEED2_HOUR && now.minute() == FEED2_MINUTE && hasDosedFood2 == false) {
  
    hasDosedFood2 = true;
    Food2_ON();
    Serial.println("Feeding2 ON");
  
  }  else {
    Food2_OFF();
  }
}

void Food1_ON() {
  DateTime now = RTC.now();
  Feedtime1_H = now.hour();
  Feedtime1_M = now.minute();
  Feedtime1_S = now.second();
  digitalWrite(RelayFeeder, 1);
  Feed1_count ++;
  delay (Feed1_delay);
  digitalWrite(RelayFeeder, 0);
  Feed1Complete();
}

void Feed1Complete() {
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(2);
  tft.setCursor(0, 100);
  tft.print("Fed fish ");
  tft.print(Feed1_count);
  tft.println(" time(s) on ");
  tft.print(Feed1_day);
  tft.print(" / ");
  tft.print(Feed1_month);
  tft.print(" ");
  if (Feedtime1_H < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime1_H);
  } else
    tft.print(Feedtime1_H);
  tft.print(": ");
  if (Feedtime1_M < 10) { //PRINT A 0 IN FRONT OF MINUTES IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime1_M);
  } else
    tft.print(Feedtime1_M);
  tft.print(": ");
  if (Feedtime1_S < 10) {//PRINT A 0 IN FRONT OF SECONDS IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime1_S);
  } else  tft.println(Feedtime1_S);
}

void Food1_OFF() {
  digitalWrite(RelayFeeder, 0);
}


void Food2_ON() {
  DateTime now = RTC.now();
  Feedtime2_H = now.hour();
  Feedtime2_M = now.minute();
  Feedtime2_S = now.second();
  digitalWrite(RelayFeeder, 1);
  Feed2_count ++;
  delay (Feed2_delay);
  digitalWrite(RelayFeeder, 0);
  Feed2Complete();
}

void Feed2Complete() {
  tft.setTextColor(GREEN, BLACK); //set color for DOSING
  tft.setTextSize(2);
  tft.setCursor(0, 100);
  tft.print("Fed fish ");
  tft.print(Feed2_count);
  tft.println(" time(s) on ");
  tft.print(Feed2_day);
  tft.print(" / ");
  tft.print(Feed2_month);
  tft.print(" ");
  if (Feedtime2_H < 10) {//PRINT A 0 IN FRONT OF HOUR IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime2_H);
  } else
    tft.print(Feedtime2_H);
  tft.print(": ");
  if (Feedtime2_M < 10) { //PRINT A 0 IN FRONT OF MINUTES IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime2_M);
  } else
    tft.print(Feedtime2_M);
  tft.print(": ");
  if (Feedtime2_S < 10) {//PRINT A 0 IN FRONT OF SECONDS IF LESS THAN 10
    tft.print ('0');
    tft.print(Feedtime2_S);
  } else  tft.println(Feedtime2_S);
}

void Food2_OFF() {
  digitalWrite(RelayFeeder, 0);
}

