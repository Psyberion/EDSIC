// =============================================================================
// Full Spectrum Scanner (FSS) control panels
// EDSIC (Elite Dangerous Ships Integrated Control) Sub-Project
// EDSIS (Elite Dangerous Ships Integrated Systems) Project
// Copyright ©2023 David McMurray
// =============================================================================

// Pins
int b_actv = 2;   // FSS activate/deactivate
int i_actv = 3;
int b_stg1 = 4;   // Stage 1 - pre-charge containment grid
int i_stg1 = 5;
int b_stg2 = 6;   // Stage 2 - engage power coupling
int i_stg2 = 7;
int b_honk = 8;   // Engage system honk
int i_honk = 9;
int b_cncl = 10;  // Cancel all stages of system honk (1, 2 and engage)
int i_cncl = 11;

// State flags (functions)
bool ff_actv = false;

// State flags (buttons)
volatile bool fb_actv = false;
volatile bool fb_stg1 = false;
volatile bool fb_stg2 = false;
volatile bool fb_honk = false;
volatile bool fb_cncl = false;

// State flags (indicators)
bool fi_actv = false;
bool fi_stg1 = false;
bool fi_stg2 = false;
bool fi_honk = false;
bool fi_cncl = false;

// Timers
unsigned long t_actv = 0;

void setup() {
  // Buttons
  pinMode(b_actv,INPUT_PULLUP);
  pinMode(b_stg1,INPUT_PULLUP);
  pinMode(b_stg2,INPUT_PULLUP);
  pinMode(b_honk,INPUT_PULLUP);
  pinMode(b_cncl,INPUT_PULLUP);
  
  // Indicators
  pinMode(i_actv,OUTPUT);
  pinMode(i_stg1,OUTPUT);
  pinMode(i_stg2,OUTPUT);
  pinMode(i_honk,OUTPUT);
  pinMode(i_cncl,OUTPUT);

  // Init indicators
  digitalWrite(i_actv,LOW);
  digitalWrite(i_stg1,LOW);
  digitalWrite(i_stg2,LOW);
  digitalWrite(i_honk,LOW);
  digitalWrite(i_cncl,LOW);

  // Register interrupt service routines
  attachInterrupt(digitalPinToInterrupt(b_actv),Activate,FALLING);
  attachInterrupt(digitalPinToInterrupt(b_stg1),Stage1,FALLING);
  attachInterrupt(digitalPinToInterrupt(b_stg2),Stage2,FALLING);
  attachInterrupt(digitalPinToInterrupt(b_honk),Honk,FALLING);
  attachInterrupt(digitalPinToInterrupt(b_cncl),Cancel,FALLING);

  // Start serial comms
  Serial.begin(115200);
  Serial.println("FSS system initiated");
}

void loop() {
  // Activate/deactivate pressed
  if (fb_actv) {
    Serial.println("Activate button pressed");
    ff_actv = !ff_actv;
    if (ff_actv) {
      Serial.println("FSS on");
      fi_actv = true;
      digitalWrite(i_actv,HIGH);
      t_actv = millis();  
    }
    else {
      Serial.println("FSS off");
      i_actv = false;
      digitalWrite(i_actv,LOW);
      t_actv = 0;

      if (fi_stg1) {
        fi_stg1 = false;
        digitalWrite(i_stg1,LOW);
      }
      if (fi_stg2) {
        fi_stg2 = false;
        digitalWrite(i_stg2,LOW);
      }
      if (fi_honk) {
        fi_honk = false;
        digitalWrite(i_honk,LOW);
      }
      if (fi_cncl) {
        fi_cncl = false;
        digitalWrite(i_cncl,LOW);
      }
    }
  }

  if (ff_actv) {
    // Active light flash when on
    if (fi_actv) {
      if (millis()-t_actv >= 1800) {
        fi_actv = false;
        digitalWrite(i_actv,LOW);
        t_actv = millis();
      }
    }
    else {
      if (millis()-t_actv >= 200) {
        fi_actv = true;
        digitalWrite(i_actv,HIGH);
        t_actv = millis();
      }
    }

    // Stage 1 pressed
    if (fb_stg1 && !fi_stg1) {
      Serial.println("Stage 1 pressed");
      fi_stg1 = true;
      digitalWrite(i_stg1,HIGH);
    }

    // Stage 2 pressed
    if (fb_stg2 && fi_stg1 && !fi_stg2) {
      Serial.println("Stage 2 pressed");
      fi_stg2 = true;
      digitalWrite(i_stg2,HIGH);
    }

    // Honk pressed
    if (fb_honk && fi_stg1 && fi_stg2 && !fi_honk) {
      Serial.println("Honk pressed");
      fi_honk = true;
      digitalWrite(i_honk,HIGH);
    }

    // Cancel pressed
    if (fb_cncl) {
      Serial.println("Cancel pressed");
      fi_cncl = true;
      digitalWrite(i_cncl,HIGH);
    }
  }
  
  ResetButtonFlags();
}

void Activate() {
  fb_actv = true;
}

void Stage1() {
  fb_stg1 = true;
}

void Stage2() {
  fb_stg2 = true;
}

void Honk() {
  fb_honk = true;
}

void Cancel() {
  fb_cncl = true;
}

void ResetButtonFlags() {
  fb_actv = false;
  fb_stg1 = false;
  fb_stg2 = false;
  fb_honk = false;
  fb_cncl = false;
}
