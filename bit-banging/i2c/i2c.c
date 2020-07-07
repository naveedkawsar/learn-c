// This implementation is largely inspired by Jack Ganssle's
// video on bit-banging I2C using two GPIO lines.
// See original video:
// https://www.youtube.com/watch?v=VfpLyJvaBmc
// His source code:
// http://www.ganssle.com/video/episode8-code.txt
// The Wikipedia entry for I2C is good reference material:
// https://en.wikipedia.org/wiki/I%C2%B2C

#include "i2c.h"

enum pin_value
{
    LOW = 0,
    HIGH = 1
};

#if DEBUG
// The idea to plot the state of each serial line when any
// one transitions (either 0 -> 1 or 1 -> 0) in Excel comes from
// EEVblog's bit-banging tutorial:
// https://www.youtube.com/watch?v=fm13tIe5wSc
// Source code: https://gitlab.com/Sepps/generic-bitbang-library-v1


#include <stdio.h>
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
struct i2c_bus i2c_bus_values[1024];
int32_t i2c_value_index;

// There is only one debug array, which holds every I2C
// SDA and SCL transitions.  This assumes that only one
// i2c_bus is being used.  If testing multiple i2c_bus, make
// i2c_bus_values into a strucuture with the buffer and index
// as members.  Then the debug functions will take a pointer
// to the instance of the i2c_bus_values object
void reset_i2c_debug_array(void)
{
    int32_t i;
    for (i = 0; i < ARRAYSIZE(i2c_bus_values); i++)
    {
        i2c_bus_values[i].sda = HIGH;
        i2c_bus_values[i].scl = HIGH;
    }
    i2c_value_index = 1; // First frame has default pulled up SDA, SCL
}

void update_i2c_debug_array(const struct i2c_bus *i2c)
{
    if (i2c_value_index < ARRAYSIZE(i2c_bus_values))
    {
        i2c_bus_values[i2c_value_index].sda = i2c->sda;
        i2c_bus_values[i2c_value_index].scl = i2c->scl;
        i2c_value_index++;
    }
}

void print_i2c_debug_array(void)
{
    int32_t i;
    puts("SDA:");
    for (i = 0; i < i2c_value_index; i++)
    {
        printf("%d, ", i2c_bus_values[i].sda);
    }
    puts("");
    puts("SCL:");
    for (i = 0; i < i2c_value_index; i++)
    {
        printf("%d, ", i2c_bus_values[i].scl);
    }
}
#endif

void set_scl(struct i2c_bus *i2c, enum pin_value value)
{
    if (value <= HIGH) // Clamping
    {
        i2c->scl = value;
#if DEBUG
    update_i2c_debug_array(i2c);
#endif
    }
}

enum pin_value get_scl(const struct i2c_bus *i2c)
{
    return (enum pin_value)i2c->scl;
}

void set_sda(struct i2c_bus *i2c, enum pin_value value)
{
    if (value <= HIGH) // Clamping
    {
        i2c->sda = value;
    }
#if DEBUG
    update_i2c_debug_array(i2c);
#endif
}

// Unused, since we don't try to read data from the bus
enum pin_value get_sda(const struct i2c_bus *i2c)
{
    return (enum pin_value)i2c->sda;
}

void toggle_scl(struct i2c_bus *i2c)
{
    while (get_scl(i2c) != HIGH)
    {
        set_scl(i2c, HIGH);
    }
    set_scl(i2c, LOW); // No 
}

// Start condition is signaled by SDA being pulled low while SCL stays high.
// SCL is pulled low so SDA can set the first data bit level while keeping SCL low 
void i2c_start(struct i2c_bus *i2c)
{
    set_sda(i2c, HIGH);
    set_scl(i2c, HIGH);
    set_sda(i2c, LOW);
    set_scl(i2c, LOW);
}

// Stop condition is signaled when SCL rises, followed by SDA rising.
void i2c_stop(struct i2c_bus *i2c)
{
    set_scl(i2c, LOW);
    set_sda(i2c, LOW);
    set_scl(i2c, HIGH);
    set_sda(i2c, HIGH);
}

void i2c_write_byte(struct i2c_bus *i2c, uint8_t byte)
{
    uint32_t i; // Only need to iterate over 8 bits
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80) // Transmit MSB first
        {
            set_sda(i2c, HIGH);
        }
        else
        {
            set_sda(i2c, LOW);
        }
        toggle_scl(i2c);
        byte <<= 1; // Shift left to transmit next significant bit
    }
    // Fake ACK from slave
    set_sda(i2c, LOW);
    // CLK in fake ACK
    toggle_scl(i2c);
}

void i2c_write_msg(struct i2c_bus *i2c, const uint8_t *data, int32_t num_bytes)
{
    int32_t i;
    if (num_bytes > 0)
    {
        i2c_start(i2c);
        for (i = 0; i < num_bytes; i++)
        {
            i2c_write_byte(i2c, data[i]);
        }
        i2c_stop(i2c);
    }
}