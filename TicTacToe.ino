#include <TrueRandom.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS  10  // Chip select line for TFT display
#define TFT_DC   8  // Data/command line for TFT
#define TFT_RST  -1  // Reset line for TFT (or connect to reset)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5
uint16_t pixelColor;
uint8_t buttonPressed, currentDirection;
int turnPlayer1 = 1, turnPlayer2 = 0;
int i, selection = 0, playerColor, winner = 0, catsGame = 0, totalBoxesUsed;
int boxUsed[] = {0,0,0,0,0,0,0,0,0}; // 0 means nothing. 1 means X played there. 2 means O played there.
//int boxSelect[] = {0,0,0,0,1,0,0,0,0}; // 0 means the player is not currently hovering over that box. 1 means the player is currently hovering over that box.

void drawBoard();
void tileSelect();
void placeGameChip(int selection);
void isCatsGame();
void isWinner();
void winnerDisplay();
void catsGameDisplay();

void setup(void) {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(0x0000);
  pixelColor = ST7735_WHITE;
  drawBoard();
}

uint8_t readButton(void) {
  float a = analogRead(3);
  a *= 5.0;
  a /= 1024.0;
  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}

void loop() {
  uint8_t b = readButton();
  if (winner == 0 && catsGame == 0){
    tileSelect();
    isCatsGame();
    isWinner();
  }
}

void drawBoard(){
  tft.drawRect(0, 20, tft.width(), tft.height()-20, ST7735_MAGENTA);
  tft.drawRect(0, 20, tft.width()/3, tft.height()-20, ST7735_MAGENTA);
  tft.drawRect(0, 20, (tft.width()/3)*2, tft.height()-20, ST7735_MAGENTA);
  tft.drawRect(0, 20, tft.width(), (tft.height()-20)/3, ST7735_MAGENTA);
  tft.drawRect(0, 20, tft.width(), ((tft.height()-20)/3)*2, ST7735_MAGENTA);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(4, 10);
  tft.print("P1=RED");
  tft.setCursor(36, 0);
  tft.print("TicTacToe Dia S");
  tft.setCursor(85, 10);
  tft.print("P2=BLUE");
}

void tileSelect(){
  uint8_t gameButton = readButton();

  if (turnPlayer1 == 1){
    playerColor = ST7735_RED;
  } else if (turnPlayer2 == 1){
    playerColor = ST7735_BLUE;
  }
  
  if (gameButton == BUTTON_LEFT)
    if (selection != 0 && selection != 3 && selection != 6)
      selection--;
  if (gameButton == BUTTON_RIGHT)
    if (selection != 2 && selection != 5 && selection != 8)
      selection++;
  if (gameButton == BUTTON_UP)
    if (selection >= 2)
      selection = selection - 3;
  if (gameButton == BUTTON_DOWN)
    if (selection <= 6)
      selection = selection + 3;
  
  if (selection == 0){ 
    // ROW 1
    tft.fillRect(3, 22, 6, 6, playerColor);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 1){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, playerColor);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 2){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, playerColor);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 3){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, playerColor);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 4){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, playerColor);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 5){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, playerColor);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 6){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, playerColor);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 7){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, playerColor);
    tft.fillRect(86, 113, 6, 6, ST7735_BLACK);
  } else if (selection == 8){
    // ROW 1
    tft.fillRect(3, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 22, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 22, 6, 6, ST7735_BLACK);
    
    // ROW 2
    tft.fillRect(3, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 67, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 67, 6, 6, ST7735_BLACK);

    // ROW 3
    tft.fillRect(3, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(44, 113, 6, 6, ST7735_BLACK);
    tft.fillRect(86, 113, 6, 6, playerColor);
  }

  if (gameButton == BUTTON_SELECT){
    placeGameChip(selection);
    return;
  }
  delay(100);
}

