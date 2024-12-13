#define RED_LED 16    
#define GREEN_LED 17  

void setup() {
  pinMode(RED_LED, OUTPUT);   
  pinMode(GREEN_LED, OUTPUT); 
}

void loop() {
  digitalWrite(RED_LED, HIGH);  
  digitalWrite(GREEN_LED, LOW); 
  delay(1000);                  

  digitalWrite(RED_LED, LOW); 
  digitalWrite(GREEN_LED, HIGH); 
  delay(1000);                 
}
