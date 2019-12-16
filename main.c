#include <16F887.h>
#device  ADC = 10 *=16
#FUSES HS NOWDT NOBROWNOUT NOLVP
#use delay(CRYSTAL=20M)
#use rs232(UART1)
 
// Define SPI Pins                                                             
#define lcd_sclk  PIN_C3 
#define lcd_sda   PIN_C5             
#define lcd_dc    PIN_D3                     
#define lcd_cs    PIN_A5                                                                            
#define lcd_res   PIN_D2  

// Define orther source files
#include "5110lib\3310(modified).c" 
//#include "C:\Users\NGUYEN DUY LINH\Downloads\Goc.c"

int buffer[16];

char state = 0;
char k;
char l;
char count = 0;
int hr1 , hr2, min1, min2, temp1, temp2, date1, date2, hum1, hum2;
int incode;
char cnt;
boolean ready = 0;

int index = 0;

void init()
{
    setup_adc_ports(NO_ANALOGS);
    setup_adc (ADC_OFF) ;
    setup_spi (FALSE);
    setup_uart(115200);
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_EXT);
    ext_int_edge(H_TO_L);
    setup_timer_1( T1_INTERNAL | T1_DIV_BY_2);
    Set_timer0(0);
    lcd_init ();
}
//!
//!void time2digit(char hr, char min, char sec, char *hr1, char *hr2, char *min1, char *min2, char *sec1, char *sec2){
//!    *hr1 = hr/10 + 0x30;
//!    *hr2 = hr%10 + 0x30;
//!    *min1 = min/10 + 0x30;
//!    *min2 = min%10 + 0x30;
//!    *sec1 = sec/10 + 0x30;
//!    *sec2 = sec%10+ 0x30;
//!}
//!
//!void time_display(char hr1, char hr2, char min1, char min2, char sec1, char sec2, char x, char y){
//!    if (x>24 || y>4) return;
//!    lcd_print_digit(hr1 -0x30, x, y);
//!    lcd_print_digit(hr2-0x30, x + 12, y);
//!    lcd_print_digit(min1-0x30, x + 24, y);
//!    lcd_print_digit(min2-0x30, x + 36, y);
//!    lcd_print_digit(sec1-0x30, x + 48, y);
//!    lcd_print_digit(sec2-0x30, x + 60, y);
//!}


void buzz(){
   output_high(PIN_D1);
   output_high(PIN_A4);
   delay_ms(50);
   output_low(PIN_A4);
   output_low(PIN_D1);
}


#int_rda
void uart_receive(){
   incode = getc();
//!   sprintf(str, "%d\n", incode);
//!   printf(str);
   if (incode == '#')
   {  
      ready=true;
      return;
   }
   if (ready){
//!        if (index<8){
//!         buffer[index] = incode;
//!        }
//!        else{ index =0; ready = false;
//!        }
//!   }
      cnt = count;
      switch(index){
      
      case 0: 
         hr1 = incode;
         index++;
         break;
      case 1:
         hr2 = incode;
         index++;
         break;
      case 2: 
         min1 = incode;
         index++;
         break;
      case 3: 
         min2 = incode;
         index++;
         break;
      case 4: 
         date1 = incode;
         index++;
         break;
      case 5: 
         date2 = incode;
         index++;
         break;
      case 6: 
         temp1 = incode;
         index++;
         break;
      case 7: 
         temp2 = incode;
         index++;
         break;
      case 8: 
         hum1 = incode;
         index++;
         break;
      case 9 :
         hum2 = incode;
         index=0;
         ready = false;
         break;
      }
         if (cnt!= count) {
            lcd_clear();
            cnt = count;
         }
   }
}


#int_ext
void encoder(){
   if (!state){
       state = 1;
       l = input(PIN_D4);
       k = input(PIN_D5);
       Set_timer1(0);
       if (k)
       {
         count++;
       }
//!       else 
//!       {
//!         count--;
//!       }
       if (count > 3) count = 0;
//!       if (count < 0) count = 1; 
    }
    else {
         if (get_timer1() > 6250){
         state = 0;
         Set_timer1(0);
         }
    }
}


void main()
{
    init(); 
    char sec;
    char blink;
    char x= 4, y=2;
    buzz();
    WHILE (true)                                        
    { 
         if (cnt!= count) {
            lcd_clear();
            cnt = count;
         }
         if (count == 0){
            if (cnt!= count) {
            lcd_clear();
            cnt = count;
         }
             lcd_gotoxy(0,0);
            lcd_putc("16/12/2019"); 
            lcd_print_digit(hr1 -0x30, x, y);
            lcd_print_digit(hr2-0x30, x + 12, y);
            lcd_print_digit(11, x + 24, y);
            lcd_print_digit(min1-0x30, x + 36, y); 
            lcd_print_digit(min2-0x30, x + 48, y);
//!            lcd_putc(tempd2);
            cnt = count;
            blink++;
            //delay_ms(300);
         }
         if (count ==  1)
         {
            //lcd_clear();
            if (cnt!= count) {
               lcd_clear();
               cnt = count;
            }
            lcd_gotoxy(4,1);
            lcd_putc("Nhiet do: ");
            lcd_gotoxy(5,2);
            lcd_putc(temp1);
            lcd_putc(temp2);
            lcd_putc(" ");
            lcd_data(0x03);
            lcd_data(0x03);
            lcd_data(0x00);
            lcd_putc("C");
            lcd_gotoxy(4,3);
            lcd_putc("Do am: ");
            lcd_gotoxy(5,4);
            lcd_putc(hum1);
            lcd_putc(hum2);
            lcd_putc(" %");
            //delay_ms(200);
            cnt= count;
         }
         if (count == 2)
         {
         if (cnt!= count) {
            lcd_clear();
            cnt = count;
         }
            //lcd_clear();
            //lcd_putc("Man hinh 3");
            lcd_gotoxy(0,0);
           // int16 i; 
           for (int16 i=0; i<=504;i++) lcd_data(sunny[i]);
//!           lcd_gotoxy(1,0);
//!           lcd_putc("SUNNY");
            //delay_ms(50);
           //for(i=0; i<504; i++) 
          // cnt = count;
         }
         if (count == 3)
         {
         
         if (cnt!= count) {
            lcd_clear();
            cnt = count;
         }
         
         char x= 4, y=2;
         int a, b, c, d;
         a = 1; b = 2; c = 1; d = 4;
            //lcd_clear();
            lcd_gotoxy(1,0);
            lcd_putc("Arlam at:"); 
            lcd_print_digit(a, x, y);
            lcd_print_digit(b, x + 12, y);
            lcd_print_digit(11, x +24, y);
            lcd_print_digit(c, x + 36,y);
            lcd_print_digit(d, x +48, y);
            if(((hr1-0x30==a) && (hr2 - 0x30 == b)) && ((min1 - 0x30 == c) && (min2-0x30 ==d))){
            buzz();
            }
            cnt = count;
            //delay_ms(500);
         }
        // delay_ms(30);
      } 
 } 
