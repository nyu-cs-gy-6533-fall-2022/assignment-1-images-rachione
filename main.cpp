#include <iostream>
#include <exception>
#include "MyImageClass.h"
using namespace std;
int main() {
	MyImageClass image1("./inputs/Mandrill.ppm");
	MyImageClass image2("./inputs/tandon_stacked_color.ppm");
	MyImageClass image3("./inputs/Empire_State_Building.ppm");
	MyImageClass img;

	cout << "first pixel value is " << image1[0] << endl;

	//Image Arithmetic
	img = image1 + image2;
	img.save("./outputs/addition.ppm");
	img = image1 - image2;
	img.save("./outputs/subtraction.ppm");

	image3 += image2;
	image3.save("./outputs/additionassignment.ppm");
	image3 -= image2;
	image3.save("./outputs/subtractionassignment.ppm");

	img = image2 * 3.1f;
	img.save("./outputs/multiplication.ppm");


	//Image processing
	img = image1.gammaCorrection(2.f);
	img.save("./outputs/gammaCorrection.ppm");

	img = image1.alphaCompositing(image2, 0.85f);
	img.save("./outputs/alphaCompositing.ppm");

	img = image1.edgeDetection();
	img.save("./outputs/edgeDetection.ppm");





	return 0;
}