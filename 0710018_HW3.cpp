/*
Student No.: 0710018
Student Name: 張宸愷
Email: kie4280ann@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <pthread.h>
#include <semaphore.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

struct thread_arg {
  int thread_index;
  int start_index;
  int end_index;
};

std::vector<int> data1, data2;
sem_t start_sems[15], finish_sem;
thread_arg args[16];

void error(std::string a) {
  std::cout << a << std::endl;
  exit(EXIT_FAILURE);
}

void *distributeMT(void *);
void distributeST(int start, int end, int thread_id);
void bsort(std::vector<int> &data, int start, int end);
void merge(std::vector<int> &data, int left_start, int left_end,
           int right_start, int right_end);

void *distributeMT(void *thread_a) {
  thread_arg *arg = (thread_arg *)thread_a;
  sem_wait(&start_sems[arg->thread_index]);
  if (arg->thread_index < 7) {
    thread_arg &left = args[arg->thread_index * 2 + 1];
    left.start_index = arg->start_index;
    left.end_index = (arg->start_index + arg->end_index) / 2;
    thread_arg &right = args[arg->thread_index * 2 + 2];
    right.start_index = (arg->start_index + arg->end_index) / 2 + 1;
    right.end_index = arg->end_index;
    sem_post(&start_sems[arg->thread_index * 2 + 1]);
    sem_post(&start_sems[arg->thread_index * 2 + 2]);
    sem_wait(&start_sems[arg->thread_index]);
    sem_wait(&start_sems[arg->thread_index]);
    merge(data1, left.start_index, left.end_index, right.start_index,
          right.end_index);

  } else {
    bsort(data1, arg->start_index, arg->end_index);
  }

  if (arg->thread_index == 0) {
    sem_post(&finish_sem);
  } else {
    sem_post(&start_sems[(arg->thread_index - 1) / 2]);
  }
  return 0;
}

void distributeST(int start, int end, int thread_id) {
  if (thread_id < 7) {
    int middle = (start + end) / 2;
    distributeST(start, middle, thread_id * 2 + 1);
    distributeST(middle + 1, end, thread_id * 2 + 2);
    merge(data2, start, middle, middle + 1, end);
  } else {
    bsort(data2, start, end);
  }
}

void merge(std::vector<int> &data, int left_start, int left_end,
           int right_start, int right_end) {
  int ls = left_start, rs = right_start;
  std::vector<int> sorted;
  sorted.reserve(right_end - left_start + 1);
  while (true) {
    if (ls > left_end) {
      sorted.insert(sorted.end(), data.begin() + rs,
                    data.begin() + right_end + 1);
      break;
    } else if (rs > right_end) {
      sorted.insert(sorted.end(), data.begin() + ls,
                    data.begin() + left_end + 1);
      break;
    } else {
      if (data.at(ls) < data.at(rs)) {
        int d = data.at(ls);
        sorted.push_back(d);
        ls++;
      } else {
        int d = data.at(rs);
        sorted.push_back(d);
        rs++;
      }
    }
  }
  std::swap_ranges(sorted.begin(), sorted.end(), data.begin() + left_start);
}

void bsort(std::vector<int> &data, int start, int end) {
  for (int s = end; s >= start; --s) {
    for (int a = start; a < s; ++a) {
      if (data.at(a) > data.at(a + 1)) {
        std::swap(data.at(a), data.at(a + 1));
      }
    }
  }
}

int main() {
  std::string inputstr;
  std::cin >> inputstr;
  std::fstream inputFile(inputstr, std::ios::in);
  if (!inputFile) {
    error("error opening file");
    exit(EXIT_FAILURE);
  }
  int input_size;
  inputFile >> input_size;
  data1.reserve(input_size);
  for (int a = 0; a < input_size; ++a) {
    int num;
    inputFile >> num;
    data1.push_back(num);
  }
  data2 = data1;
  inputFile.close();

  for (int a = 0; a < 7; ++a) {
    sem_init(start_sems + a, 0, 0);
  }
  sem_init(&finish_sem, 0, 0);
  pthread_t thread;
  args[0].start_index = 0;
  args[0].end_index = data1.size() - 1;
  for (int a = 0; a < 15; ++a) {
    args[a].thread_index = a;
    if (pthread_create(&thread, nullptr, distributeMT, args + a) != 0) {
      error("Error creating pthread");
    }
    pthread_detach(thread);
  }
  std::chrono::time_point<std::chrono::high_resolution_clock> start =
      std::chrono::high_resolution_clock::now();
  sem_post(&start_sems[0]);
  sem_wait(&finish_sem);
  std::chrono::time_point<std::chrono::high_resolution_clock> end =
      std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> dur = end - start;
  std::cout << "MT sort: " << dur.count() << "ms" << std::endl << std::endl;

  std::fstream MTfile("output1.txt", std::ios::out | std::ios::trunc);
  for (int a = 0; a < data1.size(); ++a) {
    MTfile << data1.at(a) << " ";
  }
  MTfile.close();
  start = std::chrono::high_resolution_clock::now();
  distributeST(0, data2.size() - 1, 0);
  end = std::chrono::high_resolution_clock::now();
  dur = end - start;
  std::cout << "ST sort: " << dur.count() << "ms" << std::endl << std::endl;

  std::fstream STfile("output2.txt", std::ios::out | std::ios::trunc);
  for (int a = 0; a < data2.size(); ++a) {
    STfile << data2.at(a) << " ";
  }
  STfile.close();
  return 0;
}