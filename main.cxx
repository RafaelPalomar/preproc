// TCLAP includes
#include <tclap/CmdLine.h>

// ITK includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImageFileWriter.h>

// STD includes
#include <iostream>
#include <string>


// Input arguments structure
struct InputArguments {

  std::string inputFile;
  bool labelMap;
  std::string outputBase;

};


// ------------------------------------------------------------------------------
// PROTOTYPES

void ParseCommandLineArguments(int argc, char *argv[]);


// -----------------------------
// ENTRY POINT

int main(int argc, char *argv[]){

  // Parse command line arguments

  // Image types
  typedef itk::Image<float, 3> ImageType;
  typedef itk::Image<unsigned char, 4> LabelMapType;

  // Readers types
  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  typedef itk::ImageFileReader<LabelMapType> LabelMapReaderType;

  ImageReaderType

  return 0;
}

// ------------------------------------------------------------------------------
// Parse argumetns

void ParseCommandLineArguments(int argc, char **argv){


  try {

  TCLAP::CmdLine cmd ("Extracts slices from medical volumes",'',"0.1");

  TCLAP::ValueArg<std::string> inputImage ("i", "input", "Input file", true,'', "string");
  TCLAP::ValueArg<std::string> outputBaseName ("o", "output", "Input file basename", true,'', "string");
  TCLAP::SwitchArg labelMap ("l", "labelmap", "Is the input file a labelmap?", false);



  } catch (TCLAP::ArgException &e)
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl;}


}
