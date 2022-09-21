
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
using namespace std;


bool is_number(const string &s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

struct PPM
{
    string magicNum;
    int width, height, maxCol;
    vector<char> pixels;
};
class MyImageClass {

public:
    PPM img;
    MyImageClass(string path = "") {
        if (path == "")return;

        imread(path);
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
        int byte;

        if (img.magicNum == "P3" || img.magicNum == "P2") {
            int p;
            while (file >> p) {
                img.pixels.push_back(p);
            }

        } else if (img.magicNum == "P6" || img.magicNum == "P5") {
            //includes whitespace
            file.unsetf(ios_base::skipws);
            copy(istream_iterator<char>(file), istream_iterator<char>(), back_inserter(img.pixels));


        } else {
            cout << "Nope!" << endl;
        }

        file.close();
    }

    //alway P6 for outputs cause others are slow as f
    void save(string path = "") {
        ofstream file(path, ios::out | ios::binary);
        file << "P6" << endl << img.width << " " << img.height << endl << img.maxCol << endl;
        copy(img.pixels.begin(), img.pixels.end(), ostreambuf_iterator<char>(file));

        file.close();

    }


    MyImageClass operator + (MyImageClass const &cls) {
        MyImageClass resCls = MyImageClass();
        resCls.img.width = img.width;
        resCls.img.height = img.height;
        resCls.img.maxCol = img.maxCol;
        for (int i = 0; i < img.pixels.size(); i++) {
            resCls.img.pixels.push_back(min(255, img.pixels[i] + cls.img.pixels[i]));
        }

        return resCls;
    }
    MyImageClass& operator += (MyImageClass const &cls) {
        for (int i = 0; i < img.pixels.size(); i++) {
            img.pixels[i] = min(255, img.pixels[i] + cls.img.pixels[i]);
        }
        return *this;
    }
    MyImageClass operator - (MyImageClass const &cls) {
        MyImageClass resCls = MyImageClass();
        resCls.img.width = img.width;
        resCls.img.height = img.height;
        resCls.img.maxCol = img.maxCol;
        for (int i = 0; i < img.pixels.size(); i++) {
            resCls.img.pixels.push_back(max(0, img.pixels[i] - cls.img.pixels[i]));
        }

        return resCls;
    }
    MyImageClass& operator -= (MyImageClass const &cls) {
        for (int i = 0; i < img.pixels.size(); i++) {
            img.pixels[i] = max(0, img.pixels[i] - cls.img.pixels[i]);
        }
        return *this;
    }

};