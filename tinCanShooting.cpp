/** 
  * Sketch    TinCanShooting.cpp
  * Author    2018-04-12 Ch. Geiser
  * Purpose   Simulates a tin can shooting on a I2C-LCD.
  *           Press the pushbutton to start a new round of shootings
  *           As an intro it also shows how to display some 
  *           special characters knowing their octal code.
  * 
  * Board     Arduino Uno R3
  *                    ---.                    .---
  *           I2C-LCD:    |-- GND  -->  GND  --|
  *                       |-- Vcc  -->  5V   --|  Arduino 
  *                       |-- SDA  -->  A4   --|  UNO R3
  *                       |-- SCL  -->  A5   --|
  *                    ---`                    |
  *                            .-------- D13 --| (input pullup)
  *                            |               |           
  *                            '--o o--- Gnd --|
  *                               ¨T¨          `---
  *                             pushbutton 
  * 
  * Remarks   How many balls do we have to throw on average to hit
  *           all the "cans" on the LCD?
  * 
  *           The probability to hit a can follows the harmonic serieas.      
  *
  *           For a 2 x 16 LCD we have 32 cans and the number of shots 
  *           on average to hit all of them is:
  *           N = 32 x (1 + 1/2 + 1/3 + 1/4 + ... + 1/32)  
  *           Sum of the harmonic series 1..32:            4.058495195
  *           An approximation for that is ln(32) + gamma: 4.042951567
  *           with gamma (Euler-Mascheroni-constant):      0.5772156649
  *           Therefor the number of shots on average is:  32 * 4.058 ~= 130
  * 
  *           For a 4 x 20 display we have 80 cans and we get:
  *           Sum of the harmonic series 1..80:            4.965479278
  *           Approximation     ln(80) + gamma:            4.959242299
  *           Number of shots on average:                  80 * 4.965 ~= 397
  */
 
#include "tinCanShooting.h"

uint8_t target[LCD_ROWS][LCD_COLUMNS];   // 4 rows by 20 columns                                           

void setup()
{
  Serial.begin(COMPORT_SPEED);
  pinMode(PIN_PUSHBUTTON, INPUT_PULLUP),
  randomSeed(analogRead(0));   // Initialize random generator with nois from analog output

  lcd.init();
  
  lcd.createChar(0, ball0);   // load custom characters    
  lcd.createChar(1, ball1);
  lcd.createChar(2, ball2); 
  lcd.backlight();
  greetings("Tin Can Alley", 2000);
}

void loop()
{
  shootTinCans(NBR_OF_ROUNDS);
  askAgain();
}

void askAgain()
{
  int row = (LCD_ROWS == 2) ? 0 : 2;
  lcd.setCursor(0, row);
  lcd.print("Push button to");
  lcd.setCursor(0, row + 1);
  lcd.print("start new round");
  while (digitalRead(PIN_PUSHBUTTON));
}

/**
 * Displays a welcome message and some special characters
 */
void greetings(const char* txt, int msWait)
{
  lcd.clear();
  lcd.print(txt);  
  lcd.setCursor (0, 1);
  // Show some special characters
  lcd.print("\337\340\341\342\343\344\345\346\350\357\364\365\366\367");
  lcd.display(); 
  bounce(); 
  delay(msWait);

  // Scroll out greeting text to the left
  char *p = const_cast<char *>(txt); // Set pointer to beginning of text 
  while ((*p++) != 0)   // Advance pointer int txt 
  {
    lcd.setCursor(0,0); // Reposition the cursor
    lcd.print(p);       // print text from ponter onto terminating /0 
    lcd.print(" ");     // clear character at end of text
    delay(150);
  }
  delay(500); 
}

/**
 * Bounces the ball over the screen
 */
void bounce()
{
  for(int i = 14; i < LCD_COLUMNS; i++)
  {
    bouncingBall(i, 0);
    lcd.setCursor(i, 0); lcd.print(' ');
  }
  
  for(int i = LCD_COLUMNS-1; i >= 0; i--)
  {
    bouncingBall(i, 1);
    lcd.setCursor(i, 1); lcd.print(' ');
  }  
}

/**
 * Displays a bouncing ball
 */
void bouncingBall(byte col, byte row)
{
  for (int i = 0; i < 3; i++)
  {
    lcd.setCursor(col, row );
    lcd.print(char(i));
    delay (250);
  }
}

void placeTinCans(char x)
{
  for (int i = 0; i < LCD_ROWS; i++)
  {
    for (int j = 0; j < LCD_COLUMNS; j++)
    {
      lcd.setCursor(j, i);
      lcd.print(x);
      target[i][j] = 1;
    }
  }
}

/**
 * Shoot at the tin cans until all are fallen
 * and repeat for n roundsS
 */
void shootTinCans(int rounds)
{
  char buf[64];
  static uint16_t totalShots = 0;
  static uint16_t totalRounds = 0; 
  
  for (int i = 0; i < rounds; i++)
  {
    int hits = 0;
    int shots = 0;
    placeTinCans('x');
    
    while (hits < (LCD_COLUMNS * LCD_ROWS))
    {
      int row = random(0, LCD_ROWS);
      int col = random(0, LCD_COLUMNS);
      lcd.setCursor(col, row);
      lcd.print(char(1));
      delay(250);
      lcd.setCursor(col, row);
      lcd.print(' ');
      shots++;
      if (target[row][col] == 1) 
      {
        hits++;
        target[row][col] = 0;
      } 
    }
    
    totalShots += shots;
    totalRounds++;

    lcd.clear();
    sprintf(buf, "S: %5d R: %3d", shots, totalRounds);
    lcd.print(buf);
    sprintf(buf, "\366: %5d M: %3d", totalShots, totalShots/(totalRounds));
    lcd.setCursor(0,1); 
    lcd.print(buf);

    sprintf(buf, "Shots = %d, Rounds = %d, Mean = %d\r\n", totalShots, totalRounds, totalShots/totalRounds);
    Serial.print(buf);
    delay(4000);  
  }
}


