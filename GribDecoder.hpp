// GribDecoder.h

#ifndef GRIBDECODER_H
#define GRIBDECODER_H

#include <string>

class GribDecoder {
public:
    GribDecoder(const std::string& filename);
    void decode();

private:
    std::string filename;
    int start_position_of_section1;

    void findGrib();
    void find7777();
    void readMessageLength();
    void readSection1Data();
    void readSection2Data();
};

#endif // GRIBDECODER_H
