#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Servo.h> 

Servo servo; 

int buzz=6;

int x;
int uid[4];
int kart[3][4]{ // массив для трех меток
  {
    131, 71, 41,23    }  
  ,
  { 
    76, 59, 165, 51   }
  , 
  {
    162, 91,142, 96    } 
};
 
MFRC522 mfrc522(10,9); //(SS_PIN, RST_PIN)   
 
void setup() {
  Serial.begin(9600);        
  while (!Serial);          
  SPI.begin();              
  mfrc522.PCD_Init();  
  //    
  pinMode(buzz,OUTPUT);

  servo.attach(7); //привязываем привод к порту 7


  Serial.println("Поднесите карту");
}
 
void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  Serial.print("UID: ");
  for (byte y = 0; y < (sizeof(kart)/sizeof(int)/4); y++) {
    for (byte i = 0; i < 4; i++) {
      uid[i] = mfrc522.uid.uidByte[i];
      if(uid[i]==kart[y][i]){
        x++;
      }
    } 
    if(x<4){
      x=0;
    }
  }
 
  for (byte i = 0; i < 4; i++) {
    Serial.print(uid[i]);
    Serial.print(" ");
  }
  Serial.println(x);
  Serial.println();
 
  if(x>=4){
    
    Serial.println("Доступ разрешен");
     //
  tone(buzz,890);
  delay(330);
  noTone(buzz);

  servo.write(90); //ставим вал под 180
  delay(2000); //ждем 2 секунды

  //  
  }
  else{
    Serial.println("У вас нет доступа");
    //
  tone(buzz,100);
  delay(300);
  noTone(buzz);
  delay(300);

  servo.write(0); //ставим вал под 0
  delay(2000); //ждем 2 секунды
  //
  }
  x=0;   
}
