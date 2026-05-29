#include <LiquidCrystal.h>
#include <Servo.h>

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int SERVO_PIN = 11;
const int LED_ALERTA = 13;

LiquidCrystal lcd(12, 7, 5, 4, 3, 2);
Servo propulsorDesvio;

unsigned long tempoAnteriorLeitura = 0;
const long intervaloLeitura = 500; 

float ultimaDistanciaValida = 400.0;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_ALERTA, OUTPUT);

  propulsorDesvio.attach(SERVO_PIN);
  propulsorDesvio.write(0); // Servo em 0 graus

  lcd.begin(16, 2);
  lcd.print("SISTEMA DE BORDO");
  lcd.setCursor(0, 1);
  lcd.print("STATUS: ONLINE");

  delay(1000); 
  lcd.clear();
}

void loop() {
  unsigned long tempoAtual = millis();

  // Execucao a cada 500ms
  if (tempoAtual - tempoAnteriorLeitura >= intervaloLeitura) {
    tempoAnteriorLeitura = tempoAtual;

    float distanciaAtual = lerUltrassonico();

    // Validacao de leitura do sensor
    if (distanciaAtual <= 0 || distanciaAtual > 400) {
      distanciaAtual = ultimaDistanciaValida; 
      lcd.setCursor(0, 1);
      lcd.print("ERR: SENSOR FALH");
    } else {
      ultimaDistanciaValida = distanciaAtual; 
    }

    // Estado 1: Critico (< 30cm)
    if (distanciaAtual < 30.0) {
      propulsorDesvio.write(90); 
      digitalWrite(LED_ALERTA, HIGH); 

      lcd.setCursor(0, 0);
      lcd.print("CRITICO! OBJETO ");
      lcd.setCursor(0, 1);
      lcd.print("DIST: ");
      lcd.print(distanciaAtual);
      lcd.print("cm   ");

    // Estado 2: Alerta (30cm a 100cm)
    } else if (distanciaAtual >= 30.0 && distanciaAtual <= 100.0) {
      propulsorDesvio.write(0);
      digitalWrite(LED_ALERTA, !digitalRead(LED_ALERTA)); // Pisca LED

      lcd.setCursor(0, 0);
      lcd.print("ALERTA: DETRITO ");
      lcd.setCursor(0, 1);
      lcd.print("DIST: ");
      lcd.print(distanciaAtual);
      lcd.print("cm   ");

    // Estado 3: Nominal (> 100cm)
    } else {
      propulsorDesvio.write(0);
      digitalWrite(LED_ALERTA, LOW);

      lcd.setCursor(0, 0);
      lcd.print("ORBITA SEGURA   ");
      lcd.setCursor(0, 1);
      lcd.print("DIST: ");
      lcd.print(distanciaAtual);
      lcd.print("cm   ");
    }
  }
}

// Leitura do sensor HC-SR04
float lerUltrassonico() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30ms

  if (duracao == 0) {
    return -1.0; 
  }

  return duracao * 0.0343 / 2.0; 
}
