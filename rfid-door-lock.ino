/*
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */
 //8B 65 03 0C
 //76 05 5B FA

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <LedControl.h>

int DIN = A5; 
int CS =  A6;
int CLK = A7;

LedControl lC(DIN,CLK,CS,0);
byte count[10][3] = 
{
  {0x7E,0x42,0x7E},
  {0x08,0x04,0x7E},
  {0x64,0x52,0x4c},
  {0x4A,0x4A,0x7E},
  {0x0E,0X08,0x7E},
  {0x5E,0x52,0X72},
  {0x7E,0x52,0x72},
  {0x02,0x02,0x7E},
  {0x7E,0x4A,0x7E},
  {0x5E,0x52,0x7E}
};


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SS_PIN 53
#define RST_PIN 5

int counter = 0;
const int button = 49;//Open from inside
const int AccessLED = 9;
const int DenyLED = 7;
const int Buzzer = 8;
const int unlock = 10;
bool flag = 0;//0=> denies access to everyone 1=> gives acces to everyone
long int buttP = 3927310080;
long int buttN = 4161273600;
long int buttEQ = 4127850240;
long int buttCH = 3108437760;
byte id[6] = {0x00,0x00,0x00,0x00,0x00,0x00};

class person {
public:
  person( byte i[6] = id , bool a =  false, const char* n = "Guest")
  {
    setperson(n, i, a);
  }
  void setperson(const char* n, byte i[6], bool a)
  {
    Name = n;
    for(int j = 0;j<6;j++)
    {
   ID[j] = i[j] ;
    }
    setaccess(a) ;
  }
  void setaccess(bool a)
  {access =a;}
  const char* getname()
  {
    return Name;
   }
  byte ID[6];
   bool getaccess()
   {return access;}
  ~person()
  {}
private:
  const char* Name;
  
  bool access;
};









byte iD[6] = {0x63, 0x05, 0xB6, 0x25};

byte iD1[6] = {0x76, 0x05 ,0x5B,0xFA};

byte iD2[6] = {0x91,0x64,0x90,0x09};
person J(iD1,true,"Jawad");
person R(iD,true,"Rita");
person People[100]={J,R};




 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

void setup() { 
  pinMode(button,INPUT);
  pinMode(AccessLED,OUTPUT);
  pinMode(DenyLED,OUTPUT);
  pinMode(unlock,OUTPUT);
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
 lC.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
lC.setIntensity(0,15);       // Set the brightness to maximum value
lC.clearDisplay(0); 
printLC();// and clear the display
  }



