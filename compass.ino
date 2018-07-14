Compass::Compass(){
  //initializer stuff
  Wire.beginTransmission(COMPASS_ADDRESS); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void Compass::takeMeasurement() {
  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(COMPASS_ADDRESS, 6, true);
  int x,y,z = 0;
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  comp.x = comp.x*0.9 + 0.1*x;
  comp.y = comp.y*0.9 + 0.1*y;
  comp.z = comp.z*0.9 + 0.1*z;
}

void Compass::printDirection(){
  Serial.print("\rCompX: ");
  Serial.print(comp.x);
  Serial.print("  CompY: ");
  Serial.print(comp.y);
  Serial.print("  CompZ: ");
  Serial.print(comp.z);
  Serial.print("                ");
}

float Compass::getAzimuth(){
  takeMeasurement();
  xyz ref(0, 0, 1);
  float magComp = magnitude(comp);
  float magRef = magnitude(ref);

  float angle = acos(dot(comp, ref) / (magComp * magRef));
  Serial.print(" compass angle to ref = "); Serial.println(angle * 180 / 3.141596);
  return angle;
}

