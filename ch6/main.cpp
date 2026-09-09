#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

/*
 * Simple calculator Program
 * Version 1:
 * Take an input like `5-3.2*(74.1+3)-1.7/4+5%2;` from cin
 * return the right answer to cout
 * First Tokenise it to '5', '-', '3.2' etc
 * Then parse that into the mathematical expression
 * And simplify that down to the right number
 * The grammar is:

   Statement:
     Expression
     Print
     Quit
   Print:
     ";"
   Quit:
     "q"
   Expression:
     Term
     Expression "+" Term
     Expression "-" Term
   Term:
     Primary
     Term "*" Primary
     Term "/" Primary
     Term "%" Primary
   Primary:
     Number
     " (" Expression ") "
     "-" Primary
     "+" Primary
   Number:
     floating point literal
 */

// define some magic constants
constexpr char number = '8';
constexpr char quit = 'q';
constexpr char print = ';';
constexpr std::string prompt = "> ";
constexpr std::string result = "= ";

class Token {
public:
  char kind;
  double value;
  Token(char k) : kind{k}, value{0.0} {}
  // It's bad practice to use raw chars - it would be better to define a scoped
  // enum
  Token(char k, double v) // NOLINT(bugprone-easily-swappable-parameters)
      : kind{k}, value{v} {}
};

class Token_stream {
public:
  auto get() -> Token;
  void putback(Token t);
  void ignore_up_to(char c);

private:
  bool full = false;
  Token buffer{0};
};

void Token_stream::ignore_up_to(char c) {
  // check the buffer for a `c` before setting it non-full
  if (full && c == buffer.kind) {
    full = false;
    return;
  }
  full = false;
  // then search the input for a `c`
  char ch = 0;
  while (std::cin >> ch)
    if (ch == c)
      return;
}

void Token_stream::putback(Token t) {
  if (full)
    throw std::runtime_error("putback() into a full buffer");
  buffer = t;
  full = true;
}

auto Token_stream::get() -> Token {
  if (full) {
    full = false;
    return buffer;
  }
  char ch = 0;
  if (!(std::cin >> ch))
    throw std::runtime_error("no input");

  switch (ch) {
  case print:
  case quit:
  case '%':
  case '(':
  case ')':
  case '+':
  case '-':
  case '*':
  case '/':
    return Token{ch};
  case '.':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9': {
    std::cin.putback(ch); // luckily we can put the char that we just read
    // back into the cin stream to be read into the val below
    double val = 0;
    std::cin >> val;
    return Token{number, val};
  }
  default:
    throw std::runtime_error("Bad token!");
  }
}
// This is bad practice; we should actually have the Token_stream and its
// parsing functions in a class Calculator instead.
Token_stream ts; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
auto expression() -> double;

auto primary() -> double {
  Token t = ts.get();
  switch (t.kind) {
  case '(': { // same as line 70
    double d = expression();
    t = ts.get();
    if (t.kind != ')')
      throw std::runtime_error("')' expected.");
    return d;
  }
  case number:
    return t.value;
  case '-':
    return -primary();
  case '+':
    return primary();
  default:
    throw std::runtime_error("'Primary' expected");
  }
}

auto term() -> double {
  double left = primary();
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
    case '*':
      left *= primary();
      t = ts.get();
      break;
    case '/': { // curly braces to keep d's scope from extending to the default
      // because otherwise we can hit the default block without defining s/th
      // that is in-scope. Which C++ doesn't like.
      double d = primary();
      if (d == 0)
        throw std::runtime_error("Div by 0! The Maths police are on the way!");
      left /= d; // not primary(), as that shifts to the next token (line 73)
      t = ts.get();
      break;
    }
    case '%': {
      double d = primary();
      if (d == 0)
        throw std::runtime_error("Div by 0! The Maths police are on the way!");
      left = std::fmod(left, d);
      t = ts.get();
      break;
    }
    default:
      ts.putback(t);
      return left;
    }
  }
}

auto expression() -> double {
  double left = term();
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
    case '+':
      left += term();
      t = ts.get();
      break;
    case '-':
      left -= term();
      t = ts.get();
      break;
    default:
      ts.putback(t);
      return left;
    }
  }
}

void clean_up_mess() { ts.ignore_up_to(print); }

void calculate() {
  while (std::cin)
    try {
      std::cout << prompt;
      Token t = ts.get();
      while (t.kind == print)
        t = ts.get();
      if (t.kind == quit)
        return;
      ts.putback(t);
      std::cout << result << expression() << '\n';
    } catch (std::exception &e) {
      std::cerr << "Error: " << e.what() << '\n';
      clean_up_mess();
    }
}

auto main() -> int {
  std::cout << "ch 5.3.1 - first attempt at calculator!\n"
            << "Enter int op int expression.\n"
            << "You can chain the expression eg 12+3*4/3 with BODMAS.\n"
            << "We can only handle +,-,*,/ ops at present.\n"
            << "Add a q to end the expression: ";
  try {
    calculate();
    return 0;
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Hmm. Unknown exception occurred. \n";
    return 2;
  }
}
