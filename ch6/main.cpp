#include <cmath>
#include <exception>
#include <iostream>
#include <numbers>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/*
 * Simple calculator Program
 * Version 1:
 ** Take an input like `5-3.2*(74.1+3)-1.7/4+5%2;` from cin
 ** return the right answer to cout
 ** First Tokenise it to '5', '-', '3.2' etc
 ** Then parse that into the mathematical expression
 ** And simplify that down to the right number
 * Version 2:
 ** Add option of defining variables with the `let` keyword
 * The grammar is:

   Calculation:
     Statement
     Print
     Quit
     Calculation Statement
   Statement:
     Declaration
     Expression
   Declaration:
     "let" Name "=" Expression
   Name:
     letter
     Name letter
     Name digit
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
const char let = 'L';
const char name = 'a';
const std::string decl_keywd = "let";

class Token {
public:
  char kind{0};
  double value{0.0};
  std::string name;
  Token() = default; // modern version of :kind{0} {}
  Token(char ch) : kind{ch} {}
  // It's bad practice to use raw chars - as an improvement, it would be better
  // to define a scoped enum when we come back to make this really robust
  Token(char ch, double val) // NOLINT(bugprone-easily-swappable-parameters)
      : kind{ch}, value{val} {}
  // move semantics are awesome!
  Token(char ch, std::string n) : kind{ch}, name{std::move(n)} {}
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

class Variable {
public:
  std::string name;
  double value;
};

std::vector<Variable>
    var_table; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

auto get_value(const std::string &s) -> double {
  for (const Variable &v : var_table)
    if (v.name == s)
      return v.value;
  throw std::runtime_error("trying to read undefined var " + s);
}

void set_value(const std::string &s, double d) {
  for (Variable &v : var_table)
    if (v.name == s) {
      v.value = d;
      return;
    }
  throw std::runtime_error("trying to write undef var " + s);
}

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
  buffer = std::move(t);
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
  case '=':
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
    if (isalpha(ch)) {
      std::string s;
      s += ch;
      while (std::cin.get(ch) && (isalpha(ch) || isdigit(ch)))
        s += ch;
      std::cin.putback(ch);
      if (s == decl_keywd)
        return Token{let}; // if we get "let", return the L token.
      return Token{name, s};
    }
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
  case name:
    return get_value(t.name);
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

auto is_declared(const std::string &var) -> bool {
  for (const Variable &v : var_table)
    if (v.name == var)
      return true;
  return false;
}

auto define_name(const std::string &var, double val) -> double {
  if (is_declared(var))
    throw std::runtime_error(var + " was declared twice.");
  var_table.push_back(Variable{.name = var, .value = val});
  return val;
}

auto declaration() -> double {
  Token t = ts.get();
  if (t.kind != name)
    throw std::runtime_error("name expected in declaration");
  Token t2 = ts.get();
  if (t2.kind != '=')
    throw std::runtime_error("= missing in declaration of " + t.name);
  double d = expression();
  define_name(t.name, d);
  return d;
}

auto statement() -> double {
  Token t = ts.get();
  switch (t.kind) {
  case let:
    return declaration();
  default:
    ts.putback(t);
    return expression();
  }
  return 0;
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
      std::cout << result << statement() << '\n';
    } catch (std::exception &e) {
      std::cerr << "Error: " << e.what() << '\n';
      clean_up_mess();
    }
}

auto main() -> int {
  std::cout << "ch 6.8.3 - second attempt at calculator!\n"
            << "Enter int op int expression.\n"
            << "You can chain the expression eg 12+3*4/34%5;\n"
            << "We can handle +,-,*,/,% ops at present.\n"
            << "pi and e are understood\n"
            << "`let boxHeight = 5` works\n"
            << "End each expression with a ;\n"
            << "Quit with a q";
  try {
    define_name("pi", std::numbers::pi);
    define_name("e", std::numbers::e);
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
