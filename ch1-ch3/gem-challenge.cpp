#include <algorithm>
#include <iostream>
#include <vector>

void analysis(std::vector<double>);

auto main() -> int {
  // Quick intro / usage
  std::cout << "My chapters 1-3 challenge.\n";
  std::cout
      << "This should accept some server memory usage stats in a certain\n"
      << "format, sort and dedupe them, and print them in a different\n"
      << "format. Once I've got that working I'll implement some basic\n"
      << "analysis - count, sum, mean, median. Later maybe I'll add SD.";

  std::cout
      << "Enter your list of container name & memory usages. Then 'ok done'.\n";
  std::cout << "e.g.\n jellyfin 1.2G\ndockge 20.4M\ntraefik 260k\nok done\n\n";

  // declare / initialize vars
  std::string server_name;
  double mem_size{-1.0};
  double normalized_mem{-1.0};
  char unit{'0'};
  const int k{1024};
  std::vector<std::pair<std::string, double>> server_stats;
  std::vector<std::pair<std::string, double>> deduped_server_data;
  std::vector<double> mems;

  // main input loop
  while (std::cin >> server_name >> mem_size >> unit) {
    normalized_mem = 0.0;
    switch (unit) {
    case 'T':
      normalized_mem = mem_size * k * k;
      break;
    case 'G':
      normalized_mem = mem_size * k;
      break;
    case 'M':
      normalized_mem = mem_size;
      break;
    case 'k':
      normalized_mem = mem_size / k;
      break;
    default:
      std::cout << "Sorry, I don't know that unit. Entry ignored.\n";
      continue;
    };
    server_stats.emplace_back(server_name,
                              normalized_mem); // add to end of vector
  };

  // sort, de-dupe the server stats and print them.
  std::ranges::sort(server_stats);
  for (const auto &[name, mem] : server_stats) {
    if (deduped_server_data.empty() ||
        name != deduped_server_data.back().first) {
      deduped_server_data.emplace_back(name, mem);
      std::cout << name << ": " << mem << "M\n";
    }
  }

  // reserve some memory before the loop for memory efficiency
  mems.reserve(deduped_server_data.size());
  for (const auto &item : deduped_server_data) {
    mems.push_back(item.second);
  }
  analysis(mems);
}

// I wonder if this should be a separate file that the linker brings in?
void analysis(std::vector<double> mems) {
  auto count =
      static_cast<short>(mems.size()); // because we don't need/want a long
  double sum{0};
  double mean{0};
  double median{0};
  //double sd{0}; // TODO
  if (count == 0) {
    std::cout << "no mem usages recorded\nAve calcs abandoned.\n";
  } else {
    for (double mem : mems) {
      sum += mem;
    }
    mean = sum / count;
  }
  std::ranges::sort(mems);
  bool even_count{count % 2 == 0};
  // we use half_count as an index, so let's explicitly cast the assignment
  size_t half_count{static_cast<size_t>(count) / 2};
  if (even_count) {
    median = (mems[half_count]+mems[half_count-1]) /2;
  } else {
    median = mems[half_count];
  }
  std::cout << "Count: " << count
    << "\nMean: " << mean
    << "M\nMedian: " << median
    << "M\n";
}
