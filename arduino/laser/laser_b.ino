/*
36 laser
B  A
38 37
40 39
42 41
*/
#define A_A 37
#define A_B 39
#define A_C 41
#define B_A 38
#define B_B 40
#define B_C 42
#define LASER 36

void LASER_ON()
{
  digitalWrite(LASER, HIGH);
}
void LASER_OFF()
{
  digitalWrite(LASER, LOW);
}

void A_ON_FW()
{
  digitalWrite(A_A, HIGH);
  digitalWrite(A_B, HIGH);
  digitalWrite(A_C, LOW);
}
void A_ON_BK()
{
  digitalWrite(A_A, LOW);
  digitalWrite(A_B, HIGH);
  digitalWrite(A_C, HIGH);
}
void A_STOP()
{
  digitalWrite(A_A, LOW);
  digitalWrite(A_B, HIGH);
  digitalWrite(A_C, LOW);
}
void A_OFF()
{
  digitalWrite(A_A, LOW);
  digitalWrite(A_B, LOW);
  digitalWrite(A_C, LOW);
}

void B_ON_FW()
{
  digitalWrite(B_A, HIGH);
  digitalWrite(B_B, HIGH);
  digitalWrite(B_C, LOW);
}
void B_ON_BK()
{
  digitalWrite(B_A, LOW);
  digitalWrite(B_B, HIGH);
  digitalWrite(B_C, HIGH);
}
void B_STOP()
{
  digitalWrite(B_A, LOW);
  digitalWrite(B_B, HIGH);
  digitalWrite(B_C, LOW);
}
void B_OFF()
{
  digitalWrite(B_A, LOW);
  digitalWrite(B_B, LOW);
  digitalWrite(B_C, LOW);
}
void FUN_WAIT(int time)
{
  for(int i=0;i<time;i++)
  {
       asm volatile("nop");
  }
}
