#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

static struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{"off", no_argument, 0, 'o'},
	{"watchdog", no_argument, 0, 'w'},
	{"volume", required_argument, 0, 'v'},
	{"device", required_argument, 0, 'd'},
	{"disable-watchdog", required_argument, 0, 'l'},
	{"enable-watchdog", required_argument, 0, 'm'},
};

const char *package_string = PACKAGE_STRING;

void usage() {
	fprintf(stderr, "%s\n\n", package_string);
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  --help                  This message\n");
	fprintf(stderr, "  -d, --device=NAME       The device name to communicate with\n");
	fprintf(stderr, "  -w, --watchdog          Keep the device on (default)\n");
	fprintf(stderr, "  -v, --volume=NUM        Set the volume level (0-63)\n");
	fprintf(stderr, "  -o, --off               Turn the device off\n");
	fprintf(stderr, "  -l, --disable-watchdog  Keep the device on\n");
	fprintf(stderr, "  -m, --enable-watchdog   Allow the device to turn off\n");
	exit(-2);
}

int main(int argc, char **argv) {

	char *device_name;
	int device_set = 0;
	int volume = 0;
	int volume_set = 0;
	int watch_dog = 1;
	int enable_watchdog = 0;
	int disable_watchdog = 0;

	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "hd:v:olmw", long_options, &option_index);

		if (c == -1) {
			break;
		}

		switch (c) {
			case 0:
				break;

			case 'o':
				volume_set = 1;
				volume = 0;
				watch_dog = 0;
				break;

			case 'w':
				watch_dog = 1;
				break;

			case 'd':
				device_set = 1;
				device_name = optarg;
				break;

			case 'v':
				volume_set = 1;
				volume = atoi(optarg);
				watch_dog = 0;
				break;

			case 'l':
				watch_dog = 1;
				disable_watchdog = 1;
				break;
			case 'm':
				watch_dog = 1;
				enable_watchdog = 1;
				break;

			default:
				fprintf(stderr, "Unknown option\n");
			case '?':
			case 'h':
				usage();
		}
	}

	int dev_handle;

	if (device_set == 0) {
		device_name = "/dev/maple";
	}
	dev_handle = open(device_name, O_RDWR | O_NOCTTY | O_NDELAY);

	if (dev_handle < 0) {
		fprintf(stderr, "Unable to connect to device: %s\n", device_name);
		exit(-1);
	}

	struct termios options;
	tcgetattr(dev_handle, &options);
	cfsetispeed(&options, B57600);
	cfsetospeed(&options, B57600);
	tcsetattr(dev_handle, TCSANOW, &options);
	
	int result = -1;

	if (watch_dog) {
		const char message[] = "w\n";
		result = write(dev_handle, &message, strlen(message));
	} else if (volume_set) {
		const char message[512] = {0};
		sprintf(message, "v%d\n", volume);
		result = write(dev_handle, &message, strlen(message));
	}

	if (enable_watchdog) {
		const char message[] = "i\n";
		result = write(dev_handle, &message, strlen(message));
	} else if (disable_watchdog) {
		const char message[] = "o\n";
		result = write(dev_handle, &message, strlen(message));
	}

	if (result < 0) {
		fprintf(stderr, "Unable to send signal.\n");
		exit(-1);
	}

	return 0;
}
