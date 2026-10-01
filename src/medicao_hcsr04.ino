const int TRIG = 27;
const int ECHO = 26;

const float DISTANCIA_MAXIMA = 100.0;

void setup() {
    Serial.begin(115200);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop() {

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long tempo = pulseIn(ECHO, HIGH);

    float distancia = tempo * 0.0343 / 2;

    if (distancia > DISTANCIA_MAXIMA) {
        distancia = 0;
    }

    Serial.print("Distância: ");
    Serial.print(distancia);
    Serial.println(" cm");

    delay(500);
}
