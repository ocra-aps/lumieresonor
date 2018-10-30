#include <picciled.h>

CRGB ** leds;

void fade_3pat(Pattern &interp_pat, Pattern p1, Pattern p2, Pattern p3, size_t counter);
bool fade_2pat(Pattern &interp_pat, Pattern p1, Pattern p2, size_t counter);

void setup() {
  init_all();

  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, 5>(leds[0], LED_PER_STRIP[0]);
  FastLED.addLeds<NEOPIXEL, 4>(leds[1], LED_PER_STRIP[1]);
  FastLED.addLeds<NEOPIXEL, 3>(leds[2], LED_PER_STRIP[2]);
}

size_t counter = 1, old_counter;
size_t timestep = 100; // in ms
bool active = false;

size_t mic_max = 0;
size_t sampling = 4;  // ogni 25
size_t thresh_mic_min = 930;
size_t thresh_mic_max = 1100;

Pattern current_pat, old_pat, rnd_pat;

void loop() {
  size_t mic_val = analogRead(0);

  // every 25 iterations
  // use the mic_max to trigger a pulse
  if( counter % sampling == 0 ){
    if( mic_max > thresh_mic_min && mic_max < thresh_mic_max ){
      Serial.print("max");
      Serial.println(mic_max);
      active = true;
      old_pat = current_pat;
      counter = 0;

      rnd_pat = palette.get_rnd();
    }
    mic_max = 0;
  }
  else{
    if( mic_val > mic_max ) mic_max = mic_val;
  }

  // led evolution
  if( active ){
    active = fade_2pat(current_pat, old_pat, rnd_pat, counter);
    counter++;
  }
  else{
    counter = old_counter;
    fade_3pat(current_pat, pnoise1, pnoise2, pnoise3, counter);
    counter++;
    old_counter = counter;
  }

  load_pattern(current_pat);
  FastLED.show();
  FastLED.delay(timestep);
}

/////////////////////////////// functions ///////////////////////////////
void fade_3pat(Pattern &interp_pat, Pattern p1, Pattern p2, Pattern p3, size_t counter){
  // time intervals
  size_t t1 = 2000; // fading time [ms]
  size_t s1 = 300;  // pause time [ms]
  size_t t2 = 2000; // fading time [ms]
  size_t s2 = 200;  // pause time [ms]
  size_t t3 = 3000; // fading time [ms]
  size_t s3 = 200;  // pause time [ms]

  // time parameters
  size_t time = counter * timestep;  // in ms
  size_t T1 = t1;
  size_t T2 = T1 + s1;
  size_t T3 = T2 + t2;
  size_t T4 = T3 + s2;
  size_t T5 = T4 + t3;
  size_t T6 = T5 + s3;
  size_t period = T6;
  size_t t = time % period; // ensure 0 <= t < period

  if (t < T1){
    interpolate_pattern(interp_pat,t,t1,p1,p2);
  }
  else if (t >= T1 && t < T2){
    interp_pat = p2;
  }
  else if (t >= T2 && t < T3){
    interpolate_pattern(interp_pat,t-T2,t2,p2,p3);
  }
  else if (t >= T3 && t < T4) {
    interp_pat = p3;
  }
  else if (t >= T4 && t < T5){
    interpolate_pattern(interp_pat,t-T4,t3,p3,p1);
  }
  else if (t >= T5 && t < T6) {
    interp_pat = p1;
  }
  else{
    interp_pat = all_off;
  }
}

bool fade_2pat(Pattern &interp_pat, Pattern p1, Pattern p2, size_t counter){
  // time intervals
  size_t t1 = 1000; // fading time [ms]
  size_t s1 = 150;  // pause time [ms]
  size_t t2 = 1000; // fading time [ms]

  // time parameters
  size_t time = counter * timestep;  // in ms
  size_t T1 = t1;
  size_t T2 = T1 + s1;
  size_t T3 = T2 + t2;
  size_t period = T3; // in ms
  size_t t = time % period; // per assicurare 0 <= t < period

  bool active = true;

  if (t < T1){
    interpolate_pattern(interp_pat,t,t1,p1,p2);
  }
  else if (t >= T1 && t < T2){
    interp_pat = p2;
  }
  else if (t >= T2 && t < T3 - timestep){
    interpolate_pattern(interp_pat,t-T2,t2,p2,p1);
  }
  else if (t >= T3 - timestep){
    interpolate_pattern(interp_pat,t-T2,t2,p2,p1);     // siccome il tempo incrementa di timestep alla volta
    active = false;                                    // serve un intervallo di chiusura
  }
  else{
    interp_pat = all_off;
  }

  return active;
}
