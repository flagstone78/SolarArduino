#define max14 16384

Encoder::Encoder(uint8_t address, int16_t Offset, bool EncoderInvert)
:addr(address), offset(Offset), encoderInvert(EncoderInvert){
  outOfRange = false;
}

//Get the position from the as5048 encoder
uint16_t Encoder::getPosition(){

  I2c.read(addr,0xFB,5,dataBuffer);
  outOfRange = dataBuffer[0];
  int16_t val = dataBuffer[3] << 6 | dataBuffer[4];
  if(encoderInvert){val = max14 - val;}

  //Serial.print(val);
  //Serial.println("/16383 ");
  
  val -= offset;
  if(val < 0){val += max14;}
  else if (val >= max14){val -= max14;}
  return (val);
}

//convert raw value into angle
float Encoder::getAngle(){
  return 2*PI*getPosition()/16384.0; //convert 14bit value to angle
}

uint8_t Encoder::getOutOfRange(){return outOfRange;}
