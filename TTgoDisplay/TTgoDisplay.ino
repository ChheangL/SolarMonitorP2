//This code will first try to draw a coordinate graph for solar energy meter.:
// the Paramemter of the drawing function will be
/*
 * CoordinateGraph(h_min, h_max, v_min, v_max, title)
 * how do we calculate the size of the display"
 * #define M_SIZE 1
 * 
 */
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Button2.h"
#include "WiFi.h"
#include <Wire.h>

/////////////////////////////
#define BUTTON_1            35 //pin of left button
#define BUTTON_2            0  //pin of right button
Button2 btn1(BUTTON_1);        //define function of Btn1
Button2 btn2(BUTTON_2);        //define function of Btn2
int page = 0;                  //Home page
/*
 * the button function should be able to detect rebounce and pressing of the btn
 */
/////////////////////////////
/////////////////////////////
#define M_SIZE 1
#define MIN_T 23
#define MAX_T 23.01       //24hours
#define TFT_GREY 0x5AEB
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */ //the color use RGB565 formate
#define TFT_RED      0xF800
#define TFT_TGREY 0xEF5D
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint32_t updateTime = 0;       // time for next update
double sampling_rate = 0;
//////////////////////////////
void changePage(int8_t increment){
  if (page > MIN_PAGE || page < MAX_PAGE){
    page = page + increment;
  }
}
void button_init()
{
    btn1.setLongClickHandler([](Button2 & b) {
        btnCick = false;
        int r = digitalRead(TFT_BL);
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Press again to wake up",  tft.width() / 2, tft.height() / 2 );
        espDelay(6000);
        digitalWrite(TFT_BL, !r);

        tft.writecommand(TFT_DISPOFF);
        tft.writecommand(TFT_SLPIN);
        //After using light sleep, you need to disable timer wake, because here use external IO port to wake up
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
        // esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
        delay(200);
        esp_deep_sleep_start();
    });
    btn1.setPressedHandler([](Button2 & b) {
        Serial.println("Detect Voltage..");
        changePage(-1);
    });

    btn2.setPressedHandler([](Button2 & b) {
        changePage(1);
        Serial.println("btn press wifi scan");
        screen(page);
    });
}


//////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // For debug
  tft.init();
  tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);

  Draw_graph(90,100,10,MIN_T,MAX_T,2); // Draw analogue meter
  sampling_rate=(double)(MAX_T-MIN_T)/(224.0-25.0);
  updateTime = millis();
}
uint32_t iteration =0;
void loop() {
  if(updateTime<=millis()){
    updateTime = millis() + (sampling_rate*3600000);
    int value = random(0,100);
    plotPoint(value,iteration);
    iteration ++;
  }

}

void Draw_graph(float v_min, float v_max,uint8_t Vgaps,float h_min, float h_max,uint8_t Hgaps){
  tft.setTextColor(TFT_BLACK);
  //outline
  tft.fillRect(0, 0, M_SIZE*239, M_SIZE*131, TFT_GREY);
  tft.fillRect(1, M_SIZE*3, M_SIZE*234, M_SIZE*125, TFT_WHITE);
  uint16_t x0 = 20+5;     //position x-min
  uint16_t y0 = 10;     //position y-max
  uint16_t x1 = 234-10; //position x-max
  uint16_t y1 = 125-20; //position y-min

  uint16_t pix_gapV = 0;
  uint16_t pix_gapH = 0;
  //draw the horizontal line first
  tft.drawLine(x0-10,y1,x1+5,y1,TFT_BLACK);
  //draw the vertical line second
  tft.drawLine(x0,y0-5,x0,y1+10,TFT_BLACK);
  
  //draw tick on vertical line
  pix_gapV = (y1-y0)/(Vgaps);
  for(int i=y1; i>=y0;i-=pix_gapV){
    float gap_size = (v_max-v_min)/Vgaps;
    if (i!=y1){ 
      tft.drawLine(x0-4,i,x0+1,i,TFT_RED );
      tft.drawLine(x0+1,i,x1+1,i,TFT_TGREY ); //applying grids
    }
    tft.drawCentreString(String(v_min+((Vgaps+1)-i/pix_gapV)*gap_size,1), x0-15, i, 1);
  }
  //draw tick on horizontal line
  pix_gapH = (x1-x0)/(Hgaps);
  for(int i=x0; i<=x1;i+=pix_gapH){
    float gap_size = (h_max-h_min)/Hgaps;
    if (i!=x0) {
      tft.drawLine(i,y1-1,i,y1+4,TFT_RED);
      tft.drawLine(i,y0+3,i,y1-1,TFT_TGREY);//applying grids
    }
    tft.drawCentreString(String(h_min+((i-x0)/pix_gapH)*gap_size,1), i, y1+5, 1);
  }


}
uint16_t px = 20+5+1;
uint16_t py = 125-20-1;
void plotPoint(int value, uint32_t iterations){
  uint16_t y = map(value, 0,100,125-20-1,10);
  uint16_t x = iterations + 20+5+1;
  tft.drawLine(px,py,x,y,TFT_RED);
  if (x>234-10) {
    Draw_graph(90,100,10,MIN_T,MAX_T,2);
    iteration =0;
    px = 20+5+1;
    py = 125-20-1;
  }else{
    px=x;
    py=y;
  }
}