const int RECV_PIN = 3;




 
void loop() {
 

 IRrecv irrecv(RECV_PIN);
 decode_results results;
   irrecv.enableIRIn();
   delay(200);
digitalWrite(AccessLED,LOW);
digitalWrite(DenyLED,LOW);
if(digitalRead(button))
{
  if(counter>0)
  counter--;
  printLC();
  Serial.println("button");
   lcd.clear();
  lcd.print("Door opened");
  lcd.setCursor(0,1);
  lcd.print("for exiting");
  Serial.println("Access Granted");
  Serial.print("Welcome ");
  Open();
  analogWrite(8,160);
  digitalWrite(AccessLED,HIGH);
    delay(250);
    analogWrite(8,255);
    delay(250);
    analogWrite(8,160);
    delay(500);
    digitalWrite(AccessLED,LOW);
    analogWrite(8,0);
    delay(2600);
    Close();return;
  }
if (irrecv.decode())
{
  //display pressed button code
unsigned long int value = irrecv.decodedIRData.decodedRawData;
  Serial.println(value);
  //button 4 opens from inside
  if(value == buttCH)
    {    if(counter<100)
      counter++;
      printLC();
      //Display pressed button code im hex
      Serial.println(value, HEX);
      //print on lcd
      lcd.clear();
      lcd.print("Door Open");
      lcd.setCursor(0,1);
      lcd.print("for entering");
      //Open door
      digitalWrite(AccessLED,HIGH);
      analogWrite(8,160);
      Open();
      delay(2500);
      Close();
       delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            analogWrite(8,0);
      digitalWrite(AccessLED,LOW);
      
     
      //repeat loop()
      return;
    }
   else if(value == buttEQ)
   {
      AccessAll(flag);
    
      if(flag == 0)
      {
        analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
        lcd.clear();
    
        Serial.println("Access taken from everyone");
 
        lcd.print("Access taken");
        lcd.setCursor(0,1);
        lcd.print("from everyone");
          delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
        flag = 1;return;
      }
    
      else
      {
        Serial.println("Everyone has access");
        analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
        lcd.clear();
        lcd.print("Everyone has ");
        lcd.setCursor(0,1);
        
        lcd.print("access ");
          delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
        flag =0;return;
       }
    }
  
  else if(value == buttP)
    {
      digitalWrite(AccessLED,HIGH);      
      long int t = millis();
      while(millis()-t<5000)
      {Serial.println(value);
        if ( ! rfid.PICC_IsNewCardPresent())
        {}

        // Verify if the NUID has been readed
        else if ( ! rfid.PICC_ReadCardSerial())
        {}

        //TEST ID 
        else
        {
          int pos = Test(rfid.uid.uidByte);

          if(pos <0)
          {
            person NEW(rfid.uid.uidByte,true);
            int Pos;
            Pos = Test(id);
            People[Pos] = NEW;
            analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
            lcd.clear();
            lcd.print("New person saved");
            lcd.setCursor(0,1);
            lcd.print("with access");
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
            Serial.println("New person saved with access");
            delay(200);
            return;
          }
          else if(People[pos].getaccess()==false)
          {
            People[pos].setaccess(true);
            analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
            lcd.clear();
            lcd.print(People[pos].getname());
            lcd.setCursor(0,1);
            lcd.print("Now has access");
            Serial.print(People[pos].getname());
            Serial.println(" Now has access");
              delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
            delay(200);
            return;
           }
          lcd.clear();
          lcd.print("Card's owner ");
   
          lcd.print(People[pos].getname());
          lcd.setCursor(0,1);
          lcd.print("already has access");
          Serial.print("Card's owner ");
          Serial.print(People[pos].getname());
          Serial.println(" already has access");
          delay(200);
            return;
        }
            } Serial.println("here" );Serial.println(value); return;}

    //else resume checking for codes 
    
   else if(value == buttN)
   {
      digitalWrite(DenyLED,HIGH); 
     long int t = millis();
      while(millis()-t<5000)
      {Serial.println(results.value);
        if ( ! rfid.PICC_IsNewCardPresent())
        {}

        // Verify if the NUID has been readed
        else if ( ! rfid.PICC_ReadCardSerial())
        {}

        //TEST ID 
        else
        {
          int pos = Test(rfid.uid.uidByte);

          if(pos <0)
          {
            person NEW(rfid.uid.uidByte,false);
            int Pos;
            Pos = Test(id);
            People[Pos] = NEW;
            analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
            lcd.clear();
            lcd.print("New person saved");
            lcd.setCursor(0,1);
            lcd.print("without access");
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
            Serial.println("New person saved without access");
            delay(200);
            return;
          }
          else if(People[pos].getaccess()==true)
          {
            People[pos].setaccess(false);
            analogWrite(8,160);
            digitalWrite(AccessLED,HIGH);
            digitalWrite(DenyLED,HIGH);
            lcd.clear();
            lcd.print(People[pos].getname());
            lcd.setCursor(0,1);
            lcd.print("Now has no access");
            Serial.print(People[pos].getname());
            Serial.println(" Now has no access");
              delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            delay(250);
            analogWrite(8,255);
            delay(250);
            analogWrite(8,160);
            digitalWrite(AccessLED,LOW);
            digitalWrite(DenyLED,LOW);
            analogWrite(8,0);
            delay(200);
            return;
           }
          lcd.clear();
          lcd.print("Card's owner ");
   
          lcd.print(People[pos].getname());
          lcd.print(" already");
          lcd.setCursor(0,1);
          lcd.print("has no access");
          Serial.print("Card's owner ");
          Serial.print(People[pos].getname());
          Serial.println(" already has no access");
          delay(200);
            return;
        }
            } Serial.println("here" );Serial.println(value); 
   
            return; }
            else{
   return;}
  }
  
/*if (remote detected) {Open door and counter++}*/
/*if (button 3 detected) {Open door and counter--}*/

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//TEST ID 
int pos = Test(rfid.uid.uidByte);

  if(pos <0)
  {
    lcd.clear();
    lcd.print("UNKNOWN RFID");
    lcd.setCursor(0,1);
    lcd.print("Access Denied");
    Serial.println("NO OPEN -1");
    analogWrite(8,160);
    digitalWrite(DenyLED,HIGH);
    delay(1000);
    digitalWrite(DenyLED,LOW);
    analogWrite(8,0);
      return;
  }

  else if(People[pos].getaccess() == 0) 
  {
    lcd.clear();
    lcd.print(People[pos].getname());
    lcd.print(" attempting to");
    lcd.setCursor(0,1);
    lcd.print("enter... Access denied");
    Serial.print(People[pos].getname());
    Serial.println(" Attempting to enter");
    analogWrite(8,160);
    digitalWrite(DenyLED,HIGH);
    delay(1000);
    digitalWrite(DenyLED,LOW);
    analogWrite(8,0);
    delay(200);
  return;}

  /*Open()*/
  if(counter<100)
  counter++;
  printLC();
  lcd.clear();
  lcd.print("Access Granted");
  lcd.setCursor(0,1);
  lcd.print("Welcome ");
  lcd.print(People[pos].getname());
  Serial.println("Access Granted");
  Serial.print("Welcome ");
  Open();
  Serial.println(People[pos].getname());
  analogWrite(8,160);
  digitalWrite(AccessLED,HIGH);
    delay(250);
    analogWrite(8,255);
    delay(250);
    analogWrite(8,160);
    delay(500);
    digitalWrite(AccessLED,LOW);
    analogWrite(8,0);
    delay(2500);
    Close();

  
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
   return;
  }
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return;
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

int Test(byte i[6])
{
  //assume it doesn't exist
  for(int j = 0;j<100;j++)
  {
    int rightCount = 0;
    for(int k = 0;k<6;k++)
    {
      if(i[k] != People[j].ID[k])
      break;
      else{rightCount++;}
        
    }
    if(rightCount == 6)
    return j;
  }
  return -1; 
}

void AccessAll(bool acc)
{
  for(int i=0;i<100;i++)
  {
    People[i].setaccess(acc);
    }
  }


void Open()
{
  digitalWrite(unlock,HIGH);
}
void Close()
{
  digitalWrite(unlock,LOW);
  }
  void printLC()
  {
    lC.clearDisplay(0); 
    for(int i=0;i<3;i++)
  {
    lC.setRow(0,i,count[counter/10][i]);
  }
  lC.setRow(0,3,0x00);
  lC.setRow(0,4,0x00);
  for(int i=5;i<8;i++)
  {
    lC.setRow(0,i,count[counter%10][i-5]);
  }
    }
