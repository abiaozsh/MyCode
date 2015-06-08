#include <IRremote.h>

int RECV_PIN = A3;//定义红外接收器的引脚为11

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外接收器
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  int readdata=0;
  while(readdata==0)
  {
    Serial.println("input");
    while (Serial.available() <= 0);
    readdata = Serial.parseInt(); 
  }
  Serial.println(readdata);
  if(readdata==1)
  {
    irsend.sendNEC(0x7510F4BD, 32);
  }
  if(readdata==2)
  {
    irsend.sendNEC(0x3D983521, 32);
  }
}
void loopa() {
  if (irrecv.decode(&results)) {
    Serial.print(results.decode_type);
    Serial.print("\t");
    Serial.print(results.bits);
    Serial.print("\t");
    Serial.print(results.rawlen);
    Serial.print("\t");
    Serial.print(results.value, HEX);//以16进制换行输出接收代码
    Serial.println();//为了便于观看输出结果增加一个空行
    irrecv.resume(); // 接收下一个值
  }
    Serial.println("null");
}

