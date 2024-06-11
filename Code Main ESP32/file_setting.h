#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Setting Wifi SSID dan Password
const char* ssid        = "sakha"             ;
const char* password    = "prime468328"       ;
const char* mqtt_server = "test.mosquitto.org";


// Parameter Topik MQTT
String topik_rx = "receiver" ;
String topik_tx = "transmitter" ;

// Pin status mp3 player posisi standby atau menyala.
const byte digital_busy = 4 ;

// Parameter lcd [ Alamat Module I2C, Jumlah baris LCD dan Jumlah Komlom LCD ]
const byte  i2c_address   = 0x27  ;
const byte  lcd_row       = 16    ;
const byte  lcd_col       = 2     ;

// Inisial Fungsi Apakah Device Terkoneksi Dengan Wifi.
bool isDeviceConnectWifi();

// Parameter Status Alat.
String  idchip        = "123344"  ;
String  name          = "null"        ;
String  key           = "null"        ;
String  status        = "null"        ;
bool    isDeviceFound = false     ;
bool    status_device = false     ;

// Timer untuk menghitung waktu timeout gps tidak mendapatkan data posisi.
unsigned long tm_gps_fail = millis();
int max_time_gps_fail     = 5000 ;

// Variabel Posisi GPS Latitude dan Longitude.
float latitude  = 0.0 ;
float longitude = 0.0 ;

// Timer untuk menghitung waktu server mengirim data ke topik mqtt.
// jika dalam beberapa waktu tidak mengirim, maka server dianggap-
// disconnect.
unsigned long tm_server_connect       = millis();
float         timeout_server_connect  = 25000 ;
bool          status_server           = false ;