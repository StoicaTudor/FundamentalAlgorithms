/*

Stoica Tudor Mihai
Group 30423

Task: You are required to implement correctly and efficiently the insert and search operations in a hash table
using open addressing and quadratic probing. You are required to evaluate the search operation for hash tables using open addressing and
quadratic probing, in the average case (remember to perform 5 runs for this). You will do this in
the following manner:

1. Select N, the size of your hash table, as a prime number around 10000 (e.g. 9973, or
10007);
2. For each of several values for the filling factor α is {0.8, 0.85, 0.9, 0.95, 0.99}, do:
a. Insert n random elements, such that you reach the required value for α (α = n/N)
b. Search, in each case, m random elements (m ~ 3000), such that approximately half
of the searched elements will be found in the table, and the rest will not be found
(in the table). Make sure that you sample uniformly the elements in the found
category, i.e. you should search elements which have been inserted at different
moments with equal probability (there are several ways in which you could ensure
this – it is up to you to figure this out)
c. Count the operations performed by the search procedure (i.e. the number of cells
accessed during the search)

What I've done:

implement a Hash class with several methods

- First, initialize initialize average effort and set max effort to -1; set an array of filling factors
- For each test, for each filling factor, use the utility entry=Hash::getRandomEntryArray(int size), in order to assign an array of random values for insert into the hash array
- After that, use hashObj.insertArrayIntoHash(entry); // using quadratic probing, insert elements in the hash array
- For inserting I used (int index = getHashID(entry.id);); we insert an entry to hashArray[index]; getHashID is the one which uses qudratic probing, until it finds an 
empty spot (id == -1); I used for that C0=0 and C1=1
- After insertion, perform the search; since we are required to search aprox 3000 elements, I searched n/3 elements (which in our case turns out to search circa 2500-3500 elements,
depending on the filling factor)
- Half of the elements shall be found and the other half shall not be found; for this, I have randomly chosen numbers ranging from 0 to N, since the randomly inserted ids 
were ranging from 0 to N too;
- If we found the element, count the effort and add it to the averageEffortFound.first array; count the number of times the element has been found in averageEffortFound.second
- If we have a maximum effort, save it into the maxEffort array
- Perform the same thing with averageEffortNotFound array
- Finally compute the average (total / nr elements)
- Output the results

N = 9973				
Filling Factor	Max Effort Found	Average Effort Found	Max Effort  Not Found	Average Effort Not Found
0.8						53					1.59922					71							7.48183
0.85					60					1.99334					78							12.8705
0.9						127					2.38911					125							20.1592
0.95					83					3.5517					171							36.6475		
0.99					255					4.93252					738							103.922


Conclusions:
- As the filling factor grows, the effort grows too. It is obvious, because it will be even harder to insert elements, as the available space diminishes;
- Average Effort Not Found is much more bigger than Average Effort Found, since Average Effort Not Found tends to reach O(n), while Average Effort Found is much more faster
- Still, Hash tables are a effective way of making a trade of between speed and space, since calling an element from the hash array requires O(1)

*/


#include <cstdlib>
#include <iostream>
#include <utility>
#define NR_TESTS 5
#define DEFAULT_C0 0 
#define DEFAULT_C1 1

using namespace std;

class Hash {
 public:
  static const int MAX_LENGTH_NAME = 30;

  static struct Entry {
    int id = -1; // by default, the id is -1
    char name[MAX_LENGTH_NAME];
  };

  static const int N = 9973;  // set N to 9973

 private:
  double fillingFactor;
  int n;
  Entry hashArr[N];
  int currentNrElementsInHashArr = 0;
  int C0, C1;

 public:
  Hash() {
    // empty constructor
  }

  Hash(int n, double fillingFactor,int C0, int C1) {
    this->n = n;
    this->fillingFactor = fillingFactor;
    this->C0 = C0;
    this->C1 = C1;
  }

  void insertArrayIntoHash(Entry entry[]) {
    for (unsigned int i = 0; i < this->n; i++) {
      this->insert(entry[i]);
    }
  }

  /*
   get pair of <indexOfFoundElement,search effort>
   */
  pair<int, int> searchId(int id) {
    pair<int, int> searchResult;

    for (int i = 0; i < N; i++) {
      int searchIndex = (id + i * C0 + i * i * C1) % N;

      if (hashArr[searchIndex].id == -1) {  // empty

        searchResult.first = -1;  // not found
        searchResult.second = i + 1;

        return searchResult;
      } else if (hashArr[searchIndex].id != id) {
        // repeat the iteration
        continue;
      } else {
        // if we found the id
        searchResult.first = searchIndex;
        searchResult.second = i + 1;

        return searchResult;
      }
    }

    searchResult.first = -1;  // not found
    searchResult.second = N;  // infinit nr of effort

    return searchResult;
  }

 private:
  void insert(Entry entry) {
    int index = getHashID(entry.id);

    if (index > 0) {
      this->hashArr[index] = entry;
    } else {
      // cout << "COULDN'T FIND FREE INDEX\n";
    }
  }

