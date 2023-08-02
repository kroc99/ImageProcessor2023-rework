//
// Created by Kristian O'Connor on 3/10/23.
//
#include "pixelhead.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void write(ofstream &file, image &image){
    Header header = image.header;
    vector<pixel> pixel = image.pixels;
    int pixelSize = header.height * header.width;
    //read file header to image header
    file.write(&header.idLength, sizeof(header.idLength));
    file.write(&header.colorMapType, sizeof(header.colorMapType));
    file.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
    file.write((char *) &header.colorMapOrigin, sizeof(header.colorMapOrigin));
    file.write((char *) &header.colorMapLength, sizeof(header.colorMapLength));
    file.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
    file.write((char *) &header.xOrigin, sizeof(header.xOrigin));
    file.write((char *) &header.yOrigin, sizeof(header.yOrigin));
    file.write((char *) &header.width, sizeof(header.width));
    file.write((char *) &header.height, sizeof(header.height));
    file.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    file.write(&header.imageDescriptor, sizeof(header.imageDescriptor));
    image.header = header;

    for(int c = 0; c < pixelSize; c++){
        file.write((char*)&pixel[c].blue, sizeof(pixel[c].blue));
        file.write((char*)&pixel[c].green, sizeof(pixel[c].green));
        file.write((char*)&pixel[c].red, sizeof(pixel[c].red));
    }
}

image read(ifstream &file){
    Header header;
    image image;
    int pixelSize;

    file.read(&header.idLength, sizeof(header.idLength));
    file.read(&header.colorMapType, sizeof(header.colorMapType));
    file.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    file.read((char *) &header.colorMapOrigin, sizeof(header.colorMapOrigin));
    file.read((char *) &header.colorMapLength, sizeof(header.colorMapLength));
    file.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    file.read((char *) &header.xOrigin, sizeof(header.xOrigin));
    file.read((char *) &header.yOrigin, sizeof(header.yOrigin));
    file.read((char *) &header.width, sizeof(header.width));
    file.read((char *) &header.height, sizeof(header.height));
    file.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    file.read(&header.imageDescriptor, sizeof(header.imageDescriptor));
    image.header = header;

    pixelSize = header.width * header.height;
    for(int c = 0; c < pixelSize; c++){
        pixel tempPixel;
        file.read((char*)&tempPixel.blue, sizeof(tempPixel.blue));
        file.read((char*)&tempPixel.green, sizeof(tempPixel.green));
        file.read((char*)&tempPixel.red, sizeof(tempPixel.red));
        image.pixels.push_back(tempPixel);
    }
    return image;
}

bool operator==(const image baseimage, const image image2){
    Header h1 = baseimage.header, h2 = image2.header;
    if(h1.width != h2.width)
        return false;
    if(h1.height != h2.height)
        return false;
    int len = h2.width * h2.height;
    for(int c = 0; c < len; c++){
        unsigned char red1 = baseimage.pixels[c].red, green1 = baseimage.pixels[c].green, blue1 = baseimage.pixels[c].blue;
        unsigned char red2 = image2.pixels[c].red, green2 = image2.pixels[c].green, blue2 = image2.pixels[c].blue;
        if(red1 != red2 || green1 != green2 || blue1 != blue2)
            return false;
    }
    return true;
}

image subtract(const image baseimage, const image image2){

    // Create a new image object to store the subtracted image
    image n_Image;

    // Set the header of the new image to be the same as the header of the base image
    n_Image.header = baseimage.header;

    // Calculate the total number of pixels in the base image
    int len = baseimage.header.width * baseimage.header.height;

    // Loop through each pixel in the base image and subtract the corresponding pixel in the other image
    for(int c = 0; c < len; c++){

        // Extract the RGB values of the pixel from the base image
        int redVal = (int)baseimage.pixels[c].red;
        int greenVal = (int)baseimage.pixels[c].green;
        int blueVal = (int)baseimage.pixels[c].blue;

        // Subtract the corresponding RGB values of the pixel in the other image from the RGB values of the base image pixel
        redVal -= (int)image2.pixels[c].red;
        greenVal -= (int)image2.pixels[c].green;
        blueVal -= (int)image2.pixels[c].blue;

        // Ensure that the RGB values are not negative
        if(redVal < 0)
            redVal = 0;
        if(greenVal < 0)
            greenVal = 0;
        if(blueVal < 0)
            blueVal = 0;

        // Convert the resulting RGB values to unsigned char and create a new pixel object
        unsigned char red = redVal;
        unsigned char green = greenVal;
        unsigned char blue = blueVal;
        pixel tempPixel;
        tempPixel.red = red;
        tempPixel.green = green;
        tempPixel.blue = blue;

        // Add the new pixel to the subtracted image object
        n_Image.pixels.push_back(tempPixel);
    }

    // Return the subtracted image object
    return n_Image;
}

