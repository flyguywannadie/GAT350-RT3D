#include "StringUtils.h"

namespace nc 
{
	std::string ToUpper(const std::string& str)
	{
		std::string rstr = ""; // create a return string

		for (int i = 0; i < str.size(); i++) { // make a for loop to loop through the characters in the string
			char newchar = str[i]; // get the character in the string

			if (newchar < 123 && newchar > 96) { // check if the number is within the range of lowercase character values
				newchar -= 32; // if so, shift it to the uppercase character
			}

			rstr += (newchar); // add it to the return string
		}
		
		return rstr; // return the finished string
	}

	std::string ToLower(const std::string& str)
	{
		std::string rstr = ""; // create a return string

		for (int i = 0; i < str.size(); i++) { // make a for loop to loop through the characters in the string
			char newchar = str[i]; // get the character in the string

			if (newchar < 91 && newchar > 64) { // check if the number is within the range of uppercase character values
				newchar += 32; // if so, shift it to the lowercase character
			}

			rstr += (newchar); // add it to the return string
		}

		return rstr; // return the finished string
	}

	bool IsEqualIgnoreCase(const std::string& str1, const std::string& str2)
	{
		return (ToLower(str1) == ToLower(str2)); // just compare the lowercase version of both strings to see if equal
	}

	std::string CreateUnique(const std::string& str)
	{
		// I got this function from Professor Maple in teams
		static uint32_t unique = 0; // static variable so that it will always be available to memory. that means it wont reset when called upon later.

		return str + std::to_string(unique++); // combine the string with the number, then add to it.
	}
}