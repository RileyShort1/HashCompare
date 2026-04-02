# Linear vs. Quadratic Probing Hash Table Analysis

An empirical comparison of two open-addressing hash table schemes — linear probing and quadratic probing — benchmarked on insert and lookup performance using a real-world dataset of 28,766 unique names.

---

## Background

Both tables use open addressing, meaning collisions are resolved by probing for another slot within the same backing array rather than chaining. The two schemes differ in how they probe:

- **Linear probing** checks the next slot sequentially (index + 1, + 2, ...). Simple and cache-friendly, but prone to primary clustering — long runs of occupied cells that slow future insertions and lookups.
- **Quadratic probing** jumps to the next perfect square away from the home index (index + 1, + 4, + 9, ...). This spreads elements more evenly and reduces clustering, at the cost of a lower maximum load factor and larger memory footprint.

The `Hash_Table_QP` class inherits from `Hash_Table_LP` and overrides only the probing logic and load factor, keeping the comparison clean.

---

## Implementation Details

Both tables use the **djb2 hash function** and support `insert`, `contains`, and `remove`. Slots track three states: `ACTIVE`, `VACANT`, and `DELETED` — allowing correct behavior after removals without breaking probe chains.

| Property | Linear | Quadratic |
|----------|--------|-----------|
| Max load factor | 0.75 | 0.49 |
| Probing step | +1, +2, +3, ... | next perfect square |
| Backing array growth | 2x on rehash | 2x on rehash |

The lower load factor for quadratic probing is required for the probing sequence to be guaranteed to find an empty slot.

---

## Results

Tested on 28,766 unique names from the SSA baby names dataset (`yob2020.txt`), deduplicated and shuffled with a fixed seed for reproducibility.

**Insert**

| | Linear | Quadratic |
|-|--------|-----------|
| Total insert time | 11,742.8 µs | 15,201.6 µs |
| Rehashes | 14 | 15 |
| Collisions at home index | 15,514 | 10,230 |
| Final backing array size | 49,152 | 98,304 |

Quadratic probing takes ~29% longer to build the table. It rehashes a comparable number of times but ends up with a backing array twice the size due to its lower max load factor. Linear probing has significantly more collisions at the home index, reflecting its clustering behavior.

**Lookup**

| | Linear | Quadratic |
|-|--------|-----------|
| Avg find time | 117.0 ns | 89.1 ns |
| Found at home index | 20,251 | 24,488 |

Quadratic probing is ~24% faster on average lookups. Because it distributes elements more evenly, more elements are found directly at their home index (24,488 vs. 20,251), meaning fewer probing steps are needed on average.

**Summary:** Linear probing is faster and more memory-efficient to build. Quadratic probing is faster to search. The tradeoff depends on whether the table is read-heavy or write-heavy.

---

## Project Structure

```
HashCompare/
├── HashCompare.cpp       # Benchmark driver: insert timing, collision counting, find timing
├── Hash_Table_LP.h       # Linear probing hash table (templated, base class)
├── Hash_Table_QP.h       # Quadratic probing hash table (inherits from LP, overrides probing)
└── yob2020.txt           # SSA baby names dataset (source data)
```

---

## Building and Running

**Requirements:** A C++17-compatible compiler.

```bash
g++ -O2 -std=c++17 HashCompare.cpp -o HashCompare
./HashCompare
```

**Note:** The dataset path in `HashCompare.cpp` is currently hardcoded to a local absolute path. Update this line before running:

```cpp
std::vector<std::string> nameList = getStrings("yob2020.txt");
```

---

## Potential Improvements

- Make the dataset path a command-line argument rather than a hardcoded string
- Test across multiple dataset sizes to observe how the performance gap changes with scale
- Add a remove-heavy workload to measure the effect of `DELETED` tombstones on probe chain length
- Compare against separate chaining as a third baseline

