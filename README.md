# EquationSolver

## Overview
I heard a lot about [caffe](https://github.com/BVLC/caffe) and [deep learning](http://cs231n.github.io/) whilst getting my masters at USC. I finally got a [raspberry pi](https://www.raspberrypi.org/) and decided to train some [convolutional neural nets](http://cs231n.github.io/convolutional-networks/) or cnns or dnns (deep neural networks, it's all the same fancy lingo) to detect hand written equations like you see in the photo and go on to solve them.

![example input image](https://github.com/TernaK/EquationSolver/blob/master/equation_solver/src/data/test4.jpg)

I set up the pi and slowly built the system piece by piece on my mac and pi concurrently because I had difficulty using [opencv](https://github.com/opencv/opencv)'s implementation of caffe dnns - it crashed on my mac but worked fine on the pi. The other issue is I love to develop using Xcode as I feel right at home, but the pi doesn't have Xcode.

These major parts make up the system;
- Equation extraction using a rectangle cue.
- Character extraction to obtain the written equation.
- Parsing and mathematical evaluation of the written equation.
- Data collection, extraction, and tools to generate caffe readable datasets.

The code is predominantly C++ but a crucial chunk is python because it's great for quickly moving data in and out as I had to when creating my datasets. The rest is [cmake](https://cmake.org/examples/) which I learned working for the great guys at [Planck Aero](http://www.planckaero.com/team.html) and it makes building projects with multiple dependencies easy, a dream really.
