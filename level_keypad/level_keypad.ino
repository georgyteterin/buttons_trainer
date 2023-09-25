#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


int blue_led = 7;
int yellow_led = 6;
int green_led = 5;
int red_led = 4;

int blue_sound = 13;
int yellow_sound = 12;
int green_sound = 11;
int red_sound = 10;


long btn, old_btn;

uint32_t tmr, tmr1, tmr_btn, tmr_counter;

const byte ROWS1 = 2;
const byte COLS1 = 2;
char keys1[ROWS1][COLS1] = {
  {'1','2'},
  {'3','4'},
};
byte rowPins1[ROWS1] = {24, 22};  
byte colPins1[COLS1] = {28, 26};

Keypad keypad1 = Keypad(makeKeymap(keys1), rowPins1, colPins1, ROWS1, COLS1);

const byte ROWS2 = 2;
const byte COLS2 = 2;
char keys2[ROWS2][COLS2] = {
  {'1','2'},
  {'3','4'},
};
byte rowPins2[ROWS2] = {32, 30};  
byte colPins2[COLS2] = {36, 34};

Keypad keypad2 = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS2, COLS2);

int level = 5;
float interval = 2;
int buzzerPin = 19;
int score = 0;

long gamestart, time, global_time, average_time;
int counter = 0;


bool ch_level = false;
bool ch_interval = false;
bool go_game = false;
bool play = false;
bool lcd_counter = false;
bool final_scene = false;
bool pred_final_scene = false;

void(* resetFunc) (void) = 0;

void setup() {  
  pinMode(blue_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);

  
  pinMode(blue_sound, OUTPUT);
  pinMode(yellow_sound, OUTPUT);
  pinMode(green_sound, OUTPUT);
  pinMode(red_sound, OUTPUT);

  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  int key1 = keypad1.getKey();
  digitalWrite(blue_led, HIGH);
  digitalWrite(yellow_led, HIGH);
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, HIGH);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Hello!");
  tone(buzzerPin, 261, 250);
  delay(2000);
  lcd.clear();
  lcd.print("Click a");
  lcd.setCursor(0,1);
  lcd.print("to start");
  digitalWrite(blue_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  randomSeed(analogRead(A0));
}

  
void loop() {
  int key1 = keypad1.getKey();
  int key2 = keypad2.getKey();
  bool blue_btn = !digitalRead(A1);
  bool yellow_btn = !digitalRead(A2);
  bool green_btn = !digitalRead(A3);
  bool red_btn = !digitalRead(A4);

  
  switch (key1){
    case 49:
      ch_level = true;
  }   
  if (ch_level == true){
    if (millis() - tmr> 500) { 
        lcd_level();
        tmr = millis(); 
      } 
    switch (key1){
      case 52:
        if (level >= 5 & level < 29){    
          level = level + 3;
        }
        else if (level == 29){
          level = 5;
          
        }  
      break;
      case 50:
        if (level > 5 & level <= 29){
          level = level - 3;;
          
        }  
        else if (level == 5){  
          level = 29;
          
        }        
      break;

      case 51:
        ch_level = false; 
        ch_interval = true;
        }      
    }
/// выбор интервала
    if (ch_interval == true){
    if (millis() - tmr> 500) { 
        lcd_interval();
        // Serial.print("interval is "); Serial.println(interval);
        tmr = millis(); 
      } 
    switch (key2){
      case 52:
        if (interval >= 2 & interval < 15){    
          interval = interval + 0.25;
        }
        else if (interval == 15){
          interval = 2;
          
        }  
      break;
      case 50:
        if (interval > 2 & interval <= 15){
          interval = interval - 0.25;
          
        }  
        else if (interval == 2){  
          interval = 15; 
        }        
      break;

      case 51:
        interval = interval*1000;
        Serial.println(interval);        
        ch_interval = false; 
        lcd_counter = true;
        }      
    }
  if (lcd_counter == true){
    lcd.clear();
    lcd.print("3...");
    delay(1000);
    lcd.clear();
    lcd.print("2...");
    delay(1000);
    lcd.clear();
    lcd.print("1...");
    delay(1000);
    lcd.clear();  
    lcd.print("Go!!!");
    delay(1000);
    lcd.clear();
    go_game = true;
    lcd_counter = false;    
  }
  if (go_game == true){ 
if (counter < level){  
      if (play == false){
      if (millis() - tmr> 500) { 
        lcd_score();
        tmr = millis(); 
      }          
      while (btn == old_btn){
      btn = random(1, 5);
      }      
      digitalWrite(blue_led, LOW);
      digitalWrite(yellow_led, LOW);
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, LOW);               
        delay(2500);
        Serial.print("btn is "); Serial.println(btn);
        Serial.print("counter is "); Serial.println(counter);

        switch (btn){
    case 1:  
      digitalWrite(blue_led, HIGH);
      tone(blue_sound, 261, 250);  
    break;
    
    case 2:
      digitalWrite(yellow_led, HIGH);
      tone(yellow_sound, 261, 250);
    break;
    
    case 3:
      digitalWrite(green_led, HIGH);
      tone(green_sound, 261, 250);
    break;

    case 4:
      digitalWrite(red_led, HIGH);
      tone(red_sound, 261, 250);
    break;
        }           
        old_btn = btn; 
        time = millis() - gamestart;
        global_time = global_time + time;
        gamestart = millis();             
        play = true;  
      } 
      


 if (play == true){  
  switch (btn){
     case 1:
     if (blue_btn ==0 and millis() - gamestart > interval){
        play = false;
        counter++;
      }
      else if (blue_btn == 1){
        play = false;
        counter++;
        score ++;
      }
      
      break;
      
      case 2:
     if (yellow_btn ==0 and millis() - gamestart > interval){
        play = false;
        counter++;
      }
      else if (yellow_btn == 1){
        play = false;
        counter++;
        score ++;
      }
      break;  
      
    case 3:
      if (green_btn ==0 and millis() - gamestart > interval){
        play = false;
        counter++;
      }
      else if (green_btn == 1){
        play = false;
        counter++;
        score ++;
      }
      
      break;  
           
      case 4:
      if (red_btn ==0 and millis() - gamestart > interval){
        play = false;
        counter++;
      }
      else if (red_btn == 1){
        play = false;
        counter++;
        score ++;
      }
      break;
        }
        }              
      }
else if (counter == level){
  digitalWrite(blue_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  lcd_score();
  delay(3000);
  go_game = false; 
  final_scene = true; 
    }      
  }
  if (final_scene == true){
    average_time = global_time/level;    
    if (millis() - tmr > 500){
    lcd_last_scene();
    tmr = millis();
    }
    switch(key1){
    case 49:
    resetFunc();
    }   
}
}

                                  

           
void lcd_level(){
  lcd.clear();
  lcd.print("Press per game:");
  lcd.setCursor(0,1);
  lcd.print(level);
}

void lcd_interval(){
  lcd.clear();
  lcd.print("interval:");
  lcd.setCursor(0,1);
  lcd.print(interval);
  lcd.print(" seconds");
  }



void lcd_score(){
  lcd.clear();
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print("/");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  lcd.print(millis() - gamestart); 
  lcd.print(" ms");
}

void lcd_last_scene(){
  lcd.clear();
  lcd.print("a - Play again");
  lcd.setCursor(0, 1);
  lcd.print("off - Goodbye!");            
  }





