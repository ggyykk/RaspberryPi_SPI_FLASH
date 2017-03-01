#include <stdio.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

extern void spi_flash_device_id(void);
extern void spi_flash_sector(int addr);
extern void spi_flash_page_write(char *output, int addr, int length);
extern void spi_flash_page_read(char *input, int addr, int length);
		
int main(void)	
{
	wiringPiSetup();
	int fd = wiringPiSPISetup(0, 1000000);
	if(fd < 0)
	{
		printf("Error Setup\n");		
		return(-1);
	}
	spi_flash_device_id();

	char buf_t[] = "spi_flash_test";
	int len = strlen(buf_t) + 1;
	char buf_r[256] = {0};
	spi_flash_sector_erase(0x00000);
	spi_flash_page_write(buf_t, 0x00000, len);
	spi_flash_page_read(buf_r, 0x00000, len);
	printf("%s\n", buf_r);

	return(0);
}

