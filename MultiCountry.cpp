#include <iostream>
#include <string>
#include "SingleCountry.h"
#include <fstream>
#include "MultiCountry.h"
#include <cmath>
#include <unordered_map>
#include <vector>
#include <unordered_set>   
#include <tuple>       
#include <utility>
#include <set>



//constructor
MultiCountry::Node::Node(double minimum, double maximum, Node * L, Node * R) : mini(minimum), maxi(maximum), left(L), right(R){

    data = new std::string[512];
    nodeCodes = new std::string[512];
}

//destructor
MultiCountry::Node::~Node(){

    delete[] data;
    data = nullptr;
    delete[] nodeCodes;
    nodeCodes = nullptr;
    delete left;
    delete right;
    left = nullptr;
    right = nullptr;

}



//sets the head to nullptr
MultiCountry::MultiCountry() : root(nullptr){


    for(int i = 0; i < 512; i++){

        all_country[i] = nullptr;
    }

}

//loads all the country's timeseries into the all_country array
void MultiCountry::load(std::string fileName){

    loadIndex(fileName);

    std::ifstream file(fileName);

    std::string line;

    //have two countryRead names to determine whether or not a new SingleCountry needs to be created
    std::string countryName1 = "";
    std::string countryName2 = "";

    std::string countryCodeRead = "";

    bool secondEntry = false;

    

    // 1. Go through the entire file

    while(std::getline(file, line)){   

        secondEntry = false;

        countryCodeRead = "";

        countryName1 = "";

        // 2. Finds the country of each time series
        for(int i = 0; i < line.length(); i++){

            if(secondEntry){

                if(line[i] == ','){

                    break;
                }

                countryCodeRead += line[i];
            }

            else{

                if(line[i] == ','){

                    secondEntry = true;
                    continue;
                }

                countryName1 += line[i];
            }
        }

       

        //if the next country is the same country, go to the next line
        if(countryName1 == countryName2){

            continue;
        }

       

        int index = getIndex(countryCodeRead);

        //if the country got removed first
        if(index == -1){

            continue;
        }

        all_country[index] = new SingleCountry(fileName);
        all_country[index]->load(countryName1);

        
        countryName2 = countryName1;
    
    }

    file.close();

    std::cout << "success" << std::endl;

}






void MultiCountry::list(std::string countryName){

    bool isValid = false;


    for(int i = 0; i < 512; i++){

        if(all_country[i] == nullptr){

            continue;
        }

        if(all_country[i]->country_name() == countryName){
        
            all_country[i]->list();
            isValid = true;
            break;
        }
    }

    if(!isValid){

        std::cout << "failure" << std::endl;
    }

    
}

void MultiCountry::test_print(){

    for(int i = 0; i < 512; i++){

        if(all_country[i] != nullptr){

            std::cout << all_country[i]->country_name() << std::endl;
        }

        if(all_country[i] == nullptr){

            std::cout << "EMPTY" << std::endl;
        }
    }

}

void MultiCountry::range(std::string seriesCode){

    double minimum;
    double maximum;

    bool firstOne = true;

    for(int i = 0; i < 512; i++){

        if(all_country[i] == nullptr){

            continue;
        }

        LinkedList<TimeSeries>::Node *ptr = all_country[i]->getCountryList()->getHead();

        while(ptr != nullptr){

            if(ptr->data->get_seriesCode() == seriesCode){

                break;
            }

            ptr = ptr->next;
        }

        if(ptr == nullptr){

            std::cout << "failure" << std::endl;
            return;
        }


        if(firstOne){

            minimum = ptr->data->mean();
            maximum = ptr->data->mean();

            firstOne = false;
        }

        else{

            if(ptr->data->mean() < minimum && ptr->data->isValid()){

                minimum = ptr->data->mean();
            }

            if(ptr->data->mean() > maximum && ptr->data->isValid()){

                maximum = ptr->data->mean();
            }
        }

        // std::cout << "The current mean is: " << ptr->data->mean() << " from: " << ptr->data->country << std::endl;
        // std::cout << "The updated min is: " << min << std::endl;
        // std::cout << "The update max is: " << max << std::endl;


    }

        

    std::cout << minimum << " " << maximum << std::endl;


}

