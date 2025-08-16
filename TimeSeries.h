#ifndef TIMESEREIS_H  
#define TIMESEREIS_H  
#include <iostream>
#include <string>


class TimeSeries {

    public:

        //variables

        int* years;        
        double* data;

        int yearSize;      
        int dataSize; 

        int numYears;      
        int numData; 

        std::string country;
        std::string countryCode;

        std::string series;
        std::string seriesCode;
        
        
    //Constructor
    TimeSeries();


    //resize function
    void resize();

    bool load(std::string inputFile, int lineNum);

    bool print();

    bool add(int newYear, double newData);

    bool update(int newYear, double newData);

    double mean();

    bool is_monotonic();

    void best_fit(double &m, double &b);

    void load_line(std::string line);

    std::string get_seriesCode();

    bool isValid();


    //destructor
    ~TimeSeries();

};

#endif