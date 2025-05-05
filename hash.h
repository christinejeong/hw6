#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        // array to store up to 5 base-36 converted values (6 characters)
        unsigned long long w[5] = {0, 0, 0, 0, 0};

        // get total length of the input string
        int length = k.length();
        // track how many 6-character groups we've processed
        int groupCount = 0;

        // process 6 characters at a time in a loop starting from end of string
        for(int groupEnd = length; groupEnd > 0 && groupCount < 5; groupEnd-= 6) {
          // hold the base-36 value for the current group
          unsigned long long groupValue = 0;

          // convert a group of up to 6 characters into a base-36 number
          // start from the character at max(0, i-6) up to (but not including) i
          for(int charIdx = std::max(0, groupEnd - 6); charIdx < groupEnd; ++charIdx) {
            // convert to lowercase
            char lowerChar = std::tolower(k[charIdx]);
            // map character to base-36 digit (0-35)
            HASH_INDEX_T num = letterDigitToNumber(lowerChar);
            // base-36 conversion (shift left and add next digit)
            groupValue = groupValue * 36 + num;
          }

          // store result in "w" array from right to left
          w[4 - groupCount] = groupValue;
          // move to next group
          groupCount++;
        }
        // compute final hash value
        unsigned long long hashValue = 0;
        for(int index = 0; index < 5; ++index) {
          // multiply each w[i] by its corresponding random or fixed r[i]
          hashValue += rValues[index] * w[index];
        }
        return hashValue;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(std::isalpha(letter)) {
          // convert lowercase 'a'-'z' to 0-25
          return std::tolower(letter) - 'a';
        }
        else if(std::isdigit(letter)) {
          // convert '0'-'9' to 26-35
          return 26 + (letter - '0');
        }
        // default case
        return 0;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
