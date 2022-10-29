#include <assert.h>
#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void print_elapsed(clock_t* t)
{
    const clock_t t0 = *t;
    *t = clock();
    const double dt = (double) (*t - t0) / CLOCKS_PER_SEC;
    printf("... %0.6f seconds\n", dt);
}


void convert_f2_to_f4(const size_t n, const _Float16 *h, float *f)
{
    for (size_t i=0; i<n; ++i) {
        f[i] = h[i];
    }
}


int main(int argc, char *argv[])
{
    // Create 16-bit floating point type.
    hid_t f2_t = H5Tcopy(H5T_NATIVE_FLOAT);
    H5Tset_fields(f2_t, 15, 10, 5, 0, 10);
    H5Tset_ebias(f2_t, 15);
    H5Tset_size(f2_t, 2);

    // Create complex types.
    hid_t c4_t = H5Tcreate(H5T_COMPOUND, 4);
    H5Tinsert(c4_t, "r", 0, f2_t);
    H5Tinsert(c4_t, "i", 2, f2_t);

    hid_t c8_t = H5Tcreate(H5T_COMPOUND, 8);
    H5Tinsert(c8_t, "r", 0, H5T_NATIVE_FLOAT);
    H5Tinsert(c8_t, "i", 4, H5T_NATIVE_FLOAT);

    // Create some data.
    const size_t m = 10000;
    const size_t n = 1100;
    void *z4 = calloc(m * n, 4);
    assert(z4);

    void *z8 = calloc(m * n, 8);
    assert(z8);

    // Create a file.
    puts("create");
    printf("dims = {%ld, %ld}\n", m, n);
    const char fn[] = "/tmp/test.h5";
    hid_t f = H5Fcreate(fn, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    assert(f != H5I_INVALID_HID);

    // dataspace
    const hsize_t dims[] = {m, n};
    hid_t dspace = H5Screate_simple(2, dims, dims);
    assert(dspace != H5I_INVALID_HID);

    // dataset
    const char dset_name[] = "z";
    hid_t dset = H5Dcreate2(f, dset_name, c4_t, dspace, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);

    // track runtime
    clock_t t = clock();

    // write
    puts("write c4");
    H5Dwrite(dset, c4_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, z4);
    print_elapsed(&t);

    // read as c4 (no conversion)
    puts("read as c4");
    H5Dread(dset, c4_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, z4);
    print_elapsed(&t);

    puts("convert c4 to c8");
    convert_f2_to_f4(2 * m * n, z4, z8);
    print_elapsed(&t);

    // read as c8
    puts("read as c8");
    H5Dread(dset, c8_t, H5S_ALL, H5S_ALL, H5P_DEFAULT, z8);
    print_elapsed(&t);

    puts("done");
    H5Dclose(dset);
    H5Sclose(dspace);
    H5Fclose(f);
    free(z8);
    free(z4);
    H5Tclose(c8_t);
    H5Tclose(c4_t);
    H5Tclose(f2_t);
    return 0;
}
