#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


#define TFT_MISO 12
#define TFT_LED 7
#define TFT_SCK 13
#define TFT_MOSI 11
#define TFT_DC 9
#define TFT_RESET 8
#define TFT_CS 10

#define BUTTON_A 4
#define BUTTON_B 2
#define BUTTON_RIGHT 7
#define BUTTON_LEFT 8
#define BUTTON_UP 6
#define BUTTON_DOWN 5



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);

int startScreen(String gameNames[]);


int width, height;
#define YDISTANCE (height - width) / 2

//Chess declare
class chessPiece {
public:
};

void setup() {
  Serial.begin(9600);
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  tft.begin();
  tft.setRotation(0);  //Vertical con los pines abajo
  tft.fillScreen(ILI9341_BLACK);

  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_LEFT, INPUT);

  width = tft.width();
  height = tft.height();

  randomSeed(analogRead(A0));
  startupScreen();
}

void loop() {

  newScreen();
  int chosenGame = 0;
  String gameNames[] = { "ms", "2048", "match3" };

  chosenGame = startScreen(gameNames, 3);
  Serial.println(chosenGame);
  if(chosenGame == 1) {
    newScreen();
    headerText("Minesweeper");
    while(minesweeper()){

    }

  } else if(chosenGame == 2) {
    newScreen();
    headerText("2048");
    ini2048();
  } else if(chosenGame == 3) {
    newScreen();
    headerText("Match 3");
    playGame3in3();
  }
}

void newScreen() {
  tft.fillScreen(ILI9341_BLACK);
}
void newScreenLines(int space, int size) {
  tft.fillScreen(ILI9341_BLACK);
  for(int i = 0; i < height; i += space) {
    tft.fillRect(0, i, width, size, ILI9341_GREENYELLOW);
  }
}

void startupScreen() {
  newScreen();
  int rectWidth = 32;
  int rectHeight = 128;
  int insideRectMargen = 4;
  int colorLogo = ILI9341_WHITE;
  tft.fillRect(width/2 - rectWidth/2, height/2 - rectHeight/2, rectWidth, rectHeight, colorLogo);
  tft.fillRect(width/2 - rectHeight/2, height/2 - rectWidth/2, rectHeight, rectWidth, colorLogo);
  tft.fillCircle(width/2, height/2, rectWidth/2 - insideRectMargen, ILI9341_BLACK);
  tft.fillTriangle(width/2 - rectHeight/2, height/2, width/2 - rectHeight/2 + rectWidth, height/2 - rectWidth/2, width/2 - rectHeight/2 + rectWidth, height/2 - rectWidth/2 + rectWidth, ILI9341_RED);
  tft.fillTriangle(width/2 + rectHeight/2, height/2, width/2 + rectHeight/2 - rectWidth, height/2 + rectWidth/2, width/2 + rectHeight/2 - rectWidth, height/2 + rectWidth/2 - rectWidth, ILI9341_BLUE);
  delay(4000);
}



#define margen 20
#define inBetweenSquares 5
#define squareRadius 10
#define squareHeight 100
#define squareY 110
#define squareColor ILI9341_PURPLE


