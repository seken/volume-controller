// Sample main.cpp file. Blinks the built-in LED, sends a message out
// USART2, and turns on PWM on pin 2.

#include "wirish.h"

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
    init();
}

class Potentiometer {
	public:
		Potentiometer(HardwareSPI *port, unsigned int &count = 6);

		void setA(const char &value, unsigned int &chip = 0);
		void setB(const char &value, unsigned int &chip = 0);
		void set(const char &num, const char &value);
		void setAll(const char *values);

	private:
		HardwareSPI *m_spi;
		const unsigned int m_count;
		const static unsigned char CHANGE_ENABLED;
		const static unsigned char CHANGE_DISABLED;
};

Potentiometer::CHANGE_ENABLED = 0x00;
Potentiometer::CHANGE_DISABLED = 0xC0;

Potentiometer::Potentiometer(HardwareSPI *port) :
		m_spi(port),
		m_count(6) {}

void Potentiometer::setA(const char &value, unsigned int &chip) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if ((i/6) == chip && i%6 < 3) {
			buf[i] = value | CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	setAll(buf);
}

void Potentiometer::setB(const char &value, unsigned int &chip) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if ((i/6) == chip && i%6 >= 3) {
			buf[i] = value | CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	setAll(buf);
}

void Potentiometer::set(const char &num, const char &value) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if (num == i) {
			buf[i] = value | CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	}
	setAll(buf);
}

void Potentiometer::setAll(const char *values) {
	m_spi->write(buf, m_count);
}

int main(void) {
    /* Set up the LED to blink  */
    pinMode(BOARD_LED_PIN, OUTPUT);

    /* Send a message out USART2  */
    Serial2.begin(9600);
    Serial2.println("Hello world!");

    /* Send a message out the usb virtual serial port  */
    SerialUSB.println("Hello!");

    while (true) {
		toggleLED();
		delay(100);
    }

    return 0;
}
