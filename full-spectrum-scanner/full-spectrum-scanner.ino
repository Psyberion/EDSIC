// Pins
int bFSS = 2;
int bStg1 = 4;
bool bFSSpressed = false;
int bStg1pressed = 0;
int iFSS = 3;
int iStg1 = 5;
unsigned long iFSSdelay = 0;
boolean FSSon = false;

void setup() {
  // Buttons
  pinMode(bFSS,INPUT_PULLUP);
  pinMode(bStg1,INPUT_PULLUP);
  // Indicators
  pinMode(iFSS,OUTPUT);
  digitalWrite(iFSS,LOW);
  pinMode(iStg1,OUTPUT);
  digitalWrite(iStg1,LOW);
}

void loop() {
  // -----------------------------------
  // FSS
  if (digitalRead(bFSS) == LOW) {
    if (!bFSSpressed) {
      bFSSpressed = true;
      if (!FFSon) {
        FFSon = true;
        iFSSdelay = millis();
        digitalWrite(iFSS,HIGH);
      }
    }
  }
  else
    bFSSpressed = false;
    
  if (iFSSdelay > 0 && millis() - iFSSdelay > 2000) {
    digitalWrite(iFSS,LOW);
    if (bFSSpressed == 0)
      iFSSdelay = 0;
  }    

  // -----------------------------------
  // Stage 1
  if (
  if (digitalRead(bStg1) == LOW) {
    if (bStg1pressed == 0) {
      bStg1pressed = 1;
      digitalWrite(iStg1,HIGH);
    }
  }
}