  int getHashID(int id) {
    for (int i = 0; i < N; i++) {
      int index = (id + C0 * i + C1 * i * i) % N;

      if (hashArr[index].id == -1) {
        // we found a free sport

        return index;
      }
    }

    return -1;
  }

 public:
  void printHashArr() {
    for (int i = 0; i < N; i++) {
      // cout << hashArr[i].id << " " << hashArr[i].name << endl;
      cout << hashArr[i].id << " ";
    }

    cout << endl;
  }

 private:
  static char getRandomChar() { return (rand() % ('a' - 'A' - 1) + 'A'); }

  static char* getRandomString() {
    int randomNrCharacters = rand() % MAX_LENGTH_NAME;

    char* randomString = new char[randomNrCharacters];

    for (int i = 0; i < randomNrCharacters - 1; i++) {
      randomString[i] = getRandomChar();
    }

    randomString[randomNrCharacters - 1] = '\0';

    return randomString;
  }

 public:
  static Entry* getRandomEntryArray(int size) {
    Entry* entryRandomArr = new Entry[size];

    for (int i = 0; i < size; i++) {
      entryRandomArr[i].id = rand() % N;
      char* randomString = getRandomString();

      for (int k = 0; k <= strlen(randomString); k++) {
        entryRandomArr[i].name[k] = randomString[k];
      }
    }

    return entryRandomArr;
  }
};

void doTask() {
  double fillingFactor[5] = {0.8, 0.85, 0.9, 0.95, 0.99};

  Hash hashObj;
  Hash::Entry* entry;

  int maxEffortFound[5];     // for each filling factor, a max
  int maxEffortNotFound[5];  // for each filling factor, a max

  pair<double,int> averageEffortFound[5]; // pair, effort and nr of searched items
  pair<double, int> averageEffortNotFound[5]; // pair, effort and nr of searched items

  for (int j = 0; j < 5; j++) { // initialize average effort
    averageEffortFound[j].first = averageEffortNotFound[j].first = 0;
    averageEffortFound[j].second = averageEffortNotFound[j].second = 0;
  }

  for (int i = 0; i < 5; i++) {
    maxEffortFound[i] = maxEffortNotFound[i] = -1; // set max effort to -1
  }

  for (int i = 0; i < NR_TESTS; i++) { // for each test
    for (unsigned int j = 0;
         j < sizeof(fillingFactor) / sizeof(fillingFactor[0]); j++) { // for each filling factor
		 
      int n = (int)(Hash::N * fillingFactor[j]); // number of elements is N * filling factor

      hashObj = Hash(n, fillingFactor[j],DEFAULT_C0,DEFAULT_C1); // initialize hash object
      entry = hashObj.getRandomEntryArray(n); // get entry array from utility method Hash.getRandomEntryArray(int size)

      hashObj.insertArrayIntoHash(entry); // using quadratic probing, insert elements in the hash array
      // hashObj.printHashArr();

      // for a certain filling factor, for a certain test, we perform the search
      int effortFound = 0, effortNotFound = 0;

      for (int k = 0; k < n; k++) { // i from 0 to N * filling factor
        if (rand() % 3 == 0) {  // aprox. 3000 elements -> I used this, such that it will return true in on third of times (aprox 3000 times / aprox 10000)
			
		// perform search
          pair<int, int> searchResult = hashObj.searchId(
              rand() %
              hashObj.N);  // a 50-50 probability that we find the element

          if (searchResult.first != -1) {  // we found the element
		  
            averageEffortFound[j].second++; // increment nr of elements found
            effortFound = searchResult.second; // this is the effortFound
            averageEffortFound[j].first += effortFound; // add effortFound to total effort
           
            if (effortFound > maxEffortFound[j]) { // look for the maximum
              maxEffortFound[j] = effortFound;
            }
          } else {
              averageEffortNotFound[j].second++; // increment nr of elements not found
            effortNotFound = searchResult.second; // this is the effortFound
            averageEffortNotFound[j].first += effortNotFound; // add effortFound to total effort

            if (effortNotFound > maxEffortNotFound[j]) { // look for the maximum
              maxEffortNotFound[j] = effortNotFound;
            }
          }
        }
      }
    }
  }

  for (int i = 0; i < 5; i++) { // compute the average (total / nr elements)
    averageEffortFound[i].first /= (double) (averageEffortFound[i].second); 
    averageEffortNotFound[i].first /= (double)(averageEffortNotFound[i].second);
  }

  cout << "N = " << Hash::N << endl;

  for (int j = 0; j < sizeof(fillingFactor) / sizeof(fillingFactor[0]); j++) {
    cout << "for filling factor alfa = " << fillingFactor[j]
         << "   we have maxEffortFound   =   " << maxEffortFound[j]
         << "   and    maxEffortNotFound  =   " << maxEffortNotFound[j] << endl;

    cout << "for filling factor alfa = " << fillingFactor[j]
         << "   we have averageEffortFound   =   " << averageEffortFound[j].first
         << "   and    averageEffortNotFound  =   " << averageEffortNotFound[j].first
         << endl;

    cout << endl << endl;
  }
}

int main() {
  doTask();
  return 0;
}