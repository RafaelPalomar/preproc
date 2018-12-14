// TCLAP includes
#include <tclap/CmdLine.h>

// ITK includes
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkFlipImageFilter.h>
#include <itkFixedArray.h>

// STD includes
#include <iostream>
#include <string>
#include <sstream>

// Input arguments structure
typedef struct InputArguments {

  std::string inputFile;
  std::string outputBase;
  std::string outputFileFormat;
  unsigned int size;
  bool labelMap;
  bool flipx;
  bool flipy;

} INPUTARGUMENTS;


// ------------------------------------------------------------------------------
// PROTOTYPES

int ParseCommandLineArguments(int argc, char *argv[], INPUTARGUMENTS &arguments);
int ProcessLabelMap(INPUTARGUMENTS &arguments);
int ProcessImage(INPUTARGUMENTS &arguments);


// -----------------------------
// ENTRY POINT

int main(int argc, char *argv[]){

  // Parse command line arguments
  INPUTARGUMENTS inputArguments;
  if (ParseCommandLineArguments(argc, argv, inputArguments))
    {
    return EXIT_FAILURE;
    }

  int ret;
  if (inputArguments.labelMap)
    {
    ret = ProcessLabelMap(inputArguments);
    }
  else
    {
    ret = ProcessImage(inputArguments);
    }

  return ret;
}

// ------------------------------------------------------------------------------
// Parse argumetns

int ParseCommandLineArguments(int argc, char **argv, INPUTARGUMENTS &arguments ){
  try {

  TCLAP::CmdLine cmd ("Extracts slices from medical volumes",' ',"0.1");

  TCLAP::ValueArg<std::string> inputImage ("i", "input", "Input file", true,"", "string");
  TCLAP::ValueArg<std::string> outputBaseName ("o", "output", "Input file basename", true,"", "string");
  TCLAP::ValueArg<std::string> outputFileFormat ("f", "file", "Output file format", true,"", "string");
  TCLAP::ValueArg<unsigned int> size ("s", "size", "Output size", false, 0,"integer");
  TCLAP::SwitchArg labelMap ("l", "labelmap", "Is the input file a labelmap?", false);
  TCLAP::SwitchArg flipX ("x", "flipx", "Flip the results (X axis)", false);
  TCLAP::SwitchArg flipY ("y", "flipy", "Flip the results (Y axis)", false);

  cmd.add(inputImage);
  cmd.add(outputBaseName);
  cmd.add(outputFileFormat);
  cmd.add(size);
  cmd.add(labelMap);
  cmd.add(flipX);
  cmd.add(flipY);

  cmd.parse(argc, argv);

  arguments.inputFile = inputImage.getValue();
  arguments.outputBase = outputBaseName.getValue();
  arguments.outputFileFormat = outputFileFormat.getValue();
  arguments.labelMap = labelMap.getValue();
  arguments.size = size.getValue();
  arguments.flipx = flipX.getValue();
  arguments.flipy = flipY.getValue();

  } catch (TCLAP::ArgException &e)
    { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; return 1;}

  return 0;

}

