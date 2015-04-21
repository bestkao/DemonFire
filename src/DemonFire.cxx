#include "DemonFire.h"

int main(int argc, char **argv){
    Segmentation::doSegmentation(NULL);
    Registration::doRegistration(NULL, NULL);
    return 0;
}