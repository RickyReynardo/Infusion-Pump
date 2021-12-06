#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <stdint.h>
#include "TouchScreen.h"

// == TFT Touch Screen System ==
//TFT 3.5
#define YP A2
#define XM A3
#define YM 8
#define XP 9
/*
//TFT 2.4
#define YP A1
#define XM A2
#define YM 7
#define XP 6
*/
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define minpressure 250
#define maxpressure 1000
TouchScreen ts = TouchScreen(XP,YP,XM,YM,300);
int X,Y;

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
// ========== warna costum ===========
#define LightGreen      tft.color565(237,255,159)

// ======== Menyatakan keadaan =========
int currentdis = 0; //keadaan layar
int displaylama = 0; //untuk membantu menstabilkan layar apabila keadaan layar sama seperti sebelumnya
/*
 * 0 : display Menu
 * 1 : display Bolus //kalau perlu
 * 2 : display Mode A
 * 3 : display Mode B
 * 4 : display Mode C
 * 5 : display Mode lainnya
 * 6 : display Input Angka
 * 7 : display Run Mode A
 * 8 : display Ganti angka
*/
int dis = 0; //kondisi yang sedang terjadi
/*
 * 0 : nothing happen
 * 1 : keadaan Mode Bolus
 * 2 : keadaam Mode A terpilih
 * 3 : keadaan Mode B terpilih
 * 4 : keadaan Mode C terpilih
 * 5 : keadaan Mode lainnya terpilih
 * 21 : keadaan Variable 1 dari Mode A terpilih
 * 22 : keadaan Variable 2 dari Mode A terpilih
 * 23 : keadaan Variable 3 dari Mode A terpilih
 * 02 : keadaan run Mode A
*/

// ====== variable untuk menjalankan syringe ======
int VTBI  ;
int times ;
int rate ;

//untuk penentuan input angka
int angka = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  //pinMode(...,OUTPUT); //variable output untuk kontrol run sistem
  //pinMode(....,OUTPUT); // variable output untuk kontrol run sistem

  tft.begin(0x9486);
  tft.setRotation(3);
  MenuTetap();
  displays(0);

}

void loop() {
  TSPoint p = waitTouch();
  Y = p.y; X = p.x;
  Serial.print("X : "); Serial.print(X);
  Serial.print("\t Y : "); Serial.println(Y);

  //untuk sistem touchscreen setiap keadaan
  if(dis == 0){
    //display dari menu
    currentdis = 0;
    //sistem touch ditampilan menu
    menu();
  }
  else if(dis != 0){
    //Bolus
    if (dis == 1){
      backtouch();
    }
    //Mode A
    else if(dis == 2){
      ATouch();
      backtouch();
    }
    //Mode B
    else if(dis == 3){
      backtouch();
    }
    //Mode C
    else if(dis == 4){
      backtouch();
    }
    //lainnya
    else if(dis == 5){
      backtouch();
    }
    else if(dis == 21 | dis == 22 | dis == 23){
      InputTouch();
      backtouch();
    }
    
  }

  if(currentdis != displaylama){
    displaylama = currentdis;
    displays(currentdis);
  }
  //menampilkan angka input
  else if(currentdis == 8){
    displays(currentdis);
  }
  delay(200);
  
}

TSPoint waitTouch() {
  TSPoint p;
  do {
    p = ts.getPoint(); 
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    
    // menstabilkan layar
    // apabila ukuran yang terdeteksi sama dengan ukuran sebelumnya, sistem baru akan mengubah ukuran pada display TFT
  } while((p.z < minpressure )|| (p.z > maxpressure));
  return p;
}

