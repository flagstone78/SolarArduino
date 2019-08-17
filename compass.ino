Compass::Compass(){
  //initializer stuff
  /*Wire.beginTransmission(COMPASS_ADDRESS); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();*/
  I2c.write(COMPASS_ADDRESS,0x02,(uint8_t)0);
}

void Compass::takeMeasurement() {
  int x, y, z = 0;
  /*//Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(COMPASS_ADDRESS, 6, true);
 
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }*/
  uint8_t data[6];
  I2c.read(COMPASS_ADDRESS,0x03,6,data);
  x = data[0] << 8 | data[1];      
  z = data[2] << 8 | data[3];  
  y = data[4] << 8 | data[5];

  /*
  //QMC588L
  Wire.beginTransmission(COMPASS_ADDRESS);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(COMPASS_ADDRESS, 6);

  if (6 <= Wire.available()) {
  x = Wire.read(); //X lsb
  x |= Wire.read() << 8; //X msb
  z = Wire.read(); //Z lsb
  z |= Wire.read() << 8; //Z msb
  y = Wire.read(); //Y lsb
  y |= Wire.read() << 8; //Y msb
  }*/

  comp.x = x;// comp.x*0.9 + 0.1*x;
  comp.y = y;// comp.y*0.9 + 0.1*y;
  comp.z = z;// comp.z*0.9 + 0.1*z;
}

void Compass::printDirection(){
  takeMeasurement();
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
  xyz ref(1, 0, 0);
  float magComp = magnitude(comp);
  float magRef = magnitude(ref);

  float angle = acos(dot(comp, ref) / (magComp * magRef));
  Serial.print(" compass angle to ref = "); Serial.println(angle * 180 / 3.141596);
  return angle;
}
