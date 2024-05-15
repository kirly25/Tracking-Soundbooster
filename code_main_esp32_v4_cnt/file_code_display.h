// Library Display.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


// Inisial Objeck LCD.
LiquidCrystal_I2C lcd(  i2c_address, lcd_row, lcd_col);

// Inisial Timer.
unsigned long tmLCD = millis();

// Tampilan LCD Dashboard.
void lcd_display_realtime( String text1 , String text2 , int update_ ){
  if( (millis()-tmLCD) > update_){
    lcd.clear();
    //
    lcd.setCursor(0,0);
    lcd.print(text1);
    //
    lcd.setCursor(0,1);
    lcd.print(text2); 
    // 
    tmLCD = millis();
  }
}



// Tampilan LCD Kalibrasi Load Sensor
void lcd_display_holdon( String text1 , String text2 , int delay_ ){
  lcd.clear();
  //
  lcd.setCursor(0,0);
  lcd.print(text1);
  //
  lcd.setCursor(0,1);
  lcd.print(text2); 
  //
  delay( delay_ );
}



// Kode Multi Display

// paremeter Number Display 
byte idx_display = 0 ;
byte maxDisplay  = 5 ;

// Pindah Otomatis.
unsigned long tmChangeAuto = millis();
int intervalChange = 2000 ;

void changeAuto(){
  if( (millis()-tmChangeAuto) > intervalChange ){
    // Counter
    idx_display++ ;
    // Reset Counter Display,
    if( idx_display >= maxDisplay ){
      idx_display = 0 ;
    }
    // Reset
    tmChangeAuto = millis();
  }
}

// Fungsi Menampilkan Data Display
void lcd_basic_multi(){

  // Panggil Fungsi Change Auto.
  changeAuto();
  
  if( idx_display == 0 ){
    
    String stat = "Wifi Disconnect" ;
    if( isDeviceConnectWifi() ) stat = "Wifi Connect" ;

    String stat2 = "Server Disconnect" ;
    if( Firebase.ready() ) stat2 = "Server Connect" ;

    lcd_display_realtime(
        stat  ,
        stat2 ,
        700
      );
  }
  else if( idx_display == 1 ){
    String stat = "GPS Not Found" ;
    if( isGPS_Success ) stat = "GPS Found" ;
    lcd_display_realtime(
        "Status GPS",
        stat ,
        700
      );
  }
  else if( idx_display == 2 ){
    lcd_display_realtime(
        "Lat  : " + String(latitude,3) ,
        "Long : " + String(longitude,3),
        700
      );
  }
  else if( idx_display == 3 ){
    float temp = (millis() - timer_deteksi) ;
    String stat = "Tidak Ada Getar" ;
    if( isDeteksiGetaran() ) stat = "Ada Getaran" ;
    lcd_display_realtime(
        "Status MPU6050",
        stat ,
        700
      );
  }
  else if( idx_display == 4 ){
    lcd_display_realtime(
        "Nama Perangkat" ,
        name ,
        700
      );
  }
}

void inisial_lcd(){
  // Inisial Wire.
  Wire.begin();
  // Inisial LCD 2x16 .
  lcd.begin();
  lcd.backlight();
  lcd.print("Loading,,,");
  //
  lcd_display_holdon("Test Multi","Auto..",1000);
}
