# SimulatedCauchyOptimization

(Progetto svolto in autonomia da Luca Muscarnera)

Implementation of a stochastic simulation of heat diffusion on an unbounded domain in order to exploit the regularizing effect of fundamental solution of heat equation on a given function, which has to be optimized

# Reasons

Let $\mathcal f : \mathbb R \rightarrow \mathbb R^+$ a non-convex function. Suppose that we are interested in finding $x^\star$ , which is defined as $x^\star := argmax f$.
Let's assume that f , not only is non-convex, but is also not differentiable. The proposed algorithms tries to build a framework for global optimization in pathological situations.
Finally, we extend this whole framework to a discrete case where we try to model the heat diffusion on the ```Permutation Lattice``` and try to exploit the same way of reasoning to optimization problems on permutations.

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

# Overview of the library - What's the idea?

The main idea of the library is to have a tool for optimizing objects. We try to find a common method for optimizing object in various space, with the notable example of permutations which do not have commutative sum.
In particular, from a Developement point of view , the library implements Compile Time Polymorphism using ``` CONCEPTS ``` , a new C++ tool introduced in the last few years.
In particular Concepts, allow the programmer to define polymorphism not in the sense of describing classes with hierarchy but instead describing a certain subset of the space of classes.
Doing so , concepts allow a great freedom in the definition of polymorphism , allowing at the same time the correct functioning of the application.
In fact, the idea behind the concepts is to allow compilation every time a certain class respects given conditions. Therefore , templates become powerful tools which enables a very generalist framework of programming.
Moreover, as I said, concepts are evaluated in compile time , reducing even the minimal overhead which abstract classes may have caused.

# Overview of the library - How we exploited the concept of Vectorial

We defined the concept ```Vectorial``` , which we mathematically defineed - with an abuse of notation - as all the spaces which have a notion of ```neighbourhood```z and a ```sum``` operation (not necessarly commutative).This concepts directly translates in a subset of possible classes that can be fed to the ```Optimizer``` template class.
Here is clear the role of the Vectorial concept : since Optimizer needs the operations that make a space a ```Vectorial```, then we let the template accept only elements in this family of classes.
Moreover, a notable fact, the relaxing of a function is NOT DEPENDENT from the space of reference, so this was implemented in the Optimizer class.


# Overview of the library - How does the optimization happens?
After that we initialized the optimizer with the correct set of parameters, the optimizer calculate the generalized gradient.
Generalized gradient , in this context, is a notion of weaker derivative described as "the direction that improves the most the optimization locally".
Every space has , in the vectorial concept, a generalized gradient.
While for Vector and Real , gradient was computed with the same approach of the computation of the solution ( I invite you to read some of the mathematical proofs in the AID folder) , in the Permutation class generalized gradient was implemented with a particular approach ; in fact it returns the GREEDY DIRECTION of improvement, but not in the original function, using instead the relaxed one.
This difference of approach was not a problem semantically, thanks to our weaker notion of derivative.
After the calculation of the gradient, the new solution is updated and so on until desired

# Overview of the library - How can we customize the optimization?

Non-Convex optimization is a difficult task. Especially, since this library approach the problem with a stochastic flavour it is often necessary to choose the correct set of HyperParamers.
A part from the properties of the optimizer itself (e.g. Precision and Time, which describe respectively the precision of the approximation and the time passed in the simulated heat diffusion) , we can also exploit the idea of ```CALLBACKS``` to interact with the optimzire during the procedure.
CALLBACKS are simply functors with a call operator, which can be fed to the Optimizer with the ```.addCallback``` method before the launch of the Optimizer.
In practice callbacks, offers the possibilty of passing every kind of class with a call operator to the Optimizer, giving the possibility not only to run function but also to expolit functions with a state during the optimization.
Notable example of this is the ```simpleCallback``` : the method that show optimization basic data (value of the variable) is a callback, but also ```backwardInTime```, that reduce the time every a certain number of steps.
Moreover callbacks may have a special method "init", which is called when they are saved in the optimizer. Is important to state that we used ```requires``` in order to make the definition of the method totally not mandatory, in order to improve flexibility in the development without introducing risks in the structure of the code.


# Overview of the library - What is the computational cost? Improvement through parallelism

Since computation of the gradient is the main bottleneck in the procedure, and unluckly the dependency between the steps of procedures does not allow to parallelize directly the generalizedGradient function , we tried to achieve a good level of parallelization mainly in the vectorial classes.
All the tests allow to set the number of active threads, in order to observe their performance changes.



# Compilation of the library

```console
make
```


REMARK: In every test, it is possible to (select the number of threads in parallel by simply putting it after the program name)

# Running test 1

Tests the travelling salesman on the verteces of a convex polygon, which represents one of the few cases where the solution is known.
Running the optimization with an ad hoc schedule, we are able to converge to the global optimum.
(it is interesting if you think the probability by choosing randomly would be $O(\frac{1}{n!})$

After running ```make``` it is sufficient to perform two steps

1) running the test
```
cd build
./test1 <n_threads>
```
2) Produce the visualization
```
python ../Visualizer/Test1/Test1.py
```

# Running real test

Optimizes a simple non convex function.
Keep in mind that the visualization procedure is very heavy, therefore is not a test intended to be an evaluation of the performance.
If it is desired to evaluate purely the performance with real number optimization, is necessary to remove the callbacks used to implement the graphics (see later for an explanation about callbacks)

1) running the test
```
cd build
./real_test <n_threads>
```

2) Produce the visualization
```
python ../Visualizer/RealTest/RealTest.py
```

The visualization will show the so called "perceived function" : what does the variable see at a certain time? It shows the relaxing through the procedure.

# Running Vector test

On the opposite we have a test without visualization. A non convex function similar to the one of the real test is optimized in five dimensions.


