#define ENCODER_DO_NOT_USE_INTERRUPTS

#include <Arduino.h>
#include <Encoder.h>

static const int ROTARY_SW  = 2;
static const int ROTARY_CLK = 3;
static const int ROTARY_DT = 4;

static void encoder_switch_isr(void);
static void encoder_rotate_isr(void);

static Encoder rotary(ROTARY_DT, ROTARY_CLK);

static long rotVal = 0;

int getRotaryValue()
{
  static long lastEncoderPos = 0;
  long newEncoderPos = rotVal;
  int diff = lastEncoderPos - newEncoderPos;
  if (diff != 0) {
    Serial.println("Alte Position: " + String(lastEncoderPos) + " Neue Position: " + String(newEncoderPos) + " diff: " + String(diff));
    lastEncoderPos = newEncoderPos;
  }
  return diff;
}

void setup()
{
  pinMode(ROTARY_SW, INPUT_PULLUP);
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ROTARY_SW), encoder_switch_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_CLK), encoder_rotate_isr, FALLING);
  Serial.begin(115200);
}

volatile bool flag = false;

void loop()
{
#if 0
  static bool x = false;
  Serial.println("Hello World");
  delay(500);

  digitalWrite(LED_BUILTIN, x ? HIGH : LOW);
  x = !x;
#endif
  if (flag) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    flag = false;
  }
  getRotaryValue();
}

static void encoder_switch_isr(void)
{
  int counter = 0;
  for(int i = 0; i < 20; i++) {
    if(digitalRead(ROTARY_SW) == 0) {
      counter++;
    } else {
      counter = 0;
    }
    delay(4);
  }
  if(counter >= 5) {
    flag = true;
  }
}

static void encoder_rotate_isr(void)
{
  int counter_clk = 0;
  int counter_dt = 0;

  for (int i = 0; i < 100; i++) {
    if (!digitalRead(ROTARY_CLK)) {
      counter_clk++;
    } else {
      counter_clk = 0;
    }
    counter_dt += digitalRead(ROTARY_DT) ? -1 : 1;
    delayMicroseconds(500);
  }
  if (counter_clk >= 20) {
    rotVal += (counter_dt > 0) ? -1 : 1;
//    flag = true;
  }
}
