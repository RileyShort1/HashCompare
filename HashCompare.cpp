#include "Hash_Table_LP.h"
#include "Hash_Table_QP.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std::chrono;
using namespace std;

// source of strings used
// https://www.ssa.gov/oact/babynames/limits.html
// https://github.com/dwyl/english-words.git

class Tests {
public:

    vector<string> getStrings(string filename)
    {
        vector<string> nameList;
        string str;

        fstream names;
        names.open(filename, ios::in);

        while (std::getline(names, str))
        {
            std::stringstream ss(str);
            if (ss.good())
            {
                std::string substr;
                std::getline(ss, substr, ','); // Grab first strings till first comma
                nameList.push_back(substr);      // Push into the vector
            }
        }

        // remove duplicates
        sort(nameList.begin(), nameList.end());
        nameList.erase(unique(nameList.begin(), nameList.end()), nameList.end());

        // shuffle names
        auto rng = std::default_random_engine(15); // ***       SEED FOR RAND       ***
        std::shuffle(std::begin(nameList), std::end(nameList), rng);

        return nameList;
    }

    void timingTests(string filename)
    {
        Hash_Table_LP<string> linear;
        Hash_Table_QP<string> quad;
        vector<string> nameList;

        // get vector of names
        nameList = getStrings(filename);

        cout << "Name list size: " << nameList.size() << endl;
        double total_time_linear = 0;
        double total_time_quad = 0;

        int num_collisions_quad = 0;
        int num_collisions_linear = 0;
     
        for (size_t i = 0; i < nameList.size(); i++)
        {
            auto start_linear = high_resolution_clock::now();
            linear.insert(nameList[i]);
            auto stop_linear = high_resolution_clock::now();
            duration<double, std::micro> single_time_linear = stop_linear - start_linear;

            auto start_quad = high_resolution_clock::now();
            quad.insert(nameList[i]);
            auto stop_quad = high_resolution_clock::now();
            duration<double, std::micro> single_time_quad = stop_quad - start_quad;


            total_time_linear += single_time_linear.count(); // add single find time to pool of times for avl tree k
            total_time_quad += single_time_quad.count(); // add single find time to pool of times for splay tree k

            // check for collision
            if (quad._find_pos(nameList[i]) != quad._get_hash_modulus(nameList[i])) { num_collisions_quad++; }
            if (linear._find_pos(nameList[i]) != linear._get_hash_modulus(nameList[i])) { num_collisions_linear++; }
        }
             
        cout<< fixed << "total time to create a linear probing table with " << nameList.size() << " strings = " << total_time_linear << " microseconds" << endl;
        cout << fixed << "total time to create a quadratic probing table with " << nameList.size() << " strings = " << total_time_quad << " microseconds" << endl;

        cout << "Number of rehashes needed for linear creation: " << linear.num_rehash << endl;
        cout << "Number of rehashes needed for quadratic creation: " << quad.num_rehash << endl;

        cout << "number of collisions for linear creation = " << num_collisions_linear << endl;
        cout << "number of collisions for quadratic creation = " << num_collisions_quad << endl;


        cout << "size of linear = " << linear.get_size() << "\nsize of backing array = " << linear._elems.size() << endl;
        cout << "size of quadratic = " << quad.get_size() << "\nsize of backing array = " << quad._elems.size() << endl;

        cout << "num names in each table = " << nameList.size() << endl;

        //============================================= Find Tests ==========================================

        cout << endl;
        cout << "FIND TIMES:" << endl;
        cout << endl;

        // shuffle finds for find tests
        auto rng = std::default_random_engine(25); // ***       SEED FOR RAND       ***
        std::shuffle(std::begin(nameList), std::end(nameList), rng);

        double total_time_linear_find = 0;
        double total_time_quad_find = 0;

        int num_found_instantly_quad = 0;
        int num_found_instantly_linear = 0;

        for (size_t i = 0; i < nameList.size(); i++)
        {
            auto start_linear_find = high_resolution_clock::now();
            linear.contains(nameList[i]);
            auto stop_linear_find = high_resolution_clock::now();
            duration<double, std::micro> single_time_linear_find = stop_linear_find - start_linear_find;

            auto start_quad_find = high_resolution_clock::now();
            quad.contains(nameList[i]);
            auto stop_quad_find = high_resolution_clock::now();
            duration<double, std::micro> single_time_quad_find = stop_quad_find - start_quad_find;

            // add up single find times
            total_time_linear_find += single_time_linear_find.count(); 
            total_time_quad_find += single_time_quad_find.count(); 

            // check for number of items found in constant time
            if (quad._get_hash_modulus(nameList[i]) == quad._find_pos(nameList[i])) { num_found_instantly_quad++; }
            if (linear._get_hash_modulus(nameList[i]) == linear._find_pos(nameList[i])) { num_found_instantly_linear++; }    
        }

        // get average single time
        total_time_linear_find /= nameList.size();
        total_time_quad_find /= nameList.size();

        cout << "avg time for one linear find = " << total_time_linear_find << " microseconds" << endl;
        cout << "avg time for one quadratic find = " << total_time_quad_find << " microseconds" << endl;

        cout << "num strings found instantly for linear = " << num_found_instantly_linear << endl;
        cout << "num strings found instantly for quadratic = " << num_found_instantly_quad << endl;

      
    }
};

int main()
{
    Tests x;

    x.timingTests("yob2020.txt");
    cout << "\n--------------\n\n";
    x.timingTests("words.txt");
    // "words.txt", "yob2020.txt"

    return 0;
}