int startScreen(String gameNames[], int numOfGames) {
  tft.fillScreen(ILI9341_BLACK);
  //title
  tft.setCursor(30, 2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(5);
  tft.print(" Game\n Arcade");

  //dynamic game squares
  //int numOfGames = sizeof(gameNames);
  int squareWidth = (width - margen * 2 - inBetweenSquares * (numOfGames - 1)) / numOfGames;

  for (int i = 0; i < numOfGames; i++) {
    tft.drawRoundRect(margen + i * (inBetweenSquares + squareWidth), squareY, squareWidth, squareHeight, squareRadius, squareColor);

    text(gameNames[i], margen + i * (inBetweenSquares + squareWidth), squareY + squareHeight + 5, ILI9341_WHITE, 1, false);
  }
  unsigned long previousMillis = 0;
  const long interval =300; 
  unsigned long currentMillis;
  boolean gameChosen = false;
  int gameFocus = 0;
  while (!gameChosen) {
    unsigned long currentMillis = millis();
    tft.fillTriangle(margen + gameFocus * (inBetweenSquares + squareWidth), squareY - 15, margen + gameFocus * (inBetweenSquares + squareWidth) + squareWidth, squareY - 15, margen + gameFocus * (inBetweenSquares + squareWidth) + squareWidth / 2, squareY - 1, ILI9341_WHITE);
    delay(500);
    tft.fillTriangle(margen + gameFocus * (inBetweenSquares + squareWidth), squareY - 15, margen + gameFocus * (inBetweenSquares + squareWidth) + squareWidth, squareY - 15, margen + gameFocus * (inBetweenSquares + squareWidth) + squareWidth / 2, squareY - 1, ILI9341_BLACK);
    delay(500);
    
    if (digitalRead(BUTTON_RIGHT) == HIGH && currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (gameFocus == numOfGames-1) {
          gameFocus = 0;
        } else {
          gameFocus ++;
        }
    }

    if (digitalRead(BUTTON_LEFT) == HIGH  && currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
        if (gameFocus == 0) {
          gameFocus = numOfGames-1;
        } else {
          gameFocus --;
        }
    }

    if (digitalRead(BUTTON_A) == HIGH) {
      gameChosen = true;
    }
    if (digitalRead(BUTTON_B) == HIGH) {
      infoScreen();
    }
      
  }
  return gameFocus +1;
}




void infoScreen() {
  newScreenLines(5, 1);
  tft.drawPixel(0, 0, 0xFFFFF);
  tft.drawPixel(1, 1, 0xFFFFF);
  tft.drawPixel(1, 0, 0xFFFFF);
  tft.drawPixel(0, 1, 0xFFFFF);
  text((String) "Game Arcade \ninfo screen", 2, 20, ILI9341_WHITE, 3, true);
  text((String) "Screen size:\n"+width + " x " + height+"\nOrientation:\nvertical", 2, 80, ILI9341_WHITE, 2, true);
  //tft.println((String)width + " x " + height);
  text((String)"Menus       - Diego\nMinesweeper - Diego\n2048        - Daniel\nMatch 3     - Nerea", 2, 160, ILI9341_WHITE, 2, true);
  while (true) {
    if (digitalRead(BUTTON_B) == HIGH) {
        loop();
    }
  }
}

void text(String text, int16_t cursorX, int16_t cursorY, uint16_t color, uint8_t size, boolean textWrap) {
  tft.setCursor(cursorX, cursorY);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setTextWrap(textWrap);
  tft.print(text);
}

void headerText(String text1) {
  tft.fillRect(0, 0, width, YDISTANCE, ILI9341_BLACK);
  text(text1, 10, 5, ILI9341_WHITE, 2, true);
}

void bottomText(String text1) {
  tft.fillRect(0, YDISTANCE + width, width, YDISTANCE, ILI9341_BLACK);
  text(text1, 10, 5 + YDISTANCE + width, ILI9341_WHITE, 2, true);
}

boolean endGamePrompt(String gameName) {
  headerText("Finished!");
  delay(1000);
  headerText("A-Retry//B-Exit");
  return checkRetry(gameName);
}

boolean endGamePromptBoolean(bool win, String gameName) {
  if(win){
    headerText("You Won!");
  } else {
    headerText("Game Over");
  }
  delay(1000);
  headerText("A-Retry//B-Exit");
  return checkRetry(gameName);
}

boolean endGamePrompt2P(bool winP1, String gameName) {
  if(winP1){
    headerText("Player 1 wins!");
  } else {
    headerText("Player 2 wins!");
  }
  delay(1000);
  headerText("A-Retry//B-Exit");
  return checkRetry(gameName);
}

boolean exitGamePrompt(String gameName) {
  headerText("A-Exit//B-Back");
  boolean selected = false;
  while(!selected) {
    if (digitalRead(BUTTON_A) == HIGH) {
      selected = true;
      return true;
    } else if (digitalRead(BUTTON_B) == HIGH) {
      selected = true;
      headerText(gameName);
      return false;
    }
  }
}

boolean checkRetry(String gameName) {
  boolean selected = false;
  while(!selected) {
    if (digitalRead(BUTTON_A) == HIGH) {
      selected = true;
      tft.fillRect(0, YDISTANCE, width, width + YDISTANCE, ILI9341_BLACK);
      headerText(gameName);
      return true;
    } else if (digitalRead(BUTTON_B) == HIGH) {
      selected = true;
      return false;
    }
  }
}


//minesweeper
//
//
//Por Diego
boolean minesweeper() {
  int numBooms = numOfMinesChooser();
  int foundBooms = 0;
  Serial.println(numBooms);
  int boardLength = 8;
  String board[8][8];
  int cellLenght = width / boardLength - 2;

  boolean boardReveal[8][8];
  
  int focusCellLenght = width / boardLength;
  int yFocusDistance = YDISTANCE;
  int focusX = 0;
  int focusY = 0;

  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      board[i][j] = "";
      boardReveal[i][j] = false;
    }
  }

  int currentBooms = 0;
  while (currentBooms != numBooms) {
    int randomNumber = random(8);
    int randomNumber2 = random(8);
    if (board[randomNumber][randomNumber2] == "*")
      continue;
    board[randomNumber][randomNumber2] = "*";
    currentBooms++;
    Serial.println("i:");
    Serial.println(randomNumber);
    Serial.println("j:");
    Serial.println(randomNumber2);
    Serial.println("");
  }

  //generateNumbers
  int adyacentBombs;
  for (int i = 0; i < boardLength; i++) {
    for (int j = 0; j < boardLength; j++) {
      adyacentBombs = 0;
      if (!(board[i][j].equals("*"))) {
        if ((i != 0)) {
          if (board[i - 1][j].equals("*"))
            adyacentBombs++;
        }
        if ((i != boardLength - 1)) {
          if (board[i + 1][j].equals("*"))
            adyacentBombs++;
        }
        if ((j != 0)) {
          if (board[i][j - 1].equals("*"))
            adyacentBombs++;
        }
        if ((j != boardLength - 1)) {
          if (board[i][j + 1].equals("*"))
            adyacentBombs++;
        }
        if ((i != 0) && (j != 0)) {
          if (board[i - 1][j - 1].equals("*"))
            adyacentBombs++;
        }
        if ((i != boardLength - 1) && (j != 0)) {
          if (board[i + 1][j - 1].equals("*"))
            adyacentBombs++;
        }
        if ((i != 0) && (j != boardLength - 1)) {
          if (board[i - 1][j + 1].equals("*"))
            adyacentBombs++;
        }
        if ((i != boardLength - 1) && (j != boardLength - 1)) {
          if (board[i + 1][j + 1].equals("*"))
            adyacentBombs++;
        }
      }
      if (adyacentBombs > 0) {
        board[i][j] = (String)adyacentBombs;
      }
    }
  }

  for(int i = 0; i < boardLength; i++) {
    for(int j = 0; j < boardLength; j++) {
      Serial.print(board[j][i]);
      if(board[j][i].equals(""))
        Serial.print("_");
    }
    Serial.println("");
  }

  //Start play
  printBoard(board, boardReveal, boardLength, cellLenght);
  drawFocus(focusX, focusY,focusCellLenght, yFocusDistance, true);
  boolean win = false;
  boolean finish = false;
  
  int remainingCells = boardLength*boardLength - numBooms;
  String text = (String)remainingCells;
  while(!finish) {
    
    if (digitalRead(BUTTON_A) == HIGH) {
      
      if (printCell(board, boardReveal, focusX, focusY, cellLenght)) {
        boardReveal[focusX][focusY] = true;
        
        if (board[focusX][focusY].equals("*")) {
          finish = true;
          break;
        }

        if (board[focusX][focusY] == "") {
          if (focusX != 0 && !boardReveal[focusX - 1][focusY]) {
            boardReveal[focusX - 1][focusY] = true;
            remainingCells--;
          }
          if (focusX != 7 && !boardReveal[focusX + 1][focusY]) {
            boardReveal[focusX + 1][focusY] = true;
            remainingCells--;
          }
          if (focusY != 0 && !boardReveal[focusX][focusY - 1]) {
            boardReveal[focusX][focusY - 1] = true;
            remainingCells--;
          }
          if (focusY != 7 && !boardReveal[focusX][focusY + 1]) {
            boardReveal[focusX][focusY + 1] = true;
            remainingCells--;
          }
          if (focusX != 0 && focusY != 0 && !boardReveal[focusX - 1][focusY - 1]) {
            boardReveal[focusX - 1][focusY - 1] = true;
            remainingCells--;
          }
          if (focusX != 7 && focusY != 7 && !boardReveal[focusX + 1][focusY + 1]) {
            boardReveal[focusX + 1][focusY + 1] = true;
            remainingCells--;
          }
          if (focusX != 0 && focusY != 7 && !boardReveal[focusX - 1][focusY + 1]) {
            boardReveal[focusX - 1][focusY + 1] = true;
            remainingCells--;
          }
          if (focusX != 7 && focusY != 0 && !boardReveal[focusX + 1][focusY - 1]) {
            boardReveal[focusX + 1][focusY - 1] = true;
            remainingCells--;
          }  
        }
        remainingCells--;
        text = remainingCells;
        Serial.println(remainingCells);
        bottomText(text);
      }
      delay(300);
      if (remainingCells == 0) {
        win = true;
        finish = true;
      }
    }

    if (digitalRead(BUTTON_B) == HIGH) {
      if (exitGamePrompt("Minesweeper")){
        finish = true;
      }
      delay(300);
    }

    if (digitalRead(BUTTON_RIGHT) == HIGH) {      
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, false);
      if (focusX == 7) {
        focusX = 0;
      } else{
        focusX += 1;
      }
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, true);
      delay(300);
    }

    if (digitalRead(BUTTON_LEFT) == HIGH) {
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, false);
      if (focusX == 0) {
        focusX = 7;
      } else {
        focusX -= 1;
      }
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, true);
      delay(300);
    }

    if (digitalRead(BUTTON_UP) == HIGH) {
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, false);
      if (focusY == 0) {
        focusY = 7;
      } else {
        focusY -= 1;
      }
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, true);
      delay(300);
    }

    if (digitalRead(BUTTON_DOWN) == HIGH) {
            drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, false);
      if (focusY == 7) {
        focusY = 0;
      } else {
        focusY += 1;
      }
      drawFocus(focusX , focusY, focusCellLenght, yFocusDistance, true);
      delay(300);
    }
  }
  return endGamePromptBoolean(win, "Minesweeper");
}

