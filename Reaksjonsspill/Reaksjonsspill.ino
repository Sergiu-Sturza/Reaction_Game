  #include <Servo.h>
  
  // Pins, placeholder pins, endre til de som er i bruk
  const int button1 = 7;
  const int button2 = 6;
  const int green1 = 4;
  const int red1 = 2;
  const int green2 = 3;
  const int red2 = 8;
  const int buzzer = A0;
  const int resetButton = 5;

  //Servo
  const int servoPin = 9; // Placeholder
  Servo myServo;
  int servoAngle = 50;
  int servoTurn = 1;
  unsigned long previousServo = 0;
  const int SERVO_REFRESH = 15;

  // Konstanter
  const int WAIT_MIN = 2000;
  const int WAIT_MAX = 5000;
  const int ROUND_PAUSE = 2000;
  const int DISPLAY_REFRESH = 5;

  // 7-segment pinner A-G, placeholder verdier
  const int segmentPins[7] = {10, 11, 12, 13, A3, A2, A1};
  
  // Segmentmønster for sifre 0-9
  byte digits[10][7] = {
  {1,1,1,1,1,1,0}, //0
  {0,1,1,0,0,0,0}, //1
  {1,1,0,1,1,0,1}, //2
  {1,1,1,1,0,0,1}, //3
  {0,1,1,0,0,1,1}, //4
  {1,0,1,1,0,1,1}, //5
  {1,0,1,1,1,1,1}, //6
  {1,1,1,0,0,0,0}, //7
  {1,1,1,1,1,1,1}, //8
  {1,1,1,1,0,1,1}  //9
  };

  // Display-enable, placeholder verdier
  const int display1 = A4;
  const int display2 = A5;

  // Poeng
  int score1 = 0;
  int score2 = 0;

  // State
  enum GameState {IDLE, WAIT_FOR_SIGNAL, REACTION, ROUND_END};
  GameState state = IDLE;
  
  // Tidtakere
  unsigned long startTime = 0;
  unsigned long waitTime = 0;
  unsigned long previousDisplay = 0;
  
  // Multiplexing
  bool currentDisplay = false; // false = display1, true = display2
  
  void setup() {
    // Knapper
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(resetButton, INPUT_PULLUP);

    // LED og buzzer
    pinMode (green1, OUTPUT);
    pinMode (red1, OUTPUT);
    pinMode (green2, OUTPUT);
    pinMode (red2, OUTPUT);
    pinMode (buzzer, OUTPUT);

    // 7-Segment
    for (int i = 0; i < 7; i++){
    pinMode(segmentPins[i], OUTPUT);
    }

    pinMode(display1, OUTPUT);
    pinMode(display2, OUTPUT);

    // Servo
    myServo.attach(servoPin);
    myServo.write(110);

    // Tilfeldig millis, slik at buzzeren ikke blir forutsigbar
    randomSeed(millis());
    
    GameState state = IDLE;
}
  
  void loop() {

    unsigned long currentMillis = millis();

    // Reset knapp
    if(digitalRead(resetButton) == LOW){
      resetGame();
    }

    // 7-segment multiplexing (Skrur av og på displayene slik at det ser ut som begge er på samtidig og kan vise forskjellige verdier)
    if (currentMillis - previousDisplay >= DISPLAY_REFRESH) {
    previousDisplay = currentMillis;
    if (currentDisplay) {
      displayDigit(score2 % 10, display2);
    } else {
      displayDigit(score1 % 10, display1);
    }
    currentDisplay = !currentDisplay;
  }
  
  // Bevegelse av servomotor
  if(currentMillis - previousServo >= SERVO_REFRESH) {
    previousServo = currentMillis;
    servoAngle += servoTurn;
    if (servoAngle >= 170 || servoAngle <= 50) {
      servoTurn = -servoTurn;
    }
    myServo.write(servoAngle);
  }
    
    switch (state) {

    case IDLE:
      // Slå av alle LED-lysene og gjør klar til ny runde, start ventetid, fra min til max.
      digitalWrite(green1, LOW);
      digitalWrite(green2, LOW);
      digitalWrite(red1, LOW);
      digitalWrite(red2, LOW);
      
      waitTime = random(WAIT_MIN, WAIT_MAX);
      startTime = currentMillis;
      state = WAIT_FOR_SIGNAL;

      break;

    case WAIT_FOR_SIGNAL:
      // Sjekke etter tidlig trykk, kall earlyPress hvis sant. Lager lyd når ventetiden er over og bytter state til react.
      if (digitalRead (button1) == LOW){
        earlyPress(1);
      } if (digitalRead (button2) == LOW) {
        earlyPress(2);
      }
      if (currentMillis - startTime >= waitTime) {
        state = REACTION;
        tone(buzzer, 1000, 200);
    }
      
      break;

    case REACTION:
      // Sjekke etter hvem som trykte først og gi poeng deretter. Oppdatere poeng, bytte state når det er trykt.
      if(digitalRead (button1) == LOW){
        wonRound(1);
      } if (digitalRead (button2) == LOW) {
        wonRound(2);
      }
      break;

    case ROUND_END:
      // Kort pause før ny runde, tilbake igjen til IDLE for å starte ny runde.
      if (currentMillis - startTime >= ROUND_PAUSE) {
      state = IDLE;
      }
      break;
  }

}
  
  //Funksjoner
  // Reseter spillet, setter begge poengene til 0.
  void resetGame() {
    score1 = 0;
    score2 = 0;
    state = IDLE;
  }

  // Reduserer poeng til den som trykket for tidlig, tilsvarende rød LED skal lyse.
  void earlyPress(int player) {
    if(player == 1) {
      digitalWrite(red1, HIGH);
      if (score1 > 0) score1--;
    }
    else {
      digitalWrite(red2, HIGH);
      if (score2 > 0) score2--;
    }
    startTime = millis();
    state = ROUND_END;
  }
  
  // Øker poeng til den som trykket først, tilsvarende grønn LED skal lyse.
  void wonRound(int player) {
    if (player == 1){
      digitalWrite(green1, HIGH);
      score1++;
    }
    else {
      digitalWrite(green2, HIGH);
      score2++;
    } 
    
    // Reseter score om pooeng er større enn 9.
    if(score1 > 9 || score2 > 9) {
      resetGame();
    }
    
    startTime = millis();
    state = ROUND_END;
  }

  // Siffer til valgt display
  void displayDigit(int num, int displayPin) {
  digitalWrite (display1, LOW);
  digitalWrite(display2, LOW);
  for (int i = 0; i < 7; i++)
    digitalWrite(segmentPins[i], digits[num][i]);
  digitalWrite(displayPin, HIGH);
  }