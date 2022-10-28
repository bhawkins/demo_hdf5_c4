#include <stdio.h>
#include <hdf5.h>

int main(int argc, char* argv[])
{
    printf("hello\n");

    hid_t f2 = H5Tcopy(H5T_NATIVE_FLOAT);
    H5Tset_fields(f2, 15, 10, 5, 0, 10);
    H5Tset_ebias(f2, 15);
    H5Tset_size(f2, 2);

    hid_t c4 = H5Tcreate(H5T_COMPOUND, 4);
    H5Tinsert(c4, "r", 0, f2);
    H5Tinsert(c4, "i", 2, f2);

    H5Tclose(c4);
    H5Tclose(f2);
    return 0;
}
