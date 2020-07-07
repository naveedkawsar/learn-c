#include "i2c.h"
#define ARRAYSIZE(x) (sizeof(x) / sizeof (x[0]))

int main(int argc, char **argv)
{
    struct i2c_bus i2c_test;
    uint8_t data_out[2] = "GO";
#if DEBUG
    reset_i2c_debug_array();
    i2c_write_msg(&i2c_test, data_out, ARRAYSIZE(data_out));
    print_i2c_debug_array();
#else
    i2c_write_msg(&i2c_test, data_out, ARRAYSIZE(data_out));
#endif
    return 0;
}