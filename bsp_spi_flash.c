#include <wiringPiSPI.h>
#include <stdio.h>

unsigned char buffer[256];

void spi_flash_wait(void)
{
	char operationDone = 1;
	do
	{
		buffer[0] = 0x05;
		wiringPiSPIDataRW(0, buffer, 2);
		operationDone = (buffer[1]) & 0x01;
	}
	while(operationDone == 1);
}

void spi_flash_device_id(void)
{
	buffer[0] = 0x9f;
	wiringPiSPIDataRW(0, buffer, 4);
	printf("Manufacturer ID = %x\n", buffer[1]);
	printf("Memory Type = %x\n", buffer[2]);
	printf("Capacity = %x\n", buffer[3]);
	spi_flash_wait();

	buffer[0] = 0xab;
	wiringPiSPIDataRW(0, buffer, 5);
	printf("Device ID = %x\n", buffer[4]);
	spi_flash_wait();
}

void spi_flash_write_enable(void)
{
	buffer[0] = 0x06; 
	wiringPiSPIDataRW(0, buffer, 1);
	spi_flash_wait();
}

void spi_flash_sector_erase(int SectorAddr)
{
	spi_flash_write_enable();
	buffer[0] = 0x20;
	buffer[1] = (SectorAddr & 0xFF0000) >> 16;
	buffer[2] = (SectorAddr & 0xFF00) >> 8;
	buffer[3] = (SectorAddr & 0xFF);
	wiringPiSPIDataRW(0, buffer, 4);
	spi_flash_wait();
}

void spi_flash_page_write(char *output, int addr, int length)
{
	int count = 0;
	spi_flash_write_enable();
	buffer[0] = 0x02;
	buffer[1] = (addr & 0xFF0000) >> 16;
	buffer[2] = (addr & 0xFF00) >> 8;
	buffer[3] = (addr & 0xFF);
	for(count=0; count<length; count++)
	{
		buffer[count+4] = output[count];	
	}
	wiringPiSPIDataRW(0, buffer, length + 4);
	spi_flash_wait();
}

void spi_flash_buffer_write(char *output, int addr, int length)
{
	int pageNum = length / 256;
	int pageRemain = length % 256;
	if(pageNum == 0)
	{
		spi_flash_page_write(output, addr, length);
	}
	else
	{	
		while(pageNum--)
		{
			spi_flash_page_write(output, addr, 256);
			addr += 256;
			output += 256;	
		}
		spi_flash_page_write(output, addr, pageRemain);
	}
}

void spi_flash_page_read(char *input, int addr, int length)
{
	int count = 0;
	buffer[0] = 0x03;
	buffer[1] = (addr & 0xFF0000) >> 16;
	buffer[2] = (addr & 0xFF00) >> 8;
	buffer[3] = (addr & 0xFF);
	wiringPiSPIDataRW(0, buffer, length + 4);
	for(count=0; count<length;count++)
	{
		input[count] = buffer[count+4];
	}
	spi_flash_wait();
}


