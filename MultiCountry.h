#include <iostream>
#include <string>
#include "SingleCountry.h"
#include <fstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>   
#include <tuple>       
#include <utility>   
#include <set>

//help from chat gpt
struct RelationshipHash {

    std::size_t operator()(const std::tuple<std::string, double, std::string>& t) const;

};



class MultiCountry {
    
    public:

        class Node{

            public:

                //data and next
                std::string *data;
                std::string *nodeCodes;
                int nodeSize;
                double mini;
                double maxi;
                Node *left;
                Node *right;

                
                //constructor
                Node(double minimum, double maximum, Node * L, Node * R);

                //destructor
                ~Node();

        };


        


        //head
        MultiCountry::Node * root;

        //512 sized array
        SingleCountry** all_country = new SingleCountry *[512];

        std::string hashMap[512];

        double max;
        double min;
        int numCountries;
        std::string BTSeriesCode;
        bool hasTree = false;
        std::string previousTreeCode;


        //tuple and set stuff

        //so whenever I say RelTuple, i mean this
        using RelTuple = std::tuple<std::string, double, std::string>;

        
        //graphing stuff
        std::unordered_map<SingleCountry*, std::unordered_map <SingleCountry*, std::set <RelTuple> > > graph;


        std::vector<SingleCountry *> greaterCountries;
        std::vector<SingleCountry *> lessCountries;
       
        

        //sets the head to nullptr
        MultiCountry();

        //loads all the country's timeseries into the all_country array
        void load(std::string fileName);
        

        void list(std::string countryName);

        void test_print();


        void range(std::string seriesCode);

        void setRange(std::string seriesCode);

        //the function called to build the binary tree
        void buildBinaryTree(std::string seriesCode, bool print);

        //the function called recursively to build the subsequent subtrees
        Node* buildSubTree(double minimum, double maximum, std::string seriesCode, bool isLeft, std::string codes[], int size);

        void printNode(Node *ptr);

        void find(double mean, std::string op);
    
        void lessThan(Node* ptr, double mean);

        void greaterThan(Node *ptr, double mean);

    
        void deleteCountry(std::string countryName, bool calledFromRemove);    

        void deleteEmptyNodes(Node* ptr, Node* parent, std::string countryName, double countryMean);


        void limits(std::string condition);

        void destroyTree(Node* &ptr);
        
        int codeToW(std::string countryCode);

        int primaryHash(int W);

        int secondaryHash(int W);

        void hashFunction(std::string countryCode);
        
        void loadIndex(std::string fileName);

        void lookup(std::string countryCode);

        void remove(std::string countryCode);

        int getIndex(std::string countryCode);

        void insert(std::string countryCode, std::string fileName);

        void initialize();

        void updateEdges(std::string seriesCode, double threshold, std::string relation);
        

        void lessEdges(Node* ptr, std::string seriesCode, double threshold, std::string relation);

        void greaterEdges(Node *ptr, std::string seriesCode, double threshold, std::string relation);

        void adjacent(std::string countryCode);

        void path(std::string countryCode1, std::string countryCode2);

        void relationships(std::string countryCode1, std::string countryCode2);

        void printRelationship(RelTuple link);

        bool DFS(SingleCountry *current, SingleCountry *target, std::unordered_set<SingleCountry*>& visited);


    //destructor
    ~MultiCountry();


};

