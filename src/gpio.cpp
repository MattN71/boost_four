#include "gpio.h"


int32_t initPin(volatile uint32_t *gpio_base, uint8_t pin, enum GPIO_MODE mode, enum GPIO_DRIVE drive, enum GPIO_RESISTOR res, enum GPIO_SPEED speed) {
	//Set MODER[1:0] bits
	gpio_base[0] &= ~(0x3 << (2 * pin));
	gpio_base[0] |= (mode << (2 * pin));

	//Set OTYPER[0] bit
	gpio_base[1] &= ~(0x1 << pin);
	gpio_base[1] |= (drive << pin);

	//Set OSPEEDR[1:0] bits
	gpio_base[2] &= ~(0x3 << (2 * pin));
	gpio_base[2] |= (speed << (2 * pin));

	//Set PUPDR[1:0] bits
	gpio_base[3] &= ~(0x3 << (2 * pin));
	gpio_base[3] |= (res << (2 * pin));

	return 0;
}

int32_t initPinAlt(volatile uint32_t *gpio_base, uint8_t pin, enum GPIO_ALT_FUNC func) {
	volatile uint32_t *afr_base;
	if (pin < 8) {
		afr_base = &gpio_base[8]; //offset = 0x20;
	} else {
		afr_base = &gpio_base[9]; //offset = 0x24;
		pin -= 8;
	}

	*afr_base &= ~(0xF << (4 * pin));
	*afr_base |= (func << (4 * pin));

	return 0;
}



int32_t drivePin(volatile uint32_t *gpio_base, uint8_t pin, bool val) {

    if (((gpio_base[0] >> (pin * 2)) & 0x3) != GPIO_MODE_OUTPUT) {
        return -2; //Pin is not setup for output
    }

    gpio_base[6] |= (1 << (pin + (!val * 16)));

    return 0;
}


int32_t drivePinFast(volatile uint32_t *gpio_base, uint8_t pin, bool val) {

    gpio_base[6] |= (1 << (pin + (!val * 16)));

    return 0;
}




int32_t readPin(volatile uint32_t *gpio_base, uint8_t pin) {
	if (((gpio_base[0] >> (pin * 2)) & 0x3) != GPIO_MODE_INPUT) {
		return -2; //Pin is not setup for INPUT
	}

	//Cast to bool so that return value is either 0 or 1.
	return (bool)(gpio_base[4] & (1 << pin));
}

int32_t readPinFast(volatile uint32_t *gpio_base, uint8_t pin) {
	//Cast to bool so that return value is either 0 or 1.
	return (bool)(gpio_base[4] & (1 << pin));
}
