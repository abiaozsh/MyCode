/*
const C_LASER_ON =    51;
 const C_LASER_OFF =   52;
 const C_A_ON_FW =     53;
 const C_A_ON_BK =     54;
 const C_A_STOP =      55;
 const C_A_OFF =       56;
 const C_B_ON_FW =     57;
 const C_B_ON_BK =     58;
 const C_B_STOP =      59;
 const C_B_OFF =       60;
 const C_NOP =         61;
 const C_REP =         62;
 */

struct data{
  byte cmd;
  byte wait1;
  byte wait2;
};

data datas[1000];
int datalen=0;

void setup() {                
  pinMode(13, OUTPUT);     
  pinMode(36, OUTPUT);     
  pinMode(37, OUTPUT);     
  pinMode(38, OUTPUT);     
  pinMode(39, OUTPUT);     
  pinMode(40, OUTPUT);     
  pinMode(41, OUTPUT);     
  pinMode(42, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {

  if(Serial.available() > 0)
  {
    int idx=0;
    while(1)
    {
      byte data = Serial.read();
      if(data>50&&data<61)
      {      
        datas[idx].cmd = data;
        datas[idx].wait1 = Serial.read();
        datas[idx].wait2 = Serial.read();
        idx++;
      }
      else
      {
        if(idx>0)
        {
          datalen = idx;
        }
        break;
      }
    }
  }


  for(int i=0;i<datalen;i++)
  {
    switch(datas[i].cmd)
    {
    case 51:
      LASER_ON();
      break;
    case 52:
      LASER_OFF();
      break;
    case 53:
      A_ON_FW();
      break;
    case 54:
      A_ON_BK();
      break;
    case 55:
      A_STOP();
      break;
    case 56:
      A_OFF();
      break;
    case 57:
      B_ON_FW();
      break;
    case 58:
      B_ON_BK();
      break;
    case 59:
      B_STOP();
      break;
    case 60:
      B_OFF();
      break;
    }
    if(datas[i].wait1>0)
    {
      int wait1 = datas[i].wait1;
      wait1= 1<<wait1;
      delay(wait1);
    }
    if(datas[i].wait2>0)
    {
      int wait2 = datas[i].wait2;
      wait2= 1<<wait2;
      FUN_WAIT(wait2);   
    }

  }



  LASER_OFF();
  A_OFF();
  B_OFF();

  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  delay(250);
}






