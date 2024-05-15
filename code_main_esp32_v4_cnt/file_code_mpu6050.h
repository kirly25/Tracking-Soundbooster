#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void inisial_mpu6050() {
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}


float mid_amp   = 0.0 ;
float magnitudo = 0.0 ;
unsigned long timer_deteksi = millis();

bool isDeteksiGetaran(){
  return (millis() - timer_deteksi) < 5000 ;
}


unsigned long tms_get_decibel = 0.0 ;
float max_db = 0.0 ;
float min_db = 1000.0 ;
float ampl   = 0.0 ;
float res_amp = 0.0 ;

void read_dbs( float value ){
  if( max_db < value ){
    max_db = value ;
  }

  if( min_db > value ){
    min_db = value ;
  }
  ampl = max_db - min_db ;
  res_amp = ampl - mid_amp ;
  if( res_amp < 0 ) res_amp = 0.0 ;

  if( (millis()-tms_get_decibel) > 500 ){
    max_db = 0.0 ;
    min_db = 1000.0 ;
    tms_get_decibel = millis();
  }
}

void readMagnitude() {
  
  float sampleCount = 10 ;
  float totalX = 0.0 , totalY = 0.0 , totalZ = 0.0 ;
  for (int i = 0; i < sampleCount; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    totalX += a.acceleration.x;
    totalY += a.acceleration.y;
    totalZ += a.acceleration.z;
    delay(10);
  }

  float avgX  = totalX / sampleCount;
  float avgY  = totalY / sampleCount;
  float avgZ  = totalZ / sampleCount;
  float rata  = ( avgX + avgY + avgZ ) / 3.0 ;

  magnitudo = rata ;
  read_dbs( rata );
  if( res_amp > 0.5 ){
    timer_deteksi = millis();
  }
}


void get_min_mpu6050(){
  unsigned long tm_get = millis();
  while( (millis()-tm_get) < 5000 ){
    readMagnitude();
    mid_amp = ampl ;
    delay(10);
  }
}