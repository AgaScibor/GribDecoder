// GribDecoder.cpp

#include "GribDecoder.h"
#include <iostream>
#include <fstream>

GribDecoder::GribDecoder(const std::string& filename) : filename(filename) {}

void GribDecoder::decode() {
    std::ifstream grib(filename, std::ios::binary | std::ios::in);

    if (!grib.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku!\n";
        return;
    }

    char byte1, byte2, byte3, byte4;
    std::streampos position7777 = -1;
    std::streampos positionGRIB = -1;
    std::streampos start_position_of_section1 = -1;

    auto findGrib = [&]() {
        while (!grib.eof()) {
            grib.read(&byte1, 1);
            if (byte1 == 'G') {
                grib.read(&byte2, 1);
                grib.read(&byte3, 1);
                grib.read(&byte4, 1);
                if (byte2 == 'R' && byte3 == 'I' && byte4 == 'B') {
                    positionGRIB = grib.tellg();
                    std::cout << "Grib znajduje sie na pozycji: " << positionGRIB << "\n";
                    break;
                }
            }
        }
        };

    auto find7777 = [&]() {
        while (!grib.eof()) {
            grib.read(&byte1, 1);
            if (byte1 == '7') {
                grib.read(&byte2, 1);
                grib.read(&byte3, 1);
                grib.read(&byte4, 1);
                if (byte2 == '7' && byte3 == '7' && byte4 == '7') {
                    position7777 = grib.tellg();
                    std::cout << "7777 znajduje sie na pozycji: " << position7777 << "\n";
                    break;
                }
            }
        }
        };

    auto readMessageLength = [&]() {
        grib.read(&byte1, 1);
        grib.read(&byte2, 1);
        grib.read(&byte3, 1);
        int message_length = ((int)byte1 << 16) | ((int)byte2 << 8) | (int)byte3 & 0xff;
        std::cout << "Dlugosc wiadomosci: " << message_length << "\n";
        };

    auto readSection1Data = [&]() {
        if (start_position_of_section1 != -1) {
            grib.seekg(start_position_of_section1);
            char section1_data[100];
            grib.read(section1_data, 100);
            std::cout << "==  Sekcja 1  ==\n";
            std::cout << "Dlugosc sekcji 1: " << 100 << "\n";
            
        }
        else {
            std::cout << "Blad: Poczatkowa pozycja sekcji 1 nie zostala zapisana.\n";
        }
        };

    auto readSection2Data = [&]() 
        {
        
        };

    findGrib();
    find7777();

    if (positionGRIB != -1 && position7777 != -1) {
        int distance = position7777 - positionGRIB;
        std::cout << "Dystans miedzy 7777 a GRIB: " << distance << " bajt\n";
        start_position_of_section1 = positionGRIB ; 
    }
    else {
        std::cout << "Nie znaleziono wszystkich znacznikow.\n";
    }

    readMessageLength();
    readSection1Data();
    readSection2Data();

    grib.close();
}