void MultiCountry::setRange(std::string seriesCode){

    bool firstOne = true;

    for(int i = 0; i < 512; i++){

        if(all_country[i] == nullptr){

            continue;
        }

        LinkedList<TimeSeries>::Node *ptr = all_country[i]->getCountryList()->getHead();

        while(ptr != nullptr){

            if(ptr->data->get_seriesCode() == seriesCode){

                break;
            }

            ptr = ptr->next;
        }

        double meanValue = (ptr == nullptr) ? 0 : ptr->data->mean();

        if (firstOne) {

            min = meanValue;
            max = meanValue;
            firstOne = false;


        } else {


            min = std::min(min, meanValue);
            max = std::max(max, meanValue);

        }
        


    }

}

//the function called to build the binary tree
void MultiCountry::buildBinaryTree(std::string seriesCode, bool print){

    
    //destroy the existing tree if its already there
    if(hasTree){

        previousTreeCode = BTSeriesCode;

        destroyTree(root);
    }

    else{

        previousTreeCode = seriesCode;
    }

    int left_size = 0;
    int right_size = 0;
    std::string left_codes[512];
    std::string right_codes[512];

    hasTree = true;

    setRange(seriesCode);

    BTSeriesCode = seriesCode;

    

    root = new Node(min, max, nullptr, nullptr);

    root->nodeSize= 512;

    double midpoint = (max + min) / 2;

    for(int i = 0; i < 512; i++){

        if(all_country[i] == nullptr || all_country[i]->getCountryList() == nullptr || all_country[i]->getCountryList()->getHead() == nullptr){

            continue;
        }

        LinkedList<TimeSeries>::Node *ptr = all_country[i]->getCountryList()->getHead();

        while(ptr != nullptr){

            if(ptr->data->get_seriesCode() == seriesCode){

                break;
            }

            ptr = ptr->next;
        }

        if(ptr == nullptr){

            continue;
        }

        
        root->data[i] = ptr->data->country;
        root->nodeCodes[i] = ptr->data->countryCode;

        if(ptr->data->mean() < midpoint && ptr->data->mean() >= min){

            left_codes[left_size] = ptr->data->countryCode;
            left_size++;
        }

        else if(ptr->data->mean() >= midpoint && ptr->data->mean() <= max){

            right_codes[right_size] = ptr->data->countryCode;
            right_size++;
        }
        
    }

    

    root->left = buildSubTree(min, midpoint, seriesCode, true, left_codes, left_size);
    root->right = buildSubTree(midpoint, max, seriesCode, false, right_codes, right_size);

    if(print){

        std::cout << "success" << std::endl;
    }   

    

}

//the function called recursively to build the subsequent subtrees
MultiCountry::Node* MultiCountry::buildSubTree(double minimum, double maximum, std::string seriesCode, bool isLeft, std::string codes[], int size){

    Node *newNode = new Node(minimum, maximum, nullptr, nullptr);

    double newMin;
    double newMax;

    bool firstOne = true;

    
    newNode->nodeSize = size;

    //counts how many countries were added so I can put it in the correct index as well as to check if there's only one country left
    int j = 0;

    for(int i = 0; i < size; i++){

        //need to get hash index

        int index = getIndex(codes[i]);


        if(index == -1){

            continue;
        }

    
        if(all_country[index] == nullptr || all_country[index]->getCountryList() == nullptr || all_country[index]->getCountryList()->getHead() == nullptr){

            continue;
        }

        LinkedList<TimeSeries>::Node *ptr = all_country[index]->getCountryList()->getHead();

        while(ptr != nullptr){

            if(ptr->data->get_seriesCode() == seriesCode){

                break;
            }

            ptr = ptr->next;
        }

        if(ptr == nullptr){

            continue;
        }


        if(isLeft){

            //checks to see if its in the range
            if((ptr->data->mean() >= minimum) && (ptr->data->mean() < maximum)){

                newNode->data[j] = ptr->data->country;
                newNode->nodeCodes[j] = codes[j];
                j++;

                if(firstOne){

                    newMax = ptr->data->mean();
                    newMin = ptr->data->mean();
                    firstOne = false;
                }

                else{

                    if(ptr->data->mean() > newMax){

                        newMax = ptr->data->mean();
                    }

                    if(ptr->data->mean() < newMin){

                        newMin = ptr->data->mean();
                    }
                }

                
            }
        }

        else if(!isLeft){

            //checks to see if its in the range
            if((ptr->data->mean() >= minimum) && (ptr->data->mean() <= maximum)){

                newNode->data[j] = ptr->data->country;
                newNode->nodeCodes[j] = codes[j];

                j++;

                if(firstOne){

                    newMax = ptr->data->mean();
                    newMin = ptr->data->mean();
                    firstOne = false;
                }

                else{

                    if(ptr->data->mean() > newMax){

                        newMax = ptr->data->mean();
                    }

                    if(ptr->data->mean() < newMin){

                        newMin = ptr->data->mean();
                    }
                }

            }
        }
        

    }

    newNode->mini = newMin;
    newNode->maxi = newMax;

    //if there's only one country in the node
    if(j == 1){

        return newNode;
    }

    //if it happens to be empty just delete it
    if(j == 0){

        delete newNode;
        return nullptr;
    }
    

    if((newMax - newMin) < 1E-3 || newMax == newMin){

        return newNode;
    }

    double midpoint = (newMin + newMax) / 2;

    int left_size = 0;
    int right_size = 0;
    std::string left_codes[size];
    std::string right_codes[size];


    for(int i = 0; i < size; i++){

        //need to get hash index

        int index = getIndex(codes[i]);

        if(index == -1){

            continue;
        }

    
        if(all_country[index] == nullptr || all_country[index]->getCountryList() == nullptr || all_country[index]->getCountryList()->getHead() == nullptr){

            continue;
        }

        LinkedList<TimeSeries>::Node *ptr = all_country[index]->getCountryList()->getHead();

        while(ptr != nullptr){

            if(ptr->data->get_seriesCode() == seriesCode){

                break;
            }

            ptr = ptr->next;
        }

        if(ptr == nullptr){

            continue;
        }

        if(ptr->data->mean() < midpoint){

            left_codes[left_size] = codes[i];
            left_size++;
        }

        else if(ptr->data->mean() >= midpoint){

            right_codes[right_size] = codes[i];
            right_size++;
        }
        

    }

    newNode->left = buildSubTree(newMin, midpoint, seriesCode, true, left_codes, left_size);
    newNode->right = buildSubTree(midpoint, newMax, seriesCode, false, right_codes, right_size);

    return newNode;
}



