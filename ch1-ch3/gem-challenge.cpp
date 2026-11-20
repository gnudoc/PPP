#include <algorithm>
#include <iostream>
#include <vector>

auto main() -> int {

  // Quick intro / usage
  std::cout << "My chapters 1-3 challenge.\n";
  std::cout
      << "This should accept some server memory usage stats in a certain\n"
      << "format, sort and dedupe them, and print them in a different\n"
      << "format. Once I've got that working I'll implement some basic\n"
      << "analysis - count, sum, mean, median, SD.";

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

  // old less good index-based way - no bounds-checking - `i` could
  // theoretically be out-of-bounds - suppose we refactor something. for (size_t
  // i{0}; i < server_stats.size(); ++i) {
  //   if (i == 0 || server_stats[i].first != server_stats[i - 1].first) {
  //     deduped_server_data.push_back(server_stats[i]);
  //     std::cout << server_stats[i].first << ": " << server_stats[i].second
  //               << "M\n";
  //   }
  // }

  // analysis of memory values
}
