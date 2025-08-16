#include <iostream>
#include <string>
#include "SingleCountry.h"
#include "LinkedList.h"
#include <fstream>
#include "MultiCountry.h"
#include <unordered_map>  
#include <vector>     




//idek
SingleCountry::SingleCountry(){

}

SingleCountry::SingleCountry(std::string fileName){

    countryList = new LinkedList<TimeSeries>;

    inputFile = fileName;


}

LinkedList<TimeSeries>* SingleCountry::getCountryList(){

    return countryList;
}


std::string SingleCountry::country_name(){

    return singleCountryName;
}

std::string SingleCountry::country_code() const{

    return singleCountryCode;
}



void SingleCountry::load(std::string country){

    singleCountryName = country;

    singleCountryCode;

    std::ifstream file(inputFile);

    std::string line;

    // theres 4 labels then 64 data entries, but the first and last entries are stuck together so 67
    int lineNum = 0;
    std::string countryRead;
    TimeSeries *ts;

   

    // 1. Go through the entire file and find the TimeSeries the match the inputted country

       while(std::getline(file, line)){   

        

        countryRead = "";

        // 2. Finds the country of each time series
        for(int i = 0; i < line.length(); i++){

            if(line[i] == ','){

                break;
            }   
            countryRead += line[i];
        }

        // 3. Create a new TimeSeries Node and add it to the list if it matches the input country

        if(countryRead == country){

            ts = new TimeSeries;
            ts->load_line(line);

            countryList->addBack(ts);
            
            singleCountryCode = ts->countryCode;
            


        }   
        
        // 4. Keeps Track of the Line
        lineNum++;
    }

    //std::cout << "success\n";

    

    ts = nullptr;

    file.close();
}



void SingleCountry::list(){



    LinkedList<TimeSeries>::Node* ptr = countryList->getHead();

    if(countryList->getHead() == nullptr){

        std::cout << "failure\n";
        return;
    }


    std::cout << ptr->data->country << " ";
    std::cout << ptr->data->countryCode;

    while(ptr != nullptr){

        std::cout << " " << ptr->data->series;

        ptr = ptr->next;

    }

    std::cout << "\n";

    ptr = nullptr;

}


void SingleCountry::add(std::string seriesCode, int year, double data){

    if(countryList->head == nullptr){

        std::cout << "failure\n";
        return;
    }
    

    bool validSeriesCode = false;

    LinkedList<TimeSeries>::Node* ptr = countryList->getHead();

    while(ptr != nullptr){

        if(ptr->data->get_seriesCode() == seriesCode){

            validSeriesCode = true;

            ptr->data->add(year, data);

            break;

        }

        ptr = ptr->next;
    }

    if(!validSeriesCode){

        std::cout << "failure\n";
    }

    ptr = nullptr;

}

void SingleCountry::update(std::string seriesCode, int year, double data){

    if(countryList->head == nullptr){

        std::cout << "failure\n";
        return;
    }

    bool validSeriesCode = false;

    LinkedList<TimeSeries>::Node * ptr = countryList->getHead();

    while(ptr != nullptr){

        if(ptr->data->get_seriesCode() == seriesCode){

            validSeriesCode = true;

            ptr->data->update(year, data);

            break;
        }

        ptr = ptr->next;

    }

    if(!validSeriesCode){

        std::cout << "failure\n";
    }

    ptr = nullptr;


}



void SingleCountry::print(std::string seriesCode){

    if(countryList->head == nullptr){

        std::cout << "failure\n";
        return;
    }

    bool validSeriesCode = false;

    LinkedList<TimeSeries>::Node* ptr = countryList->getHead();

    while(ptr != nullptr){

        if(ptr->data->get_seriesCode() == seriesCode){

            validSeriesCode = true;

            ptr->data->print();
            break;
        }

        ptr = ptr->next;
    }

    if(!validSeriesCode){

        std::cout << "failure\n";
    }


    ptr = nullptr;
}


void SingleCountry::remove(std::string seriesCode){

        bool validSeriesCode = false;
            
            
        //if list is empty

        if(countryList->head == nullptr){

            std::cout << "failure\n";

            return;
        }

        //if im deleting the head of the list

        if(countryList->head->data->get_seriesCode() == seriesCode){

            LinkedList<TimeSeries>::Node * temp = countryList->head;

            countryList->head = countryList->head->next;

            delete temp;

            temp = nullptr;

            std::cout << "success\n";

            return;
        }

        //deleting anything else

        LinkedList<TimeSeries>::Node * ptr = countryList->getHead();

      

        while(ptr != nullptr && ptr->next != nullptr){

            if(ptr->next->data->get_seriesCode() == seriesCode){      

                validSeriesCode = true;      

                LinkedList<TimeSeries>::Node * temp = ptr->next;

                ptr->next = temp->next;

                delete temp;

                temp = nullptr;

                std::cout << "success\n";

                break;

            }

            ptr = ptr->next;


        }

        if(!validSeriesCode){

            std::cout << "failure\n";
        }

        ptr = nullptr;


}


void SingleCountry::biggest(){

    if(countryList->head == nullptr){

        std::cout << "failure\n";
        return;
    }

    bool isValid = false;

    double biggestMean = 0;

    LinkedList<TimeSeries>::Node * ptr = countryList->head;
    LinkedList<TimeSeries>::Node * big = countryList->head;

    while(ptr != nullptr){

        if(ptr->data->mean() > biggestMean){

            biggestMean = ptr->data->mean();
            big = ptr;
        }

        ptr = ptr->next;
    }

    if(biggestMean == 0){

        std::cout << "failure\n";
        return;
    }

    std::cout << big->data->get_seriesCode();

    ptr = nullptr;
    big = nullptr;

}

//for the map
bool SingleCountry::operator== (const SingleCountry& other)const{

    return this->singleCountryName == other.singleCountryName; // or however you compare countries
}



void SingleCountry::print_name(){

    std::cout << countryList->head->data->country << std::endl;

}



SingleCountry::~SingleCountry(){

    delete countryList;
    countryList = nullptr;
     
}


