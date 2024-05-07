// main.cpp

#include "GribDecoder.h"

int main() {
    GribDecoder decoder("all.grib");
    decoder.decode();

    return 0;
}
