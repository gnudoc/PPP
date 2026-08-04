#include <algorithm>
#include <iostream>
#include <vector>

void averages(std::vector<double> mems);

auto main() -> int {
  
  std::cout << "Hello from gem-challenge-1!\n";
  std::cout
      << "enter container name / mem usage pairs, then end with 'ok done'\n";
  std::cout << "e.g. jellyfin 1.2G\ndockge 2.4M\ntraefik 200k\nok done\n\n";
  std::string container;
  
  double mem{-1.0};
  char unit{'0'};
  const int thou{1024};
  std::vector<std::pair<std::string,double>> container_data;
  
  while (std::cin >> container >> mem >> unit) {
    double normalised_mem{0.0};
    switch (unit) {
    case 'T':
      normalised_mem = mem * thou * thou;
      break;
    case 'G':
      normalised_mem = mem * thou;
      break;
    case 'M':
      normalised_mem = mem;
      break;
    case 'k':
      normalised_mem = mem / thou;
      break;
    default:
      std::cout << "Sorry, I don't know that unit. Entry ignored.\n";
      continue;
    };
    container_data.emplace_back(container,normalised_mem);
  };

  std::vector<std::pair<std::string,double>> deduped_container_data;
  std::ranges::sort(container_data);
  for (size_t i{0}; i<container_data.size(); ++i) {
    if (i == 0 || container_data[i].first != container_data[i-1].first){
      deduped_container_data.push_back(container_data[i]);
      std::cout << container_data[i].first << ": " << container_data[i].second << "M\n";
    }
  }
  
  std::vector<double> mems;
  for (const auto& item : deduped_container_data) {
    mems.push_back(item.second);
  }
  averages(mems);
  return 0;
}

void averages(std::vector<double> mems) {
  double sum{0};
  double mean{0};
  double median{0};
  if (mems.size() == 0) {
    std::cout << "no mem usages recorded.\nAverage calcs abandoned .\n";
  } else {
    for (double mem : mems) {
      sum += mem;
    }
    size_t length{static_cast<size_t>(mems.size())};
    mean = sum / static_cast<double>(length);
    std::ranges::sort(mems);
    bool even_length{length % 2 == 0};
    size_t half_length{length / 2};
    if (even_length) {
      median = (mems[half_length] + mems[half_length - 1]) / 2;
    } else {
      median = mems[half_length];
    }
    std::cout << "Mean: " << mean << "M\nMedian: " << median << "M\n";
  }
}
