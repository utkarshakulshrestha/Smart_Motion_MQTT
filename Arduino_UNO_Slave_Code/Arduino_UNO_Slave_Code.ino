#include<Wire.h>
#include<Servo.h>

Servo servo;


int state = LOW;
String k = "";
void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);
  servo.attach(3);//servo to pin 3
  Serial.begin(115200);
  pinMode(7,OUTPUT);
  
  
}

void loop() {
  delay(100);
  // put your main code here, to run repeatedly:

}

void recieveEvent(int howMany){
  while(0<Wire.available()){
    char c = Wire.read();
    //Serial.print(c);
    k+=c;
  }
  
    //Serial.println();
    Serial.print(k);
    Serial.println();
    
    if(k.equals("1")){
  
      high();
    }
    else if(k.equals("0")){
      low();
      
    }
    else if(k.equals("11")){
      Serial.println("Open");
      dOpen();
    }
    else if(k.equals("00")){
      Serial.println("Close");
      dClose();
    }
  k="";
  
}
void high(){
  Serial.print("ON");
      Serial.println();
      if(state == HIGH){
        state = LOW;
      }
      
  digitalWrite(7,state);
}
void low(){
  Serial.println("OFF");
  if(state == LOW){
        state = HIGH;
      }
      
  digitalWrite(7,state);
}
void dOpen(){
  servo.write(0);
}
void dClose(){
  servo.write(180);
}
void requestEvent(){
  Wire.write("Hemlo!!");
}
