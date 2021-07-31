#ifndef gpio_h
#define gpio_h

#include <stdint.h>
#include <stdbool.h>

#define HIGH true
#define LOW false

enum GPIO_MODE {GPIO_MODE_OUTPUT = 0x1, GPIO_MODE_INPUT = 0x0, GPIO_MODE_ALTERNATE = 0x2, GPIO_MODE_ANALOG = 0x3};
enum GPIO_DRIVE {GPIO_DRIVE_PP = 0x0, GPIO_DRIVE_OD = 0x1};
enum GPIO_RESISTOR {GPIO_RESISTOR_NONE = 0x0, GPIO_RESISTOR_PULLUP = 0x1, GPIO_RESISTOR_PULLDOWN = 0x2};
enum GPIO_SPEED {GPIO_SPEED_LOW = 0x0, GPIO_SPEED_MED = 0x1, GPIO_SPEED_HIGH = 0x3};
enum GPIO_ALT_FUNC {AF0 = 0x0, AF1 = 0x1, AF2 = 0x2, AF3 = 0x3, AF4 = 0x4, AF5 = 0x5, AF6 = 0x6, AF7 = 0x7};


int32_t initPin(volatile uint32_t *gpio_base, uint8_t pin, enum GPIO_MODE mode, enum GPIO_DRIVE drive, enum GPIO_RESISTOR res, enum GPIO_SPEED speed);
int32_t initPinAlt(volatile uint32_t *gpio_base, uint8_t pin, enum GPIO_ALT_FUNC func);
int32_t drivePin(volatile uint32_t *gpio_base, uint8_t pin, bool val);
int32_t readPin(volatile uint32_t *gpio_base, uint8_t pin);


#endif //gpio_h
