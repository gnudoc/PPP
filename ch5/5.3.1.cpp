#include <iostream>
#include <stdexcept>

auto main() -> int {
  std::cout << "ch 5.3.1 - first attempt at calculator!\n"
            << "Enter int op int expression.\n"
            << "You can chain the expression eg 12+3*4/3 but no BODMAS.\n"
            << "We can only handle +,-,*,/ ops at present.\n"
            << "Add an x to end the expression: ";

  int lval = 0;
  int rval = 0;

  std::cin >> lval;
  if (!std::cin) {
    std::cerr << "No first operand";
    return 1;
  }

  for (char op = 0; std::cin >> op;) {
    if (op != 'x')
      std::cin >> rval;
    if (!std::cin) {
      std::cerr << "No operand after final operator.";
      return 1;
    }

    switch (op) {
    case '+':
      lval += rval;
      break;
    case '-':
      lval -= rval;
      break;
    case '*':
      lval *= rval;
      break;
    case '/':
      lval /= rval;
      break;
    default:
      std::cout << "Result: " << lval << '\n';
      return 0;
    }
  }
  std::cerr << "Bad expression.";
}
