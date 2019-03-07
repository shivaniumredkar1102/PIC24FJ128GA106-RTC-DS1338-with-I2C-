
// PIC24FJ128GA106 Configuration Bit Settings

// 'C' source line config statements

// CONFIG3
#pragma config WPFP = WPFP511           // Write Protection Flash Page Segment Boundary (Highest Page (same as page 85))
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable bit (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Configuration Word Code Page Protection Select bit (Last page(at the top of program memory) and Flash configuration words are not protected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select bit (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT oscillator mode selected)
#pragma config IOL1WAY = ON             // IOLOCK One-Way Set Enable bit (Write RP Registers Once)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSCO functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-speed start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Standard Watchdog Timer is enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define FCY 8000000UL
#include <xc.h>
#include <p24FJ128GA106.h>
#include <string.h>
#include <stdio.h>
#include <libpic30.h>


#define FSCK 100
#define I2C_BRG ((FCY/FSCK) - 1)
#define DS13070 0xD0
#define DS13071 0xD1

#define RS_PIN LATEbits.LATE2 // RD0 --> RS
#define E_PIN LATEbits.LATE1  // RD1 --> E 
#define RW_PIN LATEbits.LATE3 //RD5 --> RW
#define D0_PIN LATEbits.LATE0 
#define D1_PIN LATDbits.LATD6
#define D2_PIN LATDbits.LATD7
#define D3_PIN LATDbits.LATD8
#define D4_PIN LATEbits.LATE7 // RD2 --> D4
#define D5_PIN LATEbits.LATE6 // RD3 --> D5
#define D6_PIN LATEbits.LATE5 // RD4 --> D6
#define D7_PIN LATEbits.LATE4 // RD5 --> D7

#define RS_TRIS TRISEbits.TRISE2
#define E_TRIS TRISEbits.TRISE1
#define RW_TRIS TRISEbits.TRISE3
#define D0_TRIS TRISEbits.TRISE0
#define D1_TRIS TRISDbits.TRISD6
#define D2_TRIS TRISDbits.TRISD7
#define D3_TRIS TRISDbits.TRISD8
#define D4_TRIS TRISEbits.TRISE7
#define D5_TRIS TRISEbits.TRISE6
#define D6_TRIS TRISEbits.TRISE5
#define D7_TRIS TRISEbits.TRISE4


#define SDA _RG3
#define SCL _RG2
#define SDA_TRIS _TRISG3
#define SCL_TRIS _TRISG2

#include "lcd.h"
#include "DS1307.h"
RTCTime result;
 unsigned char year;
   unsigned char month;
   unsigned char day;
   unsigned char weekday;
   unsigned char hour;
   unsigned char minutes;
   unsigned char seconds;
   

void main()
{
    unsigned char *dd;
    char buffer[10];
    SDA_TRIS = 1;
    SCL_TRIS = 1;
    RS_TRIS = 0;
    E_TRIS=0;
    RW_TRIS = 0;
    D0_TRIS = 0;
    D1_TRIS = 0;
    D2_TRIS = 0;
    D3_TRIS = 0;
    D4_TRIS=0;
    D5_TRIS=0;
    D6_TRIS=0;
    D7_TRIS=0;
    
    E_PIN = 0;
    RS_PIN = 0;
    RW_PIN = 0;
    lcd_init();
    __delay_ms(100);
    lcd_cmd();
    __delay_ms(10);
    lcd_print("Hello_world :)");
    __delay_ms(1000); 
    I2CInit();
    DS1307_init();
    setRTCTime(19,3,7,3,18,28,49);
    __delay_ms(100);
    while(1)
    {
        result =  getRTCTime();
        lcd_clr();
        __delay_ms(10);
        sprintf(buffer,"%u",result.date);
        lcd_print(buffer);
        sprintf(buffer,"%u",result.month);
        lcd_print("/");
        lcd_print(buffer);
        sprintf(buffer,"%u",result.year);
        lcd_print("/");
        lcd_print(buffer);
        lcd_nextline();
        sprintf(buffer,"%u",result.hour);
        lcd_print(buffer);
        sprintf(buffer,"%u",result.minute);
        lcd_print(":");
        lcd_print(buffer);
        sprintf(buffer,"%u",result.second);
        lcd_print(":");
        lcd_print(buffer);
        __delay_ms(10);        
    
    }
       
}


