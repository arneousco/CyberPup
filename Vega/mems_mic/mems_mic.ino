void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read sound data from the microphone module
  int soundData = analogRead(A0); // Assuming the microphone is connected to analog pin A0

  // Send sound data over UART to PC
  Serial.println(soundData);

  delay(10); // Adjust the delay as per your requirements
}
