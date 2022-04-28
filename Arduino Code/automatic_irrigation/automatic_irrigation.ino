#include<LiquidCrystal.h>
#include <SoftwareSerial.h>

#define echo 9
#define trigger 10
#define tank_pump 4
#define watering_pump 13
#define moisture_sensor A0

long duration;
int distance;
int moisture_value;
int distance_percent;
int moist_percent;

SoftwareSerial SIM900(2, 3);
LiquidCrystal lcd(12,11,8,7,6,5);

void setup ()
{
  lcd.begin(20,4);
  SIM900.begin(9600);
  Serial.begin(9600);
  
  pinMode(echo,INPUT);
  pinMode(moisture_sensor,INPUT);
  pinMode(trigger,OUTPUT);
  digitalWrite(trigger,LOW);
  pinMode(watering_pump,OUTPUT);
  pinMode(tank_pump,OUTPUT);
  digitalWrite(watering_pump,LOW);
  digitalWrite(tank_pump,LOW);
  lcd.setCursor(0,1);
  lcd.print("      WELCOME");
  delay(500);
  lcd.clear(); 
}

void loop()
{
  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH); //to get the reading, we need to set the trigger to high and the low for some microseconds
  delayMicroseconds(10); 
  digitalWrite(trigger,LOW);
  duration = pulseIn(echo,HIGH);
  distance = duration * 0.017; //0.017 = formula 
  distance_percent = map(distance, 0, 1023, 0, 100); //converting to percentage
  moisture_value = analogRead(moisture_sensor);
  moist_percent = map(moisture_value, 0, 1023, 0, 100); //converting to percentage
  condition();
}

void condition()
{
  if(distance_percent < 65 && moist_percent < 85)
  {
    LCD_1();
    digitalWrite(tank_pump,HIGH);
    digitalWrite(watering_pump,HIGH);
    
    sms4();
    sms3();
    
    delay(1000);
  }

  else if(distance_percent < 65 && moist_percent > 85)
  {
    LCD_2();
    digitalWrite(tank_pump,HIGH);
    digitalWrite(watering_pump,LOW);
    
    sms4();
    sms1();
    
    delay(1000);
  }
  
  else if(distance_percent > 65 && moist_percent < 85)
  {
    LCD_3();
    digitalWrite(tank_pump, LOW);
    digitalWrite(watering_pump, HIGH);
    
    sms2();
    sms3();
    
    delay(1000);
  }
  
  else if(distance_percent > 65 && moist_percent > 85)
  {
    LCD_4();
    digitalWrite(tank_pump,LOW);
    digitalWrite(watering_pump,LOW);
    
    sms2();
    sms1();
    
    delay(1000);
  }
}

void sms1()
{
  SIM900.println("Message to \"01966901950\""); //recipient's mobile number
  SIM900.println("WATERING PUMP IS OFF"); //message to send
  
  SIM900.println((char)26); //end at command with a ^ Z, ASCII code 26
  Serial.println((char)26);
  SIM900.println();
  SIM900.println();
  SIM900.println();
}

void sms2()
{ 
  SIM900.println("Message to \"01966901950\"");// recipient's mobile number
  
  SIM900.println("TANK PUMP IS OFF"); //message to send
  Serial.println("TANK PUMP IS OFF"); //message to send
  
  SIM900.println((char)26); //end at command with a ^ Z, ASCII code 26
  Serial.println((char)26);
}

void sms3()
{
  SIM900.println("Message to \"01966901950\""); //recipient's mobile number
  
  SIM900.println("WATERING PUMP IS ON"); //message to send
  Serial.println("WATERING PUMP IS ON"); //message to send
  
  SIM900.println((char)26); //end at command with a ^ Z, ASCII code 26
  Serial.println((char)26);
  SIM900.println();
  SIM900.println();
  SIM900.println();
}

void sms4()
{
  SIM900.println("Message to \"01966901950\""); //recipient's mobile number
  
  SIM900.println("TANK PUMP IS ON"); //message to send
  Serial.println("TANK PUMP IS ON"); //message to send

  SIM900.println((char)26); //end at command with a ^Z, ASCII code 26
  Serial.println((char)26);
}

void LCD_1()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TANK LEVEL    = ");
  lcd.print(distance_percent);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("MOIST CONTENT = ");
  lcd.print(moist_percent);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("W-PUMP STATUS =");
  lcd.print(" ON");
  lcd.setCursor(0,3);
  lcd.print("T-PUMP STATUS =");
  lcd.print(" ON");
}

void LCD_2()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TANK LEVEL    = ");
  lcd.print(distance_percent);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("MOIST CONTENT = ");
  lcd.print(moist_percent);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("W-PUMP STATUS =");
  lcd.print(" OFF");
  lcd.setCursor(0,3);
  lcd.print("T-PUMP STATUS =");
  lcd.print("  ON");
}
  
void LCD_3()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TANK LEVEL    = ");
  lcd.print(distance_percent);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("MOIST CONTENT = ");
  lcd.print(moist_percent);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("W-PUMP STATUS =");
  lcd.print(" ON");
  lcd.setCursor(0,3);
  lcd.print("T-PUMP STATUS =");
  lcd.print(" OFF");
}

void LCD_4()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TANK LEVEL    = ");
  lcd.print(distance_percent);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("MOIST CONTENT = ");
  lcd.print(moist_percent);
  lcd.print("%");
  lcd.setCursor(0,2);
  lcd.print("W-PUMP STATUS =");
  lcd.print(" OFF");
  lcd.setCursor(0,3);
  lcd.print("T-PUMP STATUS =");
  lcd.print(" OFF");
}