void MultiCountry::printNode(Node *ptr){

    for(int i = 0; i < 512; i++){

        if(!ptr->data[i].empty()){

            std::cout << ptr->data[i] << " ";

        }
    }

}



void MultiCountry::find(double mean, std::string op){


    //theres no tree
    if(root == nullptr){

        std::cout << "failure" << std::endl;
        return;
    }

    //1. Equals
    if(op == "equal"){

        //arrPrint(mean, op);
        //return;

        //make a pointer to traverse the list
        MultiCountry::Node *ptr = root;

        double midpoint;

        double tolerance = 1E-3;

        while(ptr != nullptr){

            midpoint = (ptr->maxi + ptr->mini) / 2;

            if(((ptr->maxi - ptr->mini) < tolerance) && std::abs(mean - ptr->mini) < tolerance && std::abs(mean - ptr->maxi) < tolerance){
                
                printNode(ptr);
                break;
            }

            else if(mean < midpoint){

                ptr = ptr->left;
            }

            else if(mean >= midpoint){

                ptr = ptr->right;
            }

        }

        if(ptr == nullptr){

            std::cout << std::endl;
            return;
        }

        std::cout << std::endl;

    }

    //2. Less Than
    if(op == "less"){

        

        if(mean <= root->mini){

            std::cout << std::endl;
            return;
        }

        lessThan(root, mean);

        std::cout << std::endl;

    }

    //3. Greater Than
    if(op == "greater"){

        

        if(mean >= root->maxi){

            std::cout << std::endl;
            return;
        }

        greaterThan(root, mean);

        std::cout << std::endl;

    }

}




void MultiCountry::lessThan(Node* ptr, double mean){

    if(ptr == nullptr){

        return;
    }

    //base case: if the node's max < mean, print then return
    if(mean > ptr->maxi){

        printNode(ptr);
        return;
    }

    //if its less than the midpoint then go left
    double midpoint;
    midpoint = (ptr->maxi + ptr->mini) / 2;

    if(mean < midpoint){

        lessThan(ptr->left, mean);
    }

    //if its greater than the midpoint then split
    if(mean >= midpoint){

        lessThan(ptr->left, mean);
        lessThan(ptr->right, mean);
    }


    return;
}

void MultiCountry::greaterThan(Node *ptr, double mean){

    if(ptr == nullptr){

        return;
    }

    //base case: if the node's min > mean, print then return
    if(mean < ptr->mini){

        printNode(ptr);
        return;
    }

    //if its less than the midpoint then go left
    double midpoint;
    midpoint = (ptr->maxi + ptr->mini) / 2;


    //if its greater than the midpoint then go right
    if(mean >= midpoint){

        greaterThan(ptr->right, mean);
    }

    //if its less than that means i need to split
    if(mean < midpoint){

        greaterThan(ptr->left, mean);
        greaterThan(ptr->right, mean);
    }


    return;
}



