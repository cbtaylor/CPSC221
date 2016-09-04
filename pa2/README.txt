Programming Assignment #2
CPSC 221 - Summer 2015
Brian Taylor
52311859
z2b0b
Lab L0D

For my poor hash function I took the suggestion in the assignment and simply summed the ASCII values of the letters in the key, modulo the size of the table. This turned out to be a really bad hash function, causing severe bunching, unless the table was quite small. For my good hash function I used the suggested function from page 532 of the text. This required me to create a helper function to carry out modular exponentiation. Otherwise the numbers generated from the large exponents overflowed. As a result I found it easier to modify all my hash functions to take a second parameter of the size of the table. I included two more hash functions: the DJB2 hash function and the SDBM hash function, both described on the York University website (cited in the function comments). I experimented with both, but ended up only using the first one in the double hashing probe sequence. I discovered that a bug arose if I didn't explicitly prevent the increment from being zero in double hashing.

Clearly the load factor is quite important regardless of how good the hash function is. Once the load factor gets over 0.50 the average number of probes per insertion starts to rise quite rapidly.

Here are the results of the average number of probes required to populate a small hash table with 25 records.

hash      probing    load    capacity  size  avg. probes  Knuth's
function  method     factor                  per insert   formula

goodHash  quadratic  0.25     101       25      1.24      1.151
goodHash  quadratic  0.50      53       25      1.36      1.386
goodHash  quadratic  0.75      37       25      1.44      1.848

goodHash  double     0.25     101       25      1.16
goodHash  double     0.50      53       25      1.48
goodHash  double     0.75      37       25      1.48

poorHash  quadratic  0.25     101       25      1.04      1.151
poorHash  quadratic  0.50      53       25      1.12      1.386
poorHash  quadratic  0.75      37       25      1.20      1.848

poorHash  double     0.25     101       25      1.04
poorHash  double     0.50      53       25      1.28
poorHash  double     0.75      37       25      1.32

However, Knuth's formulas for the average number of probes needed by a particular algorithm are functions of the load factor (see The Art of Computer Programming, Volume 3: Sorting and Searching, Section 6.4). So, it's not really the same to determine the number of probes required to populate a hash table from a file since at the start the load factor is zero and only progressively approaches the value used in the formula.

A more appropriate test is to repeatedly insert (and then immediately remove) random keys into a hash table with a specific load factor. That's what I've done for all four combinations of hash function and probe sequence at load factors of 0.25, 0.50 and 0.75, for a hash table already populated with 500 keys. In each case I generated a random key, inserted the key, keeping track of the number of probes, and then immediately removed the key (maintaining a consistent load factor). For each combination I repeated this 100 time. The average number of probes per insertion is recorded in the table below.

hash      probing    load    capacity  size  avg. probes  Knuth's
function  method     factor                  per insert   formula

goodHash  quadratic  0.25    2003      500      1.29      1.151
goodHash  quadratic  0.50    1009      500      2.12      1.386
goodHash  quadratic  0.75     673      500      3.66      1.848

goodHash  double     0.25    2003      500      1.39
goodHash  double     0.50    1009      500      1.96
goodHash  double     0.75     673      500      3.37

poorHash  quadratic  0.25    2003      500     21.44      1.151
poorHash  quadratic  0.50    1009      500     21.44      1.386
poorHash  quadratic  0.75     673      500     21.44      1.848

poorHash  double     0.25    2003      500      2.40
poorHash  double     0.50    1009      500      3.66
poorHash  double     0.75     673      500      5.18

My experimental results are higher than the theoretical predictions of Knuth's formula. I believe this is because my hash function isn't as good as it could be. Knuth's formulas assume that all hash sequences are equally likely and that is obviously not true for any given hash function.

The numbers for poorHash show just how bad a hashing function it is, especially for the quadratic probing option. The table that results is incredibly bunched up (the 'pileup phenomenon' as Knuth calls it), so much so that there is no difference in the average number of probes per insertion, for the size I looked at, since the tables form in essentially the same way, just with differing amounts of 'whitespace'. However, when I repeated the experiments with smaller size file with 25 records I got numbers much closer to double hashing probing method: 1.46, 1.75, and 2.66.

Initially, I thought that the isPrime function could be improved upon and so I spent a fair bit of time turning pseudo code that I found for the Miller-Rabin primality test into C++ code. Then I spend some time figuring out how to time the performance of the two algorithms. It turns out that isPrime was much faster for the small numbers that were required for this program. Miller-Rabin is used primarily for truly large numbers where the algorithm encoded in the isPrime function would take inordinately long.

I've included three test files that I used in testing my program:

small.txt has 25 records
medium.txt has 500 records
large.txt has 10000 records

I left a few of the tests in my main function, but I used many more to figure out whether the program was behaving accoring to specification.

Here is a sample output from one of my tests where I populated a hash table with only 25 records and a load factor of 0.75. Then I inserted 100 random keys and removed them, counting the number of probes. The print out of the table is a special full version showing all contents of the entire table. Notice that all empty spaces have tombstones in them from the repeated insertion and deletion.

----- Full Table -----
0  FRMIUTSU  21
1  CDSZSLOZ  8
2  KFVVIXXQ  23
3  BAUKSDHH  19
4  ^^RIP^^^  0
5  ^^RIP^^^  0
6  CBRUGPAT  6
7  UUZNBEDX  16
8  JFMMCGEA  3
9  ^^RIP^^^  0
10  UXYVCIHA  1
11  TISPXANL  4
12  QOCCCTWF  13
13  YPLZNDHD  10
14  TKVNSNBE  15
15  ^^RIP^^^  0
16  ^^RIP^^^  0
17  DWXMRPQE  25
18  BOVRPGMM  5
19  GWMEMUXL  2
20  ELFIYOBP  17
21  EDBQYVMN  20
22  HSPAUMBX  12
23  LIVQCHOC  22
24  ^^RIP^^^  0
25  ^^RIP^^^  0
26  WKMRUUPX  14
27  ^^RIP^^^  0
28  ^^RIP^^^  0
29  ^^RIP^^^  0
30  YUUMKFWR  11
31  ^^RIP^^^  0
32  GSFBPSCU  24
33  ^^RIP^^^  0
34  HZXVADRR  7
35  UCBLAAGW  9
36  DKVERBIW  18
totalprobes: 293
average number of probes per insertion: 2.93
numkeys: 25 size: 37

