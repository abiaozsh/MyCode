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
char const data3[32] = {
0x04,0x01,0x04,0x01,0x02,0x02,0x02,0x02,0x03,0x02,0x03,0x02,0x03,0x02,0x03,0x02, //4
0x03,0x02,0x03,0x02,0x03,0x02,0x03,0x02,0x02,0x03,0x02,0x03,0x00,0x00,0x00,0x00  //5
};
char const dataUser[32] = {
//00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
0x02,0x02,0x10,0x10,0x02,0x0D,0x10,0x68,0x07,0x0D,0x00,0x00,0x00,0x00,0x00,0x00, //6 player1
0x02,0x13,0x10,0x98,0x0B,0x0E,0x58,0x70,0x10,0x0E,0x00,0x00,0x00,0x00,0x00,0x00  //7 player2
};
#define DRAW_BOARD    0x00
#define DRAW_SP       0x02
#define DRAW_NEXT     0x04
#define DRAW_NEXT_SP  0x06
#define DRAW_SCORE    0x08
#define TOP           0x00
#define LEFT          0x01

char const datarnd[128] = {
0x45,0x32,0x42,0x34,0x26,0x21,0x54,0x32,0x26,0x46,0x40,0x06,0x66,0x10,0x41,0x11,//<<rnd
0x62,0x16,0x20,0x04,0x42,0x02,0x35,0x51,0x12,0x53,0x50,0x36,0x55,0x40,0x03,0x52,//<<rnd
0x03,0x31,0x56,0x66,0x62,0x00,0x14,0x26,0x33,0x52,0x02,0x20,0x43,0x13,0x03,0x01,//<<rnd
0x20,0x31,0x54,0x21,0x40,0x15,0x20,0x33,0x34,0x52,0x66,0x53,0x46,0x33,0x46,0x16,//<<rnd
0x23,0x43,0x55,0x40,0x61,0x14,0x46,0x11,0x11,0x50,0x36,0x32,0x20,0x65,0x00,0x63,//<<rnd
0x05,0x45,0x23,0x16,0x44,0x10,0x63,0x62,0x26,0x14,0x34,0x50,0x25,0x25,0x22,0x14,//<<rnd
0x51,0x46,0x44,0x06,0x13,0x22,0x53,0x44,0x10,0x61,0x25,0x62,0x55,0x40,0x34,0x21,//<<rnd
0x50,0x36,0x11,0x30,0x61,0x40,0x62,0x50,0x56,0x05,0x34,0x05,0x41,0x51,0x06,0x50 //<<rnd
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
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//black
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x36,0x56,0x56,0x36,0x76,0x76,0x76,0x79,//pu
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCD,0xB5,0xBD,0xCC,0xF5,0xF5,0xB5,0xCD,//sh
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBD,0xBD,0xBA,0x38,0xBA,0xBA,0xBA,0xBA,//a
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xED,0xEB,0xEB,0xE7,0xEB,0xEB,0xED,0xED,//ke
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0E,0x75,0x75,0x1B,0x7B,0x7B,0x7B,0x1B,//y
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCF,0xB7,0x7B,0x7B,0x7B,0x7B,0xB7,0xCF,//0
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xCF,0xEF,0xEF,0xEF,0xEF,0xEF,0xC7,//1
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xBB,0xFB,0xF7,0xEF,0xDF,0xBF,0x83,//2
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xFB,0xFB,0xC7,0xFB,0xFB,0xFB,0xC7,//3
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF7,0xE7,0xD7,0xB7,0x81,0xF7,0xF7,0xE3,//4
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x83,0xBF,0xBF,0x87,0xFB,0xFB,0xBB,0xC7,//5
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xBB,0xBF,0x87,0xBB,0xBB,0xBB,0xC7,//6
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xDB,0xFB,0xF7,0xF7,0xF7,0xF7,0xF7,//7
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xBB,0xBB,0xC7,0xBB,0xBB,0xBB,0xC7,//8
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0xBB,0xBB,0xC7,0xFB,0xFB,0xFB,0xC7 //9
};

//don't forget "lda  ($08,X)"   "lda  ($00),y"  zero page transfer

#define PTR2000             0x10
#define PTR2001             0x12
#define PTR2002             0x14
#define PTR2003             0x16
#define PTR2004             0x18
#define PTR2005             0x1A
#define PTR2006             0x1C
#define PTR2007             0x1E