void MultiCountry::deleteCountry(std::string countryName, bool calledFromRemove){

    //just checks to see if the tree and country exist
    if(root == nullptr){

        std::cout << "failure" << std::endl;
        return;
    }

    bool isValid = false;

    for(int i = 0; i < 512; i++){

        if(root->data[i] == countryName){

            isValid = true;
            break;
        }

    }

    if(!isValid && !calledFromRemove){

        std::cout << "failure" << std::endl;
        return;
    }

    if(!isValid && calledFromRemove){

        return;
    }

    MultiCountry::Node *parent = nullptr;
    MultiCountry::Node *ptr = root;

    double countryMean;

    for(int i = 0; i < 512; i++){

        if(all_country[i] == nullptr){

            continue;
        }

        if(all_country[i]->country_name() == countryName){

            LinkedList<TimeSeries>::Node *curr = all_country[i]->getCountryList()->getHead();

            while(curr != nullptr){

                if(curr->data->get_seriesCode() == BTSeriesCode){

                    countryMean = curr->data->mean();
                    break;
                }

                curr = curr->next;
            }

            //that means the time series does not exist for the country and thus is not in the tree
            if(curr == nullptr){

                std::cout << "failure" << std::endl;
                return;
            }

            break;
        }
    }

    deleteEmptyNodes(ptr, parent, countryName, countryMean);

    if(calledFromRemove){

        return;
    }
    std::cout << "success" << std::endl;
}






void MultiCountry::deleteEmptyNodes(Node* ptr, Node* parent, std::string countryName, double countryMean){

    if(ptr == nullptr){

        return;
    }

    
    //scans the current node for the target country, removes it
    for(int i = 0; i < 512; i++){

        if(ptr->data[i] == countryName){

            ptr->data[i] = "";
            break;
        }
    }

    double midpoint;

    midpoint = (ptr->maxi + ptr->mini) / 2;

    //checks if the node is empty
    bool isEmpty = true;

    for(int i = 0; i < 512; i++){

        if(ptr->data[i] != ""){

            isEmpty = false;
            break;
        }
    }


    //go do the same thing to the nodes to left/right
    if(countryMean < midpoint && ptr->left != nullptr){

        deleteEmptyNodes(ptr->left, ptr, countryName, countryMean);
    }

    else if(countryMean >= midpoint && ptr->right != nullptr){

        deleteEmptyNodes(ptr->right, ptr, countryName, countryMean);
    }

    //if the current node is empty with no children, then delete if after traversing the tree

    if(isEmpty && ptr->left == nullptr && ptr->right == nullptr){

        if(parent != nullptr){

            if(parent->left == ptr){

                parent->left = nullptr;
            }

            else if(parent->right == ptr){

                parent->right = nullptr;
            }

        }

        else{

            root = nullptr;
        }

        delete ptr;
    }

    
    return;
}




void MultiCountry::limits(std::string condition){

    if(root == nullptr){

        std::cout << "failure" << std::endl;
        return;
    }

    MultiCountry::Node *ptr = root;

    //1. Lowest

    if(condition == "lowest"){

        while(ptr != nullptr){

            if(ptr->left == nullptr && ptr->right == nullptr){

                printNode(ptr);
                break;
            }

            else if(ptr->left == nullptr && ptr->right != nullptr){

                ptr = ptr->right;
                continue;
            }

            ptr = ptr->left;
        }
    }

    //2. Highest

    if(condition == "highest"){

        while(ptr != nullptr){

            if(ptr->right == nullptr && ptr->left == nullptr){

                printNode(ptr);
                break;
            }

            else if(ptr->right == nullptr && ptr->left != nullptr){

                ptr = ptr->left;
                continue;
            }

            ptr = ptr->right;
        }
    }

    std::cout << std::endl;

}

void MultiCountry::destroyTree(Node* &ptr){

    //hit the end
    if(ptr == nullptr){

        return;
    }

    //post order traversal
    destroyTree(ptr->left);
    destroyTree(ptr->right);

    //passed by refernce to a pointer so that when i set ptr to nullptr the root is gonna be nullptr cause thats prolly the only thing going in there
    delete ptr;
    ptr = nullptr;

}

int MultiCountry::codeToW(std::string countryCode){

    int W = (countryCode[0] - 'A') * 676 + (countryCode[1] - 'A') * 26 + (countryCode[2] - 'A');

    return W;

}


