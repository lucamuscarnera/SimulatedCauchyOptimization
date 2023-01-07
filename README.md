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

![No enhancing of information flow](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/information%20propagation%20standard.png)
![Enhanced information flow](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/information%20propagation.png)

In the picture you can observe the behavior of the heat diffusion without and with the optimized mechanism of information propagation.

# Usage of the test


![non convex function](https://github.com/lucamuscarnera/SimulatedCauchyOptimization/blob/main/Non%20Convex%20Function.png)
At the moment you can try the algorithm using *test1D*.
In particular, with this application you will be able to maximize an extremely invex function (which can be seen in the png file, with the maximum highlighted)
You can play with parameters in order to observe different speed / results.

1.	*-x0* : value of the initial guess
2.	*-n* : number of iterations of gradient ascent.
3.	*-r* : relaxing parameter. Recalling the physical analogy, is a parameter that quantify the how much time is passed from the instant where heat diffusion starts.
4.	*-p* : minima penalization. The mechanism will be explained later, but it basically measures the order of increase of speed of information propagation in function of the entity of the maxima.
5.	*-precision* : precision of the approximation of heat equation solution. The algorithm for heat solution simulation will be explained later, but this parameter basically describe the precision of the result.

Suggested parameters (vary *-x0* to observe stability of the solution ):
./test1D -x0 -9. -n 100 -r 4. -p 30. -precision 300
