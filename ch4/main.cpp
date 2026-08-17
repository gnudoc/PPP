#include <iostream>
#include <stdexcept>

auto main() -> int {
  std::cout << "Hello from ch4!\nHere come the drills!\n";
  try {
    bool cond = false;
    if (!cond) {
      throw std::runtime_error{"The cond is false"};
    } else {
      return 0;
    }
  } catch (std::runtime_error &e) {
    // or use std::exception to catch both runtime_error and out_of_range
    std::cerr << "error: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Hmm. Unknown exception.\n";
    return 2;
  }
}