void printBoard(String board[][8], boolean boardReveal[][8], int boardLength, int cellLenght) {
  tft.fillRect(0, YDISTANCE, width, width, 0x8410);
  int cellShadeGirth = 2;
  for (int i = 0; i < boardLength; ++i) {
    for (int j = 0; j < boardLength; ++j) {
        tft.fillRect((1 + i * (cellLenght + 2)), YDISTANCE + (1 + j * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        
        tft.fillRect((1 + i * (cellLenght + 2)), YDISTANCE + (1 + j * (cellLenght + 2)), cellShadeGirth, cellLenght, ILI9341_WHITE);
        tft.fillRect((1 + i * (cellLenght + 2)), YDISTANCE + (1 + j * (cellLenght + 2)), cellLenght, cellShadeGirth, ILI9341_WHITE);
        tft.fillRect((1 + i * (cellLenght + 2) + cellLenght - cellShadeGirth), YDISTANCE + (1 + j * (cellLenght + 2)), cellShadeGirth, cellLenght, 0x8410);
        tft.fillRect((1 + i * (cellLenght + 2)), YDISTANCE + (1 + j * (cellLenght + 2)) + cellLenght - cellShadeGirth, cellLenght, cellShadeGirth, 0x8410);    
    }
  }
}
boolean printCell(String board[][8], boolean boardReveal[][8], int focusX, int focusY, int cellLenght) {
  int textOffset = 8;
  
  if (boardReveal[focusX][focusY] == true)
    return false;
  tft.fillRect((1 + focusX * (cellLenght + 2)), YDISTANCE + (1 + focusY * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
  text(board[focusX][focusY], (textOffset + 1 + focusX * (cellLenght + 1)), textOffset + YDISTANCE + (1 + focusY * (cellLenght + 1)), determineCellColor(board,focusX,focusY), 2, true);
  
  if (board[focusX][focusY] == "") {
    if (focusX != 0) {
        tft.fillRect((1 + (focusX - 1) * (cellLenght + 2)), YDISTANCE + (1 + focusY * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX-1][focusY], (textOffset + 1 + (focusX - 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + focusY * (cellLenght + 1)), determineCellColor(board,focusX-1,focusY), 2, true);
    }
    if (focusX != 7) {
        tft.fillRect((1 + (focusX + 1) * (cellLenght + 2)), YDISTANCE + (1 + focusY * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX+1][focusY], (textOffset + 1 + (focusX + 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + focusY * (cellLenght + 1)), determineCellColor(board,focusX+1,focusY), 2, true);
    }
    if (focusY != 0) {
        tft.fillRect((1 + focusX * (cellLenght + 2)), YDISTANCE + (1 + (focusY - 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX][focusY-1], (textOffset + 1 + focusX * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY - 1) * (cellLenght + 1)), determineCellColor(board,focusX,focusY-1), 2, true);
    }
    if (focusY != 7) {
        tft.fillRect((1 + focusX * (cellLenght + 2)), YDISTANCE + (1 + (focusY + 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX][focusY+1], (textOffset + 1 + focusX * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY + 1) * (cellLenght + 1)), determineCellColor(board,focusX,focusY+1), 2, true);
    }
    if (focusX != 0 && focusY != 0) {
        tft.fillRect((1 + (focusX - 1) * (cellLenght + 2)), YDISTANCE + (1 + (focusY - 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX-1][focusY-1], (textOffset + 1 + (focusX - 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY - 1) * (cellLenght + 1)), determineCellColor(board,focusX-1,focusY-1), 2, true);    
    }
    if (focusX != 7 && focusY != 7) {
        tft.fillRect((1 + (focusX + 1) * (cellLenght + 2)), YDISTANCE + (1 + (focusY + 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX+1][focusY+1], (textOffset + 1 + (focusX + 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY + 1) * (cellLenght + 1)), determineCellColor(board,focusX+1,focusY+1), 2, true);    
    }
    if (focusX != 0 && focusY != 7) {
        tft.fillRect((1 + (focusX - 1) * (cellLenght + 2)), YDISTANCE + (1 + (focusY + 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX-1][focusY+1], (textOffset + 1 + (focusX - 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY + 1) * (cellLenght + 1)), determineCellColor(board,focusX-1,focusY+1), 2, true);        
    }
    if (focusX != 7 && focusY != 0) {
        tft.fillRect((1 + (focusX + 1) * (cellLenght + 2)), YDISTANCE + (1 + (focusY - 1) * (cellLenght + 2)), cellLenght, cellLenght, 0xC638);
        text(board[focusX+1][focusY-1], (textOffset + 1 + (focusX + 1) * (cellLenght + 1)), textOffset + YDISTANCE + (1 + (focusY - 1) * (cellLenght + 1)), determineCellColor(board,focusX+1,focusY-1), 2, true);    
    }
  }

  return true;
}

int determineCellColor(String board[][8], int i, int j) {
  String cell = board[i][j];
  if(board[i][j].equals("*")) {
    return ILI9341_BLACK;
  } else if(board[i][j].equals("1")) {
    return ILI9341_BLUE;
  } else if(board[i][j].equals("2")) {
    return ILI9341_GREEN;
  } else if(board[i][j].equals("3")) {
    return ILI9341_RED;
  } else if(board[i][j].equals("4")) {
    return ILI9341_PURPLE;
  }
  return ILI9341_ORANGE;
  
}
void drawFocus(int i, int j, int focusCellLenght, int yFocusDistance, boolean isColor) {
  int actualColor;
  if (isColor)
    actualColor = ILI9341_RED;
  else{
    actualColor = 0x8410;
  }
  tft.drawRect(i * focusCellLenght, yFocusDistance + j * focusCellLenght, focusCellLenght, focusCellLenght, actualColor);
}

int numOfMinesChooser() {
  int numOfMines = 4;
  String text1 =(String)numOfMines;
  bool numOfMinesChosen = false;
  text("Choose num of mines", width/2 - 114, height/2 - 32, ILI9341_WHITE, 2, false);
  text(text1, width/2 - 8, height/2 - 8, ILI9341_WHITE, 3, false);
  tft.fillTriangle(width/2 + 16 + 8, height/2, width/2 + 8 + 8, height/2 + 4 + 8, width/2 + 8 + 8, height/2 - 4 - 8, ILI9341_WHITE);
  tft.fillTriangle(width/2 - 16 - 8, height/2, width/2 - 8 - 8, height/2 - 4 - 8, width/2 - 8 - 8, height/2 + 4 + 8, ILI9341_WHITE);
  
  while (!numOfMinesChosen) {
    if (digitalRead(BUTTON_RIGHT) == HIGH || digitalRead(BUTTON_UP) == HIGH) {
      if (numOfMines == 9) {
        numOfMines = 9;
      } else {
        numOfMines ++;
      }
      text1 = (String)numOfMines;
      tft.fillRect(width/2 - 16, height/2 - 16, 32, 64, ILI9341_BLACK);
      text(text1, width/2 - 8, height/2 - 8, ILI9341_WHITE, 3, false);
      delay(200);
    }

    if (digitalRead(BUTTON_LEFT) == HIGH || digitalRead(BUTTON_DOWN) == HIGH) {
      if (numOfMines == 1) {
        numOfMines = 1;
      } else {
        numOfMines --;
      }
      text1 = (String)numOfMines;
      tft.fillRect(width/2 - 16, height/2 - 16, 32, 64, ILI9341_BLACK);
      text(text1, width/2 - 8, height/2 - 8, ILI9341_WHITE, 3, false);
      delay(200);
    }
    if (digitalRead(BUTTON_A) == HIGH) {
      numOfMinesChosen = true;
    }
  }
  return numOfMines;
}


//2048
#define TAMANOCUAD width/4
// Función para seleccionar un hueco libre aleatoriamente
int seleccionarHuecoLibre(int matriz[][4]) {
  int i, j;

  int huecosLibres[4 * 4];
  int cantidadHuecosLibres = 0;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (matriz[i][j] == 0) {
        huecosLibres[cantidadHuecosLibres] = i * 4 + j;  
        cantidadHuecosLibres++;
      }
    }
  }

  if (cantidadHuecosLibres > 0) {
    int indiceAleatorio = random(cantidadHuecosLibres);  
    return huecosLibres[indiceAleatorio];
  } else {
    return -1;
  }
}

//FUNCIÓN DE COMPROBACIÓN SI PODEMOS MOVER CADA CUADRADO A LA DIRECCION ESPECIFICADA:
bool checkColsFils(int dir, int tipo, int matriz[][4]) {
  if(tipo == 0){ //SI COMPROBAMOS VERTICALMENTE
      if(dir == 1){ //Movimiento hacia arriba
          for(int i=0;i<3;i++){
              for(int j=0;j<4;j++){
                  if(matriz[i][j] == matriz[i+1][j] || (matriz[i][j] == 0 && matriz[i+1][j] != 0)) return true;
              }
          }
      } else if(dir == -1){ //Movimiento hacia abajo
          for(int i=3;i>0;i--){
              for(int j=0;j<4;j++){
                  if(matriz[i][j] == matriz[i - 1][j] || (matriz[i][j] == 0 && matriz[i-1][j] != 0)) return true;
              }
          }
      } else if(dir == 0){ //FUNCIONALIDAD EXTRA PARA COMPROBAR SI HEMOS LLEGADO A 2048
        for(int i=0;i<4;i++){
              for(int j=0;j<4;j++){
                  if(matriz[i][j] == 2048) return true;
              }
          }
      }
  }else if(tipo == 1){ //SI COMPROBAMOS HORIZONTALMENTE
      if(dir == 1){ //Movimiento hacia la derecha
          for(int j=3;j>0;j--){
              for(int i=0;i<4;i++){
                  if(matriz[i][j] == matriz[i][j-1] || (matriz[i][j] == 0 && matriz[i][j-1] != 0)) return true;
              }
          }
      } else if(dir == -1){ //Movimiento hacia la izquierda
          for(int j=0;j<3;j++){
              for(int i=0;i<4;i++){
                  if(matriz[i][j] == matriz[i][j+1] || (matriz[i][j] == 0 && matriz[i][j+1] != 0)) return true;
              }
          }
      }
  }
  return false;
}

void dibujarCuadrado(int num, int x, int y) {
  y += YDISTANCE;
  switch (num) {
    case 2:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xCE16);
      text(String(num), x + 18, y + 15, 0x736C, 5, false);
      break;
    case 4:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEF19);
      text(String(num), x + 18, y + 15, 0x736C, 5, false);
      break;
    case 8:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xF58F);
      text(String(num), x + 18, y + 15, 0xFFBE, 5, false);
      break;
    case 16:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xF4AC);
      text(String(num), x + 8, y + 15, 0xFFBE, 4, false);
      break;
    case 32:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xF3EB);
      text(String(num), x + 8, y + 15, 0xFFBE, 4, false);
      break;
    case 64:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xF2E7);
      text(String(num), x + 8, y + 15, 0xFFBE, 4, false);
      break;
    case 128:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEE8E);
      text(String(num), x + 12, y + 24, 0xFFBE, 2, false);
      break;
    case 256:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEE6C);
      text(String(num), x + 12, y + 24, 0xFFBE, 2, false);
      break;
    case 512:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEE4A);
      text(String(num), x + 12, y + 24, 0xFFBE, 2, false);
      break;
    case 1024:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEE27);
      text(String(num), x + 6, y + 24, 0xFFBE, 2, false);
      break;
    case 2048:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xEE27);
      text(String(num), x + 6, y + 24, 0xFFBE, 2, false);
      break;
    default:
      tft.fillRect(x + 5, y + 5, TAMANOCUAD - 10, TAMANOCUAD - 10, 0xCE16);
      break; 
  }
}

