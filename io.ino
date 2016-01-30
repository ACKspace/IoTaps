bool currentState = FALSE;//stroage for current measured button state
bool lastState = FALSE;//storage for last measured button state
bool debouncedState = FALSE;//debounced button state

uint8_t debounceInterval = 50;//wait 20 ms for button pin to settle
unsigned long timeOfLastButtonEvent = 0;//store the last time the button state changed

void handleIO()
{
  currentState = digitalRead(A3);
  unsigned long currentTime = millis();
  
  if (currentState != lastState){
    timeOfLastButtonEvent = currentTime;
  }
  
  if (currentTime - timeOfLastButtonEvent > debounceInterval){
    if (currentState != debouncedState)
    {
      //if the current state is still different than our last stored debounced state
      debouncedState = currentState;
      
      bpState = debouncedState;
    }
  }
  
  lastState = currentState;

  // Figure out the desired output state
  if (fissaMode)
  {
    digitalWrite(A0, HIGH);
  }
  else
  {
    if (bpState)
    {
      digitalWrite(A0, HIGH);
    }
    else
    {
      digitalWrite(A0, LOW);
    }
  }
}

