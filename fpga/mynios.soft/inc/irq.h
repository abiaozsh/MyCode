extern "C"
{
void setIrq(void (*irq)(void), int enable);
void sti();
void cli();
}
