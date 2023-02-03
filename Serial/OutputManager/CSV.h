/***
* ~ CSV.h
* Author : Luca Muscarnera
* Description : gestisce la scrittura su file CSV
*/
#pragma once
#include <string>
#include <fstream>

#ifndef CSV_H
#define CSV_H

class CSV
{
	public:
	CSV(std::string filename) {
	 file.open (filename, std::ofstream::out );
	 file << "";
	 file.close();
     file.open (filename, std::ofstream::app );
	}
	
	template<class T>
	void write(std::vector<T> vect) {
		for(int i = 0 ; i < vect.size();i++) {
			file << vect[i];
			if(i < vect.size() - 1 )	
				file << ",";
		}
		file <<  "\n";
		//std::clog << "line written" << std::endl;
	}
	
	~CSV() {
		file.close();
	}
	
	std::ofstream file;
};

#endif
