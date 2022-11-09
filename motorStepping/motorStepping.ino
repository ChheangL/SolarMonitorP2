#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 200

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(13,INPUT);
    digitalWrite(dirPin, HIGH);

}
bool previous = true;
bool reading = true;
unsigned t = 400;
void loop() {
  reading = digitalRead(13);
  if(digitalRead(13)&&!previous){
    t = t + 50;
    if (t>1000){
      t = 400;
    }
  }
  previous = reading;
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(t);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(t);
}
