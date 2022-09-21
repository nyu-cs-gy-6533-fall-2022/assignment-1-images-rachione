
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <valarray>
using namespace std;

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
            int p;
            while (file >> p) {
                img.pixels.push_back(p / 255.f);
            }

        } else if (img.magicNum == "P6" || img.magicNum == "P5") {
            file.get();
            //includes whitespace
            file.unsetf(ios_base::skipws);
            unsigned char p[3];
            while (file.read(reinterpret_cast<char *>(p), 3)) {
                img.pixels.push_back(p[0] / 255.f);
                img.pixels.push_back(p[1] / 255.f);
                img.pixels.push_back(p[2] / 255.f);



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
            resCls.img.pixels.push_back(clamp(img.pixels[i] + cls.img.pixels[i]));
        }

        return resCls;
    }
    MyImageClass& operator += (const MyImageClass  &cls) {
        for (int i = 0; i < img.pixels.size(); i++) {
            img.pixels[i] = clamp(img.pixels[i] + cls.img.pixels[i]);
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

    MyImageClass edgeDetection() {
        MyImageClass resCls = MyImageClass(*this);
        for (int i = 0; i < img.pixels.size(); i++) {
            float res = img.pixels[i];
            resCls.img.pixels.push_back(clamp(res));
        }

        return resCls;
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
