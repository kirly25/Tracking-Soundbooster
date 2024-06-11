// Library Arduino.h
#include <Arduino.h>

// Library Firebase
#include <WiFi.h>

// Library RTC Online 
#include <NTPClient.h>
#include <WiFiUdp.h>

//----> Kode Akses Wifi <-----------------------------------//


/*
 * Fungsi   : Konversi status wifi bentuk number ke keterangan status wifi.
 * Tujuan   : Mempermudah mengetahui kode status koneksi wifi. Karena seti-
 *            ap kode memiliki fungsi masing2. 
 *            
 * Status dalam bentuk nomor dan keterangan nya :
 * 0 : WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
 * 1 : WL_NO_SSID_AVAILin case configured SSID cannot be reached
 * 3 : WL_CONNECTED after successful connection is established
 * 4 : WL_CONNECT_FAILED if connection failed
 * 6 : WL_CONNECT_WRONG_PASSWORD if password is incorrect
 * 7 : WL_DISCONNECTED if module is not configured in station mode
 * 
 * INPUT  : Status Wifi dari fungsi 'WiFi.status()'
 * Output : String dalam bentuk keterangan.
*/
String codeWifi2Keterangan( byte codeNumber ){
  if( codeNumber == 0 ) return "IDLE.." ;
  else if( codeNumber == 1 ) return "NO SSID.." ;
  else if( codeNumber == 3 ) return "CONNECT.." ;
  else if( codeNumber == 4 ) return "FAILED.." ;
  else if( codeNumber == 6 ) return "WRONG PASS.." ;
  else if( codeNumber == 7 ) return "DISCONNECT.." ;
  return "none" ;
}



/*
 * Fungsi : Konversi IP Address Wifi Ke String. 
 * Input  : Objeck Ip Address.
 * Output : String Ip Address.
 * Contoh : Input Objeck IpAddress  '102.22.3.2'
 *          Output String IpAddress '102.22.3.2'
 */
String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}



/*
 * Fungsi : Kode pemeriksaan status koneksi ESP32 dengan Wifi.
 * Tujuan : Memastikan, bahwa esp32 masih terhubung dengan wifi-
 *          sebagai sumber koneksi internet.
 * Step   : [1] Baca ip address dynamic, Hasil koneksi wifi.
 *          [2] Jika Ip Address bukan 0.0.0.0, maka ESP32 masih-
 *              terkoneksi dengan wifi.
 *          [3] Jika Ip Address adalah 0.0.0.0, maka ESP32 -
 *              tidak terhubung dengan Wifi.
 * Output : Type Boolean.
 *          True  : Masih terhubung.
 *          False : Tidak terhubung.
 */
bool isDeviceConnectWifi(){
  return (WiFi.localIP() == IPAddress(0, 0, 0, 0)) == false ;
}




/*
 * Fungsi : Koneksi ESP32 ke Wifi.
 * Tujuan : Menghubungkan ESP32 dengan wifi, Sehingga dapat-
 *          melakukan akses internet dan menghubungkan ESP32- 
 *          ke Server Firebase.
 * Step   : [1] Mulai koneksi Wifi Dengan Data SSID dan PASS.
 *          [2] Inisial Timer Sebagai Pembatas waktu koneksi.
 *          [3] Tunggu koneksi ESP32 ke WIFI.
 *          [4] Ketika, Timer sudah mencapat batas waktu dan-
 *              belum terhubung, maka keluar perulangan. -
 *              koneksi dianggap gagal.
 *          [5] Ketika koneksi sudah selesai, Sebelum timer-
 *              maximal. koneksi dianggap berhasil dan keluar-
 *              dari perulangan.
 *          [6] Output True, Jika koneksi berhasil.
 *              Output False, Jika koneksi gagal.
 *          
 * Output : Type Boolean.
 *          TRUE : Status koneksi berhasil atau ESP32 ter-
 *                 hubung ke wifi.
 *          FALSE : Status koneksi gagal atau ESP32 tidak-
 *                  terhubung dengan wifi.
 */
bool koneksiWifi(){
  
  // Mulai koneksi WIFI dengan setting SSID, PASSWORD.
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");
  Serial.println("...");
  
  // Inisial variabel timer, pembatas koneksi ke WIFI.
  int countForSpace = 0 ;
  float timeOutWaitConnect = 10*1000;
  unsigned long timerWaitConnectWifi = millis();
  unsigned long tmUpdateWaitWifi = millis();
  
  // Counter Dot Untuk Enter.
  int nDot = 0 ;
  
  // Tunggu koneksi selesai dengan melakukan pemeriksaan status-
  // koneksi.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Kode timer pembatas koneksi. Jika sudah melampau waktu yang -
    // ditentukan dan koneksi belum berhasil, maka koneksi dianggap-
    // gagal dan keluar.
    if( (millis()-timerWaitConnectWifi) > timeOutWaitConnect ){
      // keluar perulangan.
      break;
    }
    
    // Kode pemberi dot matrix ke serial monitor. Tujuan, memberikan-
    // tanda ke serial monitor sebagai debug. Proses koneksi masih-
    // berjalan.
    if( (millis()-tmUpdateWaitWifi) > 1000 ){
      // Tampilkan Dot Pada Serial. Penanda progress akses.
      Serial.print(".");
      if( nDot > 30 ){
        nDot = 0 ;
        Serial.println();
      }
      // Reset Timer.
      tmUpdateWaitWifi = millis();
    }
    // Delay 10ms, mencegah super loop.
    delay(10);
  }

  // Periksa, jika koneksi dengan wifi berhasil.
  if( isDeviceConnectWifi() == true ){
    // Tampilkan Ip Address, Ke Serial Monitor.
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    // Return True, Tanda koneksi sukses.
    return true ;
  }else{
    // Tampilkan pesan gagal koneksi.
    Serial.println();
    Serial.println("ESP32 Failed Connect WIFI..");
    return false ;
  }

}







// Inisial NTP->RTC Online.
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {
  "Minggu", "Senin", "Selasa", 
  "Rabu", "Kamis", "Jumat", "Sabtu"
};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

// Pengambilan data jam ntp.
String getHour(){
  String result = "" ;
  result += String(timeClient.getHours()  ,DEC)   + ":"   ;
  result += String(timeClient.getMinutes(),DEC) + ":"   ;
  result += String(timeClient.getSeconds(),DEC) ;
  return result ;
}

// Pengambilan data jam tanggal.
String getDate(){
  
  time_t  epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  // Generated Tanggal .
  String result = "" ;
  result += String(monthDay     ,DEC)   + ":"   ;
  result += String(currentMonth ,DEC)   + ":"   ;
  result += "2023";
  // Return Nilai.
  return result ;
}

// Parameter Update Jam.
String time_hour = "" ;
String time_date = "" ;
unsigned long tmTimes = millis();

void updateDataNTP(){
  
  // Periksa Timer Update.
  if( (millis()-tmTimes) > 30000 ){
    // Update data NTP.
    timeClient.update();
    // Update.
    time_hour = getHour();
    time_date = getDate();
    // reset timer.
    tmTimes = millis();
  }
}
