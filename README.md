# Instructions
Executable file is in the ./build/build folder, and there're two additional folders ./inputs and ./outputs for image testing. To run the program, the current working directory should be in the top folder, and run ./build/build/main.exe.

# PPM
This program can read P2, P3, P5, and P6 formats of PPM images, and can mix different types for image arithmetic. The output is always P6 for performance. 
# Tasks

## Addition
![](figure/addition.jpg)
Mandrill + NYU
```cpp
img = image1 + image2;
```

## Subtraction
![](figure/subtraction.jpg)
Mandrill - NYU
```cpp
img = image1 - image2;
```

## Addition Assignment
![](figure/additionassignment.jpg)
Empire State Building += NYU
```cpp
image3 += image2;
```

## Subtraction Assignment
![](figure/subtractionassignment.jpg)
Empire State Building -= NYU
```cpp
image3 -= image2;
```
## array index operator
return rgb value at a specific index in image 
```cpp
//first pixel value is 164
cout << "first pixel value is " << image1[0] << endl;
```

## Gamma Correction
![](figure/gammaCorrection.jpg)
Gamma 2
```cpp
img = image1.gammaCorrection(2.f);
```

## Alpha Compositing
![](figure/alphaCompositing.jpg)
Mandrill(0.85) + NYU(0.15)
```cpp
img = image1.alphaCompositing(image2, 0.85f);
```

## Edge Detection
TBD
