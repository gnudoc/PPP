#include <iostream>
#include <stdexcept>

auto area(int x, int y) -> int {
  // calculate an area
  // the two int's might be better off as size_t?
  // but we'll try/catch them instead.
  try {
    if (x <= 0 || y <= 0) {
      throw std::runtime_error{"Physical dimensions need to be positive!"};
    } else {
      return x * y;
    }
  } catch (std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return -1;
  } catch (...) {
    std::cerr << "Oops. Unknown exception.\n";
    return -2;
  }
}

auto framed_area(int x, int y) -> int {
  // calculate area within a framed box
  // x and y must be > the frame width
  constexpr int frame_width = 2;
  try {
    if (x <= frame_width || y <= frame_width) {
      throw std::runtime_error{"The box doesn't have a positive area!"};
    } else {
      return area(x - frame_width, y - frame_width);
    }
  } catch (std::runtime_error &e) {
    // or use std::exception to catch both runtime_error and out_of_range
    std::cerr << "error: " << e.what() << '\n';
    return -1;
  } catch (...) {
    std::cerr << "Hmm. Unknown exception.\n";
    return -2;
  }
}

auto main() -> int {
  int x = 0;
  int y = 0;
  std::cout << "Here's the equiv of 4.5.2.\n";
  std::cout << "Height is: ";
  std::cin >> x;
  std::cout << "Width is: ";
  std::cin >> y;
  std::cout << framed_area(x, y);
  return 0;
}
