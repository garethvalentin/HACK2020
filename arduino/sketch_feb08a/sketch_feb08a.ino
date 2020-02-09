
int D2 = 2;
int D3 = 3;
int D4 = 4;

int D5 = 0;
int D6 = 0;
int D7 = 0;
int D8 = 8;

int AN0 = 0;                 // slider variable connecetd to analog pin 0
int AN1 = 1;                 // slider variable connecetd to analog pin 1
int x_value = 0;                  // variable to read the value from the analog pin 0
int y_value = 0;                  // variable to read the value from the analog pin 1
int x_init = 0;                 
int y_init = 0;                

int length = 15;         /* the number of notes */
char notes[] = "ccggaagffeeddc ";
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;


void setup() {

  Serial.begin(9600);
  
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);

  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
  x_init = analogRead(AN0);  
  delay(50);            
  y_init = analogRead(AN1);  
}

int treatValue(int data) {
  return (data * 9 / 1024) + 48;
}


void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(D8, HIGH);
        delayMicroseconds(tone);
        digitalWrite(D8, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}


void loop() {


    for(int i = 0; i < length; i++) {
        if(notes[i] == ' ') {
            delay(beats[i] * tempo);
        } else {
            playNote(notes[i], beats[i] * tempo);
        }
        delay(tempo / 2);    /* delay between notes */
    }


    
  int in_D2 = digitalRead(D2);
  int in_D3 = digitalRead(D3);
  int in_D4 = digitalRead(D4);
  
  Serial.print("ddddd  ");
  Serial.println(in_D2);
  Serial.println(in_D3);
  Serial.println(in_D4);


  //digitalWrite(8, HIGH);
  //delay(1000);
  //digitalWrite(8, LOW);
  //delay(1000);


  x_value = analogRead(AN0);  
  delay(50);            
  y_value = analogRead(AN1);  

  //Serial.print(x_value - x_init);
  //Serial.print("  mmm  ");
  //Serial.println(y_value - y_init);

  //delay(500);  
}
