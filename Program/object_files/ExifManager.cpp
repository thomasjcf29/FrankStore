#include "../header_files/ExifManager.h"
#include <string.h>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <iostream>

using namespace std;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
ExifManager::ExifManager(){
}

/**
Returns an object allowing you to read the exif data of the image, check it is valid
before using it. (.isValid()).
@param char* imageLocation: The location of the image to read.
*/
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

/**
Destructor called to delete the raw pointer of the image object.
*/
ExifManager::~ExifManager(){
    delete image;
}

/**
Returns the validity status of the ExifManager object. If it returns false
do not use!
@return status of object, true is valid, false is not.
*/
bool ExifManager::isValid(){
    return valid;
}

/**
Returns a string array containing all EXIF data pulled from the image. To check
size of array, use the getSize() method of this object.
@return array of EXIF Data.
*/
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

/**
@return size of string array for ExifData.
*/
size_t ExifManager::getSize(){
    return length;
}
