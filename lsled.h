#include <FastLED.h>

typedef unsigned char channel_t;

typedef struct Color{
  channel_t r,g,b;
}Color;

typedef struct Pattern{
  Color c[3][2];
}Pattern;

Pattern all_off {
  0,0,0 , 0,0,0 ,
  0,0,0 , 0,0,0 ,
  0,0,0 , 0,0,0 ,
};

// Base patterns ///////////////////////////
Pattern pnoise1 = {
  0,42,42 , 0,42,42 ,
  0,204,255 , 0,204,255 ,
  145,232,255 , 145,232,255
};

Pattern pnoise2 = {
  145,232,255 , 145,232,255 ,
  0,42,42 , 0,42,42 ,
  0,204,255 , 0,204,255
};

Pattern pnoise3 = {
  0,204,255 , 0,204,255 ,
  145,232,255 , 145,232,255 ,
  0,42,42 , 0,42,42
};

// Pulse patterns ///////////////////////////
Pattern N1 = {
  255,0,0 , 255,0,0 ,
  255,0,0 , 255,0,0 ,
  255,0,0 , 255,0,0
};

Pattern N2 = {
  255,126,0 , 255,126,0 ,
  255,126,0 , 255,126,0 ,
  255,126,0 , 255,126,0
};

Pattern N3 = {
  255,255,0 , 255,255,0 ,
  255,255,0 , 255,255,0 ,
  255,255,0 , 255,255,0
};

Pattern N4 = {
  125,255,0 , 125,255,0 ,
  125,255,0 , 125,255,0 ,
  125,255,0 , 125,255,0
};

Pattern N5 = {
  0,255,0 , 0,255,0 ,
  0,255,0 , 0,255,0 ,
  0,255,0 , 0,255,0
};

Pattern N6 = {
  0,255,125 , 0,255,125 ,
  0,255,125 , 0,255,125 ,
  0,255,125 , 0,255,125
};

Pattern N10 = {
  125,0,255 , 125,0,255 ,
  125,0,255 , 125,0,255 ,
  125,0,255 , 125,0,255
};

Pattern N11 = {
  255,0,255 , 255,0,255 ,
  255,0,255 , 255,0,255 ,
  255,0,255 , 255,0,255
};

Pattern N12 = {
  255,0,125 , 255,0,125 ,
  255,0,125 , 255,0,125 ,
  255,0,125 , 255,0,125
};

Pattern N13 = {
  125,0,255 , 125,0,255 ,
  0,255,0 , 0,255,0 ,
  0,255,125 , 0,255,125
};

Pattern N15 = {
  255,0,0 , 255,0,0 ,
  125,255,0 , 125,255,0 ,
  255,125,0 , 255,125,0
};

Pattern N16 = {
  255,255,125 , 255,255,125 ,
  255,255,0 , 255,255,0 ,
  255,0,0 , 255,0,0
};

Pattern N17 = {
  255,125,0 , 255,125,0 ,
  255,0,0 , 255,0,0 ,
  125,125,255 , 125,125,255
};

Pattern N18 = {
  255,0,125 , 255,0,125 ,
  0,0,255 , 0,0,225 ,
  0,0,125 , 0,0,125
};

Pattern N19 = {
  125,0,0 , 125,0,0 ,
  0,255,255 , 0,255,255 ,
  125,0,125 , 125,0,125
};

Pattern N20 = {
  0,0,255 , 0,0,255 ,
  125,125,255 , 125,125,255 ,
  255,0,255 , 255,0,255
};

Pattern N21 = {
  0,255,255 , 0,255,255 ,
  125,255,125 , 125,255,125 ,
  255,0,125 , 255,0,125
};

Pattern N22 = {
  255,125,255 , 255,125,255 ,
  125,0,0 , 125,0,0 ,
  125,255,255 , 125,255,255
};

Pattern N23 = {
  255,0,255 , 255,0,255 ,
  125,125,125 , 125,125,125 ,
  255,0,0 , 255,0,0
};

Pattern N24 = {
  125,255,125 , 125,255,125 ,
  255,255,255 , 255,255,255 ,
  255,125,255 , 255,125,255
};

