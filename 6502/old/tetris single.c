char const data1[256] = {
0x07,0x00,0x03,0x00,0x03,0x00,0x06,0x00,0x25,0x42,0x00,0x00,0x00,0x00,0x00,0x00, //0
0x07,0x00,0x03,0x00,0x03,0x00,0x06,0x00,0x25,0x42,0x00,0x00,0x00,0x00,0x00,0x00, //1 1
0x9B,0x00,0xA8,0x00,0x00,0x00,0x00,0x00,0x9B,0x00,0xA8,0x00,0x00,0x00,0x00,0x00, //2
0x9B,0x00,0xA8,0x00,0x00,0x00,0x00,0x00,0x9B,0x00,0xA8,0x00,0x00,0x00,0x00,0x00, //3 2
0x70,0x00,0xAB,0x00,0x06,0x00,0x00,0x00,0x95,0x00,0x80,0x04,0x00,0x00,0x00,0x00, //4
0x70,0x00,0xAB,0x00,0x06,0x00,0x00,0x00,0x95,0x00,0x80,0x04,0x00,0x00,0x00,0x00, //5 3
0x07,0x00,0x98,0x00,0x60,0x00,0x00,0x00,0xB0,0x04,0xA5,0x00,0x00,0x00,0x00,0x00, //6
0x07,0x00,0x98,0x00,0x60,0x00,0x00,0x00,0xB0,0x04,0xA5,0x00,0x00,0x00,0x00,0x00, //7 4
0x07,0x00,0x03,0x00,0x48,0x00,0x00,0x00,0x00,0x07,0x25,0x0A,0x00,0x00,0x00,0x00, //8
0x95,0x00,0x30,0x00,0x60,0x00,0x00,0x00,0x2B,0x04,0x06,0x00,0x00,0x00,0x00,0x00, //9 5
0x4B,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x07,0x00,0x28,0x04,0x00,0x00,0x00,0x00, //A
0x70,0x00,0x30,0x00,0xA5,0x00,0x00,0x00,0x25,0x09,0x00,0x06,0x00,0x00,0x00,0x00, //B 6
0x70,0x00,0xF5,0x04,0x00,0x00,0x00,0x00,0x70,0x00,0xC5,0x00,0x60,0x00,0x00,0x00, //C
0xD5,0x04,0x60,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x4E,0x00,0x06,0x00,0x00,0x00, //D 7
0x00,0x01,0x05,0x03,0x01,0x05,0x06,0x07,0x08,0x07,0x06,0x0B,0x03,0x0B,0x0E,0x08, //E chg
0x00,0x01,0x04,0x03,0x04,0x01,0x06,0x07,0x06,0x09,0x0A,0x07,0x0C,0x09,0x03,0x0A  //F chg
};
char const data2[64] = {
0x11,0x11,0x04,0x00,0x11,0x11,0x04,0x00,0x22,0x00,0x22,0x00,0x22,0x00,0x22,0x00, //0
0x22,0x01,0x32,0x00,0x22,0x01,0x32,0x00,0x21,0x02,0x23,0x00,0x21,0x02,0x23,0x00, //1
0x11,0x02,0x33,0x00,0x22,0x02,0x13,0x00,0x12,0x01,0x31,0x00,0x22,0x02,0x33,0x00, //2
0x32,0x00,0x22,0x02,0x23,0x00,0x21,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  //3
};
char const data3[64] = {
0x04,0x01,0x04,0x01,0x02,0x02,0x02,0x02,0x03,0x02,0x03,0x02,0x03,0x02,0x03,0x02, //4
0x03,0x02,0x03,0x02,0x03,0x02,0x03,0x02,0x02,0x03,0x02,0x03,0x00,0x00,0x00,0x00, //5
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //6 free
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  //7 free
};
char const datarnd[128] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//<<rnd
0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,//<<rnd
0x11,0x11,0x11,0x11,0x12,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,//<<rnd
0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,//<<rnd
0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x44,0x44,0x44,0x44,0x44,0x44,0x44,//<<rnd
0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x45,0x55,0x55,0x55,0x55,//<<rnd
0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x56,0x66,0x66,//<<rnd
0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66 //<<rnd
};
char const img1[256] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//0
0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,0x01,0x03,0x3F,0x3F,0x3F,0x3F,0x7F,0xFF,//1
0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,//2
0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//3
0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x7F,0xFF,//4
0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x01,0x03,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,//5
0xFF,0xFF,0x03,0x03,0x03,0x03,0xFF,0xFF,0x01,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//6
0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x7F,0xFF,//7
0xFF,0xFF,0x03,0x03,0x03,0x03,0xC3,0xC3,0x01,0x03,0xFF,0xFF,0xFF,0xFF,0xBF,0x3F,//8
0xC3,0xC3,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0x3E,0x3C,0x3F,0x3F,0x3F,0x3F,0x7F,0xFF,//9
0xC3,0xC3,0x03,0x03,0x03,0x03,0xFF,0xFF,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//10
0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC3,0xC3,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,//11
0xC3,0xC3,0x00,0x00,0x00,0x00,0xFF,0xFF,0x3E,0x3C,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//12
0xC3,0xC3,0xC0,0xC0,0xC0,0xC0,0xC3,0xC3,0x3E,0x3C,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,//13
0xFF,0xFF,0x00,0x00,0x00,0x00,0xC3,0xC3,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xBF,0x3F,//14
0xC3,0xC3,0x03,0x03,0x03,0x03,0xC3,0xC3,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0xBF,0x3F,//15
};
char const img2[256] = {
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//0 black
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x38,0x24,0x24,0x38,0x20,0x20,0x20,0x20,//1 P
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,//2 U
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x40,0x40,0x38,0x04,0x04,0x04,0x78,//3 S
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44,//4 H
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x10,0x28,0x44,0x7C,0x44,0x44,0x44,0x44,//5 A
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x44,0x48,0x50,0x60,0x50,0x48,0x44,0x44,//6 K
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C,//7 E
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,//8 Y
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//A
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//B
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//C
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//D
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//E
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 //F
};