void nuevoCuadAleatorio(int matriz[][4]){
  int hueco = seleccionarHuecoLibre(matriz);
  if (hueco != -1) {
    int num = random(5);
    if(num == 4){
      matriz[(hueco/4)][(hueco%4)] = 4;
    }else{
      matriz[(hueco/4)][(hueco%4)] = 2;
    }
  }
}

void imprimirMatriz(int matriz[][4]){  
  for(int i=0; i<4; i++){
    for(int j=0; j<4; j++){
      dibujarCuadrado(matriz[i][j], j*TAMANOCUAD, i*TAMANOCUAD);     
    }
  }
}

void ini2048() {
  tft.fillRect(0, YDISTANCE, width, width, 0xBD74);
  int matriz[4][4];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matriz[i][j] = 0;   
    }
  }
  
  int xinicial = random(0, 3);
  int yinicial = random(0, 3);
  matriz[yinicial][xinicial] = 2;

  int xinicial2 = random(0, 3);
  int yinicial2 = random(0, 3);
  matriz[yinicial2][xinicial2] = 2;
  
  imprimirMatriz(matriz);
  bool endGame = false;
  bool win = false;

  while (!endGame) {
    //Valores para asegurarnos que es posible mover los cuadrados
    bool movAr = checkColsFils(1, 0, matriz);
    bool movAb = checkColsFils(-1, 0, matriz);
    bool movD = checkColsFils(1, 1, matriz);
    bool movI = checkColsFils(-1, 1, matriz);

    //Si ya no podemos movernos hacia ningún lado, la partida termina
    if (movAr == false && movAb == false && movD == false && movI == false) {
      endGame = true;
      //fin(false);
    }



    if (digitalRead(BUTTON_UP) == HIGH) {
      if (movAr) {                                         //Comprobamos primero si se puede mover
        
        int sumaYaRealizada[4] = { -1, -1, -1, -1};  //Variable para evitar que se sume un numero al resultado de la suma de otro
        
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 4; j++) {

            if (matriz[i][j] == matriz[i + 1][j] && matriz[i][j] != 0) {  //Si tenemos dos numeros iguales se suman

              matriz[i][j] *= 2;
              matriz[i+1][j] = 0;
              sumaYaRealizada[j] = i;

            } else if (matriz[i][j] == 0 && matriz[i + 1][j] != 0) {  //Si tenemos un hueco, solo movemos el activo
            
              matriz[i][j] = matriz[i+1][j];
              matriz[i+1][j] = 0;
              if(i>=1){
                if ((matriz[i][j] == matriz[i - 1][j] && sumaYaRealizada[j] != i-1) || (matriz[i][j] != 0 && matriz[i-1][j] == 0)) {  //Si hay un hueco antes volvemos atrás en el bucle para rellenarlo. Si la suma ya ha sido realizada no volveremos atras en el bucle y no se sumaran
                  i = i - 1;
                  j = j - 1;
                }
              }
            
            }
          }
        }
        nuevoCuadAleatorio(matriz);
        imprimirMatriz(matriz);

      }

    } else if (digitalRead(BUTTON_DOWN) == HIGH) {
      if (movAb) {         
                                        //Comprobamos primero si se puede mover
        int sumaYaRealizada[4] = { -1, -1, -1, -1};  //Variable para evitar que se sume un numero al resultado de la suma de otro
        
        for (int i = 3; i > 0; i--) {
          for (int j = 0; j < 4; j++) {

            if (matriz[i][j] == matriz[i - 1][j] && matriz[i][j] != 0) {  //Si tenemos dos numeros iguales se suman

              matriz[i][j] *= 2;
              matriz[i-1][j] = 0;
              sumaYaRealizada[j] = i;

            } else if (matriz[i][j] == 0 && matriz[i - 1][j] != 0) {  //Si tenemos un hueco, solo movemos el activo

              matriz[i][j] = matriz[i - 1][j];
              matriz[i-1][j] = 0;

              if(i<=2){
                if ((matriz[i][j] == matriz[i + 1][j] && sumaYaRealizada[j] != i+1) || (matriz[i][j] != 0 && matriz[i+1][j] == 0)) {  //Si hay un hueco antes volvemos atrás en el bucle para rellenarlo. Si la suma ya ha sido realizada no volveremos atras en el bucle y no se sumaran
                  i = i + 1;
                  j = j - 1;
                }
              }
            }
          }
        }

        nuevoCuadAleatorio(matriz);
        imprimirMatriz(matriz);

      }
    } else if (digitalRead(BUTTON_RIGHT) == HIGH) {
      if (movD) {  //Comprobamos primero si se puede mover                    
                            
        int sumaYaRealizada[4] = { -1, -1, -1, -1};  //Variable para evitar que se sume un numero al resultado de la suma de otro
        
        for (int j = 3; j > 0; j--) {
          for (int i = 0; i < 4; i++) {

            if (matriz[i][j] == matriz[i][j - 1] && matriz[i][j] != 0) {  //Si tenemos dos numeros iguales se suman
            
              matriz[i][j] *= 2;
              matriz[i][j-1] = 0;
              sumaYaRealizada[i] = j;
            
            } else if (matriz[i][j] == 0 && matriz[i][j - 1] != 0) {  //Si tenemos un hueco, solo movemos el activo
            
              matriz[i][j] = matriz[i][j - 1];
              matriz[i][j-1] = 0;
              
               if(j<=2){
                if ((matriz[i][j] == matriz[i][j + 1] && sumaYaRealizada[i] != j+1) || (matriz[i][j] != 0 && matriz[i][j+1] == 0)) {  //Si hay un hueco antes volvemos atrás en el bucle para rellenarlo. Si la suma ya ha sido realizada no volveremos atras en el bucle y no se sumaran
                  j = j + 1;
                  i = i - 1;
                }
              }
            }
          }
        }

        nuevoCuadAleatorio(matriz);
        imprimirMatriz(matriz);

      }
    } else if (digitalRead(BUTTON_LEFT) == HIGH) {
      if (movI) {                                          //Comprobamos primero si se puede mover
       
        int sumaYaRealizada[4] = { -1, -1, -1, -1}; //Variable para evitar que se sume un numero al resultado de la suma de otro
        
        for (int j = 0; j < 3; j++) {
          for (int i = 0; i < 4; i++) {

            if (matriz[i][j] == matriz[i][j + 1] && matriz[i][j] != 0) {  //Si tenemos dos numeros iguales se suman
             
              matriz[i][j] *= 2;
              matriz[i][j+1] = 0;
              sumaYaRealizada[i] = j;
            
            } else if (matriz[i][j] == 0 && matriz[i][j + 1] != 0) {  //Si tenemos un hueco, solo movemos el activo
            
              matriz[i][j] = matriz[i][j + 1];
              matriz[i][j+1] = 0;
              if(j>=1){
                if ((matriz[i][j] == matriz[i][j - 1] && sumaYaRealizada[i] != j-1) || (matriz[i][j] != 0 && matriz[i][j-1] == 0)) {  //Si hay un hueco antes volvemos atrás en el bucle para rellenarlo. Si la suma ya ha sido realizada no volveremos atras en el bucle y no se sumaran
                  j = j - 1;
                  i = i - 1;
                }
              }
            
            }
          }
        }

        nuevoCuadAleatorio(matriz);
        imprimirMatriz(matriz);
      }
    } else if (digitalRead(BUTTON_B) == HIGH) {
      if (exitGamePrompt("2048")){
        endGame = true;
      }
      delay(100);
    }


    if(checkColsFils(0, 0, matriz)){
      endGame = true;
      //fin(true);
      win = true;
    }
  }
  endGamePromptBoolean(win, "2048");
  
}