void displays(int halaman){
  switch(halaman){
    //menu
    case 0 :
    tampilanmenu();
    break;

    //bolus kalau ada
    case 1 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    tft_print("BOLUS MODE", 3, 5,10,White,DarkGreen);
    tft_print("THIS IS BOLUS MODE", 2,110,136,DarkGreen,LightGreen);
    break;

    //tampilan A, dis == 2
    case 2 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    RunButton();
    delay(100);
    tft_print("ml/h MODE", 3, 5,10,White,DarkGreen);

    //menampilkan menu dari A Mode beserta satuannya
    //ganti" aja
    tftDrawRoundRect("VTBI", 3,118,66,164,109,5,DarkGreen,LightGreen);
    tft_print("ml", 2,251,153,DarkGreen,LightGreen);
    tftDrawRoundRect("RATE", 3,298,66,164,109,5,DarkGreen,LightGreen);
    tft_print("ml/h", 2,408,153,DarkGreen,LightGreen);
    tftDrawRoundRect("TIME",3,118,190,164,109,5,DarkGreen,LightGreen);
    tft_print("hour", 2,226,277,DarkGreen,LightGreen);    
    
    //menampilkan nilai input dari setiap variable
    nilaiinput(VTBI,128,124);
    nilaiinput(rate,308,124);
    nilaiinput(times,128,248);
    break;

    //tampilan B, dis == 3
    case 3 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    tft_print("B MODE", 3, 5,10,White,DarkGreen);
    tft_print("THIS IS B MODE", 2,110,136,DarkGreen,LightGreen);
    break;

    //tampilan C, dis == 4
    case 4 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    tft_print("C MODE", 3, 5,10,White,DarkGreen);
    tft_print("THIS IS C MODE", 2,110,136,DarkGreen,LightGreen);
    
    break;

    //tampilan lainnya, dis == 5
    case 5 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    tft_print("ELSE MODE", 3, 5,10,White,DarkGreen);
    tft_print("THIS IS ELSE MODE", 2,110,136,DarkGreen,LightGreen);
    break;

    //tampilan input angka
    case 6 :
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    HomeButton();
    inputangka();
    break;
    
    //tampilan run proses
    case 7:
    tft.fillRect(0,0,298,46,DarkGreen);
    tft.fillRect(100,47,380,273,LightGreen);
    delay(100);
    tft_print("RUN", 3, 5,10,White,DarkGreen);
    tft_print("RUN PROSES", 2,110,136,DarkGreen,LightGreen);
    
    break;

    //Menampilkan input detectbutton
    case 8 :
    tft.fillRect(142,61,301,53,LightGreen);
    tft.setCursor(152,71);
    tft.setTextColor(DarkGreen);
    tft.setTextSize(3);
    tft.print(angka);

    tft.setCursor(370,95);
    tft.setTextSize(2);

    //VTBI (ml/h & weight)
    if(dis == 21){
      tft.print("ml"); 
    }
    //Rate (ml/h & weight)
    else if(dis == 22){
      tft.print("ml/h");
    }
    //Time (ml/h)
    else if(dis == 23){
      tft.print("sec");
    }
    break;
  }
}

// ------------ Settintg Tampilan ----------
void MenuTetap(){
  tft.fillScreen(LightGreen);
  tft.fillRect(0,0,480,46,DarkGreen);
  tft.fillRect(0,46,100,275,DarkGreen);
  //Home Button
  HomeButton();
  
  /* //kalau perlu bolus, buka pagar atau bisa ganti dengan koding run button (automatis destinasi run button juga berubah, sesuaiin dengan touchnya bolus)
  //Bolus Button
  tft.fillRoundRect(10,148,80,78,5,White);
  tft.setCursor(21,203); //tulisan
  tft.setTextSize(2);
  tft.setTextColor(DarkGreen);
  tft.print("BOLUS");
  */

  //Back Button
  tft.fillRoundRect(10,234,80,78,5,White);
  tft.fillRoundRect(41,263,38,11,5,DarkGreen);
  
  tft.fillTriangle(21,269,47,257,47,281,DarkGreen);
  tft.setCursor(28,290); //tulisan
  tft.setTextSize(2);
  tft.setTextColor(DarkGreen);
  tft.print("BACK");
}

void HomeButton(){
  //Home Button
  tft.fillRoundRect(10,60,80,78,5,White);
  tft.drawRoundRect(38,89,25,16,5,DarkGreen);
  tft.fillTriangle(50,74,67,92,33,92,DarkGreen);
  tft.setCursor(27,115); //tulisan
  tft.setTextSize(2);
  tft.setTextColor(DarkGreen);
  tft.print("HOME");
}

void RunButton(){
  //Run Button
  tft.fillRoundRect(10,60,80,78,5,White);
  tft.print("RUN");
}

// --- Tampilan untuk mode yang digunakan ---
void tampilanmenu(){ 
  tft.fillRect(0,0,298,46,DarkGreen);
  tft.fillRect(100,47,380,273,LightGreen);
  HomeButton();
  tft_print("SELECT MODE",3, 5,10,White,DarkGreen);
  
  // == opsi mode ==
  tftDrawRoundRect("ml/h", 2,120,70,160,70,10,DarkGreen,LightGreen);
  tftDrawRoundRect("B", 2,300,70,160,70,10,DarkGreen,LightGreen);
  tftDrawRoundRect("C", 2,120,155,160,70,10,DarkGreen,LightGreen);
  //kalau ga ada mode lainnya hapus aja yg dibawah trus gedein ukuran mode diatas
  tftDrawRoundRect("...", 2,300,155,160,70,10,DarkGreen,LightGreen);
  tftDrawRoundRect("...", 2,120,240,160,70,10,DarkGreen,LightGreen);
  tftDrawRoundRect("...", 2,300,240,160,70,10,DarkGreen,LightGreen);
}

void inputangka(){
    tft.drawRect(135,60,315,55,DarkGreen);
    //1
    tft.fillRect(135,129,55,55, DarkGreen);
    tft_print("1",3,157,149,White, DarkGreen);
    //2
    tft.fillRect(201,129,55,55, DarkGreen);
    tft_print("2",3,222,149,White, DarkGreen);
    //3
    tft.fillRect(266,129,55,55, DarkGreen);
    tft_print("3",3,287,149,White, DarkGreen);

    //4
    tft.fillRect(135,191,55,55, DarkGreen);
    tft_print("4",3,157,211,White, DarkGreen);
    //5
    tft.fillRect(201,191,55,55, DarkGreen);
    tft_print("5",3,222,211,White, DarkGreen);
    //6
    tft.fillRect(266,191,55,55, DarkGreen);
    tft_print("6",3,287,211,White, DarkGreen);

    //7
    tft.fillRect(135,253,55,55, DarkGreen);
    tft_print("7",3,157,273,White, DarkGreen);
    //8
    tft.fillRect(201,253,55,55, DarkGreen);
    tft_print("8",3,222,273,White, DarkGreen);
    //9
    tft.fillRect(266,253,55,55, DarkGreen);
    tft_print("9",3,287,273,White, DarkGreen);

    //0
    tft.fillRect(332,253,118,55, DarkGreen);
    tft_print("0",3,384,273,White, DarkGreen);

    //OK
    tft.fillRect(332,191,118,55, DarkGreen);
    tft_print("OK",3,384,211,White, DarkGreen);

    //Backspace
    tft.fillRect(332,129,118,55, DarkGreen);
    tft.drawFastHLine(350,155,88,White);
    tft.fillTriangle(344,156,363,145,363,167,White);
}

// ----------- TOUCH SISTEM -------------
void menu(){
  //Home
  if((X >= 290) && (X <= 490) && (Y >= 160) && (Y <= 290)){
    currentdis = 0;
    dis = 0;
  }
  /* //kalau perlu bolus, buka pagar  
  //Bolus
  if((X >= 500) && (X < 690) && (Y > 160) && (Y < 290)){
    currentdis = 1;
    dis = 1;
  }
  */
  //Loading Dose
  if((Y > 350) && (Y < 625)){
    //A
    if((X >= 310) && (X < 480)){
      currentdis = 2;
      dis = 2;
    }
    //C
    else if((X >= 525) && (X < 695)){
      currentdis = 4;
      dis = 4;        
    }
    //lainnnya 2
    else if((X >= 735) && (X < 900)){
      currentdis = 5;
      dis = 5;
    }
  }
  else if((Y > 665) && (Y < 930)){
    //B
    if((X >= 310) && (X < 480)){
      currentdis = 3;
      dis = 3;          
    }
    //lainnya 1
    else if((X >= 525) && (X < 695)){
      currentdis = 5;
      dis = 5;        
    }
    //lainnya 3
    else if((X >= 735) && (X < 900)){
      currentdis = 5;
      dis = 5;        
    }
  }
}

void ATouch(){
  if((X >= 315 ) && (X <=570)){
    //VTBI
    if((Y >= 350) && (Y <=630)){
      currentdis = 6;
      dis = 21;  
    }
    //RATE
    else if((Y >=664 ) && (Y <=934)){
      currentdis = 6;
      dis = 22;      
    }
  }
  //TIME
  else if((X >= 615 ) && (X <=870) && (Y >= 350) && (Y <=630)){
    currentdis = 6;
    dis = 23;
  }
  //Run ml/h mode
  else if((X >= 290) && (X <= 490) && (Y >= 160) && (Y <= 290)){
    runproses();
    dis = 02;
  }
}

void backtouch (){
  //Home
  if((X >= 290) && (X <= 490) && (Y >= 160) && (Y <= 290)){
    angka = 0;
    currentdis = 0;
    dis = 0;
  }
  
  /*
  //Bolus
  else if((X >= 500) && (X < 690) && (Y > 160) && (Y < 290)){
    currentdis = 1;
    dis = 1;
  } 
  */
  
  //Back
  else if((X >= 718) && (X < 900) && (Y > 160) && (Y < 290)){
    if(dis == 1 | dis == 2 | dis == 3 | dis == 4 | dis == 5){
      currentdis = 0;
      dis = 0;
    }
    else if (dis == 21 | dis == 22 | dis == 23){
      angka = 0;
      currentdis = 2;
      dis = 2;
    }
  }
}

//input angka
void InputTouch(){
  if((Y > 377) && (Y < 475)){ //tombol 1 | 4 | 7
    //1
    if((X > 460) && (X < 593)){
      currentdis = 8;
      Serial.println ("tombol 1"); 
      if(angka == 0){
        angka = 1;
      }
      else{
        angka = (angka*10) + 1;
      }     
    }
    //4
    else if((X > 617) && (X < 748)){
      currentdis = 8;
      Serial.println ("tombol 4"); 
      if(angka == 0){
        angka = 4;
      }
      else{
        angka = (angka*10) + 4;
      }
    }
    //7
    else if((X > 771) && (X < 898)){
      currentdis = 8;
      Serial.println ("tombol 7"); 
      if(angka == 0){
        angka = 7;
      }
      else{
        angka = (angka*10) + 7;
      }      
    }
  }
  else if((Y > 495)&&(Y < 590)){ //tombol 2 | 5 | 8
    //2
    if((X > 460) && (X < 593)){
      currentdis = 8;
      Serial.println ("tombol 2"); 
      if(angka == 0){
        angka = 2;
      }
      else{
        angka = (angka*10) + 2;
      }
    }
    //5
    else if((X > 617) && (X < 748)){
      currentdis = 8;
      Serial.println ("tombol 5"); 
      if(angka == 0){
        angka = 5;
      }
      else{
        angka = (angka*10) + 5;
      }
    }
    //8
    else if((X > 771) && (X < 898)){
      currentdis = 8;
      Serial.println ("tombol 8"); 
      if(angka == 0){
        angka = 8;
      }
      else{
        angka = (angka*10) + 8;
      }
    }
  }
  else if((Y > 610 ) && (Y < 703 )){ //tombol 3 | 6 | 9
    //3
    if((X > 460) && (X < 593)){
      currentdis = 8;
      Serial.println ("tombol 3"); 
      if(angka == 0){
        angka = 3;
      }
      else{
        angka = (angka*10) + 3;
      }
    }
    //6
    else if((X > 617) && (X < 748)){
      currentdis = 8;
      Serial.println ("tombol 6"); 
      if(angka == 0){
        angka = 6;
      }
      else{
        angka = (angka*10) + 6;
      }
    }
    //9
    else if((X > 789) && (X < 899)){
      currentdis = 8;
      Serial.println ("tombol 9"); 
      if(angka == 0){
        angka = 9;
      }
      else{
        angka = (angka*10) + 9;
      }
    }
  }
  else if((Y > 725) && (Y < 920)){ //Backspace | run | 0
    //Clear
    if((X > 460) && (X < 593)){
      currentdis = 8;
      Serial.println("Backspace");
      angka  = 0;
    }
    
    //run
    else if((X > 617) && (X < 748)){
      currentdis = 8;
      Serial.println("OK");
      angka = 0;
      currentdis = 2;
      dis = 2;
    }
    
    //0
    else if((X > 771) && (X < 898)){
      currentdis = 8;
      Serial.println ("tombol 0"); 
      if(angka == 0){
        angka = 0;
      }
      else{
        angka = (angka*10);
      }
    }
  }
  Serial.print("Nilai input : ");
  Serial.println(angka);

  kalkulasiauto();
}

