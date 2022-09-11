#include "Hash_Table_LP.h"
#include "Hash_Table_QP.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std::chrono;

using namespace std;

// https://www.ssa.gov/oact/babynames/limits.html
class Tests {
public:

    vector<string> getNames()
    {
        vector<string> nameList;
        string str;

        fstream names;
        names.open("yob2020.txt", ios::in);

        while (std::getline(names, str))  // replace std::cin, with file input stream
        {
            std::stringstream ss(str);
            if (ss.good())
            {
                std::string substr;
                std::getline(ss, substr, ','); // Grab first names till first comma
                nameList.push_back(substr);      // Push into the vector
            }
        }

        // remove duplicates
        sort(nameList.begin(), nameList.end());
        nameList.erase(unique(nameList.begin(), nameList.end()), nameList.end());

        // shuffle names
        auto rng = std::default_random_engine(15);
        std::shuffle(std::begin(nameList), std::end(nameList), rng);

        return nameList;
    }

    void timingTests()
    {
        Hash_Table_LP<string> linear;
        Hash_Table_QP<string> quad;
        vector<string> nameList;

        // get vector of names
        nameList = getNames();

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
        
        // get average single time
        //total_time_linear /= nameList.size();
        //total_time_quad /= nameList.size();

        cout << "time for linear = " << total_time_linear << " microseconds" << endl;
        cout << "time for quad = " << total_time_quad << " microseconds" << endl;

        cout << "Num rehashes for linear: " << linear.num_rehash << endl;
        cout << "Num rehashes for quad: " << quad.num_rehash << endl;

        cout << "number of collisions for linear = " << num_collisions_linear << endl;
        cout << "number of collisions for quadratic = " << num_collisions_quad << endl;


        cout << "size of linear = " << linear.get_size() << " - size of backing array = " << linear._elems.size() << endl;
        cout << "size of quad = " << quad.get_size() << " - size of backing array = " << quad._elems.size() << endl;

        cout << "num names = " << nameList.size() << endl;

        //============================================= Find Tests ==========================================
        cout << endl;
        cout << "FIND TIMES" << endl;
        cout << endl;

        // shuffle finds for find tests
        auto rng = std::default_random_engine(25);
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
        //total_time_linear_find /= nameList.size();
        //total_time_quad_find /= nameList.size();

        cout << "time for linear find = " << total_time_linear_find << " microseconds" << endl;
        cout << "time for quad find = " << total_time_quad_find << " microseconds" << endl;

        cout << "num found instantly for linear = " << num_found_instantly_linear << endl;
        cout << "num found instantly quad = " << num_found_instantly_quad << " microseconds" << endl;

      
    }
};

int main()
{
    Tests x;

    x.timingTests();

    return 0;
}

