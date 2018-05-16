/*
  Continuous_Breakout
  
  Breakout game with no user input, just a ball bouncing. Used to test graphics.
  
  Trent Dye 2018
  
 */


#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <elapsedMillis.h>


#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
elapsedMillis timer0;
elapsedMillis timer1;
elapsedMillis timer2;

#define WIDTH 160
#define HEIGHT 128

#define PROCESS_INTERVAL 10
#define RENDER_INTERVAL 15

#define NUM_BLOCKS 24
#define ROWS 4
#define COLUMNS 6
#define BLOCK_WIDTH 25
#define BLOCK_HEIGHT 7
#define X_OFFSET 5
#define Y_OFFSET 15
#define TEXT_WIDTH 10
#define TEXT_HEIGHT 8

uint8_t w = 4;
uint8_t hw = w/2;
int x = WIDTH/2;
int y = HEIGHT/2;
int lastx = x;
int lasty = y;
int vx = 1;
int vy = 1; 

int score = 0;

int delVal = 20;

boolean blockExists[NUM_BLOCKS]; //the state of the block. the only data the block holds is whether it exists or not
boolean askToRender[NUM_BLOCKS]; //matches 1-to-1 with blocks, tells the renderer whether the block should be rendered (i.e. whether something has happened in the vicinity of the block)

int rowColors[] = {RED, MAGENTA, CYAN, BLUE}; //row index equals color

void setup() {
  //SCREEN INITIALIZATION
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  
  Serial.begin(115200);
//  Serial.println("Hello");
//  delay(1000);
  timer0 = 0;
  timer1 = 0;

  for(int i=0; i<NUM_BLOCKS; i++){
    blockExists[i] = true;
    askToRender[i] = true;
//    Serial.print(blockToXCoordinate(i));
//    Serial.print('\t');
//    Serial.println(blockToYCoordinate(i));
  }


  tft.setTextColor(WHITE, BLUE);
  tft.setTextSize(1);

  showScore(true);
}

void loop() {
  
  if(timer0 >= PROCESS_INTERVAL){
    timer0 = timer0 - PROCESS_INTERVAL;
    processTick();
    
  }

  if(timer1 >= RENDER_INTERVAL){
    timer1 = timer1 - RENDER_INTERVAL;
    renderTick();
    //Serial.println(millis());
  }


}

void renderTick(){
  renderBall();
  renderBlocks();
  
}

void processTick(){ //
  x = x + vx;
  y = y + vy;
  int n = coordinateToBlock(x, y);
  //if(n != -1) Serial.println(n);
  if(!detectWallCollision()){
    if(n != -1) detectBlockCollision(n);
  }
}

boolean detectWallCollision(){
  if(x >= WIDTH - hw){
    x = WIDTH-hw;
    vx = -vx;
    return true;
  }
  else if(x <= hw){
    x = hw;
    vx = -vx;
    return true;
  }
  if(y >= HEIGHT-hw){
    y = HEIGHT-hw;
    vy = -vy;
    return true;
  }
  else if(y <= hw){
    y = hw;
    vy = -vy;
    return true;
  }
  return false;
}

void detectBlockCollision(int n){
  if(!blockExists[n]){
    return;
  }
  int blockx = blockToXCoordinate(n);
  int blocky = blockToYCoordinate(n);
  int left = blockx;
  int right = blockx + BLOCK_WIDTH;
  int top = blocky;
  int bottom = blocky + BLOCK_HEIGHT;

  int buf = 1;

  
  if(x >= left && x <= right){ //LR collision
    if(y >= top && y <= bottom){ //UPDOWN collision
      if(x >= left && x < left + buf || x <= right && x > right - buf){
        vx = -vx;
      }
      else{
        vy = -vy;
      }
      blockExists[n] = false; 
      score++;
      showScore(true);
      askToRender[n] = true; 
    }
  }
}
void renderBall(){

  if(x-hw<TEXT_WIDTH && y-hw<TEXT_HEIGHT) return;   //don't render anything if the ball is currently in the space where the scoreboard is supposed to be
  tft.fillRect(lastx-hw, lasty-hw, w, w, BLACK);    //put a black square where the ball used to be
  tft.fillRect(x-hw, y-hw, w, w, RED);              //put a red square where the ball now is
  lastx = x;
  lasty = y;
  
}
void renderBlocks(){ // Iterating over the blocks, checks whether a block needs to be rendered, then checks whether or not it should exist (be a color) or not (be black)
  for(int i=0; i<NUM_BLOCKS; i++){
    if(askToRender[i] == true){
      if(blockExists[i] == true){
        tft.fillRect(blockToXCoordinate(i), blockToYCoordinate(i), BLOCK_WIDTH, BLOCK_HEIGHT, rowColors[i/COLUMNS]);
        tft.drawRect(blockToXCoordinate(i), blockToYCoordinate(i), BLOCK_WIDTH, BLOCK_HEIGHT, BLACK);
      }
      else{
        tft.fillRect(blockToXCoordinate(i), blockToYCoordinate(i), BLOCK_WIDTH, BLOCK_HEIGHT, BLACK);
      }
      askToRender[i] = false;
    }
  }
}

int coordinateToBlock(int x, int y){ // Given x and y coordinates of the ball, return the index (array) value of the block occupying this position

  // Check to see if we're in the area that has blocks:
  if(x < X_OFFSET || y < Y_OFFSET) return -1;
  if(x > X_OFFSET + COLUMNS * BLOCK_WIDTH || y > Y_OFFSET + ROWS * BLOCK_HEIGHT) return -1;  

  // Conversion algorithm to row/col
  int j = (y - Y_OFFSET)/BLOCK_HEIGHT;
  int i = (x - X_OFFSET)/BLOCK_WIDTH;

  // Conversion algorithm from row/col to index
  return COLUMNS*j + i;
  
}

int blockToXCoordinate(int block){ // Given index (array) value, give the left X value of the block
  return X_OFFSET + (block % COLUMNS) * BLOCK_WIDTH;
}

int blockToYCoordinate(int block){ // Given index (array) value, give the top Y value of the block
  return Y_OFFSET + (block / COLUMNS) * BLOCK_HEIGHT;
}

void showScore(boolean force){ // Prints onscreen how many blocks have been smashed

    tft.drawChar(0,0,'0'+score/10, WHITE, BLACK, 1);
    tft.drawChar(6,0,'0'+score%10, WHITE, BLACK, 1);

}