int MultiCountry::primaryHash(int W){

    int hashCode = W % 512;

    return hashCode;

}

int MultiCountry::secondaryHash(int W){


    int hashCode;

    int x = static_cast<int>(floor(W / 512.0)) % 512;

    if(x % 2 == 0){

        hashCode = x + 1;
    }

    else{

        hashCode = x;
    }

    return hashCode;

}

void MultiCountry::hashFunction(std::string countryCode){

    int w = codeToW(countryCode);

    int i = 0;

    int m = 512;

    int index = (primaryHash(w)) % m;

    if(hashMap[index] == countryCode){

        return;
    }

    //if its occupied then keep probing
    while((!hashMap[index].empty()) && i < m){

        i++;
        index = (primaryHash(w) + i * secondaryHash(w)) % m;

        if(hashMap[index] == countryCode){

            return;
        }
    }

    if(i < m){
        
        hashMap[index] = countryCode;
    }

    else {

        std::cout << "Hash Table Full. Hopefully this doesn't show up." << std::endl;
    }
    
}

void MultiCountry::loadIndex(std::string fileName){

    std::ifstream file(fileName);

    std::string line;

    //have two countryRead names to determine whether or not a new SingleCountry needs to be created
    std::string countryCode1 = "";
    std::string countryCode2 = "";


    bool secondEntry;

    // 1. Go through the entire file

    while(std::getline(file, line)){   

        secondEntry = false;

        countryCode1 = "";

        // 2. Finds the country of each time series
        for(int i = 0; i < line.length(); i++){

            if(secondEntry){

                if(line[i] == ','){

                    break;
                }

                countryCode1 += line[i];

            }

            if(line[i] == ','){

                secondEntry = true;
            }   
            
        }

        //if the next country is the same country, go to the next line
        if(countryCode1 == countryCode2){

            continue;
        }

        else{

            hashFunction(countryCode1);

        }

        countryCode2 = countryCode1;

    }

    file.close();


}

void MultiCountry::lookup(std::string countryCode){

    int w = codeToW(countryCode);

    int i = 0;

    int steps = 1;

    int m = 512;

    int index = (primaryHash(w)) % m;

    

    if(hashMap[index] == countryCode){

        std::cout << "index " << index << " searches " << steps << std::endl;
        return;
    }

    else if(hashMap[index].empty()){

        std::cout << "failure" << std::endl;
        return;
    }

    while(i < m){

        i++;
        index = (primaryHash(w) + i * secondaryHash(w)) % m;
        steps++;

        //empty spot, I'll mark previously occupied spots w "-1"
        if(hashMap[index].empty()){

            break;
        }

        if(hashMap[index] == countryCode){

            std::cout << "index " << index << " searches " << steps <<  std::endl;

            return;
        }

    }


    std::cout << "failure" << std::endl;
       
    
}



void MultiCountry::remove(std::string countryCode){

    int w = codeToW(countryCode);

    int i = 0;

    int m = 512;

    int index = (primaryHash(w)) % m;

    if(hashMap[index] == countryCode){

        hashMap[index] = "-1";

        

        if(root != nullptr){

            deleteCountry(all_country[index]->country_name(), true);
        }

        delete all_country[index];

        all_country[index] = nullptr;


        std::cout << "success" << std::endl;
        return;
    }

    else if(hashMap[index].empty()){

        std::cout << "failure" << std::endl;
        return;
    }

    while(i < m){

        i++;

        index = (primaryHash(w) + i * secondaryHash(w)) % m;

        if(hashMap[index].empty()){

            std::cout << "failure" << std::endl;
            return;
        }

        if(hashMap[index] == countryCode){

            hashMap[index] = "-1";

            if(root != nullptr){

                deleteCountry(all_country[index]->country_name(), true);
            }

            delete all_country[index];

            all_country[index] = nullptr;


            std::cout << "success" << std::endl;
            return;
        }

    }

    std::cout << "failure" << std::endl;

}


int MultiCountry::getIndex(std::string countryCode){

    int w = codeToW(countryCode);

    int i = 0;

    int m = 512;

    int index = (primaryHash(w)) % m;

    if(hashMap[index] == countryCode){

        return index;
    }

    else if(hashMap[index].empty()){

        return -1;
    }

    while(i < m){

        i++;

        index = (primaryHash(w) + i * secondaryHash(w)) % m;

        if(hashMap[index].empty()){

            return -1;
        }

        if(hashMap[index] == countryCode){

            return index;
        }

    }

    // std::cout << "you mightve done goofed lmao" << std::endl; 
    return -1;

}


