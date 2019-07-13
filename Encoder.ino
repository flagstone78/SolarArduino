#define south 14645
#define max14 16384

Encoder::Encoder(uint8_t address, uint16_t Offset, bool EncoderInvert)
:addr(address), offset(Offset), encoderInvert(EncoderInvert){
  
}

//Get the position from the as5048 encoder
uint16_t Encoder::getPosition(){
  Wire.beginTransmission(addr);
  Wire.write(0xFE);  // starting with register 0xFE (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(addr, 2, true);  // request a total of 2 registers
  int16_t val = Wire.read() << 6 | Wire.read();
  if(encoderInvert){val = max14 - val;}

  //Serial.print(val);
  //Serial.println("/16383 ");
  
  val -= offset;
  if(val < 0){val += max14;}
  else if (val >= max14){val -= max14;}
  return (val);
}