//3x3

#define GRID_SIZE 3
#define CELL_SIZE width / 3

//MÁRGENES
#define MARGIN_X 0
#define MARGIN_Y (height - width) / 2
void playGame3in3() {
  char grid[GRID_SIZE][GRID_SIZE];
  int jugador = 1;

  int cursorX = 0;
  int cursorY = 0;

  boolean endTurn = false;

  iniciarGRID(grid);
  drawGrid();
  drawCursor(cursorX, cursorY, ILI9341_GREEN);

  while (!gameOver(grid)) {
    endTurn = false;
    while (!endTurn) {

      if (digitalRead(BUTTON_RIGHT) == HIGH  && cursorX != GRID_SIZE - 1) {
        if (cursorX != 2) {
          cursorX++;
        }        
      }
      if (digitalRead(BUTTON_LEFT) == HIGH  && cursorX != 0) {
        if (cursorX != 0) {
          cursorX--;
        }
      }

      if (digitalRead(BUTTON_UP) == HIGH  && cursorY != 0) {
        if (cursorY != 0) {
          cursorY--;
        }
      }

      if (digitalRead(BUTTON_DOWN) == HIGH  && cursorY != GRID_SIZE - 1) {
        if (cursorY != 2) {
          cursorY++;
        }
      }

      if (digitalRead(BUTTON_A) == HIGH) {
          if(isValidMove(cursorX, cursorY, grid)){
            if (jugador == 1) {
              grid[cursorX][cursorY] = 'X';
              updateSquare(grid, cursorX, cursorY, jugador);
            } else if (jugador == 2) {
              grid[cursorX][cursorY] = 'O';
              updateSquare(grid, cursorX, cursorY, jugador);
            }
            
          }
          endTurn = true;
      }
      if (digitalRead(BUTTON_A) == HIGH) {
          if(isValidMove(cursorX, cursorY, grid)){
            if (jugador == 1) {
              grid[cursorX][cursorY] = 'X';
              updateSquare(grid, cursorX, cursorY, jugador);
              endTurn = true;
            } else if (jugador == 2) {
              grid[cursorX][cursorY] = 'O';
              updateSquare(grid, cursorX, cursorY, jugador);
              endTurn = true;
            }
            
          }
          
      }

      if (digitalRead(BUTTON_B) == HIGH) {
        if (exitGamePrompt("Match 3")){
          endGamePrompt("Match 3");
        }
        delay(100);
      }

      drawCursor(cursorX, cursorY, ILI9341_GREEN);
      delay(300);
      
      drawCursor(cursorX, cursorY, ILI9341_BLACK);
      delay(300);
      
    }  
      //updateCursor(posX,posY);
      
      //playerMove(grid,cursorX,cursorY);
      //drawGrid();
    jugador = switchPlayer(jugador);

    
  }
  if (endGamePrompt2P(jugador == 2, "Mantch 3")) {
    playGame3in3();
  }
  
  
}

