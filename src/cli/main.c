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

static struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{"off", no_argument, 0, 'o'},
	{"watchdog", no_argument, 0, 'w'},
	{"volume", required_argument, 0, 'v'},
	{"device", required_argument, 0, 'd'},
};

const char *package_string = PACKAGE_STRING;

void usage() {
	fprintf(stderr, "%s\n\n", package_string);
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  --help               This message\n");
	fprintf(stderr, "  -d, --device=NAME    The device name to communicate with\n");
	fprintf(stderr, "  -w, --watchdog       Keep the device on (default)\n");
	fprintf(stderr, "  -v, --volume=NUM     Set the volume level (in percent)\n");
	fprintf(stderr, "  -o, --off            Turn the device off\n");
	exit(-2);
}

int main(int argc, char **argv) {

	char *device_name;
	int device_set = 0;
	int volume = 0;
	int volume_set = 0;
	int watch_dog = 1;

	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "hd:v:o", long_options, &option_index);

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
	
	int result = -1;

	if (watch_dog) {
		const char message[] = "watchdog\n";
		result = write(dev_handle, &message, strlen(message));
	} else if (volume_set) {
		const char message[] = "volume %d\n";
		result = write(dev_handle, &message, strlen(message));
	}

	if (result < 0) {
		fprintf(stderr, "Unable to send signal.\n");
		exit(-1);
	}

	char buf[4];
	if (read(dev_handle, &buf, 4) != 4) {
		fprintf(stderr, "Device returned error\n");
		exit(-1);
	}

	return 0;
}
