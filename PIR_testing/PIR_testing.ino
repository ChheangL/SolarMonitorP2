void setup() {
pinMode(15,INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(digitalRead(15));
delay(500);
}
