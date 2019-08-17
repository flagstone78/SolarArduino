Accel::Accel(float _offsetx, float _offsety, float _offsetz):offsetx(_offsetx),offsety(_offsety),offsetz(_offsetz){  //MPU 6050
  //initializer stuff
     //Wire.begin();
     /*Wire.beginTransmission(ACCELEROMETER_ADDRESS);
     Wire.write(0x6B);  // PWR_MGMT_1 register
     Wire.write(0);     // set to zero (wakes up the MPU-6050)
     Wire.endTransmission(true);*/
     I2c.write(ACCELEROMETER_ADDRESS,0x6B,(uint8_t)0);
}


void Accel::takeAccel() {
  /*Wire.beginTransmission(ACCELEROMETER_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ACCELEROMETER_ADDRESS, 6, true);  // request a total of 14 registers
  int x = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  int y = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  int z = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)*/
  uint8_t data[6];
  I2c.read(ACCELEROMETER_ADDRESS,0x3B,6,data);
  int x = data[0] << 8 | data[1];  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  int y = data[2] << 8 | data[3];  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  int z = data[4] << 8 | data[5];  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)*/
  ac.x = ac.x*0.9 + 0.1*x;
  ac.y = ac.y*0.9 + 0.1*y;
  ac.z = ac.z*0.9 + 0.1*z;
}

void Accel::printAccel(){
  takeAccel();
  Serial.print("Accelerometer: ");
  Serial.print("AcX = "); Serial.print(ac.x);
  Serial.print(" | AcY = "); Serial.print(ac.y); //max when platform is vertical (19000 to -19000)
  Serial.print(" | AcZ = "); Serial.println(ac.z); //max when platform is flat (0 to 19000)
}

float Accel::getZenith() {
  takeAccel();
  xyz ref( offsetx, -1+offsety, offsetz );
  float magAccel = magnitude(ac);
  float magRef = magnitude(ref);
  // find the magnitude of the the cross produc
  //float mag_cross = sqrt(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
  // determine the zenith angle - angle relative to the verticle
  float angle = acos(dot(ac, ref) / (magAccel * magRef));
  //Serial.println(angle*180/3.141596);
  return angle;
}
