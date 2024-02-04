#include <stdio.h>
#include <stdlib.h>

//#define LRU_SIZE 20

typedef struct {
  int last_called_index; // the index called lastly
  int fib_n;             // index_number
  int fib_val;           // result
} LRUEntry;

LRUEntry *lru; //pointer for cache_size
int call_index = 0;
int found_in_cache = 0;
int result_cache = 0;
int cache[1000] = {1, 1};
int cache_size = 0;
int number_of_tests;

int recursive_fibo(int r);
int iterative_fib(int number_to_fib);
int cache_fibo(int c);
int fib_lru(int fl);
void help_command();

int main(int argc, char *argv[]) {
  int fib_to_get;
  float cache_hit_ratio;
  int N = atoi(argv[1]);
  cache_size = atoi(argv[2]);
  lru = malloc(cache_size* sizeof(LRUEntry)); //for cache_size
  number_of_tests = atoi(argv[3]);
  char command = argv[4][0];
  

  if (command == 'r') { // recursive
    printf("%dth Fibonacci number %d\n", N, recursive_fibo(N));
  }

  else if (command == 'i') { // iteration
    printf("%dth Fibonacci number %d\n", N, iterative_fib(N));

  }

  else if (command == 'c') { // cache with recursion
    printf("%dth Fibonacci number %d\n", N, cache_fibo(N));
  }

  else if (command == 'L') { 
    printf("%dth Fibonacci number %d\n", N, fib_lru(N));
  }

  else if (command == 'l') { // Least Recent Use
    for (int ix = 0; ix < number_of_tests; ix++) {
      fib_to_get = rand() % N;
      printf("test %d: fib %d = %d\n", ix, fib_to_get, fib_lru(fib_to_get));
      }
    cache_hit_ratio = (((float)found_in_cache)/(call_index))*100;
    printf("Cache Hit Ratio: %f%%\ncache size: %d", cache_hit_ratio, cache_size);
   // printf("counted number of found cache: %d / %d = %f\n", found_in_cache, call_index, (float)  found_in_cache / call_index);
  }

  else {
    help_command();
  }
  return 0;
}

void help_command() {
  printf("----Help Page----\n");
  printf("Type 'i' to get nth Fibonacci number by iteration\n");
  printf("Type 'r' to get nth Fibonacci number by recursion\n");
  printf("Type 'c' to get nth Fibonacci number by cache with recursion\n");
  printf("Type 'L' to get nth Fibonacci number by cache with LRU\n");
  printf("Type 'l' to get nth Fibonacci number with test cases and cache hit ratio by cache with LRU\n");
  printf("For command 'i', 'r', 'c', 'L', put |./main {nth number} 0 0 {command}| in command line.\n");
  printf("For command 'l', put |./main {max nth number} {cache size} {number of test case} {command}| in command line.\n");
  printf("Please Start Over\n");
}

int recursive_fibo(int r) {
  if (r >= 2)
    return recursive_fibo(r - 1) + recursive_fibo(r - 2);
  else {
    return r;
  }
}

int cache_fibo(int r) {
  if (r >= 2) {
    if (cache[r] != 0) {
      return cache[r];
    }
    cache[r] = cache_fibo(r - 1) + cache_fibo(r - 2);
    return cache[r];
  } else {
    return r;
  }
}

int iterative_fib(int number_to_fib) {
  int first = 0;
  int second = 1;
  int result;
  for (int i = 1; i < number_to_fib; i++) {

    result = first + second;
    first = second;
    second = result;
  }
  return result;
}

int fib_lru(int fl) {

  // Handle base cases
  if (fl == 0 || fl == 1) {
    // printf("base case %d: %d\n", fl, fl);
    //++call_index;
    //++found_in_cache;
    return fl;
  }

  for (int i = 0; i < cache_size; i++) {
    /*if value in cache?
     found_result_in_cache++
     update last_called_index for this cache entry
     return it;
 */
    if (lru[i].fib_n == fl) {
      ++call_index;
      ++found_in_cache;
      lru[i].last_called_index = call_index;
      // printf("%d is in the cache number %d", lru[i].last_called_index, fl);
      return lru[i].fib_val;
    }
  }
  // calculate it -> fib_lru(fibby-1) + fib_lru(fibby-2)
  int lru_val = fib_lru(fl - 1) + fib_lru(fl - 2);
  // printf("%dth Fibonacci number: %d\n", fl, lru_val);

  // if there is an empty cache slot, put it there
  for (int i = 0; i < cache_size; i++) {
    if (lru[i].fib_n == 0) {
      call_index++;
      lru[i].fib_n = fl;
      lru[i].last_called_index = call_index;
      lru[i].fib_val = lru_val;
      // printf("Putting the result value %d into the space %d\n", lru_val, fl);
      return lru_val;
    }
  }

  // else, replace the least recently used entry
  // find entry with lowest "last_called_index"
  // replace it, and use call_index
  int least_called_index = 0;
  for (int i = 0; i < cache_size; ++i) {
    if (lru[i].last_called_index < lru[least_called_index].last_called_index) {
      least_called_index = i;
    }
  }
  // printf("the least called index is %d\n",
         // lru[least_called_index].last_called_index);
  call_index++;
  lru[least_called_index].last_called_index = call_index;
  lru[least_called_index].fib_n = fl;
  lru[least_called_index].fib_val = lru_val;
  // printf("It is changed to %d\n", call_index);
  return lru_val;
}

// CHR = (number times found) / (number_times_looked)
// CMR = 1.0 - CHR
// CMD = (number_times_looked - number_times_found) / number_times_looked

