# SimulatedCauchyOptimization
Implementation of a stochastic simulation of heat diffusion on an unbounded domain in order to exploit the regularizing effect of fundamental solution of heat equation on a given function, which has to be optimized

# Reasons

Let $\mathcal f : \mathbb R \rightarrow \mathbb R^+$ a non-convex function. Suppose that we are interested in finding $x^\star$ , which is defined as $x^\star := argmax f$.
Let's assume that f , not only is non-convex, but is also not differentiable. The proposed algorithms tries to build a framework for global optimization in pathological situations.

# Euristics behind the algorithm

The main ingredient of the algorithm is the *Fundamental Solution of heat equation*.
In particular, we would like to analyze the effect of taking a certain reward function as initial condition of a heat-equation based
cauchy problem. 
The main reasons behind this approach are basically 2:
1.	*Instant regularization* : we know that the fundamental solution acts a regularizing operators, more formally maps functions in 
the space of continuously derivable functions. This allows us to define a notion of derivative even for function that are discontinuous, allowing
us to build a framework for a generalization of gradient-based optimization techniques.
2.	*Bump smoothing effect* : the heat diffusion tends to smooth thos local extrema which are less desiderable in sense of entity of the value.
Therefore, applying the fundamentla solution to a loss function should be able to smooth those optima which are of less interest.

![plot](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/Instant%20regularization.png)
![plot](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/bump%20regularization.png)

From a more euristical point, we know that heat diffusion happens for a exchange of information between a particle and its neighboorhood.
Moreover, very informally, we can say that the speed of the information is in a certain sense proportional to the entity of information : 
if we imagine to have a long rod of metal with two sources of heat connected to it in two different points, we have the situation where
if we take a particle which is equidistant from both, that particle will aknowledge first the presence of the heat source which has a greater distance in temperature 
from its temperature.
We try to exploit this physical intuition, to build a network of information propagation between points of the domain.

In the algorithm I present , moreover, is inserted a mechanism of penalization of the information relative to minima, in order
to make the information about "very big maxima" faster than information about "very small minima".
More details on mathematical theory will be provided in a separate file.

![plot](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/Algorithm%20improving.gif)

# Extension of the notion of heat diffusion to non-euclidean spaces

With a theoretical approach to the problem it was possible to extend the result also to discrete objects. In particular, the focus was mainly on the Permutation Lattice due to its deep implications in optimization theory.
Here we can see a comparision between the Classic Greedy algorithm VS Simulated Cauchy Greedy algorithm for Travelling salesman optimization, a very popular optimization problem where we search , in the space of all the possible permutation on a given set, the route that minimizes the length to touch all the elements in the set (it's convenient to use a permutation to represent the route , since we have necessarly to touch all elements so basically)

![TravellingSalesman](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/travel.gif)

# Compilation of the library

```console
mkdir build
cd build
cmake ..
make
```
