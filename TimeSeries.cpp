#include <iostream>
#include "TimeSeries.h"
#include <string>
#include <fstream>






//two dynamically allocated arrays
int* years;
double* data;

//max size of the arrays
int yearSize;
int dataSize;

//number of elements in the arrays
int numYears;
int numData;

std::string country;
std::string countryCode;

std::string series;
std::string seriesCode;

//constructor
TimeSeries::TimeSeries(){

    //default size to 2
    yearSize = 2;
    dataSize = 2;

    //setting the size of the arrays to the default size of 2
    years = new int[yearSize];
    data = new double[dataSize];

    //number of items in the array is 0 from the start
    numYears = 0;
    numData = 0;
}


//check to see if the array needs resizing and does it
void TimeSeries::resize(){

    //checks to see if the array needs changing
    bool sizeChanged = false;

    //if the arrays are at capacity, then double them
    if(numYears == yearSize){

        yearSize = yearSize * 2;
        sizeChanged = true;
    }


    if(numData == dataSize){

        dataSize = dataSize * 2;
        sizeChanged = true;
    }

    //if the arrys are at quarter capacity, then half them
    if(numYears == 0.25 * yearSize){

        yearSize = yearSize / 2;
        sizeChanged = true;

        if(yearSize < 2){

            yearSize = 2;
        }
    }

    if(numData == 0.25 * dataSize){

        dataSize = dataSize / 2;
        sizeChanged = true;

        if(dataSize < 2){

            dataSize = 2;
        }
    }

    if(sizeChanged == true){

        int* new_years = nullptr;
        double* new_data = nullptr;

        try{

            new_years = new int[yearSize];
            new_data = new double[dataSize];

            for (int i = 0; i < numYears; i++){

                new_years[i] = years[i];
   
            }

            for (int i = 0; i < numData; i++){

                new_data[i] = data[i];

            }

            delete[] years;
            delete[] data;

            years = new_years;
            data = new_data;



            new_years = nullptr;
            new_data = nullptr;

        } catch(const std::bad_alloc&){

            delete[] new_years;
            delete[] new_data;
        }

    }
    

}

void TimeSeries::load_line(std::string line){



    //get the country name

    int i = 0;

    while(line[i] != ','){

        country += line[i];
        i++;
    }
    

    //get to the next non-comma entry
    i++;

    while(line[i] != ','){

        countryCode += line[i];
        i++;
    }

    i++;

    while(line[i] != ','){

        series += line[i];
        i++;
    }

    i++;

    while(line[i] != ','){

        seriesCode += line[i];
        i++;
    }

    i++;

    
    

    //get the rest of the data

    std::string dataAsString;

    int j = 0;
    int year = 1960;

    //go through the entire length

    while(i < line.length()){

        //if you find a comma, take the whole of the last string, make it a double and store it along w the year

        if(line[i] == ','){

                resize();

                years[j] = year;

                year++;

                data[j] = std::stod(dataAsString);

                i++;

                j++;

                dataAsString = "";

                numYears++;
                numData++;

            
                continue;       
        }

        dataAsString += line[i];

        i++;   
    }

    //to account for the last data since it doesn't end with a comma

    resize();

    numYears++;
    numData++;

    years[j] = year;
    data[j] = std::stod(dataAsString);

    resize();
    
}

bool TimeSeries::load(std::string inputFile, int lineNum){

    if(inputFile.empty()){ 

        std::cout << "empty" << std::endl;

        return false;
    }

    std::ifstream file(inputFile);

    

    

    std::string line;

    for(int i = 0; i < lineNum; i++){

        std::getline(file, line);
    }

    

    //first four entries of the input file correspond to the country, series, and their codes
    std::getline(file, line, ',');

    country = line;


    std::getline(file, line, ',');

    countryCode = line;

    std::getline(file, line, ',');

    series = line;

    std::getline(file, line, ',');

    seriesCode = line;

    

    


    int i = 0;
    int year = 1960;
    double dataAsDouble;

    

    while(std::getline(file, line, ',')){


        

        dataAsDouble = std::stod(line);

        years[i] = year;
        data[i] = dataAsDouble;

        


        i++;
        year++;

        numYears++;
        numData++;

        


        resize();

        



        if(year > 2023){

            break;
        }

        
    }

    
   
    
    
    

 



    return true;

}

bool TimeSeries::print(){

    bool hasData = false;

    for(int i = 0; i < numYears; i++){

        if(data[i] == -1){

            continue;
        }

        else{

            hasData = true;    
            std::cout << "(" << years[i] << "," << data[i] << ") ";
        }

    }

    if(hasData == false){

        std::cout << "failure" << std::endl;
        return false;
    }

    std::cout << std::endl;

    return true;

}


