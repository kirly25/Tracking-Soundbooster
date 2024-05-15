#include "file_setting.h"
#include "file_code_mp3.h"
#include "file_code_gps.h"
#include "file_code_mpu6050.h"
#include "file_code_display.h"
#include "file_code_wifi.h"
#include "file_code_firebase.h"


void setup() {
  Serial.begin (115200); 
  Serial2.begin(9600);
	
  inisial_lcd();        // Inisial LCD.
  lcd_display_holdon("Inisial MPU6050","Proses...",700);
  inisial_mpu6050();    
  get_min_mpu6050();

  lcd_display_holdon("Inisial DF Player","Proses...",700);
  insial_df_player();   // Inisial DF Player.

  lcd_display_holdon("Koneksi Wifi","Proses...",700);
  // Koneksi ESP32 Ke Wifi.
  if( koneksiWifi() == true ){
    // ESP32 ke Firebase.
    lcd_display_holdon("Koneksi Server","Proses...",700);
    init_firebase();

    // Inisial NTP.
    lcd_display_holdon("Koneksi NTP","Proses...",700);
    timeClient.begin();
    timeClient.setTimeOffset(25200);
    
    // Pengambilan data NTP Pertama.
    lcd_display_holdon("Update NTP","Proses...",700);
    timeClient.update();
    time_hour = getHour();
    time_date = getDate();

  }else{
    // Tampilkan, koneksi gagal.
    Serial.println("Koneksi Wifi, gagal..");
    // Loop untuk berhenti.
    while( true ){
      delay(100);
    }
  }
}

bool isFirstTime = true ;
unsigned long tmUpdateData = millis();

void loop() {
  // Kode baca data gps.
  read_data_gps();

  // Tampilkan data GPS.
  //show_position();

  // Update data Jam.
  updateDataNTP();

  // update magnitudo.
  readMagnitude();


  if( isDeviceFound == false ){
    search_data_device_in_firebase();
    show_data_device();
  }

  if( isDeviceFound == true ){
    if( (millis()-tmUpdateData) > 1000 ){
      update_data_device();
      tmUpdateData = millis();
    }
  }

  if( isDeteksiGetaran() == true ){
    if( is_mp3_player_busy() == false ){
	    mp3_play ();  
      Serial.println("MP3 Play Run....");
      lcd_display_holdon("Play MP3","First .....", 2000);
    }
  }else{
    mp3_stop ();
    Serial.println("Mp3 Padam..");
  }

  //
  lcd_basic_multi();

  // delay sistem
	delay(5);
}
