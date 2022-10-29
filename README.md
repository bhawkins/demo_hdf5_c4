# Complex Float16 Conversion with HDF5

This program demonstrates the slowdown that occurs when using libhdf5
to read data stored as complex float16 into a complex float32 memory buffer.

## Build and Run

```
mkdir build
cd build
cmake ..
./demo_hdf5_c4
```

## Output

```
create
dims = {10000, 1100}
write c4
... 0.023903 seconds
read as c4
... 0.008729 seconds
convert c4 to c8
... 0.105132 seconds
read as c8
... 3.450837 seconds
done
```