//page0 0x0000 ~0x00FF
#define rand0            0x40
#define rand1            0x41
#define rand2            0x42
#define rand3            0x43
#define lastkey          0x44
#define key1             0x45
#define key2             0x46
#define getBlock_i       0x4A
#define getBlock_j       0x4B
#define getBlock_idx     0x4C
#define getBlock_ret     0x4D
#define setBoard_x       0x4E
#define setBoard_y       0x4F
#define setBoard_val     0x50
#define setBoard_base    0x51
#define getBase_x        0x52
#define getBase_y        0x53
#define getBase_hi       0x54
#define getBase_lo       0x55
#define getBase_tmp      0x56
#define movelr_n         0x57
#define getRnd7_ret      0x58
#define DrawShape_i      0x59
#define DrawShape_j      0x5A
#define Clear_hi         0x5B
#define Clear_lo         0x5C
#define Clear_i          0x5D
#define Touch_i          0x5E
#define Touch_ret        0x5F
#define AnyTouch_i       0x60
#define AnyTouch_j       0x61
#define AnyTouch_ret     0x62
#define DrawLine_i       0x63
#define DrawLine_y       0x64
#define TouchDo_i        0x65
#define TouchDo_j        0x66
#define TouchDo_k        0x67
#define TouchDo_line     0x68
#define rotate_n         0x69
#define rotate_tempPosX  0x6A
#define main_cnt         0x6B
#define PosX             0x70
#define PosY             0x71
#define NextShapeNo      0x72
#define NowShapeNo       0x73
#define NowDirectionNo   0x74






//page1: 0x0100 ~0x01FF stack

//page2: 0x0200 ~0x02FF
#define Board            0x0200//size:100(0x064)
#define DrawBuff         0x0264//size:16(0x010)
//char Board[100];
//char DrawBuff[16];//sp*4 //10*data+address


//* %b - Numerical 8-bit value
//* %w - Numerical 16-bit value
//* %l - Numerical 32-bit value
//* %v - Assembler name of a (global) variable or function
//* %o - Stack offset of a (local) variable
//* %g - Assembler name of a C label
//* %s - The argument is converted to a string
//* %% - The % sign itself

