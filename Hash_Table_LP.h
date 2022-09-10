// Hash Table with linear probing
//
#ifndef Hash_Table_LP_h
#define Hash_Table_LP_h
#include <string>
#include <iostream>
#include <vector>
#include <ctgmath>
using namespace std;

template <typename T>
class Hash_Table_LP {
protected: // protected

	struct Entry {
		T _data; // payload
		enum STATE { ACTIVE, VACANT, DELETED } _state;
		Entry(const T& d = T(), STATE st = VACANT) : _data(d), _state(st) {}
	};

	static const size_t DEFAULT_INIT_CAPACITY = 3; // first odd prime (Do not change)
	vector<Entry> _elems; // table
	size_t _size; // Does not count deleted items
	size_t _num_non_vacant_cells; // does
	float _max_load_factor;
	size_t num_collisions = 0;

	virtual size_t _get_hash_modulus(const T& str) const // uses Hash(item), ext.
	{
		/*
	  const int PRIME_CONST = 31;
	  size_t hashCode = 0;
	  for (size_t i = 0; i < items.length(); i++)
	  {
		  hashCode += items[i] * pow(PRIME_CONST, i);
	  }
	
	  return hashCode % _elems.size();
	  */

		unsigned long hash = 5381;
		for (size_t i = 0; i < str.size(); ++i)
			hash = 33 * hash + (unsigned char)str[i];
		return hash % _elems.size();
	}
	virtual void _rehash() 
	{
		vector<T> myCopy;
		size_t numNonVacant = 0;
		size_t sze = 0;
		size_t size = _elems.size() * 2;
		_elems.resize(size, Entry());

		for (size_t i = 0; i < _elems.size(); i++)
		{
			if (_elems[i]._state != Entry::STATE::VACANT)
			{
				numNonVacant++;
			}
			if (_elems[i]._state == Entry::STATE::ACTIVE)
			{
				myCopy.push_back(_elems[i]._data);
				sze++;
				_elems[i]._state = Entry::STATE::VACANT;
			}
			if (_elems[i]._state == Entry::STATE::DELETED)
			{
				_elems[i]._state = Entry::STATE::DELETED;
			}

		}
		_size = 0;
		_num_non_vacant_cells = 0;

		for (size_t j = 0; j < myCopy.size(); j++)
		{
			insert(myCopy[j]);
		}

		_num_non_vacant_cells = numNonVacant;
		_size = sze;

		return;
	}

	// Most common overrides
	virtual bool set_max_load_factor(float x) 
	{
		if (x <= 0 || x > _get_biggest_allowed_max_load_factor())
		{
			return false;
		}

		_max_load_factor = x;

		return true;
	}
	virtual float _get_biggest_allowed_max_load_factor() const 
	{
		return 0.75;
	}
	virtual size_t _find_pos(const T& item) const 
	{
		// fail if backing array is full
		if (_num_non_vacant_cells == _elems.size() || _elems.size() == 0)
		{
			return string::npos;
		}

		// scan array linearly (begining at position from hash) 
		for (size_t i = _get_hash_modulus(item); i < _elems.size(); i++)
		{
			// scan is terminated by VACANT cells
			if (_elems[i]._state == Entry::STATE::VACANT)
			{
				return i;
			}

			if (_elems[i]._data == item)
			{
				return i;
			}

			if (i == _elems.size() - 1) // loop around 
			{
				i = -1;
			}
		}
		
		return string::npos;
	}
	virtual void _grow_capacity() 
	{
		// double the size of the backing vector
		_elems.resize(_elems.size() * 2, Entry());

		return;
	}

public:
	Hash_Table_LP(size_t n = DEFAULT_INIT_CAPACITY)
	{
		set_max_load_factor(_get_biggest_allowed_max_load_factor());
		//_max_load_factor = 0.75;
		if (n == 0)
		{
			_elems.resize(DEFAULT_INIT_CAPACITY, Entry());
		}
		else {
			_elems.resize(n, Entry());
		}
		_num_non_vacant_cells = 0;
		_size = 0;

	}
	size_t get_size() const { return _size; }
	bool is_empty() const { return _size == 0; }
	bool contains(const T& item) const 
	{
		size_t pos = _find_pos(item);

		if (pos == string::npos)
		{
			return false;
		}
		else if (_elems[pos]._data != item || _elems[pos]._state == Entry::STATE::DELETED)
		{
			return false;
		}

		return true;
	}
	T& find(const T& item)
	{
		size_t pos = _find_pos(item);

		if (pos == string::npos)
		{
			throw Not_found_exception();
		}

		else if (_elems[pos]._data != item || _elems[pos]._state == Entry::STATE::DELETED)
		{
			throw Not_found_exception();
		}

		return _elems[pos]._data;
	}
	bool clear()
	{
		_elems.clear();

		return true;
	}
	bool insert(const T& item)
	{
		// insert here
		size_t pos = _find_pos(item);

		// check for collision
		if (_elems[_get_hash_modulus(item)]._state == Entry::STATE::ACTIVE)
		{
			num_collisions++;
		}

		if (pos == string::npos)
		{
			return false;
		}

		if (_elems[pos]._state == Entry::STATE::VACANT) // add elem in 
		{
			_elems[pos]._data = item;
			_elems[pos]._state = Entry::STATE::ACTIVE;
			_num_non_vacant_cells++;
			_size++;

			if (_num_non_vacant_cells >= _elems.size() * _max_load_factor)
			{
				_rehash();
			}

			return true;
		}

		if (_elems[pos]._data == item)
		{
			if (_elems[pos]._state == Entry::STATE::DELETED) // if deleted reset to active
			{
				_elems[pos]._state = Entry::STATE::ACTIVE;
				_size++;

				return true;
			}
		}

		return false;
	}
	bool remove(const T& item)
	{
		size_t pos = _find_pos(item);

		if (pos == string::npos)
		{
			return false;
		}

		if (_elems[pos]._data != item || _elems[pos]._state == Entry::STATE::DELETED)
		{
			return false;
		}

		_elems[pos]._state = Entry::STATE::DELETED;
		_size--;
		return true;
	}

	class Table_full_exception : public exception {
	public: const string to_string() const throw() { return string("Table full exception"); }
	};
	class Not_found_exception : public exception {
	public: const string to_string() const throw() { return string("Not found exception"); }
	};

	friend class Tests;
};

#endif /* Hash_Table_LP_h */
