#include "DemonFire.h"

// load DICOM image series from folder and return as a 3d itk Image
ImageType::Pointer Utility::loadDICOM(char* folder){
    // check that input folder exists
    struct stat s;
    int err = stat(folder, &s);
    if(err==-1 || !S_ISDIR(s.st_mode)) {
        return NULL;
    }
    
    // generate file names for DICOM image
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetDirectory(folder);
    nameGenerator->SetUseSeriesDetails( true );
    const FileNameList &seriesUID = nameGenerator->GetSeriesUIDs();
    
    // print out series information to help with debugging
    FileNameList::const_iterator seriesItr = seriesUID.begin();
    FileNameList::const_iterator seriesEnd = seriesUID.end();
    
    // for now, visualize the first series found in the input folder
    std::string seriesIdentifier = seriesUID.begin()->c_str();
    FileNameList fileNames = nameGenerator->GetFileNames( seriesIdentifier );
    
    ReaderType::Pointer reader = ReaderType::New();
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO( dicomIO );
    
    // read image from list of filenames as an itk Image object
    // convert itk image to a vtkImageData object with connector
    reader->SetFileNames( fileNames );
    
    try {
        reader->Update();
    } catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
        return NULL;
    }
    
    return reader->GetOutput();
}

// save DICOM image to a folder
// returns 1 if save was successful, 0 otherwise
int Utility::saveDICOM(ImageType::Pointer image, char* folder){
    mkdir(folder, 0700);
    // initialize required variables
    NumericNamesGeneratorType::Pointer namesGenerator = NumericNamesGeneratorType::New();
    ImageIOType::Pointer gdcmIO = ImageIOType::New();
    WriterType::Pointer writer = WriterType::New();
    ImageType::RegionType region = image->GetLargestPossibleRegion();
    ImageType::IndexType start = region.GetIndex();
    ImageType::SizeType  size  = region.GetSize();
    
    // set writer input and IO class
    writer->SetInput( image );
    writer->SetImageIO( gdcmIO );
    
    // generate filenames for DICOM series
    std::string format = folder;
    format += "/%06d.dcm";
    namesGenerator->SetSeriesFormat( format.c_str() );
    namesGenerator->SetStartIndex( start[2] );
    namesGenerator->SetEndIndex( start[2] + size[2] - 1 );
    namesGenerator->SetIncrementIndex( 1 );
    writer->SetFileNames(namesGenerator->GetFileNames());
    
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & excp )
    {
        std::cerr << "Exception thrown while writing the series " << std::endl;
        std::cerr << excp << std::endl;
        return 0;
    }
    
    return 1;
}