image combined(const image baseimage, const image image2, const image image3){

    // Create a new image object to store the combined image
    image n_Image;

    // Set the header of the new image to be the same as the header of the base image
    n_Image.header = baseimage.header;

    // Calculate the total number of pixels in the base image
    int len = baseimage.header.width * baseimage.header.height;

    // Loop through each pixel in the base image and combine it with the corresponding pixels in the other two images
    for(int c = 0; c < len; c++){

        // Extract the red component of the pixel from the base image
        unsigned char red = baseimage.pixels[c].red;

        // Extract the blue component of the pixel from the third image
        unsigned char blue = image3.pixels[c].blue;
        // Extract the green component of the pixel from the second image
        unsigned char green = image2.pixels[c].green;



        // Create a new pixel object and set its red, green, and blue components
        pixel p;
        p.red = red;
        p.blue = blue;
        p.green = green;

        // Add the new pixel to the combined image object
        n_Image.pixels.push_back(p);
    }

    // Return the combined image object
    return n_Image;
}

image multiply(const image baseimage, const image image2){

    // Create a new image object to store the multiplied image
    image n_Image;

    // Set the header of the new image to be the same as the header of the base image
    n_Image.header = baseimage.header;

    // Calculate the total number of pixels in the base image
    int len = baseimage.header.width * baseimage.header.height;

    // Loop through each pixel in the base image and multiply it with the corresponding pixel in the other image
    for(int c = 0; c < len; c++){

        // Extract the RGB values of the pixel from the base image, convert them to float and divide by 255
        float tempRed1 = (float)baseimage.pixels[c].red / 255;
        float tempBlue1 = (float)baseimage.pixels[c].blue / 255;
        float tempGreen1 = (float)baseimage.pixels[c].green / 255;


        // Extract the RGB values of the pixel from the second image, convert them to float and divide by 255
        float tempRed2 = (float)image2.pixels[c].red / 255;
        float tempBlue2 = (float)image2.pixels[c].blue / 255;
        float tempGreen2 = (float)image2.pixels[c].green / 255;


        // Multiply the RGB values of the two pixels and scale the result back to the range 0-255
        float blend_red = (tempRed1 * tempRed2 * 255) + 0.5f;
        float blend_blue = (tempBlue1 * tempBlue2 * 255) + 0.5f;
        float blend_green = (tempGreen1 * tempGreen2 * 255) + 0.5f;


        // Convert the resulting RGB values back to unsigned char and create a new pixel object
        unsigned char red = blend_red;
        unsigned char blue = blend_blue;
        unsigned char green = blend_green;

        pixel tempPixel;
        tempPixel.green = green;
        tempPixel.red = red;
        tempPixel.blue = blue;
        // Add the new pixel to the multiplied image object
        n_Image.pixels.push_back(tempPixel);
    }

    // Return the multiplied image object
    return n_Image;
}

image screen(const image baseimage, const image image2){
    // Create a new image object called 'n_Image' to store the result of the blending operation
    image n_Image;

    // Copy the header of 'baseimage' to the header of 'n_Image'
    n_Image.header = baseimage.header;

    // Calculate the total number of pixels in the image
    int len = baseimage.header.width * baseimage.header.height;


    for(int c = 0; c < len; c++){
        // Screen blending formula: C = 1 - (1-A) * (1-B)
        // Convert RGB values to a value between 0 and 1
        float tempRed1 = (float)baseimage.pixels[c].red / 255, tempGreen1 = (float)baseimage.pixels[c].green / 255, tempBlue1 = (float)baseimage.pixels[c].blue / 255;
        float tempRed2 = (float)image2.pixels[c].red / 255, tempGreen2 = (float)image2.pixels[c].green / 255, tempBlue2 = (float)image2.pixels[c].blue / 255;

        // Calculate the blended RGB values using the screen blending formula
        float blend_red = (1 - (1-tempRed1)*(1-tempRed2)) * 255, blend_green = (1 - (1-tempGreen1)*(1-tempGreen2))*255, blend_blue = (1 - (1-tempBlue1)*(1-tempBlue2))*255;

        // Add 0.5 to each blended RGB value for rounding purposes
        blend_red += 0.5f, blend_green += 0.5f, blend_blue += 0.5f;

        // Convert the blended RGB values to unsigned char data type
        unsigned char red = blend_red;
        unsigned char green = blend_green;
        unsigned char blue = blend_blue;

        // Create a new pixel object called 'tempPixel' and set its RGB values
        pixel tempPixel;
        tempPixel.red = red;
        tempPixel.green = green;
        tempPixel.blue = blue;

        // Add the new pixel to the 'pixels' vector of 'n_Image'
        n_Image.pixels.push_back(tempPixel);
    }

    // Return the blended image
    return n_Image;
}