#define SIZE_PALETTE 26
struct Palette{
  Pattern pat[SIZE_PALETTE];
  unsigned char index = 0;
  Palette (){
    randomSeed(42);
    index = 0;
    pat[0] = N1;
    pat[1] = N12;
    pat[2] = N13;
    pat[3] = N4;
    pat[4] = N15;
    pat[5] = N16;
    pat[6] = N10;
    pat[7] = N2;
    pat[8] = N3;
    pat[9] = N18;
    pat[10] = N4;
    pat[11] = N5;
    pat[12] = N19;
    pat[13] = N6;
    pat[14] = N17;
    pat[15] = N11;
    pat[16] = N20;
    pat[17] = N21;
    pat[18] = N22;
    pat[19] = N23;
    pat[20] = N24;
  }
  Pattern get_rnd(){
    if ( index == SIZE_PALETTA ){
      index = 0;
      for(int i=1; i<SIZE_PALETTA; ++i){
        Pattern temp = pat[i];
        int j = random(i+1,SIZE_PALETTA);
        pat[i] = pat[j];
        pat[j] = temp;
      }
    }
    return pat[index++];
  }
} palette;

///////////////////////////////////////////////
// Variabili che contengono il numero di led
// per strip, inteso come numero di led connessi
// allo stesso pin digitale di arduino, e
// la sequenza dei pin stessi
// esempio:
// PIN D3 -> [ LED 1 ] [ LED 2 ]
// PIN D4 -> [ LED 3 ] [ LED 4 ]
// PIN D5 -> [ LED 5 ] [ LED 6 ]
//
// modificare per adattare al caso concreto
// ma attenzione alla consistenza dei valori
#define NUM_STRIPS 3
int LED_PER_STRIP[] = {2,2,2};
int PIN_PER_STRIP[] = {5,4,3};

// compatibilità
extern CRGB ** leds;

/////////////////////////////////  LED base function (edit carefully) /////////////////////////////////
// funzione di inizializzazione
void init_all(){
  leds = (CRGB **) malloc(NUM_STRIPS * sizeof(CRGB *) );
  for(int i=0; i<NUM_STRIPS; ++i){
    leds[i] = (CRGB *) malloc(LED_PER_STRIP[i] * sizeof(CRGB) );
  }
}

/////////////////////////////////  LED manipulation (edit carefully) /////////////////////////////////
// funzione inline di base per
// settare il colore del singolo led
// strip i posizione j
inline void set_led_color(int i, int j, int r, int g, int b){
  // dopo aver testato l'ordine dei canali
  // aggiornare questa funzione cambiando
  // l'ordine di c.r c.g c.b
  leds[i][j] = CRGB(g,r,b);
}

// overload della precedente
// per l'oggetto di tipo Color
inline void set_led_color(int i, int j, Color c){
  // dopo aver testato l'ordine dei canali
  // aggiornare questa funzione cambiando
  // l'ordine di c.r c.g c.b
  set_led_color(i, j, c.r, c.g, c.b);
}

// funzione che carica un
// pattern di colori
inline void load_pattern(Pattern pattern){
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < LED_PER_STRIP[i]; j++) {
      set_led_color(i,j, pattern.c[i][j]);
    }
  }
}

///////////////////////////////// FADING /////////////////////////////////
enum{
  INTERP_LIN,
  INTERP_SIN,
  INTERP_LOG
};

void interpolate_pattern(Pattern &interpolated, int time, int period, Pattern start, Pattern stop, char mode = INTERP_SIN){
  float x;
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < LED_PER_STRIP[i]; j++) {
      switch(mode){
        case INTERP_SIN:
          x = 0.5 * (1 - cos( M_PI * time / (float) period));
          interpolated.c[i][j].r = (float)(stop.c[i][j].r - start.c[i][j].r) * x + start.c[i][j].r;
          interpolated.c[i][j].g = (float)(stop.c[i][j].g - start.c[i][j].g) * x + start.c[i][j].g;
          interpolated.c[i][j].b = (float)(stop.c[i][j].b - start.c[i][j].b) * x + start.c[i][j].b;
          break;
        case INTERP_LIN:
          x = time / (float) period;
          interpolated.c[i][j].r = (float)(stop.c[i][j].r - start.c[i][j].r) * x + start.c[i][j].r;
          interpolated.c[i][j].g = (float)(stop.c[i][j].g - start.c[i][j].g) * x + start.c[i][j].g;
          interpolated.c[i][j].b = (float)(stop.c[i][j].b - start.c[i][j].b) * x + start.c[i][j].b;
          break;
        case INTERP_LOG:
          x = log(period+1.0) * log(time+1.0);
          interpolated.c[i][j].r = (float)(stop.c[i][j].r - start.c[i][j].r) * x + start.c[i][j].r;
          interpolated.c[i][j].g = (float)(stop.c[i][j].g - start.c[i][j].g) * x + start.c[i][j].g;
          interpolated.c[i][j].b = (float)(stop.c[i][j].b - start.c[i][j].b) * x + start.c[i][j].b;
          break;
      }
    }
  }
}