void MultiCountry::insert(std::string countryCode, std::string fileName){

    int w = codeToW(countryCode);

    int i = 0;

    int m = 512;

    int index = (primaryHash(w)) % m;

    bool found = false;

    if(hashMap[index].empty() || hashMap[index] == "-1"){

        hashMap[index] = countryCode;

        found = true;
    }

    else if(hashMap[index] == countryCode){

        std::cout << "failure" << std::endl;
        return;
    }

    while(i < m && !found){

        i++;

        index = (primaryHash(w) + i * secondaryHash(w)) % m;

        if(hashMap[index].empty() || hashMap[index] == "-1"){

            hashMap[index] = countryCode;
    
            found = true;

            break;
        }
    
        else if(hashMap[index] == countryCode){
    
            std::cout << "failure" << std::endl;
            return;
        }

    }

 
    

        std::ifstream file(fileName);

        std::string line;

        //have two countryRead names to determine whether or not a new SingleCountry needs to be created
        std::string countryName = "";
        std::string countryCodeRead = "";

        bool secondEntry = false;


        // 1. Go through the entire file

        while(std::getline(file, line)){   

            secondEntry = false;

            countryName = "";

            countryCodeRead = "";

            // 2. Finds the country of each time series
            for(int i = 0; i < line.length(); i++){

                if(secondEntry){

                    if(line[i] == ','){

                        break;
                    }

                    countryCodeRead += line[i];
                }

                else{

                    if(line[i] == ','){

                        secondEntry = true;
                        continue;
                    }

                    countryName += line[i];
                }
                
            }

            //if the next country is the same country, go to the next line
            if(countryCodeRead == countryCode){          

                all_country[index] = new SingleCountry(fileName);
                all_country[index]->load(countryName);

                

                break;
            }

            else{

                continue;
            }

        }

        file.close();

    
    std::cout << "success" << std::endl;

}




//makes the nodes for the graph
void MultiCountry::initialize(){

    graph.clear();

    for(int i = 0; i < 512; i++){

        if(all_country[i] != nullptr){

            graph[all_country[i]] = std::unordered_map<SingleCountry*, std::set<RelTuple>>(); 

        }

    }

    std::cout << "success" << std::endl;

}


void MultiCountry::updateEdges(std::string seriesCode, double threshold, std::string relation){


    //1. equals
    if(relation == "equal"){

        //this will overwrite any existing binary tree 
        buildBinaryTree(seriesCode, false);

        bool added = false;

        //copied the code from equals from before

        //make a pointer to traverse the list
        MultiCountry::Node *ptr = root;

        double midpoint;

        double tolerance = 1E-3;

        while(ptr != nullptr){

            midpoint = (ptr->maxi + ptr->mini) / 2;

            //found the node which has all the countries

            if(((ptr->maxi - ptr->mini) < tolerance) && std::abs(threshold - ptr->mini) < tolerance && std::abs(threshold - ptr->maxi) < tolerance){

                
                //need to update the edges here

                //first, create and store the relationships from each country to each other
                RelTuple relationship = std::make_tuple(seriesCode, threshold, relation);

                int index1;
                int index2;

                //then go through all the countries and update the relationships

                for(int i = 0; i < ptr->nodeSize; i++){

                    if(ptr->nodeCodes[i].empty()){

                        continue;
                    }

                    index1 = getIndex(ptr->nodeCodes[i]);

                    for(int ii = i + 1; ii < ptr->nodeSize; ii++){

                        if(ptr->nodeCodes[ii].empty()){

                            continue;
                        }

                        index2 = getIndex(ptr->nodeCodes[ii]);

                        auto check1 = graph[all_country[index1]][all_country[index2]].insert(relationship);
                        auto check2 = graph[all_country[index2]][all_country[index1]].insert(relationship);

                        if(check1.second || check2.second){

                            added = true;
                        }

                    }
                }

                break;
            }

            else if(threshold < midpoint){

                ptr = ptr->left;
            }

            else if(threshold >= midpoint){

                ptr = ptr->right;
            }

        }

        if(ptr == nullptr || !added){

            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }

        std::cout << "success" << std::endl;

        //buildBinaryTree(previousTreeCode, false);

        return;

    }

    //2. Less Than
    if(relation == "less"){

        //this will overwrite any existing binary tree 
        buildBinaryTree(seriesCode, false);

        
        if(threshold <= root->mini){

            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }

        lessCountries.clear();

        lessEdges(root, seriesCode, threshold, relation);

        bool added = false;

        if(lessCountries.empty()){

            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }     


        //need to update the edges here

        //first, create and store the relationships from each country to each other
        RelTuple relationship = std::make_tuple(seriesCode, threshold, relation);

        //then go through all the countries and update the relationships

        for(int i = 0; i < lessCountries.size(); i++){

            for(int ii = i + 1; ii < lessCountries.size(); ii++){

                auto check1 = graph[lessCountries[i]][lessCountries[ii]].insert(relationship);
                auto check2 = graph[lessCountries[ii]][lessCountries[i]].insert(relationship);

                if(check1.second || check2.second){

                    added = true;
                }

            }
        }

        if(!added){

            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }

        std::cout << "success" << std::endl;

        //buildBinaryTree(previousTreeCode, false);

        return;

    }

    //3. Greater Than
    if(relation == "greater"){

        //this will overwrite any existing binary tree 
        buildBinaryTree(seriesCode, false);

        
        if(threshold >= root->maxi){


            std::cout << "failure" << std::endl;
            
            //buildBinaryTree(previousTreeCode, false);

            return;
        }

        greaterCountries.clear();

        greaterEdges(root, seriesCode, threshold, relation);

        bool added = false;

        if(greaterCountries.empty()){

            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }     


        //need to update the edges here

        //first, create and store the relationships from each country to each other
        RelTuple relationship = std::make_tuple(seriesCode, threshold, relation);

        int index1;
        int index2;

        //then go through all the countries and update the relationships

        for(int i = 0; i < greaterCountries.size(); i++){

            

            for(int ii = i + 1; ii < greaterCountries.size(); ii++){

                

                auto check1 = graph[greaterCountries[i]][greaterCountries[ii]].insert(relationship);
                auto check2 = graph[greaterCountries[ii]][greaterCountries[i]].insert(relationship);

                if(check1.second || check2.second){

                    added = true;
                }

            }
        }

        if(!added){


            std::cout << "failure" << std::endl;

            //buildBinaryTree(previousTreeCode, false);

            return;
        }

        std::cout << "success" << std::endl;

        //buildBinaryTree(previousTreeCode, false);

        return;

    }

}