//page0 0x0000 ~0x00FF
#define rand0               0x20
#define getRnd7_ret         0x21
#define AnyTouch_i          0x22
#define AnyTouch_j          0x23
#define Touch_i             0x24
#define getBlock_i          0x25
#define getBlock_j          0x26
#define getBlock_idx        0x27
#define getBlock_ret        0x28
#define setBoard_x          0x29
#define setBoard_y          0x2A
#define setBoard_val        0x2B
#define setBoard_base       0x2C
#define getBase_x           0x2D
#define getBase_y           0x2E
#define getBase_hi          0x2F
#define getBase_lo          0x30
#define getBase_tmp         0x31
#define getBase_item        0x32
#define getSPBase_item      0x33
#define getSPBase_x         0x34
#define getSPBase_y         0x35
#define DrawLine_i          0x36
#define DrawLine_y          0x37
#define DrawShape_i         0x38
#define DrawShape_j         0x39
#define Clear_i             0x3A
#define TouchDo_i           0x3B
#define TouchDo_j           0x3C
#define TouchDo_k           0x3D
#define movelr_n            0x3E
#define rotate_n            0x40
#define rotate_tempPosX     0x41
#define main_i              0x42
#define main_j              0x43
#define currentPlayer       0x44
#define player1On           0x45
#define player2On           0x46
#define readJoystick_player 0x47
#define CurSP_player        0x48
#define CurBoard_player     0x49 //0,0x64

//current player
#define PosX             0x50
#define PosY             0x51
#define NextShapeNo      0x52
#define NowShapeNo       0x53
#define NowDirectionNo   0x54
#define lastkey          0x55
#define key1             0x56
#define key2             0x57
#define Score0           0x58
#define Score1           0x59
#define Score2           0x5A
#define Score3           0x5B
#define TimeCount        0x5C


//page1: 0x0100 ~0x01FF stack

//page2: 0x0200 ~0x02FF
#define Board            0x0200//size:100*2(0x0C8) player board 1,2
#define Player           0x02C8//size:16*2(0x020)  player data 1,2
#define DrawBuff         0x02E8//size:16(0x010)

//* %b - Numerical 8-bit value
//* %w - Numerical 16-bit value
//* %l - Numerical 32-bit value
//* %v - Assembler name of a (global) variable or function
//* %o - Stack offset of a (local) variable
//* %g - Assembler name of a C label
//* %s - The argument is converted to a string
//* %% - The % sign itself

