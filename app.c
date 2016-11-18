/* Read some values of the accelerometer adxl345 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/file.h>

int
main(void)
{
	int ret = 0;
	int fd = 0;
	int8_t buf[2];

	fd = open("/dev/adxl345", O_RDWR);
	if (!fd) {
		printf("Opening /dev/adxl345 failed: %d\n", errno);
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