void placeGameChip(int selection){
  tft.setTextSize(4);
  tft.setTextColor(ST7735_WHITE);
  if (selection == 0 && boxUsed[0] == 0){
    tft.setCursor(13, 30);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[0] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[0] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 1 && boxUsed[1] == 0){
    tft.setCursor(55, 30);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[1] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[1] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 2 && boxUsed[2] == 0){
    tft.setCursor(100, 30);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[2] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[2] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 3 && boxUsed[3] == 0){
    tft.setCursor(13, 75);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[3] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[3] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 4 && boxUsed[4] == 0){
    tft.setCursor(55, 75);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[4] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[4] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 5 && boxUsed[5] == 0){
    tft.setCursor(100, 75);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[5] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[5] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 6 && boxUsed[6] == 0){
    tft.setCursor(13, 125);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[6] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[6] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 7 && boxUsed[7] == 0){
    tft.setCursor(55, 125);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[7] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[7] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }
  if (selection == 8 && boxUsed[8] == 0){
    tft.setCursor(100, 125);
    if (turnPlayer1 == 1){
      Serial.print("\nPLAYER 1 PLAYED\n");
      tft.print("X");
      boxUsed[8] = 1;
      turnPlayer1 = 0;
      turnPlayer2 = 1;
    } else if (turnPlayer2 == 1){
        Serial.print("\nPLAYER 2 PLAYED\n");
        tft.print("O");
        boxUsed[8] = 2;
        turnPlayer2 = 0;
        turnPlayer1 = 1;
    }
  }

  for (i=0; i<9; i=i+1) {
    Serial.print("BOX USED [");
    Serial.print(i);
    Serial.println("]");
    Serial.println(boxUsed[i]);
  }
}

void isCatsGame(){
  i = 0;
  totalBoxesUsed = 0;
  for (i=0; i<9; i=i+1) {
//    Serial.print("i=");
//    Serial.println(i);
    if (boxUsed[i] != 0){
      totalBoxesUsed = totalBoxesUsed + 1;
//      Serial.print("TOTAL BOXES USED");
//      Serial.println(totalBoxesUsed);
      catsGame = 0;
    }
  }
  if (totalBoxesUsed == 9){
    if (winner == 0){
      catsGame = 1;
      catsGameDisplay();
    }
  }
}

void isWinner(){
  if (boxUsed[0] == 1 && boxUsed[1] == 1 && boxUsed[2] == 1 ||  // Across
      boxUsed[3] == 1 && boxUsed[4] == 1 && boxUsed[5] == 1 ||  // Across
      boxUsed[6] == 1 && boxUsed[7] == 1 && boxUsed[8] == 1 ||  // Across
      boxUsed[0] == 1 && boxUsed[3] == 1 && boxUsed[6] == 1 ||  // Down or Up
      boxUsed[1] == 1 && boxUsed[4] == 1 && boxUsed[7] == 1 ||  // Down or Up
      boxUsed[2] == 1 && boxUsed[5] == 1 && boxUsed[8] == 1 ||  // Down or Up
      boxUsed[0] == 1 && boxUsed[4] == 1 && boxUsed[8] == 1 ||  // Diagonal
      boxUsed[2] == 1 && boxUsed[4] == 1 && boxUsed[6] == 1){   // Diagonal
    winner = 1;
    winnerDisplay();
  } else if (boxUsed[0] == 2 && boxUsed[1] == 2 && boxUsed[2] == 2 ||  // Across
             boxUsed[3] == 2 && boxUsed[4] == 2 && boxUsed[5] == 2 ||  // Across
             boxUsed[6] == 2 && boxUsed[7] == 2 && boxUsed[8] == 2 ||  // Across
             boxUsed[0] == 2 && boxUsed[3] == 2 && boxUsed[6] == 2 ||  // Down or Up
             boxUsed[1] == 2 && boxUsed[4] == 2 && boxUsed[7] == 2 ||  // Down or Up
             boxUsed[2] == 2 && boxUsed[5] == 2 && boxUsed[8] == 2 ||  // Down or Up
             boxUsed[0] == 2 && boxUsed[4] == 2 && boxUsed[8] == 2 ||  // Diagonal
             boxUsed[2] == 2 && boxUsed[4] == 2 && boxUsed[6] == 2){   // Diagonal
    winner = 2;
    winnerDisplay();
  } else {
    winner = 0;
  }
}

void catsGameDisplay() {
  tft.fillRect(25, 74, tft.width() - 50, 35, ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(25, 75);
  tft.setTextColor(ST7735_BLACK);
  tft.print("Cats");
  tft.setCursor(40, 90);
  tft.print("Game!");
}

void winnerDisplay(){
  tft.fillRect(25, 74, tft.width() - 50, 35, ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(25, 75);
  if (winner == 1)
    tft.setTextColor(ST7735_RED);
  if (winner == 2)
    tft.setTextColor(ST7735_BLUE);
  tft.print("Player");
  tft.print(winner);
  tft.setCursor(40, 90);
  tft.print("Wins!");
}

