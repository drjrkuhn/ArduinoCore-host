#include "Common.h"

#if USE_BOOST_TIMING
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#define TIMELIB		boost

#else

#include <chrono>
#include <thread>

#define TIMELIB		std

#endif

namespace arduino {

using time_point = TIMELIB::chrono::time_point<TIMELIB::chrono::steady_clock>;
static time_point main_start_time = TIMELIB::chrono::steady_clock::now();

unsigned long millis() {
	using namespace TIMELIB;
	return static_cast<unsigned long>(TIMELIB::chrono::duration_cast<chrono::milliseconds>(TIMELIB::chrono::steady_clock::now() - main_start_time).count());
}

unsigned long micros() {
	using namespace TIMELIB;
	return static_cast<unsigned long>(TIMELIB::chrono::duration_cast<chrono::microseconds>(TIMELIB::chrono::steady_clock::now() - main_start_time).count());
}

void delay(unsigned long ms)
{
	using namespace TIMELIB;
	TIMELIB::this_thread::sleep_for(chrono::milliseconds(ms));
}

void delayMicroseconds(unsigned long us)
{
	TIMELIB::this_thread::sleep_for(TIMELIB::chrono::microseconds(us));
}

void yield(void) {
	TIMELIB::this_thread::yield();
}


/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }

void init(void) {}
void initVariant(void) {}

void pinMode(pin_size_t pinNumber, PinMode pinMode) {}
void digitalWrite(pin_size_t pinNumber, PinStatus status) {}
PinStatus digitalRead(pin_size_t pinNumber) { return LOW; }
int analogRead(pin_size_t pinNumber) { return 0; }
void analogReference(uint8_t mode) {}
void analogWrite(pin_size_t pinNumber, int value) {}


unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout) { return 0; }
unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout) { return 0; }

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val) {}
uint8_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder) { return 0; }

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode) {}
void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param) {}
void detachInterrupt(pin_size_t interruptNumber) {}

} // namespace