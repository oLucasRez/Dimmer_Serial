#include <mbed.h>

enum State
{
  UP,
  DOWN,
  MAX,
  MIN
};
State state = UP;

DigitalOut blue_led(A2);
DigitalOut orange_led(A3);
PwmOut light(A4);
volatile char c = '\0';
Serial pc(USBTX, USBRX, 9600);

Timeout timeout_led;

void to_up();
void to_down();
void to_max();
void to_min();

void clear_all();

void onCharReceived()
{
  c = pc.getc();
}

int main()
{
  pc.attach(&onCharReceived);
  while (1)
  {
    if (c != '\0')
    {
      switch (c)
      {
      case '+':
        if (light + 0.05 == 1)
          to_max();
        else
        {
          to_up();
          light = light + 0.05;
        }
        pc.printf("LED with %d of intensity\n", light);
        break;
      case '-':
        if (light - 0.05 == 0)
          to_min();
        else
        {
          to_down();
          light = light - 0.05;
        }
        pc.printf("LED with %d of intensity\n", light);
        break;
      case '*':
        to_max();
        light = 1;
        pc.printf("LED with %d of intensity\n", light);
        break;
      case '/':
        to_min();
        light = 0;
        pc.printf("LED with %d of intensity\n", light);
        break;
      default:
        pc.printf("input error \n");
      }
      c = '\0';
    }
    wait_ms(100);
  }
}

void to_up()
{
  state = UP;
  printf("state UP\n");

  clear_all();
  blue_led = 1;
}

void to_down()
{
  state = DOWN;
  printf("state DOWN\n");

  clear_all();
  orange_led = 1;
}

void blink_blue_led_5Hz()
{
  blue_led = 1;
  wait(0.1);
  blue_led = 0;
  if (state == MAX)
    timeout_led.attach(&blink_blue_led_5Hz, 0.1);
}

void to_max()
{
  state = MAX;
  printf("state MAX\n");

  clear_all();
  blink_blue_led_5Hz();
}

void blink_orange_led_5Hz()
{
  orange_led = 1;
  wait(0.1);
  orange_led = 0;
  if (state == MIN)
    timeout_led.attach(&blink_orange_led_5Hz, 0.1);
}

void to_min()
{
  state = MIN;
  printf("state MIN\n");

  clear_all();
  blink_orange_led_5Hz();
}

void clear_all()
{
  blue_led = 0;
  orange_led = 0;
  timeout_led.detach();
}