void waitvblank(){
    asm("ldy #$00");
    asm("lda (%b),Y",PTR2002);
    asm("bmi %g",else1);
        lbl1:
        asm("lda (%b),Y",PTR2002);
        asm("bmi %g",lbl1);
        lbl2:
        asm("inc %b",rand0);
        asm("lda (%b),Y",PTR2002);
        asm("bpl %g",lbl2);
        asm("rts");
    else1:
    asm("inc %b",rand0);
    asm("lda (%b),Y",PTR2002);
    asm("bpl %g",else1);
}
void nmi(){
    asm("inc %b",rand0);
}
void getBase(){
    asm("lda %b",currentPlayer);//player 0x00,0x10
    asm("clc");
    asm("adc %b",getBase_item);
    asm("tax");
    asm("lda %v+%b,X",dataUser,TOP);
    asm("clc");
    asm("adc %b",getBase_y);
    asm("lsr");
    asm("lsr");
    asm("lsr");
    asm("sta %b",getBase_tmp);
    asm("lda %v+%b,X",dataUser,TOP);
    asm("clc");
    asm("adc %b",getBase_y);
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("asl");
    asm("clc");
    asm("adc %v+%b,X",dataUser,LEFT);
    asm("clc");
    asm("adc %b",getBase_x);
    asm("clc");
    asm("adc #$20");
    asm("sta %b",getBase_lo);
    asm("lda #$20");
    asm("adc %b",getBase_tmp);
    asm("sta %b",getBase_hi);
}
void st2005(){
    //*(char*)(0x2005) = 0;
    asm("lda #$00");
    asm("tay");
    asm("sta (%b),Y",PTR2005);
    //*(char*)(0x2005) = 0;
    asm("sta (%b),Y",PTR2005);
}
void DrawScore(){
    asm("ldy #$04");
    fory:
        asm("ldx %b,Y",Score0-1);
        asm("inx");
        asm("cpx #$0A");
        asm("stx %b,Y",Score0-1);
        asm("bne %g",else1);
        asm("ldx #$00");
        asm("stx %b,Y",Score0-1);
    asm("dey");
    asm("bne %g",fory);

    else1:
    
    asm("lda #%b",DRAW_SCORE);
    asm("sta %b",getBase_item);
    asm("lda #$00");
    asm("sta %b",getBase_x);
    asm("sta %b",getBase_y);
    getBase();
    
    waitvblank();
    asm("ldy #$00");
    asm("lda %b",getBase_hi);
    asm("sta (%b),Y",PTR2006);
    asm("lda %b",getBase_lo);
    asm("sta (%b),Y",PTR2006);
    asm("clc");
    asm("ldx #$16");
    asm("txa");asm("adc %b",Score0);asm("sta (%b),Y",PTR2007);
    asm("txa");asm("adc %b",Score1);asm("sta (%b),Y",PTR2007);
    asm("txa");asm("adc %b",Score2);asm("sta (%b),Y",PTR2007);
    asm("txa");asm("adc %b",Score3);asm("sta (%b),Y",PTR2007);
    st2005();
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
    //asm("sta %b",getBlock_idx);
    //asm("ldx %b",getBlock_idx);
    asm("tax");
    
    //if(getBlock_j & 1)
    asm("lda %b",getBlock_j);
    asm("and #$01");
    asm("beq %g",else1);
        //getBlock_ret = (data1[getBlock_idx]>>4)&0x0F;
        asm("lda %v,X",data1);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",getBlock_ret);
        asm("rts");
    else1:
    //getBlock_ret = data1[getBlock_idx]&0x0F;
    asm("lda %v,X",data1);
    asm("and #$0F");
    asm("sta %b",getBlock_ret);
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
    //asm("sta %b",getBlock_idx);
    //asm("ldx %b",getBlock_idx);
    asm("tax");
    //if(getBlock_j & 1)
    asm("lda %b",getBlock_j);
    asm("and #$01");
    asm("beq %g",else1);
        //getBlock_ret = (data2[getBlock_idx]>>4)&0x0F;
        asm("lda %v,X",data2);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",getBlock_ret);
        asm("rts");
    else1:
    //getBlock_ret = data2[getBlock_idx]&0x0F;
    asm("lda %v,X",data2);
    asm("and #$0F");
    asm("sta %b",getBlock_ret);
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
    asm("clc");
    asm("adc %b",CurBoard_player);
    asm("sta %b",setBoard_base);
    
    //setBoard_y = setBoard_x & 1;
    asm("lda %b",setBoard_x);
    asm("and #$01");
}
void setBoard(){
    calcBoardAddress();

    //if(setBoard_y)
    asm("beq %g",else1);
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
    else1:
    //Board[setBoard_base] = (Board[setBoard_base] & 0xF0) | setBoard_val;//low 4
    asm("ldx %b",setBoard_base);
    asm("lda %w,X",Board);
    asm("and #$F0");
    asm("ora %b",setBoard_val);
    asm("sta %w,X",Board);
}
void getBoard(){
    calcBoardAddress();
    //if(setBoard_y)
    asm("beq %g",else1);
        //return Board[base] >> 4;//high 4
        asm("ldx %b",setBoard_base);
        asm("lda %w,X",Board);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("sta %b",setBoard_val);
        asm("rts");
    else1:
    //return Board[base] & 0x0F;//low 4
    asm("ldx %b",setBoard_base);
    asm("lda %w,X",Board);
    asm("and #$0F");
    asm("sta %b",setBoard_val);
}
void getRnd7(){
    //return Math.floor(Math.random()*7);
    //v = datarnd[(rand0&0x7F)];
    asm("lda %b",rand0);
    asm("and #$7F");
    asm("tax");
    //asm("lda %v,X",datarnd);
    //asm("sta %b",getRnd7_ret);
    
    //if(rand0&0x80)
    asm("lda %b",rand0);
    asm("and #$80");
    
    asm("beq %g",else1);
        //return (v>>4);
        asm("lda %v,X",datarnd);
        asm("lsr");
        asm("lsr");
        asm("lsr");
        asm("lsr");
        //asm("sta %b",getRnd7_ret); not need
        asm("rts");
    else1:
    //return v&0x0F;
    asm("lda %v,X",datarnd);
    asm("and #$0F");
    //asm("sta %b",getRnd7_ret); not need
}
void DrawBuffToPPU(){
    asm("clc");
    asm("adc %b",CurSP_player);
    asm("sta $2003");
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
void getSPBase(){
    asm("lda %b",currentPlayer);//player 0x00,0x10
    asm("clc");
    asm("adc %b",getSPBase_item);
    asm("tax");
    
    //DrawShape_temp = DrawShape_j;
    asm("lda %b",getSPBase_y);
    asm("clc");
    asm("adc #$01");
    //DrawShape_temp <<= 3;
    asm("asl");
    asm("asl");
    asm("asl");
    //DrawShape_temp += DRAW_NEXT_SP_TOP;
    asm("clc");
    asm("adc %v+%b,X",dataUser,TOP);
    asm("sec");
    //DrawShape_temp--;
    asm("sbc #$01");

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
    
    //DrawShape_temp=(DrawShape_i-1)<<3
    asm("lda %b",getSPBase_x);
    asm("asl");
    asm("asl");
    asm("asl");
    asm("clc");
    asm("adc %v+%b,X",dataUser,LEFT);

    //DrawBuff[DrawShape_idx++]=DrawShape_temp;
    asm("sta %w,Y",DrawBuff);
    asm("iny");
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
            asm("stx %b",getSPBase_x);

            //getBlock_j = DrawShape_j-1;
            asm("ldx %b",DrawShape_j);
            asm("dex");
            asm("stx %b",getBlock_j);
            asm("stx %b",getSPBase_y);

            getNextBlock();
            //if(getBlock_ret)
            //asm("lda %b",getBlock_ret); a is getBlock_ret
            asm("beq %g",else1);
                asm("lda #%b",DRAW_NEXT_SP);
                asm("sta %b",getSPBase_item);
                getSPBase();
            else1:
        asm("dec %b",DrawShape_j);
        asm("bne %g",forj);
    asm("dec %b",DrawShape_i);
    asm("bne %g",fori);

    waitvblank();
    //*(char*)(0x2003)=16; //i<<2
    asm("lda #$10");
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
            //getSPBase_x = DrawShape_i - 1 + PosX
            //asm("lda %b",DrawShape_i);
            asm("txa");
            asm("clc");
            asm("adc %b",PosX);
            asm("sta %b",getSPBase_x);

            
            //getBlock_j=DrawShape_j-1;
            asm("ldx %b",DrawShape_j);
            asm("dex");
            asm("stx %b",getBlock_j);

            //getSPBase_y = DrawShape_j-1+19-PosY
            //asm("lda %b",DrawShape_j);
            asm("txa");
            asm("clc");
            asm("adc #$13");//19
            asm("sec");
            asm("sbc %b",PosY);
            asm("sta %b",getSPBase_y);

            getNowBlock();
            //if(getBlock_ret)
            // asm("lda %b",getBlock_ret); a is getBlock_ret
            asm("beq %g",else1);
                asm("lda #%b",DRAW_SP);
                asm("sta %b",getSPBase_item);
                getSPBase();
            else1:
        asm("dec %b",DrawShape_j);
        asm("bne %g",forj);
    asm("dec %b",DrawShape_i);
    asm("bne %g",fori);

    waitvblank();
    //*(char*)(0x2003)=0; //i<<2
    asm("lda #$00");
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

    st2005();
    waitvblank();
}
void Clear(){
    //for(i=0;i<100;i++)
    asm("ldx #$63");//99 times
    fori:
        //Board[i]=0;
        asm("txa");
        asm("clc");
        asm("adc %b",CurBoard_player);
        asm("tay");
        asm("lda #$00");
        asm("sta %w,Y",Board);
    asm("dex");
    asm("bne %g",fori);
    //Board[0]=0;
    asm("lda #$00");
    asm("ldx %b",CurBoard_player);
    asm("sta %w,X",Board);

    waitvblank();

    //calc base address
    asm("lda #%b",DRAW_BOARD);
    asm("sta %b",getBase_item);
    asm("lda #$00");
    asm("sta %b",getBase_x);
    asm("sta %b",getBase_y);
    getBase();

    asm("lda #$14");//20 times
    asm("sta %b",Clear_i);
    fori2:
        //*(char*)(0x2006)=getBase_hi;
        asm("lda %b",getBase_hi);
        asm("sta $2006");
        //*(char*)(0x2006)=getBase_lo;
        asm("lda %b",getBase_lo);
        asm("sta $2006");
        
        asm("clc");
        asm("adc #$20");
        asm("sta %b",getBase_lo);
        asm("lda %b",getBase_hi);
        asm("adc #$00");
        asm("sta %b",getBase_hi);

        ClearLine();
    asm("dec %b",Clear_i);
    asm("bne %g",fori2);
    
    asm("ldx #$09");
    asm("stx %b",Score0);
    asm("stx %b",Score1);
    asm("stx %b",Score2);
    asm("stx %b",Score3);
    DrawScore();
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
    //asm("lda %b",getRnd7_ret); a is getRnd7_ret
    asm("sta %b",NextShapeNo);
    DrawNextShape();
}
void Touch(){
    //for(Touch_i=4;Touch_i>0;Touch_i--)
    asm("lda #$04");//20 times
    asm("sta %b",Touch_i);
    fori:
        //getBlock_j = Touch_i-1;
        asm("ldx %b",Touch_i);
        asm("dex");
        asm("stx %b",getBlock_j);

        getNowBottom();
        //if(getBlock_ret)
        //asm("lda %b",getBlock_ret); a is getBlock_ret
        asm("beq %g",else1);
            //if(getBlock_ret-1==PosY)goto ret1;
            asm("tax");//asm("ldx %b",getBlock_ret); a is getBlock_ret
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
            //asm("lda %b",setBoard_val);  a is setBoard_val
            asm("bne %g",ret1);
        else1:
    asm("dec %b",Touch_i);
    asm("bne %g",fori);
    
    //return 0;
    asm("lda #$00");
    //asm("sta %b",Touch_ret);
    asm("rts");
    ret1:
    //return 1;
    asm("lda #$01");
    //asm("sta %b",Touch_ret);
}
void getBoardBase(){
    asm("lda #%b",DRAW_BOARD);
    asm("sta %b",getBase_item);
    asm("lda #$13");//19-getBase_y
    asm("sec");
    asm("sbc %b",getBase_y);
    asm("sta %b",getBase_y);
    getBase();
}
void DrawLine(){
    //getBase_x=0;
    asm("lda #$00");
    asm("sta %b",getBase_x);
    //getBase_y=DrawLine_y;
    asm("lda %b",DrawLine_y);
    asm("sta %b",getBase_y);
    getBoardBase();
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
        //asm("lda %b",setBoard_val); a is setBoard_val
        asm("ldx %b",setBoard_x);
        asm("inx");
        asm("inx");
        asm("sta %w,X",DrawBuff);

        asm("inc %b",setBoard_x);//setBoard_x++
    asm("dec %b",DrawLine_i);
    asm("bne %g",fori1);
    
    
    waitvblank();
    //*(char*)(0x2006)=DrawBuff[0];
    asm("ldx #$00");
    asm("lda %w,X",DrawBuff);
    asm("sta $2006");
    asm("inx");//x=1
    //*(char*)(0x2006)=DrawBuff[1];
    asm("lda %w,X",DrawBuff);
    asm("sta $2006");
    asm("inx");//x=2
    
    //for(i=2;i<12;i++)
    asm("ldy #$0A");//10 times
    fori2:
        //*(char*)(0x2007)=DrawBuff[i];
        asm("lda %w,X",DrawBuff);
        asm("sta $2007");
        asm("inx");
    asm("dey");
    asm("bne %g",fori2);
    st2005();

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
                //setBoard_x=TouchDo_i-1+PosX;
                //getBase_x =TouchDo_i-1+PosX;
                //asm("ldx %b",TouchDo_i);
                //asm("dex");
                asm("txa");
                asm("clc");
                asm("adc %b",PosX);
                asm("sta %b",setBoard_x);
                asm("sta %b",getBase_x);
                
                //getBlock_j=TouchDo_j-1;
                asm("ldx %b",TouchDo_j);
                asm("dex");
                asm("stx %b",getBlock_j);

                //setBoard_y=PosY-(TouchDo_j-1);
                //getBase_y =PosY-(TouchDo_j-1);
                asm("lda %b",PosY);
                //asm("inx");
                //asm("txa");
                asm("sec");
                asm("sbc %b",getBlock_j);//getBlock_j is TouchDo_j-1
                asm("sta %b",setBoard_y);
                asm("sta %b",getBase_y);

                getNowBlock();
                //if(getBlock_ret)
                //asm("lda %b",getBlock_ret); a is getBlock_ret
                asm("beq %g",else1);
                
                    //setBoard_val=getBlock_ret;
                    asm("lda %b",getBlock_ret);
                    asm("sta %b",setBoard_val);
                    setBoard();

                    getBoardBase();
                    //DrawBuff[TouchDo_idx++]=getBase_hi;
                    //asm("lda %b",getBase_hi); a is getBase_hi
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
                else1:
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
        asm("dex");
        asm("bne %g",fori2);
        st2005();
    }

    //clear line
    {
        //for(TouchDo_j=20;TouchDo_j>0;TouchDo_j--)
        asm("lda #$14");
        asm("sta %b",TouchDo_j);
        forj3:
            //TouchDo_line=1;
            //asm("lda #$01");
            //asm("sta %b",TouchDo_line);
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
                //asm("lda %b",setBoard_val); a is setBoard_val
                asm("bne %g",else2);
                    //TouchDo_line=0;
                    //asm("lda #$00"); a is 0
                    //asm("sta %b",TouchDo_line);
                    asm("lda #$00");
                    asm("jmp %g",break1);
                else2:
            asm("dec %b",TouchDo_i);
            asm("bne %g",fori3);
            asm("lda #$01");
            break1:
            //if(TouchDo_line)
            //asm("ldx %b",TouchDo_line);
            asm("beq %g",else5);
            //asm("bne %g",if5);
            //asm("jmp %g",else5);

                DrawScore();
                //for(TouchDo_k=0;TouchDo_k<10;TouchDo_k++)
                asm("lda #$0A");//10 times
                asm("sta %b",TouchDo_k);
                fork1:
                    //setBoard_x=TouchDo_k-1;
                    asm("ldx %b",TouchDo_k);
                    asm("dex");
                    asm("stx %b",setBoard_x);
                    
                    //if(TouchDo_j-2>=0)
                    asm("lda %b",TouchDo_j);
                    asm("sec");
                    asm("sbc #$02");
                    asm("bmi %g",else3);
                        //setBoard_y=TouchDo_j-2;
                        asm("sta %b",setBoard_y);
                        getBoard();//in setBoard_val
                        //setBoard_val=data1[setBoard_val+0xE0];
                        //asm("lda %b",setBoard_val); a is setBoard_val
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
                    //asm("lda %b",setBoard_val); a is setBoard_val
                    asm("clc");
                    asm("adc #$F0");
                    asm("tax");
                    asm("lda %v,X",data1);
                    asm("sta %b",setBoard_val);
                    setBoard();
                    
                    /*
                    //setBoard_y=TouchDo_j-1;
                    asm("ldx %b",TouchDo_j);
                    asm("dex");
                    asm("stx %b",setBoard_y);
                    //setBoard_val=0;
                    asm("lda #$00");
                    asm("sta %b",setBoard_val);
                    setBoard();
                    */
                asm("dec %b",TouchDo_k);
                asm("bne %g",fork1);
                
                //if(TouchDo_j-2>=0)
                asm("lda %b",TouchDo_j);
                asm("sec");
                asm("sbc #$02");
                asm("bmi %g",else4);
                    //DrawLine_y=TouchDo_j-2;
                    asm("sta %b",DrawLine_y);
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
                asm("inc %b",TouchDo_k);
                asm("lda %b",TouchDo_k);
                asm("cmp #$13");
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
            //asm("lda %b",setBoard_val); a is setBoard_val
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
}
void AnyTouch(){
    //if(PosX<0)
    asm("lda %b",PosX);
    asm("bpl %g",else1);
    asm("beq %g",else1);
        //AnyTouch_ret=1;
        asm("lda #$01");
        asm("rts");
    else1:

    getNowLeft();
    //if(getBlock_ret+PosX-10>0)  ->  (getBlock_ret+PosX>=11)
    //asm("lda %b",getBlock_ret); a is getBlock_ret
    asm("clc");
    asm("adc %b",PosX);
    asm("cmp #$0B");
    asm("bmi %g",else2);
        //AnyTouch_ret=1;
        asm("lda #$01");
        //asm("sta %b",AnyTouch_ret);
        asm("rts");
    else2:
    
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
            //if(getBlock_ret-1-PosY>0)  ->  (getBlock_ret-1-1>=PosY)
            asm("tax");//asm("ldx %b",getBlock_ret); a is getBlock_ret
            asm("dex");
            asm("dex");
            asm("cpx %b",PosY);
            asm("bmi %g",else3);
                //AnyTouch_ret=1;
                asm("lda #$01");
                asm("rts");
            else3:
            
            //getBlock_i=AnyTouch_i-1;
            asm("ldx %b",AnyTouch_i);
            asm("dex");
            asm("stx %b",getBlock_i);
            //setBoard_x=PosX+AnyTouch_i-1;
            asm("txa");
            asm("clc");
            asm("adc %b",PosX);
            asm("sta %b",setBoard_x);


            //getBlock_j=AnyTouch_j-1;
            asm("ldx %b",AnyTouch_j);
            asm("dex");
            asm("stx %b",getBlock_j);
            //setBoard_y=PosY-(AnyTouch_j-1);
            asm("ldx %b",PosY);
            asm("txa");
            asm("sec");
            asm("sbc %b",getBlock_j);
            asm("sta %b",setBoard_y);
            
            getNowBlock();

            getBoard();

            //if(getBlock_ret==0||setBoard_val==0){}else
            //asm("lda %b",setBoard_val); a is setBoard_val
            asm("beq %g",else4);
            asm("lda %b",getBlock_ret);
            asm("beq %g",else4);
                //AnyTouch_ret=1;
                asm("lda #$01");
                asm("rts");
            else4:
        asm("dec %b",AnyTouch_j);
        asm("bne %g",forj);
    asm("dec %b",AnyTouch_i);
    asm("bne %g",fori);
    //AnyTouch_ret=0;
    asm("lda #$00");
}
void rotate(){
    //NowDirectionNo=(NowDirectionNo+rotate_n)&3;
    asm("lda %b",NowDirectionNo);
    asm("clc");
    asm("adc %b",rotate_n);
    asm("and #$03");
    asm("sta %b",NowDirectionNo);
    
    getNowLeft();
    //if(PosX+getBlock_ret>10) -> (PosX+getBlock_ret>=11)
    //asm("lda %b",getBlock_ret); a is getBlock_ret
    asm("clc");
    asm("adc %b",PosX);
    asm("cmp #$0B");
    asm("bmi %g",else1);
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
        //asm("lda %b",AnyTouch_ret); a is AnyTouch_ret
        asm("beq %g",jp2);
            //PosX=rotate_tempPosX;
            asm("lda %b",rotate_tempPosX);
            asm("sta %b",PosX);
            asm("jmp %g",jp1);
    else1:
    
    AnyTouch();
    //if(AnyTouch_ret)asm("jmp %g",jp1);
    //asm("lda %b",AnyTouch_ret);
    asm("beq %g",jp2);
    jp1:
    //NowDirectionNo=(NowDirectionNo-rotate_n)&3;
    asm("lda %b",NowDirectionNo);
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
    //asm("lda %b",AnyTouch_ret);
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
    //asm("lda %b",Touch_ret);
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
    //asm("lda %b",Touch_ret);
    asm("beq %g",else1);
        TouchDo();
        asm("rts");
    else1:
    //PosY--;
    asm("dec %b",PosY);
    DrawShape();
}
void readJoystick(){
   asm("ldx %b",readJoystick_player);

    //*(char*)(0x4016/7)=01;
    asm("lda #$01");
    asm("sta $4016,X");
    //*(char*)(0x4016/7)=00;
    asm("lda #$00");
    asm("sta $4016,X");
    //key1=0;
    asm("sta %b",key1);

    asm("ldy #$08");
    fori:
        //key1=(key1<<1)|*(char*)(0x4016/7)&1;
        asm("lda $4016,X");
        asm("and #$01");
        asm("asl %b",key1);
        asm("ora %b",key1);
        asm("sta %b",key1);
    asm("dey");
    asm("bne %g",fori);
    asm("lda %b",key1);
}
void Clear4by4(){
    asm("lda #%b",DRAW_NEXT);
    asm("sta %b",getBase_item);
    asm("lda #$00");
    asm("tay");
    
    asm("sta %b",getBase_x);
    asm("sta %b",getBase_y);
    getBase();
    waitvblank();
    asm("ldx #$04");
    asm("stx %b",main_i);
    fori1:

        asm("ldx #$00");
        asm("lda %b",getBase_hi);
        asm("sta (%b),Y",PTR2006);
        asm("lda %b",getBase_lo);
        asm("sta (%b),Y",PTR2006);
        asm("lda #$00");
        asm("sta (%b),Y",PTR2007);
        asm("sta (%b),Y",PTR2007);
        asm("sta (%b),Y",PTR2007);
        asm("sta (%b),Y",PTR2007);

        asm("lda %b",getBase_lo);
        asm("clc");
        asm("adc #$20");
        asm("sta %b",getBase_lo);
        asm("lda %b",getBase_hi);
        asm("adc #$00");
        asm("sta %b",getBase_hi);

    asm("dec %b",main_i);
    asm("bne %g",fori1);
    
    st2005();
}
void loadplayer(){
    asm("ldy %b",currentPlayer);
    asm("ldx #$0F");
    fori:
        asm("lda %w,Y",Player);
        asm("sta %b,X",PosX);
        asm("dey");
    asm("dex");
    asm("bne %g",fori);
    asm("lda %w,Y",Player);//0
    asm("sta %b,X",PosX);
}
void saveplayer(){
    asm("ldy %b",currentPlayer);
    asm("ldx #$0F");
    fori:
        asm("lda %b,X",PosX);
        asm("sta %w,Y",Player);
        asm("dey");
    asm("dex");
    asm("bne %g",fori);
    asm("lda %b,X",PosX);
    asm("sta %w,Y",Player);
}
void mainSub(){
    loadplayer();
    //one second count
    //if(TimeCount--==0)
    asm("dec %b",TimeCount);
    asm("bne %g",else1);
        //TimeCount=50;
        asm("lda #$32");
        asm("sta %b",TimeCount);
        //slowdown();
    else1:
    
    readJoystick();
    //key2 = key1 ^ lastkey;
    //asm("lda %b",key1); a is key1
    asm("eor %b",lastkey);
    //key2 = key2 & key1;
    asm("and %b",key1);
    //if(key2!=0)
    asm("beq %g",else2);
        asm("sta %b",key2);
        //if(key2&0x01){movelr_n=1;movelr();}//button_RIGHT
        asm("and #$01");
        asm("beq %g",elseKey1);
            //asm("lda #$01"); a is 01
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
    saveplayer();
}
void Player1(){
    //currentPlayer = 0;
    asm("lda #$00");asm("sta %b",currentPlayer);
    //readJoystick_player = 0;
    asm("sta %b",readJoystick_player);
    //CurSP_player=0;
    asm("sta %b",CurSP_player);
    //CurBoard_player=0;
    asm("sta %b",CurBoard_player);
}
void Player2(){
    //currentPlayer = 0x10;
    asm("lda #$10");asm("sta %b",currentPlayer);
    //readJoystick_player = 1;
    asm("lda #$01");asm("sta %b",readJoystick_player);
    //CurSP_player=0x20;
    asm("lda #$20");asm("sta %b",CurSP_player);
    //CurBoard_player=100;
    asm("lda #$64");asm("sta %b",CurBoard_player);
}
void init(){
    Clear();
    Clear4by4();
    NextShape();
    NextShape();
    asm("lda #$32");
    asm("sta %b",TimeCount);
    saveplayer();
}
void initTitle(){
    asm("ldx #$11");asm("stx $2007");
    asm("inx");asm("stx $2007");
    asm("inx");asm("stx $2007");
    asm("inx");asm("stx $2007");
    asm("inx");asm("stx $2007");
}
void main(){
    //init ptr
    {
        asm("lda #$20");
        asm("ldy #$00");
        asm("ldx #$00");//PTR2000
        fori:
            asm("sty $10,X");
            asm("iny");
            asm("inx");
            asm("sta $10,X");
            asm("inx");
        asm("cpx #$10");//PTR2007+1
        asm("bne %g",fori);
    }
    //init pattern
    {
        asm("lda #$00");
        asm("tay");
        //*(char*)(0x2006)=0;
        asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0;
        asm("sta (%b),Y",PTR2006);
        
        //for(i=0;i<255;i++)
        asm("ldx #$00");
        for1:
            //*(char*)(0x2007)=img1[i];
            asm("lda %v,X",img1);
            asm("sta (%b),Y",PTR2007);
        asm("inx");
        asm("cpx #$FF");
        asm("bne %g",for1);
        //*(char*)(0x2007)=img1[255];
        asm("lda %v,X",img1);
        asm("sta (%b),Y",PTR2007);

        //for(i=0;i<255;i++)
        asm("ldx #$00");
        for2:
            //*(char*)(0x2007)=img2[i];
            asm("lda %v,X",img2);
            asm("sta (%b),Y",PTR2007);
        asm("inx");
        asm("cpx #$FF");
        asm("bne %g",for2);
        //*(char*)(0x2007)=img1[255];
        asm("lda %v,X",img2);
        asm("sta (%b),Y",PTR2007);
    }
    //set background black
    {
        //asm("ldy #$00"); y is still 0
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0x20;
        asm("sta (%b),Y",PTR2006);
        
        //total 380
        asm("lda #$80"); asm("sta %b",main_i);
        asm("lda #$03"); asm("sta %b",main_j);
        asm("ldx #$10");
        for4:
            asm("stx $2007");
            asm("lda %b",main_i);
            asm("bne %g",else5);
                asm("dec %b",main_j);
            else5:
            asm("dec %b",main_i);
        asm("lda %b",main_i);
        asm("bne %g",for4);
        asm("lda %b",main_j);
        asm("bne %g",for4);
    }
    //set color
    {
        //asm("ldy #$00"); y is still 0
        //*(char*)(0x2006)=0x3F;
        asm("lda #$3F");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0x00;
        asm("lda #$00");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2007)=0x2C;
        asm("lda #$2C");asm("sta (%b),Y",PTR2007);
        //*(char*)(0x2007)=0x30;
        asm("lda #$30");asm("sta (%b),Y",PTR2007);
        //*(char*)(0x2007)=0x00;
        asm("lda #$00");asm("sta (%b),Y",PTR2007);
        //*(char*)(0x2007)=0x3F;
        asm("lda #$3F");asm("sta (%b),Y",PTR2007);

        //*(char*)(0x2006)=0x3F;
        asm("lda #$3F");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0x11;
        asm("lda #$11");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2007)=0x30;
        asm("lda #$30");asm("sta (%b),Y",PTR2007);
        //*(char*)(0x2007)=0x22;
        asm("lda #$22");asm("sta (%b),Y",PTR2007);
        //*(char*)(0x2007)=0x3F;
        asm("lda #$3F");asm("sta (%b),Y",PTR2007);
    }
    //show init title
    {
        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0xC5;
        asm("lda #$C5");asm("sta (%b),Y",PTR2006);
        initTitle();

        //*(char*)(0x2006)=0x20;
        asm("lda #$20");asm("sta (%b),Y",PTR2006);
        //*(char*)(0x2006)=0xC5;
        asm("lda #$D5");asm("sta (%b),Y",PTR2006);
        initTitle();
    }
    //init ppu
    {
        //asm("ldy #$00"); y is still 0
        //*(char*)(0x2000) = 0x80; //10 00 00 00
        asm("lda #$80");asm("sta (%b),Y",PTR2000);
        //*(char*)(0x2001) = 0x18; //00 01 10 00
        asm("lda #$18");asm("sta (%b),Y",PTR2001);
        st2005();
    }
    //main cycle
    {
        while1:
            //if(!player1On)
            asm("lda %b",player1On);
            asm("bne %g",else1);
                //check player1 key
                Player1();
                readJoystick();
                //asm("lda %b",key1); a is key1
                asm("beq %g",else1);
                    asm("lda #$01");asm("sta %b",player1On);
                    init();
            else1:
        
            //if(!player2On)
            asm("lda %b",player2On);
            asm("bne %g",else2);
                //check player1 key
                Player2();
                readJoystick();
                //asm("lda %b",key1); a is key1
                asm("beq %g",else2);
                    asm("lda #$01");asm("sta %b",player2On);
                    init();
            else2:

            waitvblank();
            
            //if(player1On)
            asm("lda %b",player1On);
            asm("beq %g",else3);
                Player1();
                mainSub();
            else3:

            //if(player2On)
            asm("lda %b",player2On);
            asm("beq %g",else4);
                Player2();
                mainSub();
            else4:

        asm("jmp %g",while1);
    }
}
