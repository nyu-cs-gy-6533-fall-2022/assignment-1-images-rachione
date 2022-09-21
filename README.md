# Instructions
Executable file is in the ./build/build folder, and there're two additional folders ./inputs and ./outputs for image testing. To run the program, the current working directory should be in the top folder, and run ./build/build/main.exe.

# PPM
This program can read P2, P3, P5, and P6 formats of PPM images, and can mix different types for image arithmetic. The output is always P6 for performance. 
# Tasks

## Addition
![](outputs/addition.ppm)
```cpp
img = image1 + image2;
```

## Subtraction
```cpp
img = image1 - image2;
```

## Addition Assignment
```cpp
image3 += image2;
```

## Subtraction Assignment
```cpp
image3 -= image2;
```
## array index operator
```cpp
cout << "first pixel value " << image1[0] << endl;
```

## Gamma Correction
```cpp
img = image1.gammaCorrection(2.f);
```

## Alpha Compositing
```cpp
img = image1.alphaCompositing(image2, 0.85f);
```

## Edge Detection
TBD
