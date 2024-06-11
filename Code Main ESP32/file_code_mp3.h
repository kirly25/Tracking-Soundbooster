/* 
 *  Librari : Akses DF Player 
 *  Nama    : 
 *    [ 1 ] SoftwareSerial = Librari untuk mensetting pin ESP8266 menjadi-
 *                           pin komunikasi serial usart. 
 *                           Tujuan :
 *                           karena esp8266 hanya memiliki 1 port usart -
 *                           bawaan. Sehingga perlu 1 port lagi untuk - 
 *                           akses DF Player.
 *    [ 2 ] DFPlayer_Mini_Mp3 = Librari untuk akses ke komponen DF Player-
 *                              untuk menjalankan perintah memainkan suara.
 *                                   
 */
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


/*
 * Fungsi  : Fungsi untuk insial awal akses ke komponen DF Player.
 * 
 * Step    : [1] Inisial software serial untuk akses ke df player.
 *               setting baud rate/kecepatan transfer data. Sesuaikan-
 *               dengan default DF Player, yaitu 9600.
 *               
 *           [2] Setting objeck software serial ke objeck df player.
 *               Sehingga perintah df player akan menggunakan saluran-
 *               komunikasi software serial.
 *           
 *           [3] Setting volume awal DF Player saat akan di jalankan.
 *               
 *           [2] Inisial pin input untuk port busy Df Player. Pin-
 *               untuk membaca status DF Player dalam kondisi-
 *               diam atau sedang menjalankan suara.
 *               
 *               
 */

 void insial_df_player(){
    mp3_set_serial (Serial2);    // Set objeck DF Player
    delay(1);                     // Delay
    mp3_set_volume (15);          // Setting Volume Df Player.
    pinMode(digital_busy, INPUT); // Inisial pin busy Df Player.
 }


/*
 * Fungsi : Fungsi untuk membaca status dari DF Player. Status DF Player- 
 *          dalam kondisi sibuk atau tidak.
 *          
 * Step   : [1] Baca pin digital input dari pin busy.
 *          [2] Jika pin digital adalah false, maka kondisi sibuk.
 *          [3] Jika pin digital adalah true, maka kondisi diam.
 *          
 * output : Output akan berbeda dengan staus digital. Sesuai dengan-
 *          nama fungsi. "is_mp3_player_busy" atau apakah mp3player
 *          dalam kondisi sibuk.
 *          
 *          
 *          Output Fungsi :
 *          True  = DF Player dalam kondisi sibuk.
 *          False = DF Player dalam kondisi tidak sibuk.
 *        
 */
 
 bool is_mp3_player_busy(){
   return digitalRead(digital_busy) == false ; 
 }
