#include "../header_files/ExifManager.h"
#include <string.h>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <iostream>

using namespace std;

ExifManager::ExifManager(){
}

ExifManager::ExifManager(char* imageLocation){

    try{
        Exiv2::Image::AutoPtr test = Exiv2::ImageFactory::open(imageLocation);
        if(test.get() != 0 && test->good()){
            valid = true;
            image = test.release();
        }
        else{
            valid = false;
            cout << "[ERROR]: Could not open image does it exist!?" << endl;
        }
    }
    catch(exception){
        valid = false;
        cout << "[ERROR]: Could not open image does it exist!?" << endl;
    }

    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()){
         cout << "[ERROR]: No Exif Data in this file!" << endl;
    }

    else{
        Exiv2::ExifData::const_iterator end = exifData.end();
        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
            string key = i->key();
            string value = i->value().toString();

            //string result = i->key() + ":" + i->value();
            //cout << "Result: " << result << endl;

            // const char* tn = i->typeName();
            // std::cout << std::setw(44) << std::setfill(' ') << std::left
            //           << i->key() << " "
            //           << "0x" << std::setw(4) << std::setfill('0') << std::right
            //           << std::hex << i->tag() << " "
            //           << std::setw(9) << std::setfill(' ') << std::left
            //           << (tn ? tn : "Unknown") << " "
            //           << std::dec << std::setw(3)
            //           << std::setfill(' ') << std::right
            //           << i->count() << "  "
            //           << std::dec << i->value()
            //           << "\n";
        }
    }
}

ExifManager::~ExifManager(){
    delete image;
}

bool ExifManager::isValid(){
    return valid;
}

void ExifManager::close(){

}
