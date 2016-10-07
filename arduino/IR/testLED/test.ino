
void setup() {

  pinMode(2, OUTPUT);//0
  digitalWrite(2, HIGH);
  pinMode(3, OUTPUT);//1
  digitalWrite(3, HIGH);
  pinMode(4, OUTPUT);//2
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT);//3
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);//4
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);//5
  digitalWrite(7, HIGH);
  pinMode(8, OUTPUT);//6
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT);//7
  digitalWrite(9, HIGH);
  pinMode(10, OUTPUT);//8
  digitalWrite(10, HIGH);
  pinMode(11, OUTPUT);//9
  digitalWrite(11, HIGH);
  
  pinMode(A0, OUTPUT);//
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT);//
  digitalWrite(A1, LOW);
  pinMode(A2, OUTPUT);//
  digitalWrite(A2, LOW);
  
}

void loop() {
  for(int i=2;i<=11;i++){
  digitalWrite(A0, HIGH);
  digitalWrite(i, LOW);
  delay(1);
  digitalWrite(A0, LOW);
  digitalWrite(i, HIGH);
  delay(100);
  }

  for(int i=2;i<=11;i++){
  digitalWrite(A1, HIGH);
  digitalWrite(i, LOW);
  delay(1);
  digitalWrite(A1, LOW);
  digitalWrite(i, HIGH);
  delay(100);
  }

  for(int i=2;i<=11;i++){
  digitalWrite(A2, HIGH);
  digitalWrite(i, LOW);
  delay(1);
  digitalWrite(A2, LOW);
  digitalWrite(i, HIGH);
  delay(100);
  }

}
