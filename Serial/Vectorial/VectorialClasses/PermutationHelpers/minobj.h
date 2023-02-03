// minobj.h
// definisce una classe che renda il calcolo della direzione ideale possibile in parallelo
// introducendo un operazione associativa "transform" che consente di effettuare l'operazione
// di ricerca della direzione ottimale mediante una Reduction
#ifndef MINOBJ_H
#define MINOBJ_H

	class minobj {
		
		/* classe toy costruita unicamente per permettere di fare la reduction con openmp */
		
		public:
		// transform:
		// prende in ingresso un oggetto min obj, se il val é minore del suo ...
		minobj() : val(1000) , a(0), b(0) {};
		minobj(double val,int a,int b) : val(val) , a(a) , b(b) {};
		minobj & transform(minobj & other);
		
		private:
		
		double val;
		int a;
		int b;
		
		public:
		
		double getA() { return a; }
		double getB() { return b; }
		double getVal() { return val; }
		
	};
	
#endif