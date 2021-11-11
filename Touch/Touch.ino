#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <stdint.h>
#include "TouchScreen.h"

// == for touch screen system ==
#define YP A1
#define XM A2
#define YM 7
#define XP 6

#define minpressure 10
#define maxpressure 1000
TouchScreen ts = TouchScreen(XP,YP,XM,YM, 300);

MCUFRIEND_kbv tft;

// ============ warna LCD ===============
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */

#define Softyellow      tft.color565(237,255,159)
#define mint            tft.color565(29, 119, 29)

int currentdisplay = 0; //menu

//variable untuk proses refill dan inject
#define pul 3 //pulse
#define dir 2 //direction
#define rev 3200 //steps per revolution

float f,r,i;
unsigned long ml;
unsigned long sec;
unsigned long t;

//flesibilitas syringe
int UkuranIVbag = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("START");

  uint16_t ID = tft.readID();
  ID = 0x9486;

  tft.begin(ID);
  Serial.println(ID,HEX);
  tft.setRotation(3);
  tft.fillScreen(Orange);

  //=== set gambar/posisi(kiri, atas, panjang, tebal, ....., warna) =====
  tft.drawRoundRect(57,67,207,108,60,White);
  //=== set lingkaran (titik tengah dari : kiri,atas,diameter,warna) ===
  tft.drawCircle(110,106,21,White);
  tft.drawCircle(210,106,21,White);
  tft.fillCircle(110,106,18,Red);
  tft.fillCircle(210,106,18,Red);
  //draw FastVLine (vertikat), FastHLine(horizontal), Line(bukan H & V)
  //=== set line (kiri, atas, panjang, warna)/untuk Line (kiri,atas,sudut,warna) ===
  tft.drawFastHLine(110,145,100,White);
  delay(1000);

  tft.drawFastHLine(110,106,21,White);
  tft.drawFastHLine(210,106,21,White);
  tft.drawCircle(110,106,21,Orange);
  tft.drawCircle(210,106,21,Orange);
  tft.fillCircle(110,106,18,Orange);
  tft.fillCircle(210,106,18,Orange);
  delay (200);

  tft.drawFastHLine(110,106,21,Orange);
  tft.drawFastHLine(210,106,21,Orange);
  tft.drawCircle(110,106,21,White);
  tft.drawCircle(210,106,21,White);
  tft.fillCircle(110,106,18,Red);
  tft.fillCircle(210,106,18,Red);
  delay (500);

  
  tft.drawFastHLine(110,106,21,White);
  tft.drawFastHLine(210,106,21,White);
  tft.drawCircle(110,106,21,Orange);
  tft.drawCircle(210,106,21,Orange);
  tft.fillCircle(110,106,18,Orange);
  tft.fillCircle(210,106,18,Orange);
  delay (200);

  tft.drawFastHLine(110,106,21,Orange);
  tft.drawFastHLine(210,106,21,Orange);
  tft.drawCircle(110,106,21,White);
  tft.drawCircle(210,106,21,White);
  tft.fillCircle(110,106,18,Red);
  tft.fillCircle(210,106,18,Red);
  delay (1000);

  
  tft.fillScreen(Softyellow);
  tft.fillRect(1,1,318,40,mint);
  
  c_display(currentdisplay);
}

void loop() {
  // ==tampilan atas ==
  //tft.fillRect(1,1,318,40,mint);
  
    // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > minpressure && p.z < maxpressure) {
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);

     if(currentdisplay == 0){
      if((p.x >= 210) && (p.x < 630) && (p.y > 470) && (p.y < 570)){
        resets();

        tft_print("THIS IS Loading-Dose MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      }
      
      if((p.x >= 210) && (p.x < 630) && (p.y > 620) && (p.y < 730)){
        resets();

        tft_print("THIS IS Ramp Up/Down MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      }
      if((p.x >= 210) && (p.x < 630) && (p.y > 760) && (p.y < 880)){
        resets();

        tft_print("THIS IS Body Weight MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      }    
        
      if((p.x >= 650) && (p.x < 930) && (p.y > 470) && (p.y < 570)){
        resets();

        tft_print("THIS IS ml/h MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      } 
      if((p.x >= 650) && (p.x < 930) && (p.y > 620) && (p.y < 730)){
        resets();

        tft_print("THIS IS Drip MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      }   
      if((p.x >= 650) && (p.x < 930) && (p.y > 760) && (p.y < 880)){
        resets();

        tft_print("THIS IS Sequential MODE", 2,10,102,mint,Softyellow);
        tft.fillRoundRect(192,45,140,33,10,mint);
        tft_print("B A C K", 2,205,55,White,mint);
        currentdisplay = 1;
      }    
     }
     if(currentdisplay == 1){
      if((p.x >= 660) && (p.x < 930) && (p.y > 330) && (p.y < 420)){
        resets();
        c_display(0);
        currentdisplay =0;
      }
     }
  delay(500);
  }

  
}

void menu (){
  tft_bagsize();
  tft_print("SELECT MODE", 3, 5,10,White,mint);
  // == opsi mode ==
  tft.drawRoundRect(8,90,180,40,30,mint);
  tft_print("Loading-Dose", 2,30,105,mint,Softyellow);
  
  tft.drawRoundRect(8,139,180,40,30,mint);
  tft_print("Ramp Up/Down", 2,30,154,mint,Softyellow);
  
  tft.drawRoundRect(8,187,180,40,30,mint);
  tft_print("Body-Weight", 2,30,202,mint,Softyellow);
  
  tft.drawRoundRect(192,90,120,40,30,mint);
  tft_print("ml/h", 2,204,105,mint,Softyellow);
  
  tft.drawRoundRect(192,139,120,40,30,mint);
  tft_print("Drip", 2,204,154,mint,Softyellow);
  
  tft.drawRoundRect(192,187,120,40,30,mint);
  tft_print("Sequence", 2,204,202,mint,Softyellow);

}

void resets(){
  tft.reset();
  uint16_t ID = tft.readID();
  ID = 0x9486;
  
  tft.begin(ID);
  Serial.println(ID,HEX);
  tft.setRotation(3);
  tft.fillScreen(Softyellow);
  tft.fillRect(1,1,318,40,mint);

  tft_bagsize();

}

void inputsettings(int inp){
  switch(inp){
    //Loading Dose
    case 0 :
    break;

    //Ramp Up/Down
    case 1 :
    break;

    //Body Weight
    case 2 :
    break;

    //ml/h
    case 3 :
    break;

    //TIVA
    case 4 :
    break;

    //Sequence
    case 5 :
    break;
  }
}

void tft_print(String nama,int ukuran, int x, int y, int warna, int bg){
  tft.setTextSize(ukuran);
  tft.setTextColor(warna, bg);
  tft.setCursor(x, y);
  tft.print(nama);  
} 

void tft_bagsize(){
  tft.setTextSize(2);
  tft.setTextColor(White,mint);
  tft.setCursor(240,20);
  tft.print(UkuranIVbag);
  Serial.print(UkuranIVbag);
  Serial.println("ml");
  tft.setCursor(270, 20);
  tft.print("ml");
}

void c_display (int id_hal){
  switch(id_hal){
    case 0 :
    menu();
    break;
  }
}
