const int NUM_POSTES = 3;

const int PINOS_LDR[NUM_POSTES]   = {34, 35, 32};
const int PINOS_LED[NUM_POSTES]   = {16, 17, 18};
const int PINOS_SLIDE[NUM_POSTES] = {33, 26, 14};

const int PINO_BUZZER = 19;

const int LIMIAR_LUZ = 1000;   // leitura > LIMIAR_LUZ = escuro (proxy de "noite" até o NTP)

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_POSTES; i++) {
    pinMode(PINOS_LED[i], OUTPUT);
    pinMode(PINOS_SLIDE[i], INPUT_PULLUP);
    // LDR: sem pinMode, o analogRead configura sozinho
  }

  pinMode(PINO_BUZZER, OUTPUT);
  noTone(PINO_BUZZER);

  Serial.println("--- Postes: LDR + chave de falha + buzzer ---");
}

void loop() {
  bool algumaFalha = false;   // acumulador: vira true se QUALQUER poste falhar

  for (int i = 0; i < NUM_POSTES; i++) {
    int leitura = analogRead(PINOS_LDR[i]);
    bool escuro = (leitura > LIMIAR_LUZ);            // true = "noite" (por enquanto, via LDR)

    bool falhaForcada = (digitalRead(PINOS_SLIDE[i]) == LOW);

    bool ledLigado;
    const char* modo;

    if (falhaForcada) {
      ledLigado = !escuro;   // inverte o normal: de dia forca ON, de noite forca OFF
      modo = escuro ? "FALHA: apagado a noite" : "FALHA: aceso de dia";
      algumaFalha = true;    // acumula no array de falhas
    } else {
      ledLigado = escuro;    // AUTO: à noite acende, de dia apaga
      modo = "AUTO";
    }

    digitalWrite(PINOS_LED[i], ledLigado ? HIGH : LOW);

    Serial.print("Poste ");
    Serial.print(i + 1);
    Serial.print(" | LDR GPIO");
    Serial.print(PINOS_LDR[i]);
    Serial.print(" = ");
    Serial.print(leitura);
    Serial.print(" | ");
    Serial.print(modo);
    Serial.println(ledLigado ? " | LED ON" : " | LED OFF");
  }

  if (algumaFalha) {
    tone(PINO_BUZZER, 1000);
    Serial.println("ERRO! Ao menos um poste em falha.");
  } else {
    noTone(PINO_BUZZER);
    Serial.println("Sistema normal.");
  }

  Serial.println("----------");
  delay(500);
}