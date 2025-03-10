# Flattener 
Demo C++ implementation of the terrain flattener method with 2D and 1D functions.

## Behaviour
This program creates an example heightmap to show how the algorithm works, then performs the three steps in a straightforward way: it computes the partial derivatives, quantizes them and recreates the transformed heightmap.

The main functions are IntegrateHorizontal, IntegrateVertical, Integrate2D each allowing to pass the number of iterations, a value as low as 10 should produce visible changes. Choosing the right quantization parameter is important, a very small one should recreate the original heightmap, while a too high one can produce a smoothed terrain. The deltatime dt can go from 0 to about 0.5, a value higher than 0.4 is advised.


### Horizontal integration example

<img src="/images/screen0.png" width="320">  

Original heightmap

<img src="/images/screen_h60_24.png" width="320">  

Horizontally flattened heightmap (24 iterations)

### Vertical integration example

<img src="/images/screen_v60_24.png" width="320">  

Vertically flattened heightmap (24 iterations)

## 2D integration results
A 2d scheme that allows vertical and horizontal slopes formation can be obtained combining the two previous schemes.

<img src="/images/screen60_4.png" width="320">  
2D flattened heightmap (4 iterations)

<img src="/images/screen60_60.png" width="320">  
60 iterations

<img src="/images/screen60_240.png" width="320">  
240 iterations

<img src="/images/screen30_240.png" width="320">  
Halving the quantization parameter (240 iterations)
