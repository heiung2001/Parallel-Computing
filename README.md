# Parallel Computing
There are 3 parts in this class.

## Part 1: OpenMP
In this part, I used C/C++ with the libary called "omp.h". I learned how to design parallel algorithms and applied it to some simple problems like pi calculation, fibonacci, matrix multiplication, ...

## Part 2: Grid Computing - Alchemi
My lecturer taught me to create a grid using Alchemi - a middleware base on dotnet framework and of course, you can only run it on Windows. Although it is quite outdated, but I think I've had some base knowledge about grid (and maybe cloud computing).

You can install Alchemi in [here](https://sourceforge.net/projects/alchemi/). After installed, you need to create a project with a 2.0 dotnet framework cause it isn't supported by higher version of dotnet framework. Then you find the Alchemi.Core.dll file and add it to your project. Finally, build your own grid.

In my Alchemi project, because of low version dotnet framework so i cannot use ```BigInteger``` in ```System.Numerics```. I used [this](https://www.codeproject.com/Articles/36323/BigInt) instead to compute with big numbers.

## Part 3: Some parallel technique