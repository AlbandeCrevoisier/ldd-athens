/* Read some values of the accelerometer adxl345 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stropts.h>
#include <sys/file.h>

int
main(int argc, char *argv[])
{
	int ret = 0;
	int fd = 0;
	int8_t buf[2];

	if (argc != 2) {
		printf("Usage: ./app CHANNEL\n");
		return -EINVAL;
	}

	fd = open("/dev/adxl345", O_RDWR);
	if (!fd) {
		printf("Opening /dev/adxl345 failed: %d\n", errno);
		return 0;
	}

	ret = ioctl(fd, 4242, argv[1][0]);
	if (ret == -1) {
		printf("ioctl failed: %d\n", errno);
		return 0;
	}

	ret = read(fd, buf, 2);
	if (ret < 0) {
		printf("Reading /dev/adxl345 failed: %d\n", errno);
		return 0;
	}

	printf("%d %d\n", (int) buf[0], (int) buf[1]);

	return 0;
}
