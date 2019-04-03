#include <regx51.h>
#define scandport P1
#define DBPORT P2
#define  T     11059200/12    //基本週期時間T=Fosc/12
#define  t      T/2           //半週期時間
#define  RE3     65536-t/587

sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^2;
char n[8] = {"15:59:50"};
char code number[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char code Alarm[] = {"16:00:05"};
char x50ms = 40;
char time[] = {15, 59, 50};
int input_keypad = 0, index = 7;
bit statement = 0, clock = 0;

void delay1ms(int count){
  int i, j;
  for (i = 0; i < 194; i++) {
    for (j = 0; j < count; j++) {
      ;
    }
  }
}
void delay() {
  int i;
  for (i = 0; i < 800; i++) {
    ;
  }
}
char keypad(void){
  char key=10;//assaum that 0xff is the output of keypad when not input_keypad
  P0=0xef;
  if(P0_0==0)  key='c';
  if(P0_1==0)  key='8';
  if(P0_2==0)  key='4';
  if(P0_3==0)  key='0';

  P0=0xdf;
  if(P0_0==0)  key='d';
  if(P0_1==0)  key='u';
  if(P0_2==0)  key='5';
  if(P0_3==0)  key='1';

  P0=0xbf;
  if(P0_0==0)  key='l';
  if(P0_1==0)  key='a';
  if(P0_2==0)  key='r';
  if(P0_3==0)  key='2';

  P0=0x7f;
  if(P0_0==0)  key='f';
  if(P0_1==0)  key='d';
  if(P0_2==0)  key='s';
  if(P0_3==0)  key='p';


  return key;
}
void LCD_CMD(char cmd) {
  char i;
  DBPORT = cmd;
  RS = 0;RW = 0;E = 1;
  for (i = 0; i < 40; i++) {
    ;
  }
  RS = 0;RW = 0;E = 0;
}
void LCD_DATA(char str) {
  char i;
  DBPORT = str;
  RS = 1;RW = 0;E = 1;
  for (i = 0; i < 40; i++) {
    ;
  }
  RS = 1;RW = 0;E = 0;
}
void DISP_str(char add, char *str) {
  LCD_CMD(add);
  while (*str != 0) {
    LCD_DATA(*str++);
  }
}
void split(void){
  n[0] = number[time[0]/10];
  n[1] = number[time[0]%10];
  n[3] = number[time[1]/10];
  n[4] = number[time[1]%10];
  n[6] = number[time[2]/10];
  n[7] = number[time[2]%10];
}
void T0_int(void) interrupt 1{
  x50ms--;
  if (x50ms == 0) {
    x50ms = 40;
    time[2]++;
    if (time[2] >= 60) {
      time[1]++;
      time[2] = 0;
      if (time[1] >= 60) {
        time[0]++;
        time[1] = 0;
        if (time[0] >= 24) {
          time[0] = 0;
        }
      }
    }
    split();
  }
  TL0 = (65536-50000)/256;
  TH0 = (65536-50000)%256;
}
void T1_int(void) interrupt 3{
  TL1 = RE3;
  TH1 = RE3>>8;
  P2_0 = !P2_0;
}

void main(){
  char temp[] = {"        "};
  char i = 0;
  delay1ms(5);
  LCD_CMD(0x3f);
  LCD_CMD(0xe);
  LCD_CMD(0x1);
  delay1ms(2);
  IE = 0x8a;
  TMOD = 0x11;
  TL0 = (65536-50000)/256;
  TH0 = (65536-50000)%256;
  TR0 = 1;
  TL1 = RE3;
  TH1 = RE3>>8;
  while (1) {
    DISP_str(0x83, n);
    if (keypad() != 10) {
      input_keypad = keypad();
      delay1ms(30);
    }
    if (input_keypad == 'p') {
      TR0 = 0;
    }
    if (TR0 == 0) {
      if (input_keypad == 'l') {
        index--;
        if (index == 2 || index == 5) {
          index--;
        }
        if (index < 0) {
          index = 7;
        }
      }
      else if (input_keypad == 'r') {
        index++;
        if (index == 2 || index == 5) {
          index++;
        }
        if (index > 8) {
          index = 0;
        }
      }
      else if (input_keypad == 'u') {
        if (index == 7) {
          time[2]++;
        }
        else if (index == 6) {
          time[2] += 10;
        }
        else if (index == 4) {
          time[1]++;
        }
        else if (index == 3){
          time[1] += 10;
        }
        else if (index == 1) {
          time[0]++;
        }
        else if (index == 0) {
          time[0] += 10;
        }
        if (time[0] >= 24) {
          time[0] = 0;
        }
        else if (time[1] >= 60) {
          time[1] = 0;
        }
        else if (time[2] >= 60) {
          time[2] = 0;
        }
      }
      else if (input_keypad == 'd'){
        if (index == 7) {
          time[2]--;
        }
        else if (index == 6) {
          time[2] -= 10;
        }
        else if (index == 4) {
          time[1]--;
        }
        else if (index == 3){
          time[1] -= 10;
        }
        else if (index == 1) {
          time[0]--;
        }
        else if (index == 0) {
          time[0] -= 10;
        }
        if (time[0] < 0) {
          time[0] = 23;
        }
        else if (time[1] < 0) {
          time[1] = 59;
        }
        else if (time[2] < 0) {
          time[2] = 59;
        }
      }
      else if (input_keypad == 's'){
        TR0 = 1;
      }
      split();
    }
    if (input_keypad == 'a') {
      DISP_str(0xc3, Alarm);
      statement = 1;
    }
    if (statement == 1 && time[0] == 16 && time[1] == 0 && time[2] == 5) {
      clock = 1;
    }
    if (clock == 1) {
      TR1 = 1;
      while (i < 10000) {
        DISP_str(0x83, n);
        delay1ms(2);
        i++;
        if (keypad() == 's') {
          statement = 0;
          clock = 0;
          TR1 = 0;
          break;
        }
      }
      i = 0;
    }
    input_keypad = 0;
  }
}
