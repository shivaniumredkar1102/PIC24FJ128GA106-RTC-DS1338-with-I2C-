#include "rtc.h"
unsigned short bcd2bin(unsigned short bcd_value)
{
  unsigned short temp;

  temp = bcd_value;
  // Shifting upper digit right by 1 is same as multiplying by 8.
  temp >>= 1;
  // Isolate the bits for the upper digit.
  temp &= 0x78;

  // Now return: (Tens * 8) + (Tens * 2) + Ones

  return(temp + (temp >> 2) + (bcd_value & 0x0f));
} 
char BSP_RTCC_DecToBCD (char value)
{
 return (((value / 10)) << 4) | (value % 10);
}

void I2CInit()
{
    
    int temp;
    I2C1BRG = 0x4E;
    I2C1CONbits.I2CEN = 0;
    I2C1CONbits.DISSLW = 1;  // Slew rate disabled for standard mode (100KHz)
    I2C1CONbits.I2CEN = 1;
    temp = I2C1RCV;
}
void I2C_Clock()
{
    __delay_us(1);
    SCL = 1;
    __delay_us(1);
    SCL = 0;
}

void I2C_start()
{
    I2C1CONbits.ACKDT = 0; // Reset any previous ACK
    I2C1CONbits.SEN = 1; //send start bit 
    __delay_ms(5);
}

void I2C_stop()
{
    I2C1CONbits.PEN = 1;			// Send stop bit...commented
     while(!I2C1STATbits.S);
    I2C1STATbits.P = 0; // clear stop bit 
     
    I2C1CONbits.PEN = 1;	         
    
}

int I2C_write(unsigned char dat)
{
    I2C1TRN = dat;
     while(!I2C1STATbits.TRSTAT);
     if(I2C1STATbits.ACKSTAT)
     {
         I2C_stop();
         return 0;
     }
    
    return I2C1STATbits.ACKSTAT;
    
}

unsigned char I2C_Read(void)
{
    __delay_ms(5);
    I2C1CONbits.RCEN = 1;
    while(I2C1STATbits.RBF); 
     __delay_ms(5);
    I2C1CONbits.RCEN = 0;
    return I2C1RCV; 
}

void I2C_ACK()
{
    I2C1CONbits.ACKDT = 0;			// 0 means ACK
	I2C1CONbits.ACKEN = 1;	
    while(I2C1CONbits.ACKEN);
}

void I2C_NoACK()
{   
    I2C1CONbits.ACKDT = 1;			// 1 means NACK
	I2C1CONbits.ACKEN = 1;	
    while(I2C1CONbits.ACKEN);
}
/*DS1307 initialization */

void DS1307_init()
{
    I2C_start(); // start I2C
    
    DS1307_Write(DS13070);  // connect to DS1307 by sending its ID with I2C
    DS1307_Write(0x07);     // Select the control register of DS1307 to configure it with PIC 
    
    DS1307_Write(0x00);     // write 0x00 to control register to disable SQW/OUT   
    I2C_stop();  // stop I2C communication 
    __delay_ms(1);
}

void DS1307_Write(unsigned char dat)
{
    I2C_write(dat);
    I2C_Clock();
    __delay_ms(10);
}

unsigned char DS1307_Read()
{
    unsigned char dat;
    dat = I2C_Read();
    return (dat);
}
unsigned char set_DS1307(unsigned char data, unsigned char var)
{
    unsigned char temp;
    I2C_start();
    I2C_Clock(); 
    DS1307_Write(DS13070);
    DS1307_Write(data);
    temp = BSP_RTCC_DecToBCD(var);
    DS1307_Write(temp);
    I2C_stop();
    
}

void setRTCTime(unsigned char year, unsigned char month, unsigned char date, unsigned char weekday, unsigned char hour, unsigned char minute, unsigned char second)
{ 
   
    set_DS1307(0x06,year);
     __delay_ms(1);
    set_DS1307(0x05,month);
     __delay_ms(1);
    set_DS1307(0x04,date);
     __delay_ms(1);
    set_DS1307(0x03,weekday);
     __delay_ms(1);
    set_DS1307(0x02,hour);
     __delay_ms(1);
    set_DS1307(0x01,minute);
     __delay_ms(1);
    set_DS1307(0x06,year);
    
    
}

unsigned char read_DS1307(unsigned char add)
{
    unsigned char dd;
    __delay_ms(1);  
    I2C_start();
    I2C_Clock(); 
    DS1307_Write(DS13070);   
    DS1307_Write(add);
    I2C_stop();
    __delay_ms(1);
    I2C_start();
    I2C_Clock(); 
    DS1307_Write(DS13071);
    dd = DS1307_Read();
    I2C_NoACK();
    I2C_stop();
    return dd;
}

RTCTime getRTCTime()
{
    RTCTime r;
    unsigned char dd;
    unsigned char buffer[5];
    dd = read_DS1307(0x00);
    r.second = bcd2bin(dd);
    dd = read_DS1307(0x01);
    r.minute = bcd2bin(dd);
    dd = read_DS1307(0x02);
    r.hour = bcd2bin(dd);
    dd = read_DS1307(0x03);
    r.weekday = bcd2bin(dd);
    dd = read_DS1307(0x04);
    r.date = bcd2bin(dd);
    dd = read_DS1307(0x05);
    r.month = bcd2bin(dd);
    dd = read_DS1307(0x06);
    r.year = bcd2bin(dd);
    return r;      
}





