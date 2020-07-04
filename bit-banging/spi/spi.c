#include <stdint.h>

#define LOW (0)
#define HIGH (1)

struct
{
    uint8_t cs;
    uint8_t clk;
    uint8_t mosi;
    uint8_t miso;
} Spi;

void assert_chip_select(struct Spi *spi)
{
    spi->cs = 0;
}

void release_chip_select(struct Spi *spi)
{
    spi->cs = 1;
}

// ____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____ CPOL = 0

// ‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾ CPOL = 1

//               |              |              |              |
//               |              |              |              |
// ‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾‾‾‾‾‾‾‾ CPOL = 1, CPHA = 1
//               |              |              |              |
//               |              |              |              |
// _________/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\____/‾‾‾‾\______ CPOL = 0, CPHA = 1