#include <Servo.h>
#include <DHT.h>

Servo myservo; //create servo object to control a servo

#define pirPin 12
#define greenLED 3
#define redLED 4
#define servoPin 10
#define dhtPin 8
#define buzzerPin 7

#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);


void setup() {
  Serial.begin(9600);

  pinMode(pirPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myservo.attach(servoPin);

  digitalWrite(greenLED, HIGH);      //turn on greenLED
  myservo.write(90);                 //set servo position
  dht.begin();                       // initialise the DHT sensor
}

void loop() {
  //read the PIR sensor state
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {            // detect motion
    delay(5000);                     //wait for 5 second
    if (digitalRead(pirPin) == HIGH) {
      Serial.println("detect departure");
      triggerReminder();
    }
  }
}

//trigger the reminder process
void triggerReminder() {
  digitalWrite(greenLED, LOW);            //turn off greenLED
  
  for (int i = 0; i < 2; i++) {            //blink redLED twice
    digitalWrite(redLED, HIGH);
    delay(200);
    digitalWrite(redLED, LOW);
    delay(200);
  }

  for (int pos = 0; pos <= 150; pos += 5) {    //rotate servo
    myservo.write(pos);
    delay(20); 
  }
  delay(300);
  for (int pos = 150; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(20);
  }
  
  
  rainyReminder();          //check rainy

  digitalWrite(greenLED, HIGH);         //turn greenLED back on
}

//check rainy
void rainyReminder() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: "); //print humidity
  Serial.print(humidity);
  Serial.println("%");

  if (humidity > 55) {         // high humidity
    Serial.println("Rainy Reminder: Humidity is high");
    playRainyWarning();
  }
}

//play rainy warning sound
void playRainyWarning() {
  tone(buzzerPin, 500, 200);
  delay(250);
  tone(buzzerPin, 600, 200);
  delay(250);
  tone(buzzerPin, 700, 200);
  delay(500);
}
