Accel::Accel(){
  //initializer stuff
     //Wire.begin();
     Wire.beginTransmission(ACCELEROMETER_ADDRESS);
     Wire.write(0x6B);  // PWR_MGMT_1 register
     Wire.write(0);     // set to zero (wakes up the MPU-6050)
     Wire.endTransmission(true);
}

void Accel::printAccel(){
  Wire.beginTransmission(ACCELEROMETER_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ACCELEROMETER_ADDRESS,6,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Serial.print("Accelerometer: ");
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY); //max when platform is vertical (19000 to -19000)
  Serial.print(" | AcZ = "); Serial.println(AcZ); //max when platform is flat (0 to 19000)
}