void iniciarGRID(char grid[GRID_SIZE][GRID_SIZE]) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      grid[i][j] = ' ';
    }
  }
}

void drawGrid() {
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  // Calcula el ancho y alto reales del área de juego
  int actualWidth = GRID_SIZE * CELL_SIZE;
  int actualHeight = GRID_SIZE * CELL_SIZE;

  // Calcula el desplazamiento necesario para centrar el área de juego
  int offsetX = MARGIN_X;
  int offsetY = MARGIN_Y;
  tft.drawFastHLine(0, MARGIN_Y, width, ILI9341_WHITE);
  tft.drawFastHLine(0, MARGIN_Y + width, width, ILI9341_WHITE);
  tft.drawFastVLine(0, MARGIN_Y, width, ILI9341_WHITE);
  tft.drawFastVLine(width, MARGIN_Y + 1, width - 1, ILI9341_WHITE);
  tft.drawRect(0, 0, 10, -MARGIN_X, ILI9341_BLUE);
  for (int i = 1; i < GRID_SIZE; i++) {
    tft.drawFastVLine(offsetX + i * CELL_SIZE, offsetY, actualHeight, ILI9341_WHITE);
    tft.drawFastHLine(offsetX, offsetY + i * CELL_SIZE, actualWidth, ILI9341_WHITE);
  }
}

