#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Image.hpp"
#include "iostream"
#include "string"
#include "fstream"

#define RED 0.2126
#define GREEN 0.7152
#define BLUE 0.0722

using namespace std;
using namespace sf;

char getAsciiChar(int grayscale, string ascii) {
    return ascii[(int)(grayscale/255.f*ascii.size())];
}

int main() {
try {
        string inputPath, outputPath = "";
        string ascii = "@%$=+-|:. "; // chars used in the art

        cout << "Input: ";
        cin >> inputPath;
        for (int i = 0; i < inputPath.length(); i++)
        {
            if (inputPath[i] == 'i') break;
            outputPath += inputPath[i];
        }

        int scaleX;
        cout << "Scaling: "; // for scaling the image, lower value makes the output bigger - don't go up 10
        cin>> scaleX;

        outputPath.erase(outputPath.length()-4); // get the of the old file extension
        outputPath += ".txt";
        // cout << inputPath << " --------- " << outputPath << endl;

        Image img;
        img.loadFromFile(inputPath);

        /* ---------------------------------------------------------------------------------------------------------------------------------/
        if (!img.loadFromFile(inputPath)) cerr << "[ERROR] Couldn't find an image there!"; 
        An example for catching the errors, since SFML doesn't use exceptions but returns booleans.
        -----------------------------------------------------------------------------------------------------------------------------------*/

        /* image needs to get converted to grayscale in order to get it's brightness
        so we can use it for to assign ascii chars to represent them based on their levels.
        https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems
        */
        Image grayimg(img);
        int c;
        for (int i = 0; i < img.getSize().x; ++i)
        {
            for (int y = 0; y < img.getSize().y; ++y) {
                c = img.getPixel(i, y).r * RED + img.getPixel(i, y).g * GREEN + img.getPixel(i, y).b * BLUE;
                grayimg.setPixel(i, y, (Color(c,c,c)));
            }
        }


        // creating another img for ease of scaling
        int scaleY = 2*scaleX;
        Image lowResImg(grayimg);
        for (int a = 0; a < img.getSize().x-scaleX; a+=scaleX) {
            for (int b = 0; b < img.getSize().y-scaleY; b+=scaleY) {
                for (int x = 0; x < scaleX; ++x) {
                    for (int y = 0; y < scaleY; ++y) {
                        c += grayimg.getPixel(a+x, b+y).r;
                    }
                }
            c /= scaleX * scaleY + 1;
            for (int x = 0; x < scaleX; ++x)
                for(int y = 0; y < scaleY; ++y)
                    lowResImg.setPixel(a+x, b+y, Color(c,c,c));
            }
        }

        lowResImg.saveToFile("lowres.png"); // can uncomment it to see how different scales different scales effects the image 

        string asciiArt;
        for (int i = 0; i < lowResImg.getSize().y; i+=scaleY) {
            for (int y = 0; y < lowResImg.getSize().x; y+=scaleX)
                asciiArt += getAsciiChar(lowResImg.getPixel(y, i).r, ascii);
            asciiArt += '\n';
        }     

        ofstream output(outputPath);
        if(output) output << asciiArt;

        cout << "ASCII art saved to: " << outputPath << endl;
}



catch (exception &error_) {
    cout << "[ERROR]" << error_.what() << endl;
    return 1;
}
    return 0;
}