image overlay(const image baseimage, const image image2){

    // Create a new image object and copy the header from the base image object
    image n_Image;
    n_Image.header = baseimage.header;

    // Iterate over each pixel in the base image object and blend it with the corresponding pixel in the overlay image object
    int len = baseimage.header.width * baseimage.header.height;
    for(int c = 0; c < len; c++){

        // Calculate the blending values for each color channel based on the corresponding pixel values in the base and overlay image objects
        float tempRed1 = (float)baseimage.pixels[c].red / 255, tempGreen1 = (float)baseimage.pixels[c].green / 255, tempBlue1 = (float)baseimage.pixels[c].blue / 255;
        float tempRed2 = (float)image2.pixels[c].red / 255, tempGreen2 = (float)image2.pixels[c].green / 255, tempBlue2 = (float)image2.pixels[c].blue / 255;
        float blend_red, blend_green, blend_blue;

        // If the red, green, or blue value of the overlay pixel is less than or equal to 0.5, multiply the corresponding color channel values
        if(tempRed2 <= 0.5)
            blend_red = (tempRed1 * tempRed2 * 510) + 0.5f;
        else
            blend_red = ((1 - 2*(1-tempRed1)*(1-tempRed2)) * 255) + 0.5f;
        if(tempGreen2 <= 0.5)
            blend_green = (tempGreen1 * tempGreen2 * 510) + 0.5f;
        else
            blend_green = ((1 - 2*(1-tempGreen1)*(1-tempGreen2)) * 255) + 0.5f;
        if(tempBlue2 <= 0.5)
            blend_blue = (tempBlue1 * tempBlue2 * 510) + 0.5f;
        else
            blend_blue = ((1 - 2*(1-tempBlue1)*(1-tempBlue2)) * 255) + 0.5f;

        // Convert the blended color channel values to unsigned char values and create a new pixel object with these values
        unsigned char red = blend_red;
        unsigned char green = blend_green;
        unsigned char blue = blend_blue;
        pixel tempPixel;
        tempPixel.green = green;
        tempPixel.red = red;
        tempPixel.blue = blue;

        // Add the new pixel object to the pixels vector of the new image object
        n_Image.pixels.push_back(tempPixel);
    }

    // Return the new image object with the blended pixels
    return n_Image;
}

image channel(const image baseimage, int choice){

    // Create a new image object and assign the given color channel to each pixel in the image
    // 1 is red, 2 is green, 3 is blue
    image n_Image;
    n_Image.header = baseimage.header;
    int len = baseimage.header.width * baseimage.header.height;
    unsigned char newColor;
    for(int c = 0; c < len; c++){

        // Determine the new color value based on the specified color channel choice
        switch (choice) {
            case 1:
                newColor = baseimage.pixels[c].red;
                break;
            case 2:
                newColor = baseimage.pixels[c].green;
                break;
            case 3:
                newColor = baseimage.pixels[c].blue;
                break;
        }

        // Create a new pixel object with the new color value for each color channel
        pixel p;
        p.green = newColor;
        p.red = newColor;

        p.blue = newColor;

        // Add the new pixel to the pixels vector of the new image object
        n_Image.pixels.push_back(p);
    }

    // Return the new image object with the assigned color channel
    return n_Image;
}

void flip(image &baseimage){

    // Create a new image object and copy the header from the input image object
    image temp;
    temp.header = baseimage.header;

    // Iterate over each pixel in the input image object and flip its position in the new image object
    int len = baseimage.header.height * baseimage.header.width;
    int width = baseimage.header.width;
    for(int c = 0; c < len; c+=width){
        for(int j = 0; j < width; j++) {

            // Calculate the new index for the pixel by flipping it horizontally
            int index = len - j - c - 1;

            // Retrieve the pixel value from the input image object at the new index and push it to the new image object
            pixel p = baseimage.pixels[index];
            temp.pixels.push_back(p);
        }
    }

    // Set the input image object to the new image object, which contains the flipped image
    baseimage = temp;
}

