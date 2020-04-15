#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ROWS       4    // or 2 for a LCD with 2 rows
#define LCD_COLUMNS    20   // or 16 for a LCD with 16 columns
#define LCD_ADDRESS    0x27 // I2C address
#define PIN_PUSHBUTTON 13
#define COMPORT_SPEED  9600

#define NBR_OF_ROUNDS  10

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// function prototypes
void askAgain(void);
void greetings(const char* txt, int msWait);
void bounce();
void bouncingBall(byte col, byte row);
void placeTinCans(char x);
void shootTinCans(int rounds);

// Some special characters
// ä = \341
// ö = \357
// ü = \365
// °      = \337
// alpha  = \340
// beta   = \342
// epsilon= \343
// mü     = \344
// sigma  = \345
// rho    = \346
// sqrt   = \348
// Omega  = \364
// Sigma  = \366
// pi     = \367

// 3 custom characters (falling Ball)
byte ball0[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte ball1[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

byte ball2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b10001,
  0b01110
};