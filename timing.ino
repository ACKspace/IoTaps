long previousMillis = 0;
uint8_t timerState = 0;

void pollTimer()
{
  //Call timer trigger
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > 1000)
  {
    timerTick();
    previousMillis = currentMillis;
  }
}

void timerTick()
{
  Serial.println(timerState);
  switch (timerState)
  {
    case 0:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrTapSensor + i);
        }
        initSensor(tempAddr);
        break;
      }

    case 1:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrTapSensor + i);
        }
        tapSensorTemp = readSensor(tempAddr);
        Serial.println(tapSensorTemp);
        break;
      }

      case 4:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrOptTemp1 + i);
        }
        initSensor(tempAddr);
        break;
      }

    case 5:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrOptTemp1 + i);
        }
        optSensor1Temp = readSensor(tempAddr);
        Serial.println(optSensor1Temp);
        break;
      }

      case 8:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrOptTemp2 + i);
        }
        initSensor(tempAddr);
        break;
      }

    case 9:
      {
        byte tempAddr[8];
        for (uint8_t i = 0; i < 8; i++) {
          tempAddr[i] = pgm_read_byte_near(addrOptTemp2 + i);
        }
        optSensor2Temp = readSensor(tempAddr);
        Serial.println(optSensor2Temp);
        break;
      }
  }
  timerState++;
  if(timerState > 11) timerState = 0;
}