void MultiCountry::lessEdges(Node* ptr, std::string seriesCode, double threshold, std::string relation){

    if(ptr == nullptr){

        return;
    }

    //base case: if the node's max < mean, print then return
    if(threshold > ptr->maxi){
        
        //add all countries to the set

        for(int i = 0; i < ptr->nodeSize; i++){

            int index = getIndex(ptr->nodeCodes[i]);

            if(index == -1){

                //std::cout << "error in less edges" << std::endl;
                continue;
            }

            SingleCountry *country = all_country[index];

            if(country == nullptr){

                //std::cout << "error in less edges" << std::endl;
                continue;
            }

            lessCountries.push_back(country);
        }

        return;

    }

    //if its less than the midpoint then go left
    double midpoint;
    midpoint = (ptr->maxi + ptr->mini) / 2;

    if(threshold < midpoint){

        lessEdges(ptr->left, seriesCode, threshold, relation);
    }

    //if its greater than the midpoint then split
    if(threshold >= midpoint){

        lessEdges(ptr->left, seriesCode, threshold, relation);
        lessEdges(ptr->right, seriesCode, threshold, relation);
    }

    return;
}

void MultiCountry::greaterEdges(Node *ptr, std::string seriesCode, double threshold, std::string relation){

    if(ptr == nullptr){

        return;
    }

    //base case: if the node's min > mean, print then return
    if(threshold < ptr->mini){

        //add all countries to the set

        for(int i = 0; i < ptr->nodeSize; i++){

            int index = getIndex(ptr->nodeCodes[i]);

            if(index == -1){

                //std::cout << "error in greater edges" << std::endl;
                continue;
            }

            SingleCountry *country = all_country[index];

            if(country == nullptr){

                //std::cout << "error in greater edges" << std::endl;
                continue;
            }

            greaterCountries.push_back(country);
        }

        return;
    }

    //if its less than the midpoint then go left
    double midpoint;
    midpoint = (ptr->maxi + ptr->mini) / 2;


    //if its greater than the midpoint then go right
    if(threshold >= midpoint){

        greaterEdges(ptr->right, seriesCode, threshold, relation);
    }


    //if its less than that means i need to split
    if(threshold < midpoint){

        greaterEdges(ptr->left, seriesCode, threshold, relation);
        greaterEdges(ptr->right, seriesCode, threshold, relation);
    }


    return;

}



