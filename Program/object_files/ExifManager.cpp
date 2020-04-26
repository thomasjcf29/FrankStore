#include "../header_files/ExifManager.h"
#include <string.h>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <cassert>
#include <iostream>

using namespace std;

ExifManager::ExifManager(){

}

ExifManager::ExifManager(string imageLocation){
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(imageLocation);
    if(image.good()){
        valid = true;
    }
    else{
        cout << "[ERROR]: Could not open image, check your permissions!" << endl;
    }

    // image->readMetadata();
    // Exiv2::ExifData &exifData = image->exifData();
    // if (exifData.empty()) {
    //     std::string error("No Exif data found in file");
    //     throw Exiv2::Error(Exiv2::kerErrorMessage, error);
    // }

    // Exiv2::ExifData::const_iterator end = exifData.end();
    // for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
    //     const char* tn = i->typeName();
    //     std::cout << std::setw(44) << std::setfill(' ') << std::left
    //               << i->key() << " "
    //               << "0x" << std::setw(4) << std::setfill('0') << std::right
    //               << std::hex << i->tag() << " "
    //               << std::setw(9) << std::setfill(' ') << std::left
    //               << (tn ? tn : "Unknown") << " "
    //               << std::dec << std::setw(3)
    //               << std::setfill(' ') << std::right
    //               << i->count() << "  "
    //               << std::dec << i->value()
    //               << "\n";
    // }

    cout << "Exif Manager End" << endl;
}

bool ExifManager::isValid(){
    return valid;
}

void ExifManager::close(){

}
