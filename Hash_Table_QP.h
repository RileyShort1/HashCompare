// Quadradic Probing 
//
#ifndef Hash_Table_QP_h
#define Hash_Table_QP_h
#include "Hash_Table_LP.h"
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <sstream>

using namespace std;

template <typename T>
class Hash_Table_QP : public Hash_Table_LP<T> {
public:
	Hash_Table_QP(size_t n = Hash_Table_LP<T>::DEFAULT_INIT_CAPACITY) : Hash_Table_LP<T>(n)
	{
		this->_max_load_factor = _get_biggest_allowed_max_load_factor();
	}

protected: // protected
	virtual float _get_biggest_allowed_max_load_factor() const
	{
		return 0.49f;
	}

	static int nextPerfectSquare(int N)
	{
		int nextN = (int)floor(sqrt(N)) + 1;

		return (nextN * nextN);
	}

	virtual size_t _find_pos(const T& item) const
	{
		// fail if backing array is full
		if (Hash_Table_LP<T>::_num_non_vacant_cells == Hash_Table_LP<T>::_elems.size() || Hash_Table_LP<T>::_elems.size() == 0)
		{
			return string::npos;
		}

		int firstSqAway = 0;
		size_t index = Hash_Table_LP<T>::_get_hash_modulus(item); // start at hash value
		int hashVal = index;

		while (true)
		{
			if (index > Hash_Table_LP<T>::_elems.size() - 1) // loop around array
			{
				index = index - Hash_Table_LP<T>::_elems.size();
			}

			if (Hash_Table_LP<T>::_elems[index]._state == Hash_Table_LP<T>::Entry::STATE::VACANT
				|| Hash_Table_LP<T>::_elems[index]._data == item)
			{
				return index;
			}

			index = hashVal; // reset to hash index

			// increment to next perfect sq
			index = index + this->nextPerfectSquare(firstSqAway);
			firstSqAway = this->nextPerfectSquare(firstSqAway);
		}

		return string::npos;
	}

	virtual void _grow_capacity()
	{
		// double the size of the backing vector
		size_t nxtPrime = _next_prime(Hash_Table_LP<T>::_elems.size() * 2);
		Hash_Table_LP<T>::_elems.resize(nxtPrime, typename Hash_Table_LP<T>::Entry());

		return;
	}

	// Private Helper
	static size_t _next_prime(size_t n)
	{
		int xn = n;

		if (xn < 2)
		{
			return 2;
		}

		for (size_t val = n + 1; val < 10000; val++)
		{
			if (_is_prime(val) == true)
			{
				return val;
			}
		}

		return 2;
	}

	// custom helper
	static bool _is_prime(size_t n) 
	{
		if (n < 2)
		{
			return false;
		}

		if (n == 2 || n == 3)
		{
			return true;
		}

		if (n % 2 == 0 || n % 3 == 0)
		{
			return false;
		}

		for (size_t i = 2; i < n; i++)
		{
			if (n % i == 0) {
				return false;
			}
		}

		return true;
	}

	friend class Tests;
};


#endif /* Hash_Table_QP_h */
