# Botox

A small C++ library that automatically fills holes in images, wrapped in a simple terminal application to demonstrate its usage.

## Installation

Requires [OpenCV](https://docs.opencv.org/4.x/d2/de6/tutorial_py_setup_in_ubuntu.html) to be on the build path. Run the project (CLion IDE) or build from a terminal with:
```
cmake --build ./cmake-build-debug --target lightricks
```

## Usage

The application batch-process all images from an input directory, and writes the output to an output directory. A mask is used to carve a hole in each input image and then the application fills each hole as best it can. The width and height of each input file must exactly match the mask file.

```
Botox options:
  -h [ --help ]                 Displays this help message
  -i [ --input_directory ] arg  Directory containing files to process
  -o [ --output_directory ] arg Output directory
  -m [ --mask ] arg             Mask file
  -c [ --connectivity ] arg     Boundary pixel connectivity mode (4 or 8)
  -e [ --E ] arg                E for the lightricks weighting function. Must 
                                be greater than zero.
  -z [ --Z ] arg                Z for the lightricks weighting function
```

### Example:
```
./cmake-build-debug/lightricks -i "./input" -o "./output" -m ./mask.png -c 8 -e 0.01 -z 3.0
```

![Alt text](images/Lenna_input.png?raw=true "Input image")  
(Input image)  

<kbd>![Alt text](images/mask.png?raw=true "Input mask")</kbd>  
(Input mask)  

![Alt text](images/Lenna_output.png?raw=true "Output image")  
(Output image)  

#### Note
Open CV is used by the application to conveniently read/write image files. The Botox library itself does not use OpenCV.
