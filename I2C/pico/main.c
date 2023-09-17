#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/multicore.h"

// I2C slave address
#define SLAVE_ADDRESS 0x10

// Function that runs on core 1 (heartbeat)
void core1_entry() {
    // onboard led init
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // blink 2Hz
    while (1) {
        gpio_put(25, 1);
        sleep_ms(500);
        gpio_put(25, 0); 
        sleep_ms(500); 
    }
}

// main function on core 0
int main() {
  stdio_init_all();

  // launch 2nd core function
  multicore_launch_core1(core1_entry);

  // i2c init
  i2c_init(i2c1, 100000);

  // gpio init
  gpio_set_function(19, GPIO_FUNC_I2C); // SDA
  gpio_set_function(18, GPIO_FUNC_I2C); // SCL
  gpio_pull_up(19);
  gpio_pull_up(18);

  // data to send
  uint8_t data = 'H';

  while (1) {
    // write the data 
    i2c_write_blocking(i2c1, SLAVE_ADDRESS, &data, 1, false);
    sleep_ms(3000);
  }
}
