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

        double total_time_linear = 0;
        double total_time_quad = 0;
          /*
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
        }

        total_time_linear /= nameList.size();
        total_time_quad /= nameList.size();

        cout << "time for linear = " << total_time_linear << " microseconds" << endl;
        cout << "time for quad = " << total_time_quad << " microseconds" << endl;

        cout << "number of collisions for linear = " << linear.num_collisions << endl;
        cout << "number of collisions for quadratic = " << quad.num_collisions << endl;

        cout << "size of linear = " << linear.get_size() << " - size of backing array = " << linear._elems.size() << endl;
        cout << "size of quad = " << quad.get_size() << " - size of backing array = " << quad._elems.size() << endl;

        cout << "num names = " << nameList.size() << endl;
         */
       /*
        for (size_t i = 0; i < nameList.size(); i++)
        {
            if (quad.contains(nameList[i]) == false)
            {
                cout << nameList[i] << endl;
            }

        }
        */
     
        //cout << quad.contains("Portland") << endl;

        //quad.insert("Portland");
        //cout << quad.contains("Portland");

        /*
        for (size_t i = 0; i < linear._elems.size(); i++)
        {
            if (linear._elems[i]._data == "Portland")
            {
                cout << "found it" << endl;
            }
        }
        */
        for (size_t i = 0; i < nameList.size(); i++)
        {

            auto start_linear = high_resolution_clock::now();
            linear.insert(nameList[i]);
            auto stop_linear = high_resolution_clock::now();
            duration<double, std::micro> single_time_linear = stop_linear - start_linear;
        }
    }
   // "Portland"

};

int main()
{
    Tests x;

    x.timingTests();

    return 0;
}

