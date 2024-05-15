// Library Firebase
#include <Firebase_ESP_Client.h>
// Library Arduino Json
#include <ArduinoJson.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;



// Setting WIFI.
#define WIFI_SSID "Hehehe."
#define WIFI_PASSWORD "namakamu"

// Setting data firebase.

/* 1. Define the API Key */
#define API_KEY "AIzaSyAgLPUdtfYbfRV2ziRXnP0xxV4EGOBV19I"

/* 2. Define the RTDB URL */
#define DATABASE_URL "https://gps-project-4ba2b-default-rtdb.firebaseio.com/" 

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "mrcljade@gmail.com"
#define USER_PASSWORD "badjingan12"



/*
 * Port : Port input digital status DF Player, dalam kondisi sibuk atau -
 *        tidak.
 *        
 * Tujuan : Memeriksa kondifi DF Player. Sedang dalam kondisi menjalankan-
 *          mp3/suara atau dalam kondisi diam/ menunggu perintah.
 *          
 *          Jika akan mengirim perintah mainkan suara hendak nya melakukan- 
 *          pemeriksaan terlebih dahulu. Karena jika tidak memeriksa kondisi, 
 *          maka suara yang sedang di jalankan akan di potong atau di-
 *          hentikan tiba2. ini kurang bagus.
 *          
 * Step  : [1] Baca pin digital busy. jika kondisi adalah true, maka-
 *             DF Player dalam kondisi diam/ Menunggu perintah.
 *             
 *         [2] Dalam kondisi diam, perintah mainkan suara baru bisa di -
 *             jalankan.
 *             
 *         [3] Baca pin digital busy, jika dalam kondisi false, maka-
 *             DF Player dalam kondisi memainkan mp3 player.
 *             
 *         [4] Dalam kondisi ini tunggu sampai selesai.
 *         
 * Pilihan port : Sesuaikan dengan port yang digunakan.
 * 
 */

const byte digital_busy = 4 ;


/*
 * Parameter lcd 2x16 i2c/
 */

const byte  i2c_address   = 0x27  ;
const byte  lcd_row       = 16    ;
const byte  lcd_col       = 2     ;

// Inisial Fungsi
bool isDeviceConnectWifi();
bool isDeteksiGetaran();



String  idchip        = "1234456"  ;
String  name          = "null"        ;
String  key           = "null"        ;
String  status        = "null"        ;
bool    isDeviceFound = false     ;
bool    status_device = false     ;