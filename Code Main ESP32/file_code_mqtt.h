


// Function to convert variables to JSON string
String createJsonString() {
  // Create a JSON document
  StaticJsonDocument<200> doc;

  // Add data to the JSON document
  doc["idchip"]     = idchip                ;
  doc["latitude"]   = String(latitude,7)    ;
  doc["longitude"]  = String(longitude,7)    ;
  doc["status"]     = status                ;

  // Create a string to hold the JSON output
  String jsonString;
  serializeJson(doc, jsonString);

  // Return the JSON string
  return jsonString;
}




WiFiClient espClient;
PubSubClient client(espClient);

// Function to convert String to char array and return it
char* stringToCharArray(String str) {
  // Allocate memory for the char array including the null terminator
  int strLength = str.length() + 1; // +1 for the null terminator
  char* charArray = new char[strLength];
  
  // Copy the String to the char array
  str.toCharArray(charArray, strLength);
  
  // Return the char array
  return charArray;
}




void str_separator(  String input ){
  // Menentukan karakter pemisah
  char separator = ',';

  // Array untuk menyimpan hasil pemisahan
  String parts[4];
  int index = 0;

  // Variabel sementara untuk menyimpan setiap bagian
  String temp = "";

  // Iterasi melalui setiap karakter dalam string input
  for (int i = 0; i < input.length(); i++) {
    // Jika karakter saat ini adalah pemisah, tambahkan bagian ke array
    if (input.charAt(i) == separator) {
      parts[index++] = temp;
      temp = "";  // Mengosongkan variabel sementara
      } else {
        temp += input.charAt(i);  // Menambahkan karakter ke variabel sementara
      }
  }

  // Menambahkan bagian terakhir ke array
  parts[index] = temp;

  name = parts[2];

  // Mencetak hasil pemisahan
  for (int i = 0; i <= index; i++) {
    Serial.println(parts[i]);
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "" ;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
  Serial.println("Pesan Masuk MQTT : " + msg );
  if( msg.indexOf(idchip) != -1 ){
    str_separator(msg);
  }
  // Reset timer, untuk memeberukan status. Server masih connect.
  tm_server_connect = millis();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(stringToCharArray(topik_tx));
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_realtime(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

unsigned long lastMsg     = 0;

void send_2_mqtt( String data ){

  char* topik_chr = stringToCharArray(topik_rx);
  char* data_chr  = stringToCharArray(data);

  // Kirim Data Ke MQTT.
  client.publish(topik_chr, data_chr);

  // Tampilkan Pesan Ke Serial Monitor.
  Serial.print("Publish message: ");
  Serial.println(data_chr);
}
