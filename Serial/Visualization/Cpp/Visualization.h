#pragma once
#include <functional>
#include  <vector>

template<class T>
std::vector<T> map( std::function<double(T)> f, std::vector<T> X )
{
		std::vector<double> Y(X.size());
		for(int i = 0 ; i < X.size();i++)
			Y[i] = f(X[i]);
		return Y;
}

double min(std::vector<double> x)
{
	double min = x[0];
	for(int i = 0 ; i < x.size();i++)
		if( x[i] < min)
			min = x[i];
	return min;
}


double max(std::vector<double> x)
{
	double max = x[0];
	for(int i = 0 ; i < x.size();i++)
		if( x[i] > max)
			max = x[i];
	return max;
}

void stupidPlot(std::function<double(double)> f, double x_from , double x_to,std::string title = "plot") {
    

    // costruisco la griglia
#define H 50
#define W 50

    std::vector<std::vector<char>> screen(H, std::vector<char>(W,' '));
    
    std::vector<double> x_space(W);
    for (int i = 0; i < x_space.size(); i++)
    {
        x_space[i] = (x_to - x_from) / x_space.size() * i + x_from;
    }

    std::vector<double> y_space = map<double>(f, x_space);
    
    double y_from = min(y_space);
    double y_to   = max(y_space) + 1.;


    for (int i = 0; i < y_space.size(); i++)
    {  
        int pos =  (double) y_space.size() * (y_space[i] - y_from) / (y_to - y_from);
        screen[pos][i] = 'O';
        pos--;
        while (pos >= 0) {
            if( pos * (y_to - y_from) / y_space.size() + y_from > 0)
                screen[pos][i] = '+';
            else
                screen[pos][i] = '-';
            pos--;
        }
    }

    std::cout << title << "\n";
    std::stringstream ss;

    for (int i = y_space.size() - 1; i >= 0; i--) {
        for (int j = 0; j < x_space.size(); j++)
                {
                 ss << screen[i][j] << screen[i][j];
                }

    ss << ". \t ";
    if (i % (y_space.size() / 10) == 0)
        ss << (y_to - y_from) / y_space.size() * i + y_from;
    ss << "\t \n";
    }
    
    for (int j = 0; j < x_space.size(); j++)
        if (j % (x_space.size() / 10) == 0)
            ss << (int)(j) << " ";
        else
            ss << "  ";
    ss << "\n";
    std::cout <<  ss.str();
}
