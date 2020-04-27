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
        cout << "Size: " << exifData.count() << endl;
        Exiv2::ExifData::const_iterator end = exifData.end();
        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
            string result = i->key() + ":" + i->value().toString();
            cout << "Result: " << result << endl;
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
