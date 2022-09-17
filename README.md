# HashCompare
A comparison between linear and quadratic probing hash table:

This project explores the performance differences between two types of open addressing hash table schemes: linear probing and quadradic probing. Both schemes work by probing for another space in the table if the initial one is occupied, one probing linearly and the other searching the next perfect square away from the initial hash location. Linear probing is known to result in clustering, whereas quadratic probing seeks to limit cluster formation by spreading out values. It is known that quadratic probing results in less clustering and therefore less time spent probing for values. With this in mind, we can assume that searching for a value in a quadratic hash table will be faster, on average, than searching in a linear table. Another element of performance is the maximum load factor of the hash table, which differs between both tables, with linear probing maxing out at 0.75 capacity and quadratic at 0.50 (0.49 to be on the safe side). Knowing this, we can also assume that the quadratic hash table will take up more space and require more rehashing operations than the linear table. This will likely mean it will take longer to build a quadratic hash table (insert all elements).

RESULTS:

