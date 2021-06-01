/* DAHİL EDİLEN KÜTÜPHANELER */
#include <LiquidCrystal.h> // LCD Kütüphanesi Tanımlandı
#include <DS3231.h>  // RTC Kütüphanesi Dahil Edildi
#include <Wire.h>   // Gerekli Kütüphaneler Tanımlandı
#include <SPI.h> // SPI Haberleşmesi Kütüphanesi

/* BUTTON CONNECTİON */
#define SET_BUTTON 14
#define UP_BUTTON 16
#define DOWN_BUTTON 17
#define CHANGE_BUTTON 15

/* BUTTON DEĞİŞKENLERİ */
int ButtonCounter=0;
int SetMode=0;
int ButtonState=0;

/* LCD EKRAN  AYAR DEĞİŞKENLERİ */
int DigitRow=0;
int DigitColumn=0;
int FakeRow=0;
int FakeColumn=0;
int SepetRow=0;
int SepetColumn=0;

byte karakter [8]=
{
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000  
};  // LCD Boş Karakteri

/* LED CONNECTION */
#define LED 13

/* LCD CONNECTİON */
#define RS 12
#define E  11
#define D4 10
#define D5 9
#define D6 8
#define D7 7

LiquidCrystal lcd(RS,E,D4,D5,D6,D7); // LCD NESNESİ OLUŞTURULDU

/* DS3231 CONNECTION */
#define SDA 18  
#define SCL 19

DS3231  rtc(SDA,SCL);  // RTC Nesnesi

Time t; // Gerçek Zaman Değerine Ulaşmak İçin Gerekli bir nesne tanımlaması

/* RTC Değerleri */
int Saat; 
int Dakika;
int Gun;
int Ay;
long int Yil;

int NumberDay=3;
/*
 Pazartesi=1;
 Salı=2;
 Çarşamba=3;
 Perşembe=4;
 Cuma=5;
 Cumartesi=6;
 Pazar=7;
 */
 
/* LCD Ekranında Düzgün Saat Bilgisi Basabilmek için gerekli değişkenler */
int SaatOnlar=0; 
int SaatBirler=0;
int DakikaOnlar=0;
int DakikaBirler=0;

int GunOnlar=0;
int GunBirler=0;
int AyOnlar=0;
int AyBirler=0;
int YilBinler=0;
int YilYuzler=0;
int YilOnlar=0;
int YilBirler=0;

/* FUNCTION BODY TANIMLAMALARI */
void CalculateValue(void); 
void ShowLCD(void);
void SetButtonRead(void);

void setup() 
{
  /* LCD BAŞLANGIÇ AYARLARI */
 lcd.begin(16,2);
 lcd.home();
 Serial.begin(9600);
 lcd.createChar(1,karakter);  // Oluşturulan Karaktere Numara Aktarılma Kodu

      /* I/O Settings */
 pinMode(LED,OUTPUT);
 pinMode(SET_BUTTON,INPUT);
 pinMode(UP_BUTTON,INPUT);
 pinMode(DOWN_BUTTON,INPUT);
 pinMode(CHANGE_BUTTON,INPUT);
 digitalWrite(LED,LOW);
 
       /* RTC Ayarları */
  rtc.begin(); // RTC Başlatıldı     
 //rtc.setDOW(NumberDay);               //  BU günü pazartesi olarak ayarladık
 //rtc.setTime(15, 55, 0);       //  saati 12:00:00 şeklinde ayarladık
 //rtc.setDate(27, 10, 2020);   // Tarihi 1/1/2014 olarak ayarladık

  /* LCD için Başlangıç Göstergesi */
  CalculateValue();
  ShowLCD();

}

void loop() 
{
    ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku

    while(SetMode==1) // Set Moduna Giriş.
{
  
  digitalWrite(LED,HIGH); // Set Modu Ledini Yak
  ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
  
   while(1)
   {

    if(SetMode==0) // Set Modundan Çık.
    {
      break;
    }

/***************************************************************************************/
// 0.Sütun 0.Satır
  while((DigitColumn==0) && (DigitRow==0))
  {   
     if(digitalRead(SET_BUTTON)==1) 
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
     
        if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=1;
      DigitRow=0;
      break;
    }

      /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
    /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(SaatOnlar);
    delay(200);

    if(digitalRead(UP_BUTTON)==1)
    {
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      SaatOnlar++;
      if(SaatOnlar>2)
      {
        SaatOnlar=2;
      }
      if(SaatBirler>3 && SaatOnlar>2)
      {
         SaatOnlar=2; 
      }

            if(SaatBirler>3 && SaatOnlar>1)
      {
         SaatOnlar=1; 
      }  
    }

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      SaatOnlar--;
      if(SaatOnlar<0)
      {
        SaatOnlar=0;
      }  
    }
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }

/***************************************************************************************/
// 1.Sütun O.Satır
    while(DigitColumn==1 && DigitRow==0)
  {   
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);  
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=3;
      DigitRow=0;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(SaatBirler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      SaatBirler++;
      if((SaatOnlar==1 ||SaatOnlar==0) && (SaatBirler>9))
      {
        SaatBirler=9;
      }
      else if((SaatOnlar==2) && (SaatBirler>3))
      {
        SaatBirler=3;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      SaatBirler--;
      if(SaatBirler<0)
      {
        SaatBirler=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }  

/***************************************************************************************/         
// 3.Sütun 0.Satır
      while(DigitColumn==3 && DigitRow==0)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=4;
      DigitRow=0;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(DakikaOnlar);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      DakikaOnlar++;
      if(DakikaOnlar>5)
      {
        DakikaOnlar=5;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      DakikaOnlar--;
      if(DakikaOnlar<0)
      {
        DakikaOnlar=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }

/***************************************************************************************/
// 4.Sütun 0.Satır
      while(DigitColumn==4 && DigitRow==0)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);   
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=7;
      DigitRow=0;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(DakikaBirler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      DakikaBirler++;

      if(DakikaBirler>9)
      {
        DakikaBirler=9;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      DakikaBirler--;
      if(DakikaBirler<0)
      {
        DakikaBirler=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 7.Sütun 0.Satır
      while(DigitColumn==7 && DigitRow==0)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);
 rtc.setDOW(NumberDay);  
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=3;
      DigitRow=1;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    for(int i=7; i<16; i++)
    {
    lcd.setCursor(i,0);
    lcd.write(byte(1));
    }
    delay(150);
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(rtc.getDOWStr(FORMAT_SHORT));
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      NumberDay++;
      rtc.setDOW(NumberDay);
      if(NumberDay>7)
      {
        NumberDay=7;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      NumberDay--;
      rtc.setDOW(NumberDay);
      if(NumberDay<1)
      {
        NumberDay=1;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 3.Sütun 1.Satır

      while(DigitColumn==3 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);   
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=4;
      DigitRow=1;
      break;
    }
          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(GunOnlar);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      GunOnlar++;
      if(GunOnlar>3)
      {
        GunOnlar=3;
      }
      if(GunBirler>1 && GunOnlar==3)
      {
        GunOnlar=2;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      GunOnlar--;
      if(GunOnlar<0)
      {
        GunOnlar=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 4.Sütun 1.Satır

        while(DigitColumn==4 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);   
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=6;
      DigitRow=1;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(GunBirler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      GunBirler++;
      if(GunBirler>9)
      {
        GunBirler=9;
      }
      else if(GunOnlar ==3 && GunBirler ==2)
      {
        GunBirler=1;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      GunBirler--;
      if(GunBirler<0)
      {
        GunBirler=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 6.Sütun 1.Satır
          while(DigitColumn==6 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=7;
      DigitRow=1;
      break;
    }
          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(AyOnlar);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      AyOnlar++;
      if(AyOnlar>1)
      {
        AyOnlar=1;
      }
    if(AyBirler>1 && AyOnlar>0)
    {
      AyOnlar=0;
    }
      
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      AyOnlar--;
      if(AyOnlar<0)
      {
        AyOnlar=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 7.Sütun 1.Satır

          while(DigitColumn==7 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);  
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=9;
      DigitRow=1;
      break;
    } 
          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(AyBirler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      AyBirler++;
      if(AyBirler>9)
      {
        AyBirler=9;
      }
       if(AyOnlar==1 && AyBirler==3)
      {
        AyBirler=2;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      AyBirler--;
      if(AyBirler<0)
      {
        AyBirler=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 9.Sütun 1.Satır

          while(DigitColumn==9 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=10;
      DigitRow=1;
      break;
    }
          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(YilBinler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      YilBinler++;
      if(YilBinler>2)
      {
        YilBinler=2;
      }
}
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 10.Sütun 1.Satır

          while(DigitColumn==10 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);  
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=11;
      DigitRow=1;
      break;
    }
          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(YilYuzler);
    delay(200);

         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 11.Sütun 1.Satır
  
          while(DigitColumn==11 && DigitRow==1)
  {
 ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW);   
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=12;
      DigitRow=1;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
          /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(YilOnlar);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      YilOnlar++;
      if(YilOnlar>9)
      {
        YilOnlar=9;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      YilOnlar--;
      if(YilOnlar<0)
      {
        YilOnlar=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
  
/***************************************************************************************/
// 12.Sütun 1.Satır

          while(DigitColumn==12 && DigitRow==1)
  {
ButtonState=digitalRead(SET_BUTTON); // Sürekli Set Butonunu Oku
     if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=DigitRow=20;
 SetMode=0;
 break;
  }

            if(digitalRead(CHANGE_BUTTON)==1) // İmleci Değiştirme Koşulu
    {
      while(digitalRead(CHANGE_BUTTON)==1)
      delay(100);
      DigitColumn=0;
      DigitRow=0;
      break;
    }

          /* İlgili İmlece Boş Karakter Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.write(byte(1));
    delay(200);
        /* İlgili İmlece Değeri Bas*/
    lcd.setCursor(DigitColumn,DigitRow);
    lcd.print(YilBirler);
    delay(200);
    
      if(digitalRead(UP_BUTTON)==1)
{
      while(digitalRead(UP_BUTTON)==1)
      delay(100);
      YilBirler++;
      if(YilBirler>9)
      {
        YilBirler=9;
      }
}

        if(digitalRead(DOWN_BUTTON)==1)
    {
      while(digitalRead(DOWN_BUTTON)==1)
      delay(100);
      YilBirler--;
      if(YilBirler<0)
      {
        YilBirler=0;
      }  
    }
         if(digitalRead(SET_BUTTON)==1)
  {
 digitalWrite(LED,LOW); 
 rtc.setTime((uint16_t)((SaatOnlar*10)+(SaatBirler)),(uint16_t)((DakikaOnlar*10))+(uint16_t)((DakikaBirler)), 0);   
 rtc.setDate((uint8_t )((GunOnlar*10)+(GunBirler)),(uint8_t)((AyOnlar*10)+(AyBirler)),(uint16_t)((YilBinler*1000)+(YilYuzler*100)+(YilOnlar*10)+(YilBirler)));
 FakeRow=DigitRow;
 FakeColumn=DigitColumn;
 SepetRow=FakeRow;
 SepetColumn=FakeColumn;
 DigitColumn=20;
 DigitRow=20;
 SetMode=0;
 break;
  }
  }
break;
}
}
  
  SetButtonRead();
  CalculateValue();
  ShowLCD();
}


void CalculateValue(void)
{
  t = rtc.getTime(); // Saat Bilgisi Alabilmek için Nesne Tanımlaması gibi bişi

  Saat=(int)t.hour; // Saat Bilgisini Değişkene ata
  Dakika=(int)t.min; // Dakika Bilgisini Değişkene Ata
  Gun=(int)t.date; // Gün Bilgisini Değişkene Ata
  Ay=(int)t.mon; // Ay Bilgisini Değişkene Ata
  Yil=(long int)t.year; // Yıl Bilgisini Değişkene Ata
  
  /* LCD Ekrana Düzgün Basabilmek İçin Gerekli Matematiksel İşlemler */
  SaatOnlar = Saat / 10; 
  SaatBirler = Saat % 10;
  DakikaOnlar = Dakika / 10;
  DakikaBirler = Dakika%10;
  GunOnlar = Gun/10;
  GunBirler = Gun%10;
  AyOnlar = Ay/10;
  AyBirler = Ay%10;
  YilBinler = Yil/1000;
  YilYuzler =((Yil/100)%10);
  YilOnlar = ((Yil/10)%100);
  YilBirler = Yil%10;
}

void ShowLCD(void)
{
    /* LCD Ekranda Sürekli Basılı Olmasını İstediklerim */
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(SaatOnlar);
  lcd.setCursor(1,0);
  lcd.print(SaatBirler);
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(3,0);
  lcd.print(DakikaOnlar);
  lcd.setCursor(4,0);
  lcd.print(DakikaBirler);
  lcd.setCursor(7,0);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  lcd.setCursor(0,1);
  lcd.print("==");
  lcd.setCursor(3,1);
  lcd.print(GunOnlar);
  lcd.setCursor(4,1);
  lcd.print(GunBirler);
  lcd.setCursor(5,1);
  lcd.print(".");
  lcd.setCursor(6,1);
  lcd.print(AyOnlar);
  lcd.setCursor(7,1);
  lcd.print(AyBirler);
  lcd.setCursor(8,1);
  lcd.print(".");
  lcd.setCursor(9,1);
  lcd.print(YilBinler);
  lcd.setCursor(10,1);
  lcd.print(YilYuzler);
  lcd.setCursor(11,1);
  lcd.print(YilOnlar);
  lcd.setCursor(12,1);
  lcd.print(YilBirler);
  lcd.print(" ==");
  delay(300);
}


void SetButtonRead(void)
{
 while(ButtonState==1)
 {
  delay(10);
  ButtonCounter++;
 
  if((ButtonCounter>=50))
  {
    digitalWrite(LED,HIGH);
    SetMode=1;
    DigitColumn=SepetColumn;
    DigitRow=SepetRow;
    FakeColumn=FakeRow=0;
  }  
  ButtonState=digitalRead(SET_BUTTON);  
 }
 ButtonCounter=0;  
}
