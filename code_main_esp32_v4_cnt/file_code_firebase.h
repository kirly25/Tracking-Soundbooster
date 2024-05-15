

/*
 * Fungsi : Koneksi ESP32 Ke Wifi Dan Firebase.
 * 
 * Tujuan : Menghubungkan ESP32 ke Server Firebase.
 *          Setting parameter Firebasem sebelum di gunakan.
 *          
 * Step Kerja Koneksi Firebase :
 * 
 *  1. Inisial akun firebase.
 *  2. Inisial Wifi Ke OOP Firebase.
 *  3. Setting Pengaturan Firebase.
 */
void init_firebase(){
  

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email     = USER_EMAIL;
  auth.user.password  = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);


  //WiFi reconnect timeout (interval) in ms (10 sec - 5 min) when WiFi disconnected.
  config.timeout.wifiReconnect = 10 * 1000;

  //Socket connection and SSL handshake timeout in ms (1 sec - 1 min).
  config.timeout.socketConnection = 10 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 45 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 1 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;

  //Note:
  //The function that starting the new TCP session i.e. first time server connection or previous session was closed, the function won't exit until the
  //time of config.timeout.socketConnection.
  //You can also set the TCP data sending retry with
  config.tcp_data_sending_retry = 1;

}



void sendString( String firebasePath, String val){
  if ( Firebase.RTDB.setString(&fbdo, firebasePath, val ) == false ) {
    Serial.print("Gagal menambahkan data pengguna ke Firebase: ");
    Serial.println(fbdo.errorReason());
  } else {
    Serial.println("Data : " + val + " -> Sukses");
  }
}



void show_data_device(){
  Serial.print("Key : ");
    Serial.println(key);
    //
    Serial.print("Id Device : ");
    Serial.println(idchip);
    
    Serial.print("name : ");
    Serial.println(name);
    //
    Serial.print("status : ");
    Serial.println(isDeviceFound);
    //
    Serial.println();
}


void showData_Firebase(){


  if ( Firebase.RTDB.getArray(&fbdo, "/data_device/") == false ) {
    Serial.print("Gagal membaca data dari Firebase: ");
    Serial.println(fbdo.errorReason());
    return;
  }

  // Mengonversi JSON dari Firebase ke objek JSON di Arduino
  DynamicJsonDocument jsonDocument(1024); // Sesuaikan ukuran dokumen sesuai kebutuhan
  DeserializationError error = deserializeJson(jsonDocument, fbdo.payload());

  if (error) {
    Serial.print("Gagal mengonversi JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Akses nilai per indeks
  Serial.println();
  //Serial.println("Read By Index ....");
  Serial.println();
  for (JsonPair pair : jsonDocument.as<JsonObject>()) {
    JsonObject obj = pair.value();
    Serial.print("Key : ");
    Serial.println(pair.key().c_str());
    //
    Serial.print("Id Device : ");
    Serial.println(obj["id"].as<String>());
    //
    Serial.print("latitude : ");
    Serial.println(obj["latitude"].as<String>());
    //
    Serial.print("longitude : ");
    Serial.println(obj["longitude"].as<String>());
    //
    Serial.print("name : ");
    Serial.println(obj["name"].as<String>());
    //
    Serial.print("status : ");
    Serial.println(obj["status"].as<String>());
    //
    Serial.println();
  }

}


void search_data_device_in_firebase(){


  if ( Firebase.RTDB.getArray(&fbdo, "/data_device/") == false ) {
    Serial.print("Gagal membaca data dari Firebase: ");
    Serial.println(fbdo.errorReason());
    return;
  }

  // Mengonversi JSON dari Firebase ke objek JSON di Arduino
  DynamicJsonDocument jsonDocument(1024); // Sesuaikan ukuran dokumen sesuai kebutuhan
  DeserializationError error = deserializeJson(jsonDocument, fbdo.payload());

  if (error) {
    Serial.print("Gagal mengonversi JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Akses nilai per indeks
  Serial.println();
  //Serial.println("Read By Index ....");
  Serial.println();
  for (JsonPair pair : jsonDocument.as<JsonObject>()) {

    // Parse Data Json Ke String
    JsonObject obj = pair.value();
    String temp_key     = pair.key().c_str();
    String temp_id      = obj["id"].as<String>();
    String temp_lat     = obj["latitude"].as<String>();
    String temp_lng     = obj["longitude"].as<String>();
    String temp_name    = obj["name"].as<String>();
    String temp_status  = obj["status"].as<String>();

    // Pencocokan data key.
    if( temp_id.indexOf(idchip) != -1 ){
      key           = temp_key      ;
      name          = temp_name     ;
      status        = temp_status   ;
      isDeviceFound = true          ;
      // Keluar Perulangan.
      break ;
    }
    
  }

}


void update_data_device(){
  //
  String addr = "data_device/" + key + "/" ;
  //
  sendString( addr + "latitude"   ,  String(latitude      , 6));
  sendString( addr + "longitude"  ,  String(longitude     , 6));
  sendString( addr + "mpu6050"    ,  String(magnitudo     , 2));
  sendString( addr + "status"     ,  String(isDeteksiGetaran() , DEC));
  sendString( addr + "status_gps" ,  String(isGPS_Success , DEC));
  
}


