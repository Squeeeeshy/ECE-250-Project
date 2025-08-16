#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "LinkedList.h"
#include "SingleCountry.h"
#include "MultiCountry.h"
#include <fstream>
#include <unordered_map>  
#include <vector>         



int main() {
    // Your code here

    
    MultiCountry * mc = new MultiCountry();
    

    

    // mc->load("test.txt");

    // mc->initialize();


    // mc->load("f.csv");

    // mc->initialize();

   
    
    // mc->load("test2.txt");

    // mc->initialize();


    
  
    
    std::string command;

    while(std::cin >> command){

        if(command == "LOAD"){

            std::string fileName;
            std::cin >> fileName;

            mc->load(fileName);

        }

        else if(command == "LIST"){

            std::string countryName;
            std::getline(std::cin >> std::ws, countryName);


            mc->list(countryName);
        }

        else if(command == "RANGE"){

            std::string seriesCode;
            std::cin >> seriesCode;

            mc->range(seriesCode);
           
        }

        else if(command == "BUILD"){


            std::string seriesCode;
            std::cin >> seriesCode;

            mc->buildBinaryTree(seriesCode, true);

        }

        else if(command == "FIND"){

           double mean;
           std::string operation;

           std::cin >> mean >> operation;

           mc->find(mean, operation);

        }

        else if(command == "DELETE"){
        
            std::string countryName;
        
            std::getline(std::cin >> std::ws, countryName);

            mc->deleteCountry(countryName, false);
        }

        else if(command == "LIMITS"){

          std::string condition;

          std::cin >> condition;

          mc->limits(condition);
          
        }

        else if(command == "LOOKUP"){

          std::string countryCode;

          std::cin >> countryCode;

          mc->lookup(countryCode);

        }

        else if(command == "REMOVE"){

            std::string countryCode;
  
            std::cin >> countryCode;
  
            mc->remove(countryCode);
  
        }

        else if(command == "INSERT"){

          std::string countryCode;

          std::string fileName;

          std::cin >> countryCode >> fileName;

          mc->insert(countryCode, fileName);
  
        }

        else if(command == "INITIALIZE"){

          mc->initialize();

        }

        else if(command == "UPDATE_EDGES"){

          std::string seriesCode;

          double threshold;

          std::string relation;

          std::cin >> seriesCode >> threshold >> relation;

          mc->updateEdges(seriesCode, threshold, relation);

        }

        else if(command == "ADJACENT"){

          std::string countryCode;

          std::cin >> countryCode;

          mc->adjacent(countryCode);


        }

        else if(command == "PATH"){

          std::string countryCode1;
          std::string countryCode2;

          std::cin >> countryCode1 >> countryCode2;

          mc->path(countryCode1, countryCode2);

        }

        else if(command == "RELATIONSHIPS"){

          std::string countryCode1;
          std::string countryCode2;

          std::cin >> countryCode1 >> countryCode2;

          mc->relationships(countryCode1, countryCode2);

        }

        else if(command == "EXIT"){
          
          break;
        }

        else{

            std::cout << "invalid" << std::endl;
    
        }

    }



    delete mc;
    mc = nullptr;
    return 0;


}