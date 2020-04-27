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
}

ExifManager::~ExifManager(){
    delete image;
}

bool ExifManager::isValid(){
    return valid;
}

string* ExifManager::getExifData(){
    Exiv2::ExifData &exifData = image->exifData();

    if (exifData.empty()){
         cout << "[ERROR]: No Exif Data in this file!" << endl;
         return NULL;
    }

    else{
        string* result = new string[exifData.count()];
        length = exifData.count();
        Exiv2::ExifData::const_iterator end = exifData.end();
        int y = 0;

        for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; i++){
            result[y] = i->key() + ":" + i->value().toString();
            y++;
        }

        return result;
    }
}

size_t ExifManager::getSize(){
    return length;
}
