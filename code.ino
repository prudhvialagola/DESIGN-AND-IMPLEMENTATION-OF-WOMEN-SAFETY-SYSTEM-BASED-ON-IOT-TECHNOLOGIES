
#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int sw = 8;
int buz = 9;
int relay = 10;


void ggg();
void gps();
char str[70];

String gpsstr = "";

char *test = "$GPGGA";

String lat = "No Range      ";
String lon = "No Range     ";

int temp = 0, i, dis;
boolean flag = 0;


void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(sw, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(buz, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WOMEN SAFETY");
  delay(1000);
  lcd.clear();
}

void loop() {
  gps();
  delay(1000);
  if (digitalRead(sw) == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EMERGENCY");
    digitalWrite(relay,HIGH);
    emergency();
    digitalWrite(buz, HIGH);
    delay(1000);
    digitalWrite(buz, LOW);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAT:");
    lcd.print(lat);
    lcd.setCursor(0, 1);
    lcd.print("LG:");
    lcd.print(lon);
    lcd.clear();
    iot("*LAT: " + String(lat) + "#");
    delay(1000);
    iot("*LG: " + String(lon) + "#");
    delay(1000);
    iot("*EMERGENCY#");
    delay(1000);

  }
  if (digitalRead(sw) == 0)
  {
    digitalWrite(relay,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAFE");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAT:");
    lcd.print(lat);
    lcd.setCursor(0, 1);
    lcd.print("LG:");
    lcd.print(lon);
    lcd.clear();
    iot("*LAT: " + String(lat) + "#");
    delay(1000);
    iot("*LG: " + String(lon) + "#");
    delay(1000);

  }
}
void emergency()
{
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+918309540942\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("EMERGENCY ALERT");// The SMS text you want to send
  delay(100);
  Serial.println("LAT:");// The SMS text you want to send
  delay(100);
  Serial.println(lat);// The SMS text you want to send
  delay(100);
  Serial.println("LG:");// The SMS text you want to send
  delay(100);
  Serial.println(lon);// The SMS text you want to send
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ggg()
{
  gpsstr = "";
  while (1)
  {
    while (Serial.available() > 0)
    {
      char inChar = (char)Serial.read();
      gpsstr += inChar;
      i++;
      if (i < 7)
      {
        if (gpsstr[i - 1] != test[i - 1])
        {
          i = 0;
          gpsstr = "";
        }
      }
      if (inChar == '\r')
      {
        if (i > 65)
        {
          flag = 1;
          break;
        }
        else
        {
          i = 0;
        }
      }
    }
    if (flag)
      break;
  }
}


void gps()
{
  flag = 0;
  int x = 0;
  while (flag == 0)
  {
    ggg();
    int str_lenth = i;
    lat = "";
    lon = "";
    int comma = 0;
    while (x < str_lenth)
    {
      if (gpsstr[x] == ',')
        comma++;
      if (comma == 2)
        lat += gpsstr[x + 1];
      else if (comma == 4)
        lon += gpsstr[x + 1];
      x++;
    }
    int l1 = lat.length();
    lat[l1 - 1] = ' ';
    l1 = lon.length();
    lon[l1 - 1] = ' ';
    lcd.setCursor(0, 0);
    lcd.print("l:");
    lcd.setCursor(2, 0);
    lcd.print(lat);

    lcd.setCursor(0, 1);
    lcd.print("L:");
    lcd.setCursor(2, 1);
    lcd.print(lon);
    delay(1000);

    i = 0; x = 0;
    str_lenth = 0;
    delay(1000);
    lcd.clear();
  }
}
void iot(String stringdata)
{
  for (int i = 0; i < stringdata.length(); i++)
  {
    Serial2.write(stringdata[i]);
  }
  delay(1000);
}
