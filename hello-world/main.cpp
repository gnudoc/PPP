#include <algorithm>
#include <iostream>
#include <vector>
auto main() -> int {
  constexpr double cm2m = 100.0;
  constexpr double in2m = 39.37;
  double val = -1.0;
  char unit = ' ';
  double sum = 0.0;
  std::vector<double> values;
  std::cout << "Please enter number unit pairs, eg 7.8m 1.2i 3.14c.\n";
  std::cout << "when you're done, enter a string with no numbers.\n";
  while (std::cin >> val >> unit) {
    switch (unit) {
    case 'm':
      std::cout << "Good choice of unit!";
      values.push_back(val);
      break;
    case 'c':
      std::cout << "Hmm, interesting choice.";
      values.push_back(val / cm2m);
      break;
    case 'i':
      std::cout << "Bah, use a metric unit next time ;-)";
      values.push_back(val / in2m);
      break;
    default:
      std::cout << "That wasn't a known unit.";
      break;
    }
  }

  std::cout << "Now I'll print them out sorted and normalised as metres:\n";
  std::ranges::sort(values);
  for (double v : values) {
    std::cout << v << "m\n";
    sum += v;
  }
  std::cout << "Total: " << sum << "m\n";
}
