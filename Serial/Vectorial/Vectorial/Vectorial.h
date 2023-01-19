/*
*  	Vectorial.h
*	Author:	Luca Muscarnera
*	Descr : Contiene il concept di oggetto Vectorial.
*   Un oggetto Vectorial é un oggetto possiede una rappresentazione interna come oggetto Vector, e che quindi permette di
*	essere convertito in un oggetto Vector istantaneamente semplicemente ritornando la sua rappresentazione.
*	In altri termini, un oggetto appartenente al concept Vectorial é un wrapper attorno alla classe Vector, che viene quindi decorata
*	con metodi propri dell'oggetto rappresentato
*
*	Ad esempio un'ipotetica classe Matrix ha una rappresentazione interna come Vector, ma anche metodi specializzati per la manipolazione
*	dell'oggetto come matrice ( ad esempio il metodo Matrix::get(int row,int col) che ritorna un elemento alla riga row e alla colonna col.
*	Il concetto Vectorial contiene anche i double, in modo da inglobare anche l'insieme R nei possibili spazi vettoriali.
*/
#pragma once

template <typename T>	
concept Vectorial = requires(T a, T b) {		// costruisco la sotto-collezione di tutte le classi che soddisfa il requirement
	//a += b;														/* possesso di operazione di somma    */
	//a * c;														/* possesso di operazione per scalare */
	  
	//{ a.buildCanonicalNeighbourhood(precision) } -> void;		/* deve esistere una funzione per la costruzione del vicinato canonico */
	//{ a.neighbourhood() } -> void;								/* deve esistere una funzione per la costruzione del vicinato canonico */
	//{ a.generalizedGradient() } -> T;							/* deve esistere un metodo per ottenere il vettore che descrive la 
	//															*  direzione privilegiata per la massimizzazione della funzione di costo
	//			*/
	a += b;
};