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
		Potentiometer(HardwareSPI *port, const unsigned int &pin, const unsigned int &count = 6);

		void setA(const char &value, const unsigned int &chip = 0);
		void setB(const char &value, const unsigned int &chip = 0);
		void set(const char &num, const char &value);
		void setAll(const unsigned char *values);

	private:
		HardwareSPI *m_spi;
		const unsigned int m_count;
		const unsigned int m_pin;
		const static unsigned char CHANGE_ENABLED;
		const static unsigned char CHANGE_DISABLED;
};

const unsigned char Potentiometer::CHANGE_ENABLED = 0x3F;
const unsigned char Potentiometer::CHANGE_DISABLED = 0xC0;

Potentiometer::Potentiometer(HardwareSPI *port, const unsigned int &pin, const unsigned int &count) :
		m_spi(port),
		m_count(6),
		m_pin(pin) {}

void Potentiometer::setA(const char &value, const unsigned int &chip) {
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

void Potentiometer::setB(const char &value, const unsigned int &chip) {
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

void Potentiometer::setAll(const unsigned char *values) {
	digitalWrite(m_pin, HIGH);
	delay(1);
	m_spi->write(values, m_count);
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

unsigned int last_update_time;
bool watchdog_enable;

void handler_timeout() {
	if (!watchdog_enable) return;

	if (millis() - last_update_time > 60000) {
		Serial2.println("Watchdog timeout");
		power_off();
		last_update_time = millis() - 60000;
	}
}

inline bool isdigit(const unsigned char &val) {
	return (val >= '0' && val <= '9');
}

int read_command() {
	bool vol_valid = false;
	int vol = -10;
	unsigned char character;

	while ( (character = Serial2.read()) != '\n') {
		Serial2.print((char) character);
		if (!vol_valid) {
			if (character == 'w') {
				vol = -1;
				break;

			} else if (character == 'i') {
				watchdog_enable = true;
				break;

			} else if (character == 'o') {
				watchdog_enable = false;
				power_on();
				break;

			} else if (character == 'v') {
				vol_valid = true;
				vol = 0;

			} else {
				break;
			}
		} else {
			if (isdigit(character)) {
				vol = vol*10 + character-'0';
			}
		}

		while(Serial2.available() <= 0);
	}

	return vol;
}

int main(void) {
	last_update_time = 0;
	watchdog_enable = true;

	/* Setup the power pin */
	pinMode(15, OUTPUT);
	
	/* Setup the potentiometer comms */
	pinMode(9, OUTPUT);
	digitalWrite(9, HIGH);	
	HardwareSPI spi(1);
	Potentiometer pots(&spi, 9);
	spi.begin(SPI_1_125MHZ, LSBFIRST, SPI_MODE_0);
	
	/* Setup the watchdog countdown */
	HardwareTimer timer(2);
	timer.pause();
	timer.setPeriod(1000000);
	/* TODO update to newest api */
	timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer.setCompare(TIMER_CH1, 1);
	timer.attachCompare1Interrupt(handler_timeout);
	timer.refresh();
	timer.resume();

    /* Send a message out USART2  */
    Serial2.begin(57600);
    Serial2.println("Volume Controller starting...");

    while (true) {
		if (Serial2.available() > 0) {
			int vol = read_command();
			Serial2.print("Read command as: ");
			Serial2.println(vol);
			if (vol == -1) {
				power_on();
				last_update_time = millis();
			} else if (vol >= 0) {
				power_on();
				if (vol > 63) vol = 63;
				pots.setA(vol);
				last_update_time = millis();
			}
		}
    }

    return 0;
}