void drawFigures(char grid[GRID_SIZE][GRID_SIZE]){
  // Calcula el ancho y alto reales del área de juego
  int actualWidth = GRID_SIZE * CELL_SIZE;
  int actualHeight = GRID_SIZE * CELL_SIZE;

  // Calcula el desplazamiento necesario para centrar el área de juego
  int offsetX = (tft.width() - actualWidth) / 2 + MARGIN_X;
  int offsetY = (tft.height() - actualHeight) / 2 + MARGIN_Y;

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (grid[i][j] == 'X') {
        drawX(offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE);
      } else if (grid[i][j] == 'O') {
        drawO(offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE);
      }
    }
  }
}

void drawX(int x, int y) {
  x += CELL_SIZE / 4;
  y += CELL_SIZE / 4;

  tft.drawLine(x, y, x + CELL_SIZE / 2, y + CELL_SIZE / 2, ILI9341_WHITE);
  tft.drawLine(x + CELL_SIZE / 2, y, x, y + CELL_SIZE / 2, ILI9341_WHITE);
}

void drawO(int x, int y) {
  x += CELL_SIZE / 2;
  y += CELL_SIZE / 2;

  tft.drawCircle(x, y, CELL_SIZE / 4, ILI9341_WHITE);
}

bool isValidMove(int row, int col, char grid[GRID_SIZE][GRID_SIZE]) {
  return (/*row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE && */grid[row][col] == ' ');
}

