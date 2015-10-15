/* extern "C" macro */
#ifndef C_DECL_BEGIN
    #ifdef __cplusplus
        #define C_DECL_BEGIN extern "C" {
        #define C_DECL_END }
    #else /* not c++ */
        #define C_DECL_BEGIN
        #define C_DECL_END
    #endif
#endif

#include "WProgram.h"

C_DECL_BEGIN

#include "util.h"
#include "command.h"

enum misc_t {
	SERIAL_BUFSIZE = 128,
};

enum status_t {
	STATUS_RESET = 0,
	STATUS_SEND  = 1,
};

enum serial_status_t {
	SERIAL_RESET    = 0,
	SERIAL_READING  = 1,
	SERIAL_COMPLETE = 2,
};

volatile int status = STATUS_RESET;

/* for serial reading */
volatile int serial_recv_size = 0, serial_status = SERIAL_RESET;
volatile char serial_recv_data[SERIAL_BUFSIZE];

void send_start()
{
	if (status == STATUS_RESET) {
		status = STATUS_SEND;
		serial_status = SERIAL_COMPLETE;
		serial_recv_size = 0;

		Serial1.println("send start button");
		strncpy((char *) serial_recv_data, "S.", 2);
		serial_recv_data[2] = '\0';
	}
}

void status_change()
{
	if (status == STATUS_RESET) {
		status = STATUS_SEND;
		Serial1.println("status changed");
	}
}

void init()
{
	/* internal LED */
    pinMode(13, OUTPUT);

	/* tact switch */
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);

	/* interrupt */
	attachInterrupt(14, send_start, FALLING);
	attachInterrupt(15, status_change, FALLING);

	/* serial */
	Serial1.begin(9600);

	memset((void *) serial_recv_data, 0, sizeof(char) * SERIAL_BUFSIZE);
}

void check_serial()
{
	char byte;

	if (Serial1.available()) {
		byte = Serial1.read();

		if (serial_recv_size >= SERIAL_BUFSIZE)
			serial_recv_size = 0;

		if (byte == '\n') {
			serial_status = SERIAL_COMPLETE;
			serial_recv_data[serial_recv_size] = '\0';
			serial_recv_size = 0;
			//Serial1.println("serial reading complete");
		} else {
			serial_status = SERIAL_READING;
			serial_recv_data[serial_recv_size] = byte;
			serial_recv_size++;
			//Serial1.println("serial reading...");
		}
	}
}

void blink(int pin, int wait)
{
	digitalWrite(pin, HIGH);
	delay(wait);
	digitalWrite(pin, LOW);
	delay(wait);
	digitalWrite(pin, HIGH);
	delay(wait);
	digitalWrite(pin, LOW);
	delay(wait);
}

int main(void)
{
	char command[SERIAL_BUFSIZE];

	init();
	blink(13, 500);

    while (1) {
		check_serial();

		if (serial_status == SERIAL_COMPLETE && status == STATUS_SEND) {
			status = STATUS_RESET;

			Serial1.println("sending command...");
			Serial1.println((char *) serial_recv_data);

			memset(command, 0, SERIAL_BUFSIZE);
			strncpy(command, (char *) serial_recv_data, strlen((char *) serial_recv_data));
			command_send((char *) command);

			digitalWrite(13, HIGH);
			delay(500);
		} else {
			digitalWrite(13, LOW);
		}
    }
}

C_DECL_END
