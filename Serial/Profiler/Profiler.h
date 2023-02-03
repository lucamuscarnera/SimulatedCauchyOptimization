/*
simple construction destruction Profiler c++
*/
#pragma once
#include <fstream>
#include <string>
#include <chrono>
#ifndef PROFILER_H
#define PROFILER_H

class Profiler
{
	public:
	
	static void init(std::string fname) {
		filename = fname;
		std::ofstream file (filename, std::ofstream::app );
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		file << "+-----------------------------------------------+\n";
		file << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "\n";
		file.close();
	}
	Profiler(std::string event) : event(event)  {
		start = std::chrono::high_resolution_clock::now();
		int time = std::chrono::duration_cast<std::chrono::nanoseconds>(start - beginning).count();
		std::clog << "start profiling : " << event << "at " << time  << std::endl;
	}
	
	~Profiler() {
		end   = std::chrono::high_resolution_clock::now();
		int time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beginning).count();
		std::clog << "end profiling : " << event << " at " << time << std::endl;
		std::ofstream file(filename, std::ofstream::app );
		long int ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
		file << event << " ===> \t" << ns << " ns" << "\t" << (int) (((double)ns )/1000.) << " micros" <<  "\t" << (int) (((double)ns )/1000000.) << " ms"  <<  "\n"; 
		file.close();
	}
	
	std::string event;
	static std::string filename;
	static std::chrono::time_point<std::chrono::high_resolution_clock> beginning;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;

};
std::string Profiler::filename = "";
std::chrono::time_point<std::chrono::high_resolution_clock> Profiler::beginning = std::chrono::high_resolution_clock::now();

#endif