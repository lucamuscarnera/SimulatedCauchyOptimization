# Some notes #
- The project compiles and runs without issues. Simple tests show some scalability. The project provides interesting tests (optimization of both a discrete and a continuous function). It also provides nice visualization tools.
- Some headers are missing includes. For instance `CSV.h` uses `std::vector` but does not include
`<vector` nor other header files that include it. Clearly in the context of the code it works, probably because it is always included afted another header file providing `<vector>`, yet is an errors since it reduces usability of the header file.
- Nice use of advanced c++. Even concepts and `requires` expressions!. I did not know they can be used outside the context of concepts. You have used it as a sort of type trait without the complexity of `decltype`+`declval`. Cool, nice to know.
- Why you don't declare const methods that do not modify the state of the class? You make your code less usable by forgetting `const`: In `Optimizer`
````
	int getThreads() {return threads;};
````
should be
````
	int getThreads() const {return threads;};
````
- Use of callbacks. Good.
- Lots of comments, good. Yet sometimes not very clear.
- `Neighbourhood::push_back` should take a `const` argument
- In `Vector::buildCanonicalNeighbourhood` the code `Vector z = row; canonicalNeighbourhood[i] = (z);` makes many copies of the same data. Here `emplace_back` or building directly into `canonicalNeighbourhood` would be much better. Comment applies also to other places.
- `OptimizerStatus::push_back` should take `const &` argument

```c++
void push_back(T const & event, double pippo)
	{
		history.push_back(event);
		loss.push_back(pippo);
	}
```

- If you pass potentially large objects (where large may mean just a dozen of bytes) by value and not by const reference you lose performance because of useless copies. `const &` is your friend. Use it. It doesn't bite. The only case where you may lose performance by using it is with `int` and `double`, but only if the compiler is smart enough to keep the values in processor register.
- The code looks nice (a part the many missing `const`). No time to delve into the theory though, so I cannot say if the idea has major flaws or not. In any case, it is a nice idea.  
- Number of threads is hard coded to 16 in `Optimizer.h` line 169
- In some classes some attributes which should have been private are instead left public with no particular reason.
- 
# MInor things #
- In some classes some attributes which should have been private are instead left public with no particular reason.
- A few unused parameters and unused values. Nothing serious but annoying if you activate warnings (by the way, why you don't like activating warnings?, you would have found olso other little glitches). 
- Recent versions of g++ support -std=c++20 which implies -fconcepts
- I suggest to be explicit in the level of optimization you want from the compiler. If you don't indicate anything is -O2. But maybe you may want to try -O3, and in any case if you indicate it there is no ambiguity.
- Naming convention for variables is sometimes horrible (see `pippo`)
- Some functions that you use are not the `std` version (e.g. `atoi` in `getThreadsFromQuery`, `sqrt` in `Vector.cpp`)
- There are some C-style cast for `double`s. It is not an error, but in C++ it is better to use the C++-style cast because they are safer. 
- Beware, `#pragma once` is nice but not standard
- `OptimizerModule.h` and `VectorialModule.h` are pretty much useless
- `Neighbourhood::size` should be a `const` method
- `Neighbourhood::operator[]` should be also present in the `const` version
- `static Vector zero(Vector & x)` is a bit strange: either you pass the vector as `const` or you pass only the size (which I think would make more sense)
- `Vector::getData` should come also in the `const` version 
- Should use `Optimizer<Vector> &` instead of `Optimizer<Vector> *`
- I think that it would make more sense for `Vector::canonicalNeighbourhood` to be a const reference to a reference neighborhood computed somewhere else that `static`. `static` is probably a too strict constraint. 
- Beware in defining the `Vector::operator<` as a lexicographic ordering. Indeed, $R^n$ is not ordered, so one could mistake it for an inequality in euclidean norm. `operator`s should be implemented only when their meaning is very clear.
- `Permutation::getData` should have the const version 
- You often use different integral types interchangeably , for instance `OptimizerStatus::size` should return `size_t` (and should be const). This is not an error in most cases, but it is better to be uniform.
* `Optimizer::getStatus` and `Optimizer::getVariable` should come also in a `const` version
