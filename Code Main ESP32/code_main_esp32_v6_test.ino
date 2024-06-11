#include "file_setting.h"
#include "file_code_mp3.h"
#include "file_code_gps.h"
#include "file_code_display.h"
#include "file_code_wifi.h"
#include "file_code_mqtt.h"


void setup() {
  Serial.begin (115200); 
  Serial2.begin(9600);
	
  inisial_lcd();        // Inisial LCD.

  lcd_display_holdon("Inisial DF Player","Proses...",700);
  insial_df_player();   // Inisial DF Player.

  lcd_display_holdon("Koneksi Wifi","Proses...",700);
  // Koneksi ESP32 Ke Wifi.
  if( koneksiWifi() == true ){

    // Inisial MQTT.
    lcd_display_holdon("Koneksi MQTT","Proses...",700);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

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

  while(false){
    Serial.print(analogRead(12));
    Serial.print(" - ");
    Serial.print(analogRead(14));
    Serial.print(" - ");
    Serial.print(analogRead(27));
    Serial.print(" - ");
    Serial.print(analogRead(26));
    Serial.print(" - ");
    Serial.print(analogRead(25));
    Serial.print(" - ");
    Serial.print(analogRead(33));
    Serial.print(" - ");
    Serial.print(analogRead(32));
    Serial.print(" - ");
    Serial.print(analogRead(35));
    Serial.print(" - ");
    Serial.print(analogRead(34));
    Serial.print(" - ");
    Serial.println();
    delay(1000);
  }
}

bool isFirstTime = true ;
unsigned long tmUpdateData = millis();


//-----> Kode basic handle sepeda listrik.
unsigned long tmProses = millis();
int port_handle = 32 ;
float read_vin_handle(){
  return ( float(analogRead(port_handle)) / 4099.0 ) * 5.0 ;
}
//--------------------------------------------------//

void loop() {
  // Kode akses server mqtt realtime.
  mqtt_realtime();

  // Kode baca data gps.
  read_data_gps();

  // Tampilkan data GPS.
  //show_position();

  // Update data Jam.
  updateDataNTP();


  // Kode periksa server masih mengirim topik atau tidak.
  status_server =  (millis()-tm_server_connect) > timeout_server_connect ;

  if( (millis()-tmUpdateData) > 10000 ){
    String json = createJsonString();
    send_2_mqtt( json );
    tmUpdateData = millis();
  }

  // Baca pin ADC untuk handle GAS.
  float vin_handle = read_vin_handle();
  long val_cvrt    = map( (vin_handle*100), 150, 400, 15, 27);
  int  val_int     = (int)val_cvrt ;

  // Jika di bawah 1.5Volt, Handle gas tidak di nyalakan.
  if( vin_handle < 1.5 ){
      mp3_stop ();    // Matikan Mp3 Player.
      status = "0" ;  // Set status ke nol, Motor Padam.
  }
  // Jika lebih dari 1.5, maka handle gas di tekan.
  else{
    // Jika mp3 player tidak sedang memainkan suara, maka bunyikan suara.
    if( is_mp3_player_busy() == false ){
      Serial.println("Play..." + String(is_mp3_player_busy(),DEC));
      mp3_play (1);  
    }
    // Setting mp3 player sesuai dengan tarikan handle gas.
    mp3_set_volume (val_int);
    status = "0" ;  // Set status ke nol, Motor Menyala.
  }

  //
  //lcd_display_realtime( String(vin_handle,1), String(val_int,DEC) ,500);
  lcd_basic_multi();

	delay(100);
}
