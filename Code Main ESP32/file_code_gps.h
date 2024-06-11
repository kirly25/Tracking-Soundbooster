#include <TinyGPSPlus.h>

// Inisial Objeck GPS.
TinyGPSPlus gps;

// Variabel Status GPS Err Or Not.
bool isGPS_Success = true ;




//  Inisial Fungsi.
void set_position_ToGlobal();

// Fungsi pengambilan data GPS.
void read_data_gps(){
  // Periksa jika ada pesan masuk dari GPS.
  while (Serial2.available() > 0){
    // Proses Encode Kode NMEA dari pesan char yang terbaca.
    if (gps.encode(Serial2.read()))
      // Jika kordinat di dapat. Simpan ke Variabel global. 
      set_position_ToGlobal();
  }
  // Periksa timer Gps Failure.
  if( (millis()-tm_gps_fail) > max_time_gps_fail ) isGPS_Success = false ;
}


// Fungsi pemindahan data GPS ke Variabel Global.
void set_position_ToGlobal(){
  // Periksa jika lokasi valid.
  if (gps.location.isValid())
  {
    // Set ke variabel global.
    latitude  = gps.location.lat();
    longitude = gps.location.lng() ;
    // Set True Gps Found.
    isGPS_Success = true ;
    // Reset Timer GPS.
    tm_gps_fail = millis();
  }else{
    // Set False Gps Not Found.
    isGPS_Success = false ;
  }
}



// Tampilkan data gps.
unsigned long tms = millis();

void show_position(){
  if( (millis()-tms) > 2000 ){
    // Tampilkan data gps.
    Serial.println();
    if( isGPS_Success == true ){
      if( isGPS_Success )
        Serial.println("GPS Neo 6m -> Found ");
      else
        Serial.println("GPS Neo 6m -> Not Found ");
      Serial.println("Latitude  : " + String(latitude,5));
      Serial.println("longitude : " + String(longitude,5));
    }else{
      Serial.println("GPS Error Or Not Found...");
    }
    // reset timer.
    tms = millis();
  }
}
