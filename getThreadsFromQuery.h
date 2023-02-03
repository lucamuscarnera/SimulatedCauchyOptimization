// getThreadsFromQuery.h
// dati arg e argv ritorna il numero di threads

#include <string>
#ifndef GETTHREADS_H
#define GETTHREADS_H

int getThreadsFromQuery(int argc, char ** argv)
{
	if(argc == 1)
	{
		return 8;
	}
	else {
		auto numero = (argv[1]);
		return atoi(numero);
	}
	
}

#endif