int switchPlayer(int currentPlayer) {
  if (currentPlayer == 1){
    return 2;
  }else{
    return 1;
  }
}

bool gameOver(char grid[GRID_SIZE][GRID_SIZE]) {
  if (checkWinner(grid) != 0 || boardFull(grid)){
    return true;
  }else{
    return false;
  }
}

int checkWinner(char grid[GRID_SIZE][GRID_SIZE]) {
  for (int i = 0; i < GRID_SIZE; i++) {
    if (grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2] && grid[i][0] != ' ') { //fila seguida
      if(grid[i][0] == 'X'){
        return 1;
      }else{
        return 2;
      }
    }

    if (grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i] && grid[0][i] != ' ') { //columna seguida
      if(grid[i][0] == 'X'){
        return 1;
      }else{
        return 2;
      }
    }
  }

  if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2] && grid[0][0] != ' ') { //diagonal 1
    if(grid[0][0] == 'X'){
        return 1;
      }else{
        return 2;
      }
  }

  if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0] && grid[0][2] != ' ') { //diagonal 2
    if(grid[0][2] == 'X'){
        return 1;
      }else{
        return 2;
      }
  }

  return 0; // No hay ganador
}

bool boardFull(char grid[GRID_SIZE][GRID_SIZE]) {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      if (grid[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}

void updateSquare(char grid[GRID_SIZE][GRID_SIZE], int cursorX, int cursorY, int jugador) {
  tft.fillRect(1 + CELL_SIZE * cursorX, MARGIN_Y + 1 + CELL_SIZE * cursorY, CELL_SIZE - 2, CELL_SIZE - 2, ILI9341_BLACK);
  
  int textOffset = 16;
  if(jugador == 1) {
    text("X", cursorX*CELL_SIZE + textOffset, MARGIN_Y + cursorY*CELL_SIZE + textOffset, ILI9341_BLUE, 5, false);
  } else if(jugador == 2) {
    text("O", cursorX*CELL_SIZE + textOffset, MARGIN_Y + cursorY*CELL_SIZE + textOffset, ILI9341_RED, 5, false);
  }
}

void drawCursor(int cursorX, int cursorY, int color) {
  tft.drawRect(1 + CELL_SIZE * cursorX,MARGIN_Y + 1 + CELL_SIZE * cursorY, CELL_SIZE - 2, CELL_SIZE - 2, color);
}
