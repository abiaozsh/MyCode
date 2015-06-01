PROGMEM prog_uint8_t _InitScreen[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x9F, 0xFF, 0x3F, 0x9F, 0x3F, 0xFF,   // 0x0010 (16) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0020 (32) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0030 (48) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0040 (64) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0050 (80) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x99, 0xFC,   // 0x0060 (96) pixels
  0x3F, 0x99, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF,   // 0x0070 (112) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0x9F, 0x9F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF,   // 0x0080 (128) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0090 (144) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x99, 0x99, 0x99, 0xC3, 0xF3, 0xE7, 0x0F, 0xFF, 0xFF, 0xFF,   // 0x00A0 (160) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x00B0 (176) pixels
  0xFF, 0x3C, 0x99, 0x1C, 0xFF, 0x99, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x73, 0x73,   // 0x00C0 (192) pixels
  0x73, 0x78, 0x7E, 0xFC, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x79, 0x39, 0x39, 0xF9, 0x3C,   // 0x00D0 (208) pixels
  0x7F, 0xFE, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xCF, 0xCF, 0xCF, 0xCF, 0x1F,   // 0x00E0 (224) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xF3, 0x83, 0x33, 0x33, 0x89, 0xFF,   // 0x00F0 (240) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0100 (256) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9C, 0x89, 0x48, 0xF9, 0xF9, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0110 (272) pixels
  0xFF, 0xE0, 0x66, 0x66, 0x66, 0x66, 0x26, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x98,   // 0x0120 (288) pixels
  0x33, 0x70, 0x33, 0x93, 0x98, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x0E, 0x7C,   // 0x0130 (304) pixels
  0x7C, 0x7C, 0x4C, 0x1E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7,   // 0x0140 (320) pixels
  0xE7, 0xE7, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0150 (336) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0xCC, 0xCC, 0xCC, 0xCC, 0x18, 0xFF,   // 0x0160 (352) pixels
  0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFE, 0xF0, 0xE6, 0xE6, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0170 (368) pixels
  0xF1, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8, 0xF9, 0xF9, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0180 (384) pixels
  0xFF, 0xFE, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0190 (400) pixels
  0xFF, 0x23, 0x99, 0x99, 0x99, 0x99, 0x83, 0x9F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01A0 (416) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFC, 0xFC,   // 0x01B0 (432) pixels
  0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01C0 (448) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01D0 (464) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01E0 (480) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01F0 (496) pixels
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
void DrawInitScreen()
{
  myGLCD.drawBitmap(0, 0, _InitScreen, 84, 48);
}