void MultiCountry::adjacent(std::string country_code){

    int index = getIndex(country_code);

    //if its not in the hashtable

    if(index == -1){

        std::cout << "failure" << std::endl;

        return;
    }

    SingleCountry* ptr = all_country[index];

    //if country not in the graph

    if(graph.find(ptr) == graph.end()){

        std::cout << "failure" << std::endl;

        return;
    }


    const auto& adjacentCountries = graph[ptr];

    //if there are no adjacent countries

    if(adjacentCountries.empty()){

        std::cout << "none" << std::endl;

        return;
    }

    bool firstOne = true;

    for(const auto& entry : adjacentCountries){

        std::string adjacent_country = entry.first->country_name();

        if(!firstOne){

            std::cout << " ";
        }

        std::cout << adjacent_country;

        firstOne = false;

    }

    std::cout << std::endl;

    return;

}




void MultiCountry::path(std::string countryCode1, std::string countryCode2){


    int index1 = getIndex(countryCode1);
    int index2 = getIndex(countryCode2);

    if(index1 == - 1 || index2 == -1){

        std::cout << "false" << std::endl;

        return;

    }


    SingleCountry *ptr1 = all_country[index1];
    SingleCountry *ptr2 = all_country[index2];

    if(ptr1 == nullptr || ptr2 == nullptr){

        std::cout << "false" << std::endl;

        return;
    }


    std::unordered_set<SingleCountry*> visited;

    visited.clear();

    bool hasPath;

    hasPath = DFS(ptr1, ptr2, visited);


   if(hasPath){

        std::cout << "true" << std::endl;
   }
 
   else if(!hasPath){

        std::cout << "false" << std::endl;  
   }
        
   return;
}

bool MultiCountry::DFS(SingleCountry *current, SingleCountry *target, std::unordered_set<SingleCountry*>& visited){

    //base case: if the input and the target are the same
    if(current == target){

        return true;
    }

    //if not, then add the current to the set of visited nodes
    visited.insert(current);

    //search every single neighbor of the current node
    for(const auto& adjacent : graph[current]){

        //if the neighbor is not on the visited list
        if(visited.find(adjacent.first) == visited.end()){

            //perform DFS on the neighbor, and if they find the target country along their path, return true and it'll bubble back up
            if(DFS(adjacent.first, target, visited)){

                return true;
            }
        }

    }

    return false;

}



void MultiCountry::relationships(std::string countryCode1, std::string countryCode2){


    int index1 = getIndex(countryCode1);
    int index2 = getIndex(countryCode2);

    if(index1 == - 1 || index2 == -1){

        std::cout << "none" << std::endl;

        return;

    }


    SingleCountry *ptr1 = all_country[index1];
    SingleCountry *ptr2 = all_country[index2];

    if(ptr1 == nullptr || ptr2 == nullptr){

        std::cout << "none" << std::endl;

        return;
    }

    //check if the intial country exists

    auto countryExists = graph.find(ptr1);

    if(countryExists == graph.end()){

        std::cout << "none" << std::endl;

        return;

    }


    //if no edges
    
    auto adjacentCountries = graph[ptr1];

    if(adjacentCountries.find(ptr2) == adjacentCountries.end()){

        std::cout << "none" << std::endl;

        return;
    }

    //if has edge but no relationships  

    if(adjacentCountries[ptr2].empty()){

        std::cout << "none" << std::endl;

        return;

    }


    bool firstOne = true;

    const auto& allRelationships = graph[ptr1][ptr2];
    
    for(const auto& entry : allRelationships){

        if(!firstOne){

            std::cout << " ";
        }

        printRelationship(entry);
        firstOne = false;
    }

    std::cout << std::endl;

    return;

}


void MultiCountry::printRelationship(RelTuple link){


    std::cout << "(" << std::get<0>(link) << " " << std::get<1>(link) << " " << std::get<2>(link) << ")";

}


//destructor
MultiCountry::~MultiCountry() {

    for(int i = 0; i < 512; i++){

        delete all_country[i];
    }

    delete[] all_country; 
    destroyTree(root);


}

//help from chat gpt
// std::size_t RelationshipHash::operator()(const std::tuple<std::string, double, std::string>& t) const {

//     auto h1 = std::hash<std::string>{}(std::get<0>(t));

//     auto h2 = std::hash<double>{}(std::get<1>(t));

//     auto h3 = std::hash<std::string>{}(std::get<2>(t));

//     return h1 ^ (h2 << 1) ^ (h3 << 2);

// }