#include <iostream>
#include <stdexcept>
#include <vector>

// Stroustrup makes empty classes to throw, but I think these are unnecessarily
// clunky. I think std::runtime_error("message") is better.

// I haven't implemented his expect() functionality for pre- and post- conditions

auto area(int x, int y) -> int {
  // calculate an area
  // the two int's might be better off as size_t?
  if (x <= 0 || y <= 0)
    throw std::runtime_error("Dimensions must be positive!");
  return x * y;
}

auto framed_area(int x, int y, int z) -> int {
  // calculate area within a framed box
  // x and y must be > the frame width
  if (x <= z || y <= z)
    throw std::runtime_error("Dimensions must be wider than the frame!");;
  if (z <= 0)
    throw std::runtime_error("Frame width must be positive!");;
  for (int i = 0; i < x - z; ++i) {
    for (int j = 0; j < y - z; ++j) {
      std::cout << "@";
    }
    std::cout << '\n';
  }
  return area(x - z, y - z);
}

auto main() -> int {
  // init some stuff
  int x = 0;
  int y = 0;
  int z = 0;
  std::vector<int> v;

  // do some IO
  std::cout << "Part 1: Here's the equiv of 4.6.1.\n";
  std::cout << "Height is: ";
  std::cin >> x;
  std::cout << "Width is: ";
  std::cin >> y;
  std::cout << "Frame width is: ";
  std::cin >> z;

  // try the logic and catch the exceptions
  try {
    std::cout << framed_area(x, y, z) << '\n';
    std::cout << "Part 2: the range error demo.\n"
    << "Give me some ints, followed by a q to stop: ";
    for (int i = 0; std::cin >> i;)
      v.push_back(i);
    for (size_t j = 0; j <= v.size(); ++j)
      std::cout << "v[" << j << "] == " << v.at(j) << '\n';
    // v.at(j) gives the out_of_range exception, v[j] would give UB.
  } catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << '\n';
  } catch (const std::out_of_range& e) {
    std::cerr << "Range error! Check for off-by-ones? (" << e.what() << ")\n";
  } catch (...) {
    std::cerr << "Something bad happened. I don't know what.";
  }
  return 0;
}
