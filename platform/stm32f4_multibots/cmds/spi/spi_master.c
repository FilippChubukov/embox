/**
 * @file
 * @brief  SPI Master
 *
 * @date   23.06.18
 * @author Alexander Kalmuk
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <embox/unit.h>

#include "stm32f4_discovery.h"

#define SPI_SLAVE_SYNBYTE         0x53
#define SPI_MASTER_SYNBYTE        0xAC

#define SPI_TIMEOUT_MAX           1000000000

static SPI_HandleTypeDef SpiHandle;

static int spi_init(int spi_nr) {
	SPI_TypeDef *spi;

	switch (spi_nr) {
	case 1:
		spi = SPI1;
		break;
	case 2:
		spi = SPI2;
		break;
	default:
		return -1;
	}

	SpiHandle.Instance               = spi;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.Mode              = SPI_MODE_MASTER;
	
	if (HAL_SPI_Init(&SpiHandle) != HAL_OK) {
		printf("%s\n", "HAL_SPI_Init error\n");
		return -1;
	}

	return 0;
}

static void spi_delay(int n) {
	int i = n;
	while (i--)
		;
}

static void spi_sync(void) {
	uint8_t txackbytes = SPI_MASTER_SYNBYTE;
	uint8_t rxackbytes = 0x00;

	printf("Master is trying to sync\n");

	do {
		rxackbytes = 0x0;
		/* Call SPI write function to send command to slave */
		if (HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)&txackbytes, (uint8_t *)&rxackbytes, 1, SPI_TIMEOUT_MAX) != HAL_OK) {
			printf("%s\n", ">>> spi_sync error");
		}
	} while (rxackbytes != SPI_SLAVE_SYNBYTE);
}

static int spi_test(void) {
	uint8_t res = 0;
	uint8_t cmd = 0x57;

	printf("SPI test\n");

	while (1) {
		spi_delay(1000000);
		res = 0;
		// sync
		spi_sync();
		printf(">> synchronized\n");
		// request data
		if (HAL_SPI_Transmit(&SpiHandle, &cmd, 1, SPI_TIMEOUT_MAX) != HAL_OK) {
			printf("HAL SPI Transmit error\n");
			return -1;
		}
		// data from slave
		if (HAL_SPI_Receive(&SpiHandle, &res, 1, SPI_TIMEOUT_MAX) != HAL_OK) {
			printf("HAL_SPI_Receive error 1\n");
			return -1;
		}
		printf("0x%x ", res);
	}

	return 0;
}

static void init_leds() {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
}

static void print_usage(void) {
	printf("Usage: spi_slave [-h] -s <spi_nr>\n");
}

int main(int argc, char *argv[]) {
	int res, opt;
	int spi_nr;

	if (argc <= 1) {
		print_usage();
		return -EINVAL;
	}

	while (-1 != (opt = getopt(argc, argv, "hs:"))) {
		printf("\n");
		switch (opt) {
		case '?':
		case 'h':
		default:
			print_usage();
			return 0;
		case 's':
			if ((optarg == NULL) || (!sscanf(optarg, "%d", &spi_nr))) {
				print_usage();
				return 0;
			}
			break;
		}
	}

	init_leds();
	res = spi_init(spi_nr);
	if (res < 0) {
		return -1;
	}
    BSP_LED_Toggle(LED4);
	printf("SPI master started on SPI%d\n", spi_nr);
	spi_test();

	return 0;
}