bool TimeSeries::add(int newYear, double newData){

    

    bool copyYear = false;
    bool isNewYear = true;

    //checks to see if the newYear has valid data

    for(int i = 0; i < numYears; i++){

        if(newYear == years[i]){

            isNewYear = false;

        }

        if(newYear == years[i] && data[i] != -1){

            copyYear = true;
        }
    }

    if(copyYear){

        std::cout << "failure\n";
        return false;
    }



    //code to add a new data point

    else if(isNewYear){

        //1. i need to find where the index at which new data point should go
        int targetIndex = 0;

        for(int i = 0; i < numYears; i++){

            if(newYear < years[i]){

                targetIndex = i;
                break;
            }

            else if(newYear > years[numYears - 1]){

                targetIndex = numYears;
                break;
            }

        }

        //2. resize the array to allow for the new element

        numYears++;
        numData++;

        resize();

        //3. i need to shift all the array elements to the right of the target index to the right

        for(int i = numYears - 1; i > targetIndex; i--){


            years[i] = years[i - 1];
            data[i] = data[i - 1];       
        }

        //4. Put the data in

        years[targetIndex] = newYear;
        data[targetIndex] = newData;


        std::cout << "success\n";

       return true;

    }

    //code to change a current year data

    else{

        for(int i = 0; i < numYears; i++){

            if(years[i] == newYear){

                data[i] = newData;

                break;

            }
        }

        std::cout << "success\n";
        
        return true;

    }




}


bool TimeSeries::update(int newYear, double newData){

    int targetIndex = 0;
    bool updatable = false;

    for(int i = 0; i < numYears; i++){


        if(years[i] == newYear && data[i] != -1){

            targetIndex = i;
            updatable = true;
            break;
        }
        
    }

    if(!updatable){

        std::cout << "failure" << std::endl;

        return false;
    }

    else{

        data[targetIndex] = newData;

        std::cout << "success" << std::endl;
        return true;
    }

}




TimeSeries::~TimeSeries(){

    delete[] years;
    delete[] data;

    years = nullptr;
    data = nullptr;


}


double TimeSeries::mean(){

    double total = 0;
    int numValid = 0;
    double mean = 0;
    bool isValid = false;


    for(int i = 0; i < numData; i++){


        if(data[i] != -1){

            total += data[i];
            numValid++;
            isValid = true;
        }

        else{

            continue;
        }
    }

    mean = total/numValid;


    if(isValid){


        return mean;
    }

    else{

        return 0;
    }




}


bool TimeSeries::is_monotonic(){

    bool isValid = false;
    bool isIncreasing = true;
   

    for(int i = 0; i < numData - 1; i++){

        if((data[i] < data[i + 1]) && data[i] != -1 && data[i + 1] != -1){

            isValid = true;
            isIncreasing = true;
            break;
        }

        else if(data[i] > data[i + 1] && data[i] != -1 && data[i + 1] != -1){
            
            isValid = true;
            isIncreasing = false;
            break;
        }

        else if(data[i] == data[i + 1] && data[i] != -1 && data[i + 1] != -1){

            isValid = true;
            continue;
        }
    }

    if(!isValid){

        std::cout << "failure" << std::endl;
        return false;
    }

    else if(isIncreasing){

        for(int i = 0; i < numData; i++){

            for(int j = i + 1; j < numData; j++){

                if(data[i] < data[j] && data[i] != -1 && data[j] != -1){

                    std::cout << "series is not monotonic" << std::endl;
                    return false;
                }
            }

    
        }
    }

    else if(!isIncreasing){

        for(int i = 0; i < numData; i++){

            for(int j = i + 1; j < numData; j++){

                if(data[i] > data[j] && data[i] != -1 && data[j] != -1){
                    
                    std::cout << "series is not monotonic" << std::endl;
                    return false;
                }
            }

    
        }

    }

    

    return true;
    

}



void TimeSeries::best_fit(double &m, double &b){

    bool isValid = false;

    //N is the number of valid data points
    int N = 0;

    for(int i = 0; i < numData; i++){

        if(data[i] != -1){

            isValid = true;
            N++;
        }
    }

    if(!isValid){

        m = 0;
        b = 0;

        std::cout << "failure" << std::endl;

        return;
    }

    else{

        double sumxy = 0;
        double sumx = 0;
        double sumy = 0;
        double sumxs = 0;

        for(int i = 0; i < numYears; i++){

            if(data[i] != -1){


                sumxy = sumxy + (years[i] * data[i]);
                sumx = sumx + years[i];
                sumy = sumy + data[i];
                sumxs = sumxs + (years[i] * years[i]);

            }
        }

        m = ((N * sumxy) - (sumx * sumy)) / ((N * sumxs) - (sumx * sumx));

        b = (sumy - (m * sumx)) / N;

        if(m == -0){

            m = 0;
        }

        std::cout << "slope is " << m << " intercept is " << b;

    }

}



std::string TimeSeries::get_seriesCode(){


    return seriesCode;
}


bool TimeSeries::isValid(){

    bool valid = false;

    for(int i = 0; i < numData; i++){

        if(data[i] != -1){

            valid = true;
        }
    }

    return valid;
}



