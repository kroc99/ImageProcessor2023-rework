# ImageProcessor2023-rework

Supported Operations
The library supports the following image processing operations:

Multiply: Multiply two TGA images pixel by pixel.
Subtract: Subtract one TGA image from another pixel by pixel.
Screen: Apply the screen blending mode to two TGA images.
Overlay: Overlay two TGA images using a specific blending algorithm.
Channel: Isolate a specific color channel (red, green, or blue) of a TGA image.
Boost Green: Increase the intensity of the green color channel in a TGA image.
Scale: Scale the red channel of a TGA image by a factor of 4 and set the blue channel to 0.
Flip: Flip a TGA image horizontally.
Combine: Combine three TGA images (red, green, and blue channels) into a single image.



Reading and Writing TGA Images
The library provides two functions, read() and write(), to read and write TGA images, respectively. Here's an example of how to use these functions:


int main() {
    //Open input and output file streams
    std::ifstream inputFile("input/image.tga", std::ios_base::binary);
    std::ofstream outputFile("output/processed_image.tga", std::ios_base::binary);

    //Read the TGA image from the input file
    image img = read(inputFile);

    //Perform image processing operations on 'img'

    //Write the processed image to the output file
    write(outputFile, img);

    //Close the file streams
    
    inputFile.close();
    outputFile.close();

    return 0;
}

Performing Image Processing Operations

To perform image processing operations, you can call the relevant functions provided by the library. For example, to multiply two images:


int main() {
    //open input file streams
    std::ifstream file1("input/image1.tga", std::ios_base::binary);
    std::ifstream file2("input/image2.tga", std::ios_base::binary);

    //Read the two TGA images from the input files
    image baseimage = read(file1);
    image image2 = read(file2);

    //Multiply the two images
    image result = multiply(baseimage, image2);

    //Open an output file stream
    std::ofstream outputFile("output/multiplied_image.tga", std::ios_base::binary);

    //Write the result to the output file
    write(outputFile, result);

    //Close the file streams
    file1.close();
    file2.close();
    outputFile.close();

    return 0;
}


Please note that the library assumes the TGA images are in 24-bit format (8 bits per channel for red, green, and blue) and uses the image struct to represent the image data.