void boostGreen(image &baseimage, int boost){

    // Add a specified amount to the green channel of each pixel in the image
    int len = baseimage.header.height * baseimage.header.width;
    for(int c = 0 ; c < len; c++){

        // Add the specified amount to the green channel value for the current pixel
        int tempGreen = baseimage.pixels[c].green + boost;
        if(tempGreen > 255)
            tempGreen = 255; // Cap the value at 255 if it exceeds the maximum value
        unsigned char green = tempGreen;

        // Set the new green channel value for the current pixel
        baseimage.pixels[c].green = green;
    }
}

void scale(image &baseimage){

    // Multiply the red channel by 4 and the blue channel by 0
    int len = baseimage.header.height * baseimage.header.width;
    for(int c = 0; c < len; c++){

        // Scale the red channel by multiplying it by 4
        float temp_Red = (float)baseimage.pixels[c].red / 255;
        temp_Red *= 1020; // Multiply by 4
        if(temp_Red > 255)
            temp_Red = 255;
        temp_Red += 0.5f; // Add 0.5 for rounding

        // Set the scaled red channel value and set the blue channel value to 0
        baseimage.pixels[c].red = temp_Red;
        baseimage.pixels[c].blue = 0;
    }
}



void TestNine(){
    ifstream file1, file2, file3, file4;
    file1.open("input/layer_red.tga", ios_base::binary);
    file2.open("input/layer_green.tga", ios_base::binary);
    file3.open("input/layer_blue.tga", ios_base::binary);
    file4.open("examples/EXAMPLE_part9.tga", ios_base::binary);

    image baseimage = read(file1);
    image image2 = read(file2);
    image image3 = read(file3);
    image n_Image = combined(baseimage, image2, image3);

    image expected = read(file4);

    ofstream result("output/part9.tga", ios_base::binary);
    write(result, n_Image);

    cout << "Test #9...... ";
    if(n_Image == expected)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    file2.close();
    file3.close();
    file4.close();
}
void TestTen(){
    ifstream file1, example10;
    file1.open("input/text2.tga", ios_base::binary);
    example10.open("examples/EXAMPLE_part10.tga", ios_base::binary);

    image text = read(file1);
    flip(text);

    ofstream output("output/part10.tga", ios_base::binary);
    write(output, text);
    image exampleImage = read(example10);
    cout << "Test #10...... ";
    if(text == exampleImage)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    example10.close();
}
void TestEight(){
    ifstream file1("input/car.tga", ios_base::binary),
            example8R("examples/EXAMPLE_part8_r.tga", ios_base::binary),
            example8G("examples/EXAMPLE_part8_g.tga", ios_base::binary),
            example8B("examples/EXAMPLE_part8_b.tga", ios_base::binary);
    ofstream carR("output/part8_r.tga", ios_base::binary),
            carG("output/part8_g.tga", ios_base::binary),
            carB("output/part8_b.tga", ios_base::binary);

    image car = read(file1);
    image exampleRed = read(example8R), exampleGreen = read(example8G), exampleBlue = read(example8B);
    image carRed = channel(car, 1), carGreen = channel(car, 2), carBlue = channel(car, 3);
    write(carR, carRed);
    write(carG, carGreen);
    write(carB, carBlue);
    cout << "Test #8...... ";
    if(carRed == exampleRed && carGreen == exampleGreen && carBlue == exampleBlue)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    example8R.close();
    example8G.close();
    example8B.close();
}

void TestSeven(){
    ifstream file1, example7;
    file1.open("input/car.tga", ios_base::binary);
    example7.open("examples/EXAMPLE_part7.tga", ios_base::binary);

    image car = read(file1);

    image exampleImage = read(example7);
    scale(car);
    ofstream output("output/part7.tga", ios_base::binary);
    write(output, car);

    cout << "Test #7...... ";
    if(car == exampleImage)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    example7.close();
}
void TestSix(){
    ifstream file1, example6;
    file1.open("input/car.tga", ios_base::binary);
    example6.open("examples/EXAMPLE_part6.tga", ios_base::binary);

    image car = read(file1);

    image exampleImage = read(example6);
    boostGreen(car, 200);
    ofstream output("output/part6.tga", ios_base::binary);
    write(output, car);

    cout << "Test #6...... ";
    if(car == exampleImage)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    example6.close();
}

