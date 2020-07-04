#include <stdint.h>

struct i2c_bus
{
    uint8_t sda;
    uint8_t scl;
};

void i2c_write_msg(struct i2c_bus *i2c, const uint8_t *data, int32_t num_bytes);
#if DEBUG
void reset_i2c_debug_array(void);
void print_i2c_debug_array(void);
#endif