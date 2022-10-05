
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <valarray>
using namespace std;

float average(float v) {

    return v / 2;
}

float clamp(float v) {
    return min(1.f, max(0.f, v));
}
bool is_number(const string &s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

struct PPM
{
    string magicNum;
    int width, height, maxCol;
    vector<float> pixels;
    bool isGrayScale;
};
class MyImageClass {

public:
    PPM img;
    ~MyImageClass() {

        img.pixels.clear();
        img.pixels.shrink_to_fit();
    }
    MyImageClass(string path = "") {
        if (path == "")return;

        imread(path);
    }

    MyImageClass(const MyImageClass &cls) {
        img.magicNum = cls.img.magicNum;
        img.width = cls.img.width;
        img.height = cls.img.height;
        img.maxCol = cls.img.maxCol;
        img.isGrayScale = cls.img.isGrayScale;
    }


    void imread(string path = "") {
        ifstream file(path, ios::out | ios::binary);
        if (file.fail()) {
            cout << "Image read failed!" << endl;
            return;
        }

        file >> img.magicNum;
        string unknow;
        file >> unknow;
        //skip # magic string
        if (!is_number(unknow)) {
            getline(file, unknow);
            file >> img.width;
        } else {
            img.width = stoi(unknow);
        }
        file >> img.height >> img.maxCol;


        if (img.magicNum == "P3" || img.magicNum == "P2") {
            int times =  (img.magicNum == "P3") ? 1 : 3;
            img.isGrayScale = (img.magicNum == "P2");
            int p;
            while (file >> p) {
                for (int i = 0; i < times; i++) {
                    img.pixels.push_back(p / 255.f);
                }

            }

        } else if (img.magicNum == "P6" || img.magicNum == "P5") {
            int times =  (img.magicNum == "P6") ? 1 : 3;
            img.isGrayScale = (img.magicNum == "P5");
            file.get();
            //includes whitespace
            file.unsetf(ios_base::skipws);
            unsigned char p[3];
            while (file.read(reinterpret_cast<char *>(p), 3)) {
                for (int i = 0; i < times; i++) {
                    img.pixels.push_back(p[0] / 255.f);
                    img.pixels.push_back(p[1] / 255.f);
                    img.pixels.push_back(p[2] / 255.f);
                }
            }

        } else {
            cout << "Nope!" << endl;
        }

        file.close();
    }

    //always P6 for outputs
    void save(string path = "") {
        ofstream file(path, ios::out | ios::binary);
        file << "P6" << endl << img.width << " " << img.height << endl << img.maxCol << endl;

        for (auto p : img.pixels) {
            file << static_cast<unsigned char>(p * 255);
        }
        file.close();

    }


    MyImageClass operator + (const MyImageClass  &cls) {
        MyImageClass resCls = MyImageClass(*this);
        for (int i = 0; i < img.pixels.size(); i++) {
            resCls.img.pixels.push_back(img.pixels[i] + cls.img.pixels[i]);
        }

        return resCls;
    }
    MyImageClass& operator += (const MyImageClass  &cls) {
        for (int i = 0; i < img.pixels.size(); i++) {
            img.pixels[i] = average(img.pixels[i] + cls.img.pixels[i]);
        }
        return *this;
    }
    MyImageClass operator - (const MyImageClass  &cls) {
        MyImageClass resCls = MyImageClass(*this);
        for (int i = 0; i < img.pixels.size(); i++) {
            resCls.img.pixels.push_back(clamp(img.pixels[i] - cls.img.pixels[i]));
        }

        return resCls;
    }
    MyImageClass& operator -= (const MyImageClass  &cls) {
        for (int i = 0; i < img.pixels.size(); i++) {
            img.pixels[i] = clamp(img.pixels[i] - cls.img.pixels[i]);
        }
        return *this;
    }

    int operator [] (int index) {
        return img.pixels[index] * 255;
    }

    MyImageClass gammaCorrection(float gamma) {
        MyImageClass resCls = MyImageClass(*this);
        for (int i = 0; i < img.pixels.size(); i++) {
            float res = pow(img.pixels[i], 1 / gamma);
            resCls.img.pixels.push_back(clamp(res));
        }

        return resCls;
    }

    MyImageClass alphaCompositing(const MyImageClass &backPic, float alpha) {
        MyImageClass resCls = MyImageClass(*this);
        for (int i = 0; i < img.pixels.size(); i++) {
            float res = img.pixels[i] * (alpha) + backPic.img.pixels[i] * (1.f - alpha);
            resCls.img.pixels.push_back(clamp(res));
        }

        return resCls;
    }
    MyImageClass applyKernel(float kernel[3][3]) {
        int width = img.width;
        int height = img.height;

        float r = 0.299F;
        float g = 0.587F;
        float b = 0.114F;
        MyImageClass resCls = MyImageClass(*this);

        int i = 0;
        while (i < img.pixels.size()) {
            float res = (img.pixels[i] * r) + (img.pixels[i + 1] * g) + (img.pixels[i + 2] * b);
            resCls.img.pixels.push_back(res);
            i += 3;
        }

        for (int row  = 1; row  < height   - 1; row ++) {
            for (int col  = 1; col  < width  - 1; col ++) {
                float res = 0.f;

                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 3; b++) {
                        res += resCls.img.pixels[(row - 1 + a) * width + col - 1 + b] * kernel[a][b];
                    }
                }
                resCls.img.pixels[row * width + col] = clamp(res);
            }
        }
        vector<float> temp;
        for (auto p : resCls.img.pixels) {
            for (int t = 0; t < 3; t++) {
                temp.push_back(p);
            }
        }
        resCls.img.pixels = temp;
        return resCls;
    }
    MyImageClass applyKernel(float kernelX[3][3], float kernelY[3][3]) {
        int width = img.width;
        int height = img.height;

        float r = 0.299F;
        float g = 0.587F;
        float b = 0.114F;
        MyImageClass resCls = MyImageClass(*this);

        int i = 0;
        while (i < img.pixels.size()) {
            float res = (img.pixels[i] * r) + (img.pixels[i + 1] * g) + (img.pixels[i + 2] * b);
            resCls.img.pixels.push_back(res);
            i += 3;
        }

        for (int row  = 1; row  < height   - 1; row ++) {
            for (int col  = 1; col  < width  - 1; col ++) {
                float resX = 0.f;
                float resY = 0.f;

                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 3; b++) {
                        resX += resCls.img.pixels[(row - 1 + a) * width + col - 1 + b] * kernelX[a][b];
                        resY += resCls.img.pixels[(row - 1 + a) * width + col - 1 + b] * kernelY[a][b];

                    }
                }
                resX = clamp(resX);
                resY = clamp(resY);
                float val = sqrt(resX * resX + resY * resY);
                resCls.img.pixels[row * width + col] = clamp(val);
            }
        }
        vector<float> temp;
        for (auto p : resCls.img.pixels) {
            for (int t = 0; t < 3; t++) {
                temp.push_back(p);
            }
        }
        resCls.img.pixels = temp;
        return resCls;
    }
    MyImageClass blurring() {
        float kernel[3][3] = {
            { 0.1111111111, 0.1111111111, 0.1111111111 },
            { 0.1111111111, 0.1111111111, 0.1111111111 },
            { 0.1111111111, 0.1111111111, 0.1111111111}
        };
        return applyKernel(kernel);

    }
    MyImageClass edgeDetection() {
        float kernelX[3][3] = {
            { -1, 0, 1 },
            { -2, 0, 2 },
            { -1, 0, 1 }
        };
        float kernelY[3][3] = {
            { -1, -2, -1 },
            { 0,  0,  0 },
            { 1,  2,  1 }
        };
        return applyKernel(kernelX, kernelY);
    }



};
MyImageClass operator * (float value, const MyImageClass &cls)
{
    MyImageClass resCls = MyImageClass(cls);
    for (int i = 0; i < cls.img.pixels.size(); i++) {
        resCls.img.pixels.push_back( clamp(value * cls.img.pixels[i]));
    }

    return resCls;
}
MyImageClass operator * (const MyImageClass &cls, float value)
{
    return (float)value * cls;
}

MyImageClass operator * (int value, const MyImageClass &cls)
{
    return (float)value * cls;
}

MyImageClass operator * (const MyImageClass &cls, int value)
{
    return (float)value * cls;
}
