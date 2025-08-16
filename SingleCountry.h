#ifndef SINGLECOUNTRY_H  
#define SINGLECOUNTRY_H
#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "LinkedList.h"
#include <unordered_map>  
#include <vector>     


class SingleCountry{

    private:

        //private variables

        std::string file;

        std::string inputFile;

        LinkedList<TimeSeries> *countryList;

        std::string singleCountryName;

        std::string singleCountryCode;



    public:

    //default constructor or smth
    SingleCountry();

    //constructor
    SingleCountry(std::string fileName);


    //destructor
    ~SingleCountry();

    std::string country_name();

    std::string country_code() const;

    LinkedList<TimeSeries>* getCountryList();


    void load(std::string country);

    void list();

    void add(std::string seriesCode, int year, double data);

    void update(std::string seriesCode, int year, double data);

    void print(std::string seriesCode);

    void remove(std::string seriesCode);

    void biggest();

    void print_name();

    //for the map
    bool operator== (const SingleCountry& other)const;




};



#endif