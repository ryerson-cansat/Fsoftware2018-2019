float sensorValue = 0;
int oldTime = 0;
int rpm;
int time;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 pinMode(A1,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(A1);
  time = millis() - oldTime;
  oldTime = millis();
  Serial.println(sensorValue);
  delay(100);
}