void rnd(){
    asm("clc");
    asm("lda %b",rand0);//SEED *= $01010101
    asm("adc %b",rand1);
    asm("sta %b",rand1);
    asm("adc %b",rand2);
    asm("sta %b",rand2);
    asm("adc %b",rand3);
    asm("sta %b",rand3);
    asm("clc");
    asm("lda %b",rand0);//SEED += $31415927
    asm("adc #$27");
    asm("sta %b",rand0);
    asm("lda %b",rand1);
    asm("adc #$59");
    asm("sta %b",rand1);
    asm("pha");
    asm("lda %b",rand2);
    asm("adc #$41");
    asm("sta %b",rand2);
    asm("and #$7F");//Suppress sign bit (make it positive)
    asm("tax");
    asm("lda %b",rand3);
    asm("adc #$31");
    asm("sta %b",rand3);
    asm("pla");//return bit 8-22 in (X,A)
}
void waitvblank(){
    asm("lda $2002");
    asm("bmi %g",else1);
    lbl1:
    asm("lda $2002");
    asm("bmi %g",lbl1);
    lbl2:
    asm("lda $2002");
    asm("bpl %g",lbl2);
    asm("rts");
    else1:
    asm("lda $2002");
    asm("bpl %g",else1);
}
void nmi(){
}
void getBlock(){
    //getBlock_i <<= 1;
    asm("lda %b",getBlock_i);
    asm("asl");
    //getBlock_idx += getBlock_i;
    asm("clc");
    asm("adc %b",getBlock_idx);
    asm("sta %b",getBlock_idx);

    //getBlock_j >>= 1;
    asm("lda %b",getBlock_j);
    asm("lsr");
    //getBlock_idx += getBlock_j;
    asm("clc");
    asm("adc %b",getBlock_idx);
    asm("sta %b",getBlock_idx);
    
    //getBlock_temp = getBlock_j & 1;
    asm("lda %b",getBlock_j);
    asm("and #$01");
    
    //if(getBlock_temp)
    asm("beq %g",else1);
    {
        //getBlock_ret = (data1[getBlock_idx]>>4)&0x0F;
        asm("ldx %b",getBlock_idx);
        asm("lda %v,X",data1);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",getBlock_ret);
        asm("rts");
    }
    else1:
    {
        //getBlock_ret = data1[getBlock_idx]&0x0F;
        asm("ldx %b",getBlock_idx);
        asm("lda %v,X",data1);
        asm("and #$0F");
        asm("sta %b",getBlock_ret);
    }
}
void getNowBlock(){
    //char idx = (NowShapeNo<<5)+(NowDirectionNo<<3)+(getBlock_i<<1)+(getBlock_j>>1);
    //getBlock_idx = NowShapeNo;
    asm("lda %b",NowShapeNo);
    //getBlock_idx <<= 5;
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("sta %b",getBlock_idx);
        
    //getBlock_temp = NowDirectionNo;
    asm("lda %b",NowDirectionNo);
    //getBlock_temp <<= 3;
    asm("asl");
    asm("asl");
    asm("asl");
    //getBlock_idx += getBlock_temp;
    asm("clc");
    asm("adc %b",getBlock_idx);
    asm("sta %b",getBlock_idx);

    getBlock();
}
void getNextBlock(){
    //char idx = (NextShapeNo<<5)+(i<<1)+(j>>1);
    //getBlock_idx = NextShapeNo;
    asm("lda %b",NextShapeNo);
    //getBlock_idx <<= 5;
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("sta %b",getBlock_idx);
    
    getBlock();
}
void getNowBottom(){
    //char idx = (NowShapeNo<<3)+(NowDirectionNo<<1)+(j>>1);
    //getBlock_idx = NowShapeNo;
    asm("lda %b",NowShapeNo);
    //getBlock_idx <<= 3;
    asm("asl");
    asm("asl");
    asm("asl");
    asm("sta %b",getBlock_idx);
    
    //getBlock_temp = NowDirectionNo;
    asm("lda %b",NowDirectionNo);
    //getBlock_temp <<= 1;
    asm("asl");
    //getBlock_idx += getBlock_temp;
    asm("clc");
    asm("adc %b",getBlock_idx);
    asm("sta %b",getBlock_idx);

    //getBlock_j >>= 1;
    asm("lda %b",getBlock_j);
    asm("lsr");
    //getBlock_idx += getBlock_j;
    asm("clc");
    asm("adc %b",getBlock_idx);
    asm("sta %b",getBlock_idx);

    //getBlock_temp = getBlock_j & 1;
    asm("lda %b",getBlock_j);
    asm("and #$01");


    //if(getBlock_temp)
    asm("beq %g",else1);
    {
        //getBlock_ret = (data2[getBlock_idx]>>4)&0x0F;
        asm("ldx %b",getBlock_idx);
        asm("lda %v,X",data2);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",getBlock_ret);
        asm("rts");
    }
    else1:
    {
        //getBlock_ret = data2[getBlock_idx]&0x0F;
        asm("ldx %b",getBlock_idx);
        asm("lda %v,X",data2);
        asm("and #$0F");
        asm("sta %b",getBlock_ret);
    }
}
void getNowLeft(){
    //char idx = (NowShapeNo<<2)+NowDirectionNo;
    //getBlock_idx = NowShapeNo;
    asm("lda %b",NowShapeNo);
    //getBlock_idx <<= 2;
    asm("asl");
    asm("asl");
    //getBlock_idx += NowDirectionNo;
    asm("clc");
    asm("adc %b",NowDirectionNo);
    //getBlock_ret = data3[getBlock_idx];
    asm("tax");
    asm("lda %v,X",data3);
    asm("sta %b",getBlock_ret);
}
void calcBoardAddress(){
    //x:0~9 y:0~19
    //base =y*5
    //setBoard_base=setBoard_y;
    asm("lda %b",setBoard_y);
    
    //setBoard_base+=(setBoard_y<<2);
    asm("asl");
    asm("asl");
    asm("clc");
    asm("adc %b",setBoard_y);
    asm("sta %b",setBoard_base);
    
    //setBoard_x >>= 1;
    asm("lda %b",setBoard_x);
    asm("lsr");
    //setBoard_base+=setBoard_x;
    asm("clc");
    asm("adc %b",setBoard_base);
    asm("sta %b",setBoard_base);
    
    //setBoard_y = setBoard_x & 1;
    asm("lda %b",setBoard_x);
    asm("and #$01");
}
void setBoard(){
    calcBoardAddress();

    //if(setBoard_y)
    asm("beq %g",else1);
    {
        //setBoard_val<<=4;
        asm("lda %b",setBoard_val);
        asm("asl");
        asm("asl");
        asm("asl");
        asm("asl");
        asm("sta %b",setBoard_val);
        //Board[setBoard_base] = (Board[setBoard_base] & 0x0F) | setBoard_val;//high 4
        asm("ldx %b",setBoard_base);
        asm("lda %w,X",Board);
        asm("and #$0F");
        asm("ora %b",setBoard_val);
        asm("sta %w,X",Board);
        asm("rts");
    }    
    else1:
    {
        //Board[setBoard_base] = (Board[setBoard_base] & 0xF0) | setBoard_val;//low 4
        asm("ldx %b",setBoard_base);
        asm("lda %w,X",Board);
        asm("and #$F0");
        asm("ora %b",setBoard_val);
        asm("sta %w,X",Board);
    }
}
void getBoard(){
    calcBoardAddress();
    
    //if(setBoard_y)
    asm("beq %g",else1);
    {
        //return Board[base] >> 4;//high 4
        asm("ldx %b",setBoard_base);
        asm("lda %w,X",Board);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",setBoard_val);
        asm("rts");
    }
    else1:
    {
        //return Board[base] & 0x0F;//low 4
        asm("ldx %b",setBoard_base);
        asm("lda %w,X",Board);
        asm("and #$0F");
        asm("sta %b",setBoard_val);
    }
}
void getRnd7(){
    rnd();
    //v = datarnd[(rand2&0x7F)];
    asm("lda %b",rand2);
    asm("and #$7F");
    asm("tax");
    asm("lda %v,X",datarnd);
    asm("sta %b",getRnd7_ret);
    
    //getRnd7_temp = rand2&0x80
    asm("lda %b",rand2);
    asm("and #$80");
    
    //if(getRnd7_temp)
    asm("beq %g",else1);
    {
        //return (v>>4);
        asm("lda %b",getRnd7_ret);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",getRnd7_ret);
    }
    else1:
    {
        //return v&0x0F;
        asm("lda %b",getRnd7_ret);
        asm("and #$0F");
        asm("sta %b",getRnd7_ret);
    }
    //return Math.floor(Math.random()*7);
}
void DrawBuffToPPU(){
    //for(i=0;i<16;i++)
    //DrawShape_i=0;
    asm("ldx #$00");
    fori2:
        //*(char*)(0x2004)=DrawBuff[DrawShape_i];
        asm("lda %w,X",DrawBuff);
        asm("sta $2004");
    asm("inx");
    asm("cpx #$10");
    asm("bne %g",fori2);
}
void DrawNextShape(){
    //DrawShape_idx=0;
    asm("ldy #$00");
    
    //DrawShape_i=4;
    asm("lda #$04");
    asm("sta %b",DrawShape_i);
    //for(i=4;i>0;i--)
    fori:
        //DrawShape_j=4;
        asm("lda #$04");
        asm("sta %b",DrawShape_j);
        //for(j=4;j>0;j--)
        forj:
            //getBlock_i = DrawShape_i-1;
            asm("ldx %b",DrawShape_i);
            asm("dex");
            asm("stx %b",getBlock_i);
            
            //getBlock_j = DrawShape_j-1;
            asm("ldx %b",DrawShape_j);
            asm("dex");
            asm("stx %b",getBlock_j);
            
            getNextBlock();
            //if(getBlock_ret)
            asm("lda %b",getBlock_ret);
            asm("beq %g",else1);
            {
                ////DrawBuff[DrawShape_idx++]=((4+DrawShape_j)<<3)-1;
                ////DrawShape_temp=31+(DrawShape_j<<3);
                //DrawShape_temp = DrawShape_j;
                asm("lda %b",DrawShape_j);
                //DrawShape_temp <<= 3;
                asm("asl");
                asm("asl");
                asm("asl");
                //DrawShape_temp += 31;
                asm("clc");
                asm("adc #$1F");

                //DrawBuff[DrawShape_idx++]=DrawShape_temp;
                asm("sta %w,Y",DrawBuff);
                asm("iny");
                
                //DrawBuff[DrawShape_idx++]=getBlock_ret;
                asm("lda %b",getBlock_ret);
                asm("sta %w,Y",DrawBuff);
                asm("iny");

                //DrawBuff[DrawShape_idx++]=0;
                asm("lda #$00");
                asm("sta %w,Y",DrawBuff);
                asm("iny");
                
                //DrawShape_temp=(13+DrawShape_i)<<3
                asm("lda %b",DrawShape_i);
                asm("clc");
                asm("adc #$0D");
                asm("asl");
                asm("asl");
                asm("asl");

                //DrawBuff[DrawShape_idx++]=DrawShape_temp;
                asm("sta %w,Y",DrawBuff);
                asm("iny");
            }else1:
        asm("dec %b",DrawShape_j);
        asm("bne %g",forj);
    asm("dec %b",DrawShape_i);
    asm("bne %g",fori);

    waitvblank();
    //*(char*)(0x2003)=16; //i<<2
    asm("lda #$10");
    asm("sta $2003");
    DrawBuffToPPU();
}
void DrawShape(){
    //DrawShape_idx=0;
    asm("ldy #$00");

    //DrawShape_i=4;
    asm("lda #$04");
    asm("sta %b",DrawShape_i);
    //for(i=4;i>0;i--)
    fori:
        //DrawShape_j=4;
        asm("lda #$04");
        asm("sta %b",DrawShape_j);
        //for(j=4;j>0;j--)
        forj:
            //getBlock_i=DrawShape_i-1;
            asm("ldx %b",DrawShape_i);
            asm("dex");
            asm("stx %b",getBlock_i);
            
            //getBlock_j=DrawShape_j-1;
            asm("ldx %b",DrawShape_j);
            asm("dex");
            asm("stx %b",getBlock_j);

            getNowBlock();
            //if(getBlock_ret)
            asm("lda %b",getBlock_ret);
            asm("beq %g",else1);
            {
                //DrawBuff[DrawShape_idx++]=((DrawShape_j-PosY)<<3)+167;

                //DrawShape_temp =((DrawShape_j-PosY)<<3)+167;
                asm("lda %b",DrawShape_j);
                asm("sec");
                asm("sbc %b",PosY);
                asm("asl");
                asm("asl");
                asm("asl");
                asm("clc");
                asm("adc #$A7");
            
                //DrawBuff[DrawShape_idx++]=DrawShape_temp;
                asm("sta %w,Y",DrawBuff);
                asm("iny");
                
                //DrawBuff[DrawShape_idx++]=getBlock_ret;
                asm("lda %b",getBlock_ret);
                asm("sta %w,Y",DrawBuff);
                asm("iny");
                
                //DrawBuff[DrawShape_idx++]=0;
                asm("lda #$00");
                asm("sta %w,Y",DrawBuff);
                asm("iny");
                
                //DrawShape_temp =((DrawShape_i+PosX)<<3)+8;
                asm("lda %b",DrawShape_i);
                asm("clc");
                asm("adc %b",PosX);
                asm("asl");
                asm("asl");
                asm("asl");
                asm("clc");
                asm("adc #$08");

                //DrawBuff[DrawShape_idx++]=DrawShape_temp;
                asm("sta %w,Y",DrawBuff);
                asm("iny");
            }else1:
        asm("dec %b",DrawShape_j);
        asm("bne %g",forj);
    asm("dec %b",DrawShape_i);
    asm("bne %g",fori);

    waitvblank();
    //*(char*)(0x2003)=0; //i<<2
    asm("lda #$00");
    asm("sta $2003");
    DrawBuffToPPU();
}
void ClearLine(){
    asm("lda #$00");
    //for(i=10;i>0;i--)
    asm("ldx #$0A");//10 times
    fori:
        //*(char*)(0x2007)=0;
        asm("sta $2007");
    asm("dex");
    asm("bne %g",fori);

    asm("lda #$00");
    //*(char*)(0x2005)=0;
    asm("sta $2005");
    //*(char*)(0x2005)=0;
    asm("sta $2005");
    waitvblank();
}
void Clear(){
    asm("lda #$00");
    //for(i=0;i<100;i++)
    asm("ldx #$63");//99 times
    fori:
        //Board[i]=0;
        asm("sta %w,X",Board);
    asm("dex");
    asm("bne %g",fori);
    //Board[0]=0;
    asm("sta %w",Board);

    waitvblank();
    
    //Clear_hi=0x20;
    asm("lda #$20");
    asm("sta %b",Clear_hi);
    //Clear_lo=0x62;
    asm("lda #$62");
    asm("sta %b",Clear_lo);
    
    asm("lda #$14");//20 times
    asm("sta %b",Clear_i);
    fori2:
        //*(char*)(0x2006)=Clear_hi;
        asm("lda %b",Clear_hi);
        asm("sta $2006");
        //*(char*)(0x2006)=Clear_lo;
        asm("lda %b",Clear_lo);
        asm("sta $2006");
        
        asm("clc");
        asm("adc #$20");
        asm("sta %b",Clear_lo);
        asm("lda %b",Clear_hi);
        asm("adc #$00");
        asm("sta %b",Clear_hi);

        ClearLine();
    asm("dec %b",Clear_i);
    asm("bne %g",fori2);
}
void NextShape(){
    //NowShapeNo = NextShapeNo;
    asm("lda %b",NextShapeNo);
    asm("sta %b",NowShapeNo);

    //NowDirectionNo = 0;
    asm("lda #$00");
    asm("sta %b",NowDirectionNo);

    //PosY = 19;
    asm("lda #$13");
    asm("sta %b",PosY);
    //PosX = 3;
    asm("lda #$03");
    asm("sta %b",PosX);
    
    getRnd7();
    //NextShapeNo = getRnd7_ret;
    asm("lda %b",getRnd7_ret);
    asm("sta %b",NextShapeNo);
    DrawNextShape();
}
void Touch(){
    //for(Touch_i=4;Touch_i>0;Touch_i--)
    asm("lda #$04");//20 times
    asm("sta %b",Touch_i);
    fori:
        //getBlock_j = Touch_i-1;
        asm("lda %b",Touch_i);
        asm("sta %b",getBlock_j);
        asm("dec %b",getBlock_j);

        getNowBottom();
        //if(getBlock_ret)
        asm("lda %b",getBlock_ret);
        asm("beq %g",else1);
        {
            //if(getBlock_ret-1==PosY)goto ret1;
            asm("ldx %b",getBlock_ret);
            asm("dex");
            asm("cpx %b",PosY);
            asm("beq %g",ret1);
            
            //setBoard_x=PosX+Touch_i-1;
            asm("lda %b",PosX);
            asm("clc");
            asm("adc %b",Touch_i);
            asm("tax");
            asm("dex");
            asm("stx %b",setBoard_x);

            //setBoard_y=PosY-getBlock_ret;
            asm("lda %b",PosY);
            asm("sec");
            asm("sbc %b",getBlock_ret);
            asm("sta %b",setBoard_y);
            
            getBoard();
            //if(setBoard_val)goto ret1;
            asm("lda %b",setBoard_val);
            asm("cmp #$00");
            asm("bne %g",ret1);
        }else1:
    asm("dec %b",Touch_i);
    asm("bne %g",fori);
    
    //return 0;
    asm("lda #$00");
    asm("sta %b",Touch_ret);
    asm("rts");
    ret1:
    //return 1;
    asm("lda #$01");
    asm("sta %b",Touch_ret);
}
void getBase(){
    //int base=0x2020;
    //base+=(19-getBase_y+2)<<5;
    //base+=(getBase_x+2);
    //getBase_hi=base>>8;
    //getBase_lo=base & 0x00FF;
    asm("lda #$15");//21
    asm("sec");
    asm("sbc %b",getBase_y);
    asm("sta %b",getBase_y);
    asm("lsr");
    asm("lsr");
    asm("lsr");
    asm("sta %b",getBase_tmp);
    asm("lda %b",getBase_y);
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("clc");
    asm("adc %b",getBase_x);
    asm("adc #$22");
    asm("sta %b",getBase_lo);
    asm("lda #$20");
    asm("adc %b",getBase_tmp);
    asm("sta %b",getBase_hi);
}
void DrawLine(){
    //getBase_x=0;
    asm("lda #$00");
    asm("sta %b",getBase_x);
    //getBase_y=DrawLine_y;
    asm("lda %b",DrawLine_y);
    asm("sta %b",getBase_y);
    getBase();
    //DrawBuff[0]=getBase_hi;
    asm("ldx #$00");
    asm("lda %b",getBase_hi);
    asm("sta %w,X",DrawBuff);
    //DrawBuff[1]=getBase_lo;
    asm("inx");
    asm("lda %b",getBase_lo);
    asm("sta %w,X",DrawBuff);
    
    asm("lda #$00");
    asm("sta %b",setBoard_x);//setBoard_x is count 0~9

    //for(l=0;l<10;l++)
    asm("lda #$0A");//10 times
    asm("sta %b",DrawLine_i);
    fori1:
        //setBoard_x=l;
        //setBoard_y=DrawLine_y;
        asm("lda %b",DrawLine_y);
        asm("sta %b",setBoard_y);
        getBoard();
        //DrawBuff[2+l]=setBoard_val;
        asm("ldx %b",setBoard_x);
        asm("inx");
        asm("inx");
        asm("lda %b",setBoard_val);
        asm("sta %w,X",DrawBuff);

        asm("inc %b",setBoard_x);//setBoard_x++
    asm("dec %b",DrawLine_i);
    asm("bne %g",fori1);
    
    
    waitvblank();
    //*(char*)(0x2006)=DrawBuff[0];
    asm("ldx #$00");
    asm("lda %w,X",DrawBuff);
    asm("sta $2006");
    asm("inx");
    //*(char*)(0x2006)=DrawBuff[1];
    asm("lda %w,X",DrawBuff);
    asm("sta $2006");
    asm("inx");

    //for(i=2;i<12;i++)
    asm("ldy #$0A");//10 times
    fori2:
        //*(char*)(0x2007)=DrawBuff[i];
        asm("lda %w,X",DrawBuff);
        asm("sta $2007");
        asm("inx");
    asm("dey");
    asm("bne %g",fori2);
    //*(char*)(0x2005) = 0;
    asm("lda #$00");
    asm("sta $2005");
    //*(char*)(0x2005) = 0;
    asm("sta $2005");

}
void TouchDo(){
    //add to board
    {
        //TouchDo_idx=0;
        asm("ldy #$00");
        //for(TouchDo_i=4;TouchDo_i>0;TouchDo_i--)
        asm("lda #$04");
        asm("sta %b",TouchDo_i);
        fori1:
            //for(TouchDo_j=4;TouchDo_j>0;TouchDo_j--)
            asm("lda #$04");
            asm("sta %b",TouchDo_j);
            forj1:
                //getBlock_i=TouchDo_i-1;
                asm("ldx %b",TouchDo_i);
                asm("dex");
                asm("stx %b",getBlock_i);
                //getBlock_j=TouchDo_j-1;
                asm("ldx %b",TouchDo_j);
                asm("dex");
                asm("stx %b",getBlock_j);
                getNowBlock();
                //if(getBlock_ret)
                asm("lda %b",getBlock_ret);
                asm("beq %g",else1);
                {
                    //setBoard_x=PosX+TouchDo_i-1;
                    //getBase_x =PosX+TouchDo_i-1;
                    asm("ldx %b",TouchDo_i);
                    asm("dex");
                    asm("txa");
                    asm("clc");
                    asm("adc %b",PosX);
                    asm("sta %b",setBoard_x);
                    asm("sta %b",getBase_x);
                    //setBoard_y=PosY-TouchDo_j+1;
                    //getBase_y =PosY-TouchDo_j+1;
                    asm("ldx %b",PosY);
                    asm("inx");
                    asm("txa");
                    asm("sec");
                    asm("sbc %b",TouchDo_j);
                    asm("sta %b",setBoard_y);
                    asm("sta %b",getBase_y);
                    //setBoard_val=getBlock_ret;
                    asm("lda %b",getBlock_ret);
                    asm("sta %b",setBoard_val);
                    setBoard();

                    getBase();
                    //DrawBuff[TouchDo_idx++]=getBase_hi;
                    asm("lda %b",getBase_hi);
                    asm("sta %w,Y",DrawBuff);
                    asm("iny");
                    
                    //DrawBuff[TouchDo_idx++]=getBase_lo;
                    asm("lda %b",getBase_lo);
                    asm("sta %w,Y",DrawBuff);
                    asm("iny");

                    //DrawBuff[TouchDo_idx++]=getBlock_ret;
                    asm("lda %b",getBlock_ret);
                    asm("sta %w,Y",DrawBuff);
                    asm("iny");
                }else1:
            asm("dec %b",TouchDo_j);
            asm("bne %g",forj1);
        asm("dec %b",TouchDo_i);
        asm("bne %g",fori1);
    
        //idx=0;
        asm("ldy #$00");
        waitvblank();
        //for(TouchDo_i=4;TouchDo_i>0;TouchDo_i--)
        asm("ldx #$04");
        fori2:
        {
            //*(char*)(0x2006)=DrawBuff[idx++];
            asm("lda %w,Y",DrawBuff);
            asm("sta $2006");
            asm("iny");
            //*(char*)(0x2006)=DrawBuff[idx++];
            asm("lda %w,Y",DrawBuff);
            asm("sta $2006");
            asm("iny");
            //*(char*)(0x2007)=DrawBuff[idx++];
            asm("lda %w,Y",DrawBuff);
            asm("sta $2007");
            asm("iny");
        }
        asm("dex");
        asm("bne %g",fori2);
        //*(char*)(0x2005) = 0;
        asm("lda #$00");
        asm("sta $2005");
        //*(char*)(0x2005) = 0;
        asm("sta $2005");
    }

    //clear line
    {
        //for(TouchDo_j=20;TouchDo_j>0;TouchDo_j--)
        asm("lda #$14");
        asm("sta %b",TouchDo_j);
        forj3:
            //TouchDo_line=1;
            asm("lda #$01");
            asm("sta %b",TouchDo_line);
            //for(TouchDo_i=10;TouchDo_i>0;TouchDo_i--)
            asm("lda #$0A");
            asm("sta %b",TouchDo_i);
            fori3:
                //setBoard_x=TouchDo_i-1;
                asm("ldx %b",TouchDo_i);
                asm("dex");
                asm("stx %b",setBoard_x);
                //setBoard_y=TouchDo_j-1;    
                asm("ldx %b",TouchDo_j);
                asm("dex");
                asm("stx %b",setBoard_y);
                getBoard();
                //if(setBoard_val==0)
                asm("lda %b",setBoard_val);
                asm("bne %g",else2);
                    //TouchDo_line=0;
                    asm("lda #$00");
                    asm("sta %b",TouchDo_line);
                    asm("jmp %g",break1);
                else2:
            asm("dec %b",TouchDo_i);
            asm("bne %g",fori3);
            
            break1:
            
            //if(TouchDo_line)
            asm("ldx %b",TouchDo_line);
            asm("bne %g",if5);
            asm("jmp %g",else5);
            if5:
                //for(TouchDo_k=0;TouchDo_k<10;TouchDo_k++)
                asm("lda #$0A");//10 times
                asm("sta %b",TouchDo_k);
                fork1:
                    //setBoard_x=TouchDo_k-1;
                    asm("ldx %b",TouchDo_k);
                    asm("dex");
                    asm("stx %b",setBoard_x);
                    
                    //if(TouchDo_j>=2)
                    asm("ldx %b",TouchDo_j);
                    asm("cpx #$02");
                    asm("bmi %g",else3);
                        //setBoard_y=TouchDo_j-2;
                        asm("dex");
                        asm("dex");
                        asm("stx %b",setBoard_y);
                        getBoard();//in setBoard_val
                        //setBoard_val=data1[setBoard_val+0xE0];
                        asm("lda %b",setBoard_val);
                        asm("clc");
                        asm("adc #$E0");
                        asm("tax");
                        asm("lda %v,X",data1);
                        asm("sta %b",setBoard_val);
                        setBoard();
                    else3:

                    //setBoard_y=TouchDo_j;
                    asm("ldx %b",TouchDo_j);
                    asm("stx %b",setBoard_y);
                    getBoard();
                    //setBoard_val=data1[setBoard_val+0xF0];
                    asm("lda %b",setBoard_val);
                    asm("clc");
                    asm("adc #$F0");
                    asm("tax");
                    asm("lda %v,X",data1);
                    asm("sta %b",setBoard_val);
                    setBoard();

                    //setBoard_y=TouchDo_j-1;
                    asm("ldx %b",TouchDo_j);
                    asm("dex");
                    asm("stx %b",setBoard_y);
                    //setBoard_val=0;
                    asm("lda #$00");
                    asm("sta %b",setBoard_val);
                    setBoard();
                asm("dec %b",TouchDo_k);
                asm("bne %g",fork1);
                
                //if(TouchDo_j>=2)
                asm("ldx %b",TouchDo_j);
                asm("cpx #$02");
                asm("bmi %g",else4);
                    //DrawLine_y=TouchDo_j-2;
                    asm("ldx %b",TouchDo_j);
                    asm("dex");
                    asm("dex");
                    asm("stx %b",DrawLine_y);
                    DrawLine();
                else4:
                
                //for(TouchDo_k=TouchDo_j-1;TouchDo_k<19;TouchDo_k++)
                asm("ldx %b",TouchDo_j);
                asm("dex");
                asm("stx %b",TouchDo_k);
                fork2:
                    //for(TouchDo_i=10;TouchDo_i>0;TouchDo_i--)
                    asm("lda #$0A");
                    asm("sta %b",TouchDo_i);
                    fori4:
                        //setBoard_x=TouchDo_i-1;
                        asm("ldx %b",TouchDo_i);
                        asm("dex");
                        asm("stx %b",setBoard_x);
                        
                        //setBoard_y=TouchDo_k+1;
                        asm("ldx %b",TouchDo_k);
                        asm("inx");
                        asm("stx %b",setBoard_y);
                        getBoard();

                        //setBoard_y=TouchDo_k;
                        asm("ldx %b",TouchDo_k);
                        asm("stx %b",setBoard_y);
                        setBoard();
                    asm("dec %b",TouchDo_i);
                    asm("bne %g",fori4);
                    
                    //DrawLine_y=TouchDo_k;
                    asm("ldx %b",TouchDo_k);
                    asm("stx %b",DrawLine_y);
                    DrawLine();
                asm("ldx %b",TouchDo_k);
                asm("inx");
                asm("stx %b",TouchDo_k);
                asm("cpx #$13");
                asm("bne %g",fork2);
            else5:
        asm("dec %b",TouchDo_j);
        asm("beq %g",forjext);
        asm("jmp %g",forj3);
        forjext:
        ;
    }
    
    //is gameover
    {
        //for(TouchDo_i=0;TouchDo_i<10;TouchDo_i++)
        asm("lda #$0A");
        asm("sta %b",TouchDo_i);
        fori5:
            //setBoard_x=TouchDo_i-1;
            asm("ldx %b",TouchDo_i);
            asm("dex");
            asm("stx %b",setBoard_x);
            //setBoard_y=18;
            asm("ldx #$12");
            asm("stx %b",setBoard_y);
            getBoard();

            //if(setBoard_val)
            asm("ldx %b",setBoard_val);
            asm("beq %g",else6);
                Clear();
                asm("jmp %g",break2);
            else6:
        asm("dec %b",TouchDo_i);
        asm("bne %g",fori5);
        break2:
        ;
    }
    
    NextShape();
    //draw shape
    DrawShape();
    //TODO DRAW Next
}
void AnyTouch(){
    //if(PosX<0)
    asm("lda %b",PosX);
    asm("bpl %g",else1);
    asm("beq %g",else1);
    {
        //AnyTouch_ret=1;
        asm("lda #$01");
        asm("sta %b",AnyTouch_ret);
        asm("rts");
    }else1:

    getNowLeft();
    //if(getBlock_ret+PosX-10>0)
    asm("lda %b",getBlock_ret);
    asm("clc");
    asm("adc %b",PosX);
    asm("sec");
    asm("sbc #$0A");
    asm("bmi %g",else2);
    asm("beq %g",else2);
    {
        //AnyTouch_ret=1;
        asm("lda #$01");
        asm("sta %b",AnyTouch_ret);
        asm("rts");
    }else2:
    
    //for(AnyTouch_i=4;AnyTouch_i>0;AnyTouch_i--)
    asm("lda #$04");
    asm("sta %b",AnyTouch_i);
    fori:
        //for(AnyTouch_j=4;AnyTouch_j>0;AnyTouch_j--)
        asm("lda #$04");
        asm("sta %b",AnyTouch_j);
        forj:
            //getBlock_j = AnyTouch_j-1;
            asm("ldx %b",AnyTouch_j);
            asm("dex");
            asm("stx %b",getBlock_j);
            getNowBottom();
            
            //if(getBlock_ret-1-PosY>0)
            asm("dec %b",getBlock_ret);
            asm("lda %b",getBlock_ret);
            asm("sec");
            asm("sbc %b",PosY);
            asm("bmi %g",else3);
            asm("beq %g",else3);
            {
                //AnyTouch_ret=1;
                asm("lda #$01");
                asm("sta %b",AnyTouch_ret);
                asm("rts");
            }else3:
            //getBlock_i=AnyTouch_i-1;
            asm("ldx %b",AnyTouch_i);
            asm("dex");
            asm("stx %b",getBlock_i);
            //getBlock_j=AnyTouch_j-1;
            asm("ldx %b",AnyTouch_j);
            asm("dex");
            asm("stx %b",getBlock_j);
            getNowBlock();
            
            //setBoard_x=PosX+AnyTouch_i-1;
            asm("ldx %b",AnyTouch_i);
            asm("dex");
            asm("txa");
            asm("clc");
            asm("adc %b",PosX);
            asm("sta %b",setBoard_x);
            //setBoard_y=PosY-AnyTouch_j+1;
            asm("ldx %b",PosY);
            asm("inx");
            asm("txa");
            asm("sec");
            asm("sbc %b",AnyTouch_j);
            asm("sta %b",setBoard_y);
            getBoard();
            
            //if(getBlock_ret==0||setBoard_val==0){}else
            asm("lda %b",getBlock_ret);
            asm("beq %g",else4);
            asm("lda %b",setBoard_val);
            asm("beq %g",else4);
            {
                //AnyTouch_ret=1;
                asm("lda #$01");
                asm("sta %b",AnyTouch_ret);
                asm("rts");
            }
            else4:
        asm("dec %b",AnyTouch_j);
        asm("bne %g",forj);
    asm("dec %b",AnyTouch_i);
    asm("bne %g",fori);
    //AnyTouch_ret=0;
    asm("lda #$00");
    asm("sta %b",AnyTouch_ret);
}
void rotate(){
    //NowDirectionNo=(NowDirectionNo+4+rotate_n)&3;
    asm("lda %b",NowDirectionNo);
    asm("clc");
    asm("adc #$04");
    asm("adc %b",rotate_n);
    asm("and #$03");
    asm("sta %b",NowDirectionNo);
    
    getNowLeft();
    //if(PosX+getBlock_ret>10)
    asm("lda %b",getBlock_ret);
    asm("clc");
    asm("adc %b",PosX);
    asm("cmp #$0A");
    asm("bmi %g",else1);
    asm("beq %g",else1);
        //rotate_tempPosX=PosX;
        asm("lda %b",PosX);
        asm("sta %b",rotate_tempPosX);
        //PosX=10-getBlock_ret;
        asm("lda #$0A");
        asm("sec");
        asm("sbc %b",getBlock_ret);
        asm("sta %b",PosX);
        AnyTouch();
        
        //if(0==AnyTouch_ret)jmp jp2
        asm("lda %b",AnyTouch_ret);
        asm("beq %g",jp2);
        
        //PosX=rotate_tempPosX;
        asm("lda %b",rotate_tempPosX);
        asm("sta %b",PosX);
        asm("jmp %g",jp1);
    else1:
    AnyTouch();
    //if(AnyTouch_ret)asm("jmp %g",jp1);
    asm("lda %b",AnyTouch_ret);
    asm("beq %g",jp2);
    jp1:
    //NowDirectionNo=(NowDirectionNo+4-rotate_n)&3;
    asm("lda %b",NowDirectionNo);
    asm("clc");
    asm("adc #$04");
    asm("sec");
    asm("sbc %b",rotate_n);
    asm("and #$03");
    asm("sta %b",NowDirectionNo);
    jp2:
    DrawShape();
}
void movelr(){
    //PosX+=movelr_n;
    asm("lda %b",PosX);
    asm("clc");
    asm("adc %b",movelr_n);
    asm("sta %b",PosX);
    AnyTouch();
    
    //if(AnyTouch_ret)
    asm("lda %b",AnyTouch_ret);
    asm("beq %g",else1);
        //PosX-=movelr_n;
        asm("lda %b",PosX);
        asm("sec");
        asm("sbc %b",movelr_n);
        asm("sta %b",PosX);
    else1:
    DrawShape();
}
void down(){
    Touch();
    //while(Touch_ret==0)
    while1:
    asm("lda %b",Touch_ret);
    asm("bne %g",endwhile1);
        //PosY--;
        asm("dec %b",PosY);
        Touch();
    asm("jmp %g",while1);
    endwhile1:
    TouchDo();
}
void slowdown(){
    Touch();
    //if(Touch_ret)
    asm("lda %b",Touch_ret);
    asm("beq %g",else1);
        TouchDo();
        asm("rts");
    else1:
        //PosY--;
        asm("dec %b",PosY);
        DrawShape();
}
void readJoystick(){
    asm("ldx #$01");
    //*(char*)(0x4016)=01;
    asm("stx $4016");
    //*(char*)(0x4016)=00;
    asm("dex");
    asm("stx $4016");
    //key1=0;
    asm("stx %b",key1);

    asm("ldx #$08");
    fori:
        //key1=(key1<<1)|*(char*)(0x4016)&1;
        asm("lda $4016");
        asm("and #$01");
        asm("asl %b",key1);
        asm("ora %b",key1);
        asm("sta %b",key1);
    asm("dex");
    asm("bne %g",fori);
}
void main(){
    //init pattern
    {
        asm("lda #$00");
        //*(char*)(0x2006)=0;
        asm("sta $2006");
        //*(char*)(0x2006)=0;
        asm("sta $2006");
        
        //for(i=0;i<255;i++)
        asm("ldx #$00");
        for1:
            //*(char*)(0x2007)=img1[i];
            asm("lda %v,X",img1);
            asm("sta $2007");
        asm("inx");
        asm("cpx #$FF");
        asm("bne %g",for1);
        //*(char*)(0x2007)=img1[255];
        asm("lda %v,X",img1);
        asm("sta $2007");

        //for(i=0;i<255;i++)
        asm("ldx #$00");
        for2:
            //*(char*)(0x2007)=img2[i];
            asm("lda %v,X",img2);
            asm("sta $2007");
        asm("inx");
        asm("cpx #$FF");
        asm("bne %g",for2);
        //*(char*)(0x2007)=img1[255];
        asm("lda %v,X",img2);
        asm("sta $2007");
    }
    
    //clear SP
    {
        //*(char*)(0x2003)=0; //i<<2
        asm("lda #$00");
        asm("sta $2003");
        //for(i=255;i>0;i--)
        asm("ldx #$FF");
        for3:
            //*(char*)(0x2004)=0;
            asm("sta $2004");
        asm("dex");
        asm("bne %g",for3);
        //*(char*)(0x2004)=0;
        asm("sta $2004");
    }
    
    //set background black
    {
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta $2006");
        //*(char*)(0x2006)=0x20;
        asm("sta $2006");
        //for(i=0;i<255;i++)
        asm("lda #$10");
        asm("ldx #$FF");
        for4:
            //*(char*)(0x2007)=0x10;
            asm("sta $2007");
        asm("dex");
        asm("bne %g",for4);
        //*(char*)(0x2007)=0x10;
        asm("sta $2007");
        
        //for(i=0;i<255;i++)
        asm("ldx #$FF");
        for5:
            //*(char*)(0x2007)=0x10;
            asm("sta $2007");
        asm("dex");
        asm("bne %g",for5);
        //*(char*)(0x2007)=0x10;
        asm("sta $2007");

        //for(i=0;i<255;i++)
        asm("ldx #$FF");
        for6:
            //*(char*)(0x2007)=0x10;
            asm("sta $2007");
        asm("dex");
        asm("bne %g",for6);
        //*(char*)(0x2007)=0x10;
        asm("sta $2007");
        //for(i=0;i<128;i++)
        asm("ldx #$80");
        for7:
            //*(char*)(0x2007)=0x10;
            asm("sta $2007");
        asm("dex");
        asm("bne %g",for7);
    }

    //set color
    {
        //*(char*)(0x2006)=0x3F;
        asm("lda #$3F");asm("sta $2006");
        //*(char*)(0x2006)=0x00;
        asm("ldx #$00");asm("stx $2006");
        
        //*(char*)(0x2007)=0x2C;
        asm("ldx #$2C");asm("stx $2007");
        //*(char*)(0x2007)=0x30;
        asm("ldx #$30");asm("stx $2007");
        //*(char*)(0x2007)=0x00;
        asm("ldx #$00");asm("stx $2007");
        //*(char*)(0x2007)=0x3F;
        asm("sta $2007");

        //*(char*)(0x2006)=0x3F;
        asm("sta $2006");
        //*(char*)(0x2006)=0x11;
        asm("ldx #$11");asm("stx $2006");
        //*(char*)(0x2007)=0x30;
        asm("ldx #$30");asm("stx $2007");
        //*(char*)(0x2007)=0x22;
        asm("ldx #$22");asm("stx $2007");
        //*(char*)(0x2007)=0x3F;
        asm("sta $2007");
    }
    
    //show init title
    {
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta $2006");
        //*(char*)(0x2006)=0xC5;
        asm("lda #$C5");asm("sta $2006");

        //*(char*)(0x2007)=0x11;
        asm("ldx #$11");asm("stx $2007");
        //*(char*)(0x2007)=0x12;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x13;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x14;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x10;
        asm("lda #$10");asm("sta $2007");
        //*(char*)(0x2007)=0x15;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x10;
        asm("sta $2007");
        //*(char*)(0x2007)=0x16;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x17;
        asm("inx");asm("stx $2007");
        //*(char*)(0x2007)=0x18;
        asm("inx");asm("stx $2007");
        
        //*(char*)(0x2005) = 0;
        asm("stx $2005");
        //*(char*)(0x2005) = 0;
        asm("stx $2005");
    }

    //init ppu
    {
        //*(char*)(0x2000) = 0x80; //10 00 00 00
        asm("lda #$80");asm("sta $2000");
        //*(char*)(0x2001) = 0x18; //00 01 10 00
        asm("lda #$18");asm("sta $2001");
        //*(char*)(0x2005) = 0;
        asm("lda #$00");asm("sta $2005");
        //*(char*)(0x2005) = 0;
        asm("sta $2005");
    }
    //wait for key pressed (randomize)
    while1:
        rnd();
        readJoystick();
        asm("lda %b",key1);
        //if(key1)break;
        asm("bne %g",endwhile1);
    asm("jmp %g",while1);
    endwhile1:
    
    
    Clear();

    //clear 4*4 nextShape
    {
        waitvblank();
        asm("ldx #$00");
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta $2006");
        //*(char*)(0x2006)=0xAE;
        asm("lda #$AE");asm("sta $2006");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta $2006");
        //*(char*)(0x2006)=0xCE;
        asm("lda #$CE");asm("sta $2006");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta $2006");
        //*(char*)(0x2006)=0xEE;
        asm("lda #$EE");asm("sta $2006");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2006)=0x21;
        asm("lda #$21");asm("sta $2006");
        //*(char*)(0x2006)=0x0E;
        asm("lda #$0E");asm("sta $2006");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2007)=0;
        asm("stx $2007");
        //*(char*)(0x2005) = 0;
        asm("stx $2005");
        //*(char*)(0x2005) = 0;
        asm("stx $2005");
    }
    
    NextShape();
    NextShape();

    //main cycle
    {
        //main_cnt = 50;
        asm("lda #$32");
        asm("sta %b",main_cnt);

        while2:
            rnd();
            waitvblank();
            
            //one second count
            //if(main_cnt--==0)
            asm("dec %b",main_cnt);
            asm("bne %g",else1);
                //main_cnt=50;
                asm("lda #$32");
                asm("sta %b",main_cnt);
                slowdown();
            else1:
            
            readJoystick();
            //key2 = key1 ^ lastkey;
            asm("lda %b",key1);
            asm("eor %b",lastkey);
            //key2 = key2 & key1;
            asm("and %b",key1);
            //if(key2!=0)
            asm("beq %g",else2);
                asm("sta %b",key2);
                //if(key2&0x01){movelr_n=1;movelr();}//button_RIGHT
                asm("and #$01");
                asm("beq %g",elseKey1);
                    asm("lda #$01");
                    asm("sta %b",movelr_n);
                    movelr();
                elseKey1:
                //if(key2&0x02){movelr_n=-1;movelr();}//button_LEFT
                asm("lda %b",key2);
                asm("and #$02");
                asm("beq %g",elseKey2);
                    asm("lda #$FF");
                    asm("sta %b",movelr_n);
                    movelr();
                elseKey2:
                //if(key2&0x04)down();//button_DOWN
                asm("lda %b",key2);
                asm("and #$04");
                asm("beq %g",elseKey3);
                    down();
                elseKey3:
                //if(key2&0x08)slowdown();//button_UP
                asm("lda %b",key2);
                asm("and #$08");
                asm("beq %g",elseKey4);
                    slowdown();
                elseKey4:
                //if(key2&0x10);//button_START
                //if(key2&0x20);//button_SELECT
                //if(key2&0x40){rotate_n=-1;rotate();}//button_B
                asm("lda %b",key2);
                asm("and #$40");
                asm("beq %g",elseKey5);
                    asm("lda #$FF");
                    asm("sta %b",rotate_n);
                    rotate();
                elseKey5:
                //if(key2&0x80){rotate_n=1;rotate();}//button_A
                asm("lda %b",key2);
                asm("and #$80");
                asm("beq %g",elseKey6);
                    asm("lda #$01");
                    asm("sta %b",rotate_n);
                    rotate();
                elseKey6:
            else2:
            //lastkey = key1;
            asm("lda %b",key1);
            asm("sta %b",lastkey);
        asm("jmp %g",while2);
    }
}





