# Some notes #
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
- Lots of comments, good.
- The code looks very nice (a paart the missing const). No time to delve into the theory though, so I cannot say if the idea has major flaws or not. In any case, it is a nice idea.  
# MInor things #
- A few unused parameters and unused values. Nothing serious but annoying if you activate warnings (by the way, why you don't like activating warnings?). 
- Recent versions of g++ support -std=c++20 which implies -fconcepts
- I suggest to be explicit in the level of optimization you want from the compiler. If you don't indicate anything is -O2. But maybe you may want to try -O3, and in any case if you indicate it there is no ambiguity.