void TestFive(){
    ifstream file1, file2, exampleImage;
    ofstream part1("output/part5.tga", ios_base::binary);
    image baseimage, image2, n_Image, example;

    file2.open("input/pat1.tga", ios_base::binary);
    file1.open("input/layer1.tga", ios_base::binary);

    exampleImage.open("examples/EXAMPLE_part5.tga", ios_base::binary);
    image2 = read(file2);
    baseimage = read(file1);

    example = read(exampleImage);
    //mutiply both
    n_Image = overlay(baseimage, image2);
    write(part1, n_Image);

    cout << "Test #5...... ";
    if(n_Image == example)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    file2.close();
    exampleImage.close();
}
void TestFour(){
    ifstream file1, file2, file3, exampleImage;
    ofstream output("output/part4.tga", ios_base::binary);
    
    file3.open("input/pat2.tga", ios_base::binary);
    file2.open("input/circles.tga", ios_base::binary);
    file1.open("input/layer2.tga", ios_base::binary);
    
    exampleImage.open("examples/EXAMPLE_part4.tga", ios_base::binary);
    
    image baseimage = read(file1);
    image image2 = read(file2);
    image temp = multiply(baseimage, image2);

    image pat = read(file3);
    image n_Image = subtract(temp, pat);
    write(output, n_Image);

    image example = read(exampleImage);
    cout << "Test #4...... ";
    if(n_Image == example)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;
    file1.close();
    file2.close();
    file3.close();
    exampleImage.close();
}

void TestThree(){

    // Declare input and output file streams for four images
    ifstream file1, file2, file3, exampleImage;
    ofstream output("output/part3.tga", ios_base::binary);

    // Open the three input files and the example image file in binary mode
    file1.open("input/layer1.tga", ios_base::binary);
    file2.open("input/pat2.tga", ios_base::binary);
    file3.open("input/text.tga", ios_base::binary);
    exampleImage.open("examples/EXAMPLE_part3.tga", ios_base::binary);

    // Read the contents of the three input files into three image objects
    image baseimage = read(file1);
    image image2 = read(file2);
    image text = read(file3);

    // Multiply two images and assign the result to a new image object
    image temp = multiply(baseimage, image2);

    // Apply a screen blending operation to two images and assign the result to a new image object
    image n_Image = screen(text, temp);

    // Write the contents of the new image object to an output file
    write(output, n_Image);

    // Read the contents of the example image file into an image object
    image example = read(exampleImage);

    // Print a message indicating whether the test passed or failed
    cout << "Test #3...... ";
    if(n_Image == example)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;

    // Close all opened files
    file3.close();
    file2.close();
    file1.close();
    exampleImage.close();
}


void TestTwo(){

    // Declare input and output file streams for three images
    ifstream file1, file2, exampleImage;
    ofstream output("output/part2.tga", ios_base::binary);

    // Declare four image objects
    image baseimage, image2, example, n_Image;

    // Open the two input files and the example image file in binary mode
    file1.open("input/car.tga", ios_base::binary);
    file2.open("input/layer2.tga", ios_base::binary);
    exampleImage.open("examples/EXAMPLE_part2.tga", ios_base::binary);

    // Read the contents of the two input files and the example image file into three image objects
    example = read(exampleImage);
    image2 = read(file2);
    baseimage = read(file1);

    // Subtract two images and assign the result to a new image object
    n_Image = subtract(baseimage, image2);

    // Write the contents of the new image object to an output file
    write(output, n_Image);

    // Print a message indicating whether the test passed or failed
    cout << "Test #2...... ";
    if(n_Image == example)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;

    // Close all opened files
    file2.close();
    file1.close();
    exampleImage.close();
}


void TestOne(){

    // Declare input and output file streams for three images
    ifstream file1, file2, exampleImage;
    ofstream part1("output/part1.tga", ios_base::binary);

    // Declare four image objects
    image baseimage, image2, n_Image, example;

    // Open the two input files and the example image file in binary mode
    file2.open("input/pat1.tga", ios_base::binary);
    file1.open("input/layer1.tga", ios_base::binary);
    exampleImage.open("examples/EXAMPLE_part1.tga", ios_base::binary);

    // Read the contents of the two input files and the example image file into three image objects
    baseimage = read(file1);
    image2 = read(file2);
    example = read(exampleImage);

    // Multiply two images and assign the result to a new image object
    n_Image = multiply(baseimage, image2);

    // Write the contents of the new image object to an output file
    write(part1, n_Image);

    // Print a message indicating whether the test passed or failed
    cout << "Test #1...... ";
    if(n_Image == example)
        cout << "Passed!" << endl;
    else
        cout << "Failed!" << endl;

    // Close all opened files
    file1.close();
    file2.close();
    exampleImage.close();
}


int main() {


    TestOne();
    TestTwo();
    TestThree();
    TestFour();
    TestFive();
    TestSix();
    TestSeven();
    TestEight();
    TestNine();
    TestTen();
    return 0;
}