// --- Sistem Lainnya ---
void kalkulasiauto(){
  //perubahan variable
  
  // variable 1 dari mode A, misal VTBI
  if(dis == 21){
    VTBI = angka;
    Serial.print("Nilai VTBI : "); Serial.println(VTBI);
    if(times == 0 && rate != 0){
      times = VTBI/rate;
    }
    else if(times !=0 && rate == 0){
      rate = VTBI/times;
    }
    //kesimpulan mode ml/h lebih mengutamakan rate dari pada time
    else if (times !=0 && rate !=0){
      rate = VTBI/times;
    }
    
    //kalau keadaan tidak 0, maka time tidak diubah
}

  // variable 2 dari mode A, misal rate
  else if(dis == 22){
    rate = angka;
    Serial.print("Nilai Rate : "); Serial.println(rate);
    times = VTBI/rate;
  }
  
  //variable 3 dari mode A, misal time
  else if(dis == 23){
    times = angka;
    Serial.print("Waktu : "); Serial.println(times);
    rate = VTBI/times;
  }
}

void runproses(){
  //masukin mekanisme program
  
  displays(7); //TAMPILAN RUN
  /*
   * program run mekanis
   * mari anggap jalan 5 detik
   */
  delay (5000);
  
  delay(100);
  displays(2); // kembali ke tampilan Menu Mode A
  dis = 2; //kembali ke keadaan Mode A
}

// --- Format Penulisan ---
void tft_print(String isi, int ukuran, int x, int y, int warna,int bg){
  tft.setCursor(x,y);
  tft.setTextColor(warna,bg);
  tft.setTextSize(ukuran);
  tft.print(isi);
}

//tampilan untuk hasil input angka yang diberikan
void nilaiinput(int isi,int x, int y){
  tft.setCursor(x,y);
  tft.setTextColor(Purple);
  tft.setTextSize(4);
  tft.print(isi);  
}

void tftDrawRoundRect(String isi,int ukuran, int x, int y,int lebar,int tinggi,int sudut, int warna,int bg ){
  tft.drawRoundRect(x,y,lebar,tinggi,sudut,warna);
  int a = x + 10;
  int b = y + 10;
  tft.setTextSize(ukuran);
  tft.setTextColor(warna, bg);
  tft.setCursor(a, b);
  tft.print(isi);  
}
