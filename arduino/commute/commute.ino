int led = 7;
void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  
  
  }

void loop(){
  if(Serial.available()){
    int data = Serial.read();
    if(data == 'A')
      digitalWrite(led, HIGH);
    else
      digitalWrite(led, LOW);
    
    }
  
  
  }
