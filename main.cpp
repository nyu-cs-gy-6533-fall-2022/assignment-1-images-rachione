#include <iostream>
#include <exception>
#include "MyImageClass.h"
using namespace std;
int main() {
	MyImageClass image1("./inputs/Mandrill.ppm");
	MyImageClass image2("./inputs/tandon_stacked_color.ppm");
	MyImageClass image3("./inputs/Empire_State_Building.ppm");

	MyImageClass img;
	img = image1 + image2;
	img.save("./outputs/addition.ppm");
	img = image1 - image2;
	img.save("./outputs/subtraction.ppm");

	image1 += image2;
	image1.save("./outputs/additionassignment.ppm");
	image3 -= image2;
	image3.save("./outputs/subtractionassignment.ppm");



	return 0;
}