// ------------------------------------------------------------------------------
int ProcessImage(INPUTARGUMENTS &arguments)
{

  // Data types
  typedef itk::Image<short int, 3> InputImageType;
  typedef itk::Image<short int, 2> OutputImageType;
  typedef itk::ImageFileReader<InputImageType> InputImageReaderType;
  typedef itk::ImageFileWriter<OutputImageType> OutputImageWriterType;
  typedef itk::ExtractImageFilter<InputImageType, OutputImageType> ExtractImageFilterType;
  typedef itk::ResampleImageFilter<OutputImageType, OutputImageType> ResampleImageFilterType;
  typedef itk::IdentityTransform<double, 2> TransformType;
  typedef itk::FlipImageFilter<OutputImageType> FlipImageFilterType;

  // Read the input image
  InputImageReaderType::Pointer inputReader = InputImageReaderType::New();
  inputReader->SetFileName(arguments.inputFile);
  inputReader->Update();

  InputImageType::Pointer inputImage = inputReader->GetOutput();

  // Obtain the image size
  InputImageType::RegionType  fullImageRegion =
    inputImage->GetLargestPossibleRegion();
  InputImageType::SizeType inputImageSize = fullImageRegion.GetSize();

  // Create the extract region filter
  ExtractImageFilterType::Pointer extractImageFilter =
    ExtractImageFilterType::New();
  extractImageFilter->SetInput(inputImage);
  extractImageFilter->SetDirectionCollapseToIdentity();

  // Create the output image
  OutputImageType::SizeType outputImageSize;
  outputImageSize[0] = inputImageSize[0];
  outputImageSize[1] = inputImageSize[1];

  OutputImageType::IndexType outputStartPixel;
  outputStartPixel.Fill(0);

  OutputImageType::RegionType outputImageRegion(outputStartPixel,
                                                outputImageSize);
  OutputImageType::Pointer outputImage =
    OutputImageType::New();

  outputImage->SetRegions(outputImageRegion);
  outputImage->Allocate();

  // Create the writer
  OutputImageWriterType::Pointer outputWriter =
    OutputImageWriterType::New();

  // Extract slices
  for(unsigned int i=0; i<inputImageSize[2]; ++i)
    {

    InputImageType::IndexType sliceStart;
    sliceStart[0] = 0; sliceStart[1] = 0; sliceStart[2] = i;

    InputImageType::SizeType sliceSize;
    sliceSize[0] = inputImageSize[0];
    sliceSize[1] = inputImageSize[1];
    sliceSize[2] = 0;

    InputImageType::RegionType sliceRegion(sliceStart,
                                           sliceSize);

    extractImageFilter->SetExtractionRegion(sliceRegion);
    extractImageFilter->Update();

    OutputImageType::PointType outputOrigin;
    outputOrigin[0] = inputImage->GetOrigin()[0];
    outputOrigin[1] = inputImage->GetOrigin()[1];


    OutputImageType::SizeType outputSize;
    outputSize.Fill(arguments.size);

    OutputImageType::SpacingType outputSpacing;
    outputSpacing[0] = inputImage->GetSpacing()[0] *
      (static_cast<double>(sliceSize[0]) /
       static_cast<double>(outputSize[0]));
    outputSpacing[1] = inputImage->GetSpacing()[1] *
      (static_cast<double>(sliceSize[1]) /
       static_cast<double>(outputSize[1]));

    ResampleImageFilterType::Pointer resample = ResampleImageFilterType::New();
    FlipImageFilterType::Pointer flip = FlipImageFilterType::New();

    // Apply resizing filter (if needed)
    if (arguments.size != 0)
      {
      resample->SetInput(extractImageFilter->GetOutput());
      resample->SetSize(outputSize);
      resample->SetOutputOrigin(outputOrigin);
      resample->SetOutputSpacing(outputSpacing);
      resample->SetTransform(TransformType::New());
      resample->UpdateLargestPossibleRegion();
      resample->Update();
      }

    // Flip the axes (if needed)
    itk::FixedArray<bool,2> flipAxes;
    flipAxes[0] = arguments.flipx;
    flipAxes[1] = arguments.flipy;
    flip->SetFlipAxes(flipAxes);

    if (arguments.size != 0)
      {
      flip->SetInput(resample->GetOutput());
      }
    else
      {
      flip->SetInput(extractImageFilter->GetOutput());
      }

    flip->Update();

    // Set the writing parameters
    std::stringstream outputName;
    outputName << arguments.outputBase << "/"
               << std::setfill('0')
               << std::setw(4) << i
               << "." << arguments.outputFileFormat;
    outputWriter->SetFileName(outputName.str());

    outputWriter->SetInput(flip->GetOutput());
    outputWriter->Write();
    }

  return 0;
}

// ------------------------------------------------------------------------------
int ProcessLabelMap(INPUTARGUMENTS &arguments)
{
  return 0;
}
