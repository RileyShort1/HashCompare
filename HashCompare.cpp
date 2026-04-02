#include "Hash_Table_LP.h"
#include "Hash_Table_QP.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std::chrono;

class Tests {
public:

    std::vector<std::string> getStrings(std::string filename)
    {
        std::vector<std::string> nameList;
        std::string str;

        std::fstream names;
        names.open(filename, std::ios::in);

        if (names.is_open()) {
            std::cout << "File open: " << filename << std::endl;
        }
        else {
            std::cout << "File open failed: " << filename << std::endl;
            return nameList;
        }

        while (std::getline(names, str))
        {
            std::stringstream ss(str);
            if (ss.good())
            {
                std::string substr;
                std::getline(ss, substr, ',');
                nameList.push_back(substr);
            }
        }

        names.close();

        sort(nameList.begin(), nameList.end());
        nameList.erase(unique(nameList.begin(), nameList.end()), nameList.end());

        auto rng = std::default_random_engine(15);
        std::shuffle(std::begin(nameList), std::end(nameList), rng);

        return nameList;
    }

    void runPerformanceComparison()
    {
        Hash_Table_LP<std::string> linear;
        Hash_Table_QP<std::string> quad;
        std::vector<std::string> nameList = getStrings(
            R"(C:/Users/riley/CLionProjects/HashCompare/yob2020.txt)");

        if (nameList.empty()) {
            std::cout << "Could not load yob2020.txt at the hardcoded path.\n";
            return;
        }

        std::cout << "Name list size: " << nameList.size() << std::endl;
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

            total_time_linear += single_time_linear.count();
            total_time_quad += single_time_quad.count();

            if (quad._find_pos(nameList[i]) != quad._get_hash_modulus(nameList[i])) { num_collisions_quad++; }
            if (linear._find_pos(nameList[i]) != linear._get_hash_modulus(nameList[i])) { num_collisions_linear++; }
        }

        std::cout << std::fixed << "Total time (insert), linear probing: " << total_time_linear << " us\n";
        std::cout << std::fixed << "Total time (insert), quadratic probing: " << total_time_quad << " us\n";

        std::cout << "\nRehashes (linear): " << linear.get_num_rehash() << std::endl;
        std::cout << "Rehashes (quadratic): " << quad.get_num_rehash() << std::endl;

        std::cout << "\nCollisions at insert home index (linear): " << num_collisions_linear << std::endl;
        std::cout << "Collisions at insert home index (quadratic): " << num_collisions_quad << std::endl;

        std::cout << "\nLinear table size: " << linear.get_size()
                  << ", backing array: " << linear._elems.size() << std::endl;
        std::cout << "Quadratic table size: " << quad.get_size()
                  << ", backing array: " << quad._elems.size() << std::endl;

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
            duration<double, std::nano> single_time_linear_find = stop_linear_find - start_linear_find;

            auto start_quad_find = high_resolution_clock::now();
            quad.contains(nameList[i]);
            auto stop_quad_find = high_resolution_clock::now();
            duration<double, std::nano> single_time_quad_find = stop_quad_find - start_quad_find;

            total_time_linear_find += single_time_linear_find.count();
            total_time_quad_find += single_time_quad_find.count();

            if (quad._get_hash_modulus(nameList[i]) == quad._find_pos(nameList[i])) { num_found_instantly_quad++; }
            if (linear._get_hash_modulus(nameList[i]) == linear._find_pos(nameList[i])) { num_found_instantly_linear++; }
        }

        total_time_linear_find /= static_cast<double>(nameList.size());
        total_time_quad_find /= static_cast<double>(nameList.size());

        std::cout << "\n--- Find (avg per lookup) ---\n";
        std::cout << std::fixed << "Avg linear find: " << total_time_linear_find << " ns\n";
        std::cout << std::fixed << "Avg quadratic find: " << total_time_quad_find << " ns\n";
        std::cout << "Found at home index (linear): " << num_found_instantly_linear << std::endl;
        std::cout << "Found at home index (quadratic): " << num_found_instantly_quad << std::endl;
    }
};

int main()
{
    Tests test;
    test.runPerformanceComparison();
    return 0;
}
