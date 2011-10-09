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
		Potentiometer(HardwareSPI *port, const unsigned int &pin, unsigned int &count = 6);

		void setA(const char &value, unsigned int &chip = 0);
		void setB(const char &value, unsigned int &chip = 0);
		void set(const char &num, const char &value);
		void setAll(const char *values);

	private:
		HardwareSPI *m_spi;
		const unsigned int m_count;
		const unsigned int m_pin;
		const static unsigned char CHANGE_ENABLED;
		const static unsigned char CHANGE_DISABLED;
};

Potentiometer::CHANGE_ENABLED = 0x3F;
Potentiometer::CHANGE_DISABLED = 0xC0;

Potentiometer::Potentiometer(HardwareSPI *port) :
		m_spi(port),
		m_count(6),
		m_pin(pin) {}

void Potentiometer::setA(const char &value, unsigned int &chip) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if ((i/6) == chip && i%6 < 3) {
			buf[i] = value & CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	}
	setAll(buf);
}

void Potentiometer::setB(const char &value, unsigned int &chip) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if ((i/6) == chip && i%6 >= 3) {
			buf[i] = value & CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	}
	setAll(buf);
}

void Potentiometer::set(const char &num, const char &value) {
	unsigned char buf[m_count];
	for (unsigned int i = 0; i < m_count; ++i) {
		if (num == i) {
			buf[i] = value & CHANGE_ENABLED;
		} else {
			buf[i] = CHANGE_DISABLED;
		}
	}
	setAll(buf);
}

void Potentiometer::setAll(const char *values) {
	digitalWrite(m_pin, HIGH);
	delay(1);
	m_spi->write(buf, m_count);
	delay(1);
	digitalWrite(m_pin, LOW);
	delay(2);
	digitalWrite(m_pin, HIGH);
}

inline void power_on() {
	digitalWrite(15, HIGH);
}

inline void power_off() {
	digitalWrite(15, LOW);
}

int main(void) {
	/* Setup the power pin */
	pinMode(15, OUTPUT);
	
	/* Setup the potentiometer comms */
	pinMode(9, OUTPUT);
	digitalWrite(9, HIGH);	
	HardwareSPI spi(1);
	Potentiometer pots(&spi, 9);
	spi.begin(SPI_1_125MHZ, LSBFIRST, SPI_MODE_0);

    /* Send a message out USART2  */
    Serial2.begin(9600);
    Serial2.println("Hello world!");

    /* Send a message out the usb virtual serial port  */
    SerialUSB.println("Hello!");

    while (true) {
		power_on();
		delay(3000);
		power_off();
		delay(3000);
    }

    return 0;
}
