Compass::Compass(){
  //initializer stuff
  Wire.beginTransmission(COMPASS_ADDRESS); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void Compass::printDirection(){
 //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(COMPASS_ADDRESS, 6, true);
  if(6 <= Wire.available()){
    CompX = Wire.read()<<8; //X msb
    CompX |= Wire.read(); //X lsb
    CompZ = Wire.read()<<8; //Z msb
    CompZ |= Wire.read(); //Z lsb
    CompY = Wire.read()<<8; //Y msb
    CompY |= Wire.read(); //Y lsb
  }
  Serial.print("\rCompX: ");
  Serial.print(CompX);
  Serial.print("  CompY: ");
  Serial.print(CompY);
  Serial.print("  CompZ: ");
  Serial.print(CompZ);
  Serial.print("                ");
}

float Compass::getAzimuth(){
  
}

