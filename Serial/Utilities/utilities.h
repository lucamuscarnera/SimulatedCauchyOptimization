/*
* Utilities.h
* Luca Muscarnera ~ 2023
* ++++++++++++++++++++++++++++++++++++++++++++
* Contains some utilities of vector manipulation. The choice of building a library was motivated by the
* idea of treating the parallelization of the following operations in a blackbox way with respect to the
* diffusion and optimizer libraries.
*/

#pragma once
#include <vector>
#include <functional>
#include <sstream>

std::vector<double> map(std::function<double(double)> f , std::vector<double> & v) {
    std::vector<double> ret(v.size());
    for (int i = 0; i < v.size(); i++) {
        ret[i] = f(v[i]);
    }
    return ret;
}


double mean(std::vector<double>& v) {
    double ret = 0.;
    for (int i = 0; i < v.size(); i++)
        ret += v[i];
    return ret / v.size();
}

std::vector<double> pairwiseprod(std::vector<double>& u, std::vector<double>& v) {
    std::vector<double> ret(v.size());
    for (int i = 0; i < v.size(); i++) {
        ret[i] = u[i] * v[i];
    }
    return ret;
}

std::vector<double> sub(std::vector<double>& u, std::vector<double>& v) {
    std::vector<double> ret(v.size());
    for (int i = 0; i < v.size(); i++) {
        ret[i] = u[i] - v[i];
    }
    return ret;
}

double max(std::vector<double>& v) {
    double w = v[0];
    for (int i = 1; i < v.size(); i++) {
        if (v[i] > w)
            w = v[i];
    }
    return w;
}


std::vector<double> product(double k , std::vector<double>& u) {
    std::vector<double> ret(u.size());
    for (int i = 0; i < u.size(); i++) {
        ret[i] = u[i] * k;
    }
    return ret;
}

double min(std::vector<double>& v) {
    auto w = map([](double x) { return - x; }, v);
    return - max(w); 
}

void stupidPlot(std::function<double(double)> f, double x_from , double x_to,std::string title = "plot") {
    

    // costruisco la griglia
#define H 100
#define W 100

    std::vector<std::vector<char>> screen(H, std::vector<char>(W,' '));
    
    std::vector<double> x_space(W);
    for (int i = 0; i < x_space.size(); i++)
    {
        x_space[i] = (x_to - x_from) / x_space.size() * i + x_from;
    }

    std::vector<double> y_space = map(f, x_space);
    
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