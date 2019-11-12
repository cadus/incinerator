#include <Arduino.h>
#include <Encoder.h>

#define SW_ROTARY 2

static void encoder_switch_isr(void);

void setup()
{
  pinMode(SW_ROTARY, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(SW_ROTARY), encoder_switch_isr, FALLING);
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
}

static void encoder_switch_isr(void)
{
  int counter = 0;
  for(int i = 0; i < 20; i++) {
    if(digitalRead(SW_ROTARY) == 0) {
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
