#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result

 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   



 return btnNONE;  // when all others fail, return this...
}

int ylaSek = 0;
int alaSek = 0;
int ylaMin = 0;
int alaMin = 0;
int ylaTun = 0;
int alaTun = 0;

String ylatime, alatime;
boolean ylaOn = false;
boolean alaOn = false;

byte ae[8] = {
  0b01010,
  0b00000,
  0b01110,
  0b00001,
  0b01111,
  0b10001,
  0b01111,
  0b00000
};

void setup()
{
  lcd.createChar(0, ae);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Gurula");
  lcd.setCursor(0, 1);
  lcd.print("Coffee TMR v0.1");  
  delay(3000);  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Yl");
  lcd.write((uint8_t)0);  
  lcd.print(": ");
  lcd.setCursor(0,1);
  lcd.print("Ala:");
}
 
void loop()
{

 ylatime = "";
 ylatime = ylatime + ylaTun + "h ";
 ylatime = ylatime + ylaMin + "m ";
 ylatime = ylatime + ylaSek + "s";
 ylatime = ylatime + "   ";

 lcd.setCursor(5,0);
 lcd.print(ylatime);

 alatime = "";
 alatime = alatime + alaTun + "h ";
 alatime = alatime + alaMin + "m ";
 alatime = alatime + alaSek + "s";
 alatime = alatime + "   ";
 
 lcd.setCursor(5,1);
 lcd.print(alatime);
 
 lcd_key = read_LCD_buttons();

if(ylaOn) {
 ylaSek++;
}
if(alaOn) {
 alaSek++;
}

 if (ylaSek == 60) {
  ylaSek = 0;
  ylaMin++;
 }
 if (ylaMin == 60) {
  ylaMin = 0;
  ylaTun++;
 }

 if (alaSek == 60) {
  alaSek = 0;
  alaMin++; 
 }
 if (alaMin == 60) {
  alaMin = 0;
  alaTun++;
 
}
 delay(1000);
 
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.setCursor(5,1);
     lcd.print("RESET&STOP");
     delay(1000);
     lcd.setCursor(5,1);
     lcd.print("          ");
     alaSek = 0;
     alaMin = 0;
     alaTun = 0;
     alaOn = false;
     break;
     }
   case btnLEFT:
     {
     lcd.setCursor(5,0);
     lcd.print("RESET&STOP");
     delay(1000);
     lcd.setCursor(5,0);
     lcd.print("          ");
     ylaSek = 0;
     ylaMin = 0;
     ylaTun = 0;
     ylaOn = false;
     break;
     }
   case btnUP:
     {
     lcd.setCursor(5,0);
     if(ylaOn) {
      lcd.print("RESET      ");
     } else {
      lcd.print("START      ");
     }
     delay(1000);
     ylaSek++;
     lcd.clear();
     lcd.print("Yl");
     lcd.write((uint8_t)0); 
     lcd.print(":");
     lcd.setCursor(0,1);
     lcd.print("Ala:");
     ylaSek = 0;
     ylaMin = 0;
     ylaTun = 0;
     ylaOn = true;
     break;
     }
   case btnDOWN:
     {
     lcd.setCursor(5,1);
     if(alaOn) {
      lcd.print("RESET      ");
     } else {
        lcd.print("START      ");
     }
     delay(1000);
     alaSek++;
     lcd.clear();
     lcd.print("Yl");
     lcd.write((uint8_t)0);
     lcd.print(":");
     lcd.setCursor(0,1);
     lcd.print("Ala:");
     alaSek = 0;
     alaMin = 0;
     alaTun = 0;
     alaOn = true;
     break;
     }
   case btnSELECT:
     {
     lcd.clear();
     lcd.setCursor(2, 0);
     lcd.print("Vaihdetaan:");
     lcd.setCursor(2, 1);
     lcd.print("Yl");
     lcd.write((uint8_t)0); 
     lcd.print(" <-> Ala");
     delay(2000); 
     lcd.setCursor(0,0);
     lcd.print("Yl");
     lcd.write((uint8_t)0);  
     lcd.print(": ");
     lcd.setCursor(0,1);
     lcd.print("Ala: ");
     if(ylaOn) {    
      ylaSek += 2;
     }
     if(alaOn) {
      alaSek += 2;
     }
     int vaihtoSek = ylaSek;
     int vaihtoMin = ylaMin;
     int vaihtoTun = ylaTun;
     boolean vaihtoOn = ylaOn; 
     ylaSek = alaSek;
     ylaMin = alaMin;
     ylaTun = alaTun;
     ylaOn = alaOn;
     alaSek = vaihtoSek;
     alaMin = vaihtoMin;
     alaTun = vaihtoTun;
     alaOn = vaihtoOn;
     break;
     }
     case btnNONE:
     {
     break;
     }
 }

}

