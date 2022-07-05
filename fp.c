#define F_CPU 16000000UL
#include <mega32.h>
#include <stdlib.h>
#include <stdio.h>
#include <delay.h>
#include <alcd.h>

#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}

int i=0,j=0,k=0;
char nyala = 0;   // 0 mati, 1 hidup

char aP[] = {
  1,2,4,8,16,32,64,128
};

char aN[][8] = {
    {0x3c,0x7e,0xdc,0xf8,0xf8,0xfc,0x7e,0x3c}, // pacman 2
    {0x3c,0x7e,0xdf,0xff,0xf0,0xff,0x7e,0x3c}, // pacman 1
    {0x00,0x00,0x00,0x18,0x24,0x42,0xff,0x18}, // arrow 1
    {0x00,0x00,0x18,0x24,0x42,0xff,0x18,0x18}, // arrow 2
    {0x18,0x24,0x42,0xff,0x18,0x18,0x18,0x18}  // arrow 3
};

void main(){
// Declare your local variables here
char dataSuhu[20], nilai;

ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

DDRA.1 = 1;
DDRA.2 = 0;
DDRA.3 = 1;
DDRA.4 = 1;
DDRA.5 = 1;

// init LED
DDRC = 0XFF;
DDRB = 0XFF;

lcd_init(16);

while (1){
    // suhuisasi
    nilai = read_adc(0)/2.05;
    
    sprintf(dataSuhu,"%d",nilai);
    lcd_gotoxy(0,0);
    lcd_puts("Suhu :");   
    lcd_gotoxy(8,0);
    lcd_puts(dataSuhu);

    // display LED
    for(k=0; k<520; k++){
        if(nyala == 0) {
            continue;
        }
        
        PORTC = aP[i];
        PORTB = ~aN[j][i];
        delay_ms(2);
        i++;
        if(i>7){
            i=0;
        }
    }
    
    j++;
    if(j>4){
        j=0;
        i=0;
    }
       
    // suhuisasi if
    if (nilai >= 38) {
         lcd_gotoxy(0,2);
         lcd_puts("Puanas");
      } else if (nilai >= 36)  {
         lcd_gotoxy(0,2);
         lcd_puts("Mendung");
      }  else {
         lcd_gotoxy(0,2);
         lcd_puts("Mau hujan");
      }
    
    if(PINA.2==1){
        nyala = 1;
        PORTA.3 = 1;
        PORTA.4 = 1;
        PORTA.5 = 1;
        
        PORTA.1=1;
        delay_ms(50);
    }else if(PINA.2==0){
        nyala = 0; 
        PORTA.3 = 0;
        PORTA.4 = 0;
        PORTA.5 = 0;
        
        PORTA.1=0;
        delay_ms(50);
    }

    
}


}