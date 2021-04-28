#include <RBDdimmer.h>
#include "var_pin.h"
dimmerLamp dimmer(LED1_PIN, zerocross);

//void TRIAC_CTRL();
void INC_CTRL();
void DEC_CTRL();
void setspeed();


//ICACHE_RAM_ATTR void TRIAC_CTRL() {
//  if ((millis() - last_interrupted) > SWITCH_INTERRUPT_DELAY) {
//    noInterrupts();
//    LED1_State = ! LED1_State;
//    if ( LED1_State == 1)
//    {
//      Sprintln("ON Buttton pressed");
//      digitalWrite(LED1_PIN, HIGH);
//      digitalWrite(main_led, HIGH);
//      fan_speed = 4;
//    }
//    else
//    {
//      Sprintln("OFF Buttton pressed");
//      digitalWrite(LED1_PIN, LOW);
//      digitalWrite(main_led, LOW);
//      fan_speed = 0;
//    }
//    setspeed();
//    Status[0] = LED1_State;
//    update_eeprom();
//    interrupts ();
//    last_interrupted = millis();
//  }
//  //  publisher(1);
//}

ICACHE_RAM_ATTR void INC_CTRL() {
  if ((millis() - last_interrupted) > INC_DEC_INTERRUPT_DELAY) {
    noInterrupts();
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(main_led, HIGH);
    two_sectimer = millis();

    int one_sec_flag = 0;
    int two_sec_flag = 1;
    while (digitalRead(INC_PIN) == HIGH)
    {
      if (two_sec_flag == 1 && millis() - two_sectimer > (Ai_DELAY_SECONDS * 2)) {
        if (fan_speed < 4)fan_speed++;
        two_sec_flag = 0;
        one_sectimer = millis();
        Sprintln("fan speed ++ on two sec interval");
        one_sec_flag = 1;
      }
      if (one_sec_flag == 1 && (millis() - one_sectimer > Ai_DELAY_SECONDS)) {
        if (fan_speed < 4)fan_speed++;
        Sprintln("fan speed ++ on one sec interval");
        one_sectimer = millis();
      }
      digitalWrite(inc_led, LOW);
      Sprintln("inc Buttton pressed");

    }
    digitalWrite(inc_led, HIGH);

    one_sec_flag = 0;
    if (fan_speed < 4)fan_speed++;
    setspeed();
  }
  interrupts ();
  last_interrupted = millis();

}

ICACHE_RAM_ATTR void DEC_CTRL() {
  if ((millis() - last_interrupted) > INC_DEC_INTERRUPT_DELAY) {
    noInterrupts();

    two_sectimer = millis();

    int one_sec_flag = 0;
    int two_sec_flag = 1;
    while (digitalRead(INC_PIN) == HIGH)
    {
      if (two_sec_flag == 1 && millis() - two_sectimer > (Ai_DELAY_SECONDS * 2)) {
        if (fan_speed > 0)fan_speed--;
        two_sec_flag = 0;
        one_sectimer = millis();
        one_sec_flag = 1;
        Sprintln("fan speed -- on two sec interval");
      }
      if (one_sec_flag == 1 && (millis() - one_sectimer > Ai_DELAY_SECONDS)) {
        if (fan_speed > 0)fan_speed--;
        Sprintln("fan speed -- on one sec interval");
        one_sectimer = millis();
      }
      digitalWrite(inc_led, LOW);
      Sprintln("inc Buttton pressed");

    }
    digitalWrite(inc_led, HIGH);
    if (one_sec_flag == 0 && two_sec_flag == 1) {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(main_led, LOW);
    }
    one_sec_flag = 0;

    setspeed();
  }
}


void setspeed() {
  switch (fan_speed) {
    case 0: {
        if (dim) {
          dim = 0;
          dimmer.begin(NORMAL_MODE, OFF);
          dimmer.setPower(dim);
        }
        break;
      }
    case 1: {
        dim = 25;
        dimmer.begin(NORMAL_MODE, ON);
        dimmer.setPower(dim);
        break;
      }
    case 2: {
        dim = 35;
        dimmer.begin(NORMAL_MODE, ON);
        dimmer.setPower(dim);
        break;
      }
    case 3: {
        dim = 60;
        dimmer.begin(NORMAL_MODE, ON);
        dimmer.setPower(dim);
        break;
      }
    case 4: {
        dim = 100;
        dimmer.begin(NORMAL_MODE, ON);
        dimmer.setPower(dim);
        break;
      }
    default : {
        dim = 100;
        dimmer.begin(NORMAL_MODE, ON);
        dimmer.setPower(dim);
        break;
      }
  }
}
