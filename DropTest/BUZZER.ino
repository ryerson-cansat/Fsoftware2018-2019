void setupBuzzer()
{
  tone(buzzerPin,3000, 150); // Initial Buzzer
  noTone(5);
}

void startBuzzer()
{
  tone(buzzerPin,3000); //Buzzing starts
}
