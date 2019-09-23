#include <iostream>
#include <memory>

class Expression {
public:
  virtual std::string toString() { return "expression"; };
  virtual int calculate() { return 0; };
  virtual ~Expression() = default;
};

class Binary : public Expression {
  std::string toString() override { return "Binary"; };

protected:
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

class Relation : public Binary {
  std::string toString() override { return "Relation"; };
};

class Less : public Relation {
public:
  Less(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|Less: one of argument is not provided");
    }
    try {
      return left->calculate() < right->calculate() ? 1 : 0;
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|Less: one of argument is not provided");
    }
    try {
      return left->toString() + " < " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class More : public Relation {
public:
  More(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|More: one of argument is not provided");
    }
    try {
      return left->calculate() > right->calculate() ? 1 : 0;
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|More: one of argument is not provided");
    }
    try {
      return left->toString() + " > " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class Equal : public Relation {
public:
  Equal(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|Equal: one of argument is not provided");
    }
    try {
      return left->calculate() == right->calculate() ? 1 : 0;
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Relation|Equal: one of argument is not provided");
    }
    try {
      return left->toString() + " = " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class Term : public Binary {
  std::string toString() override { return "Term"; };
};

class Plus : public Term {
public:
  Plus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Plus: one of argument is not provided");
    }
    try {
      return left->calculate() + right->calculate();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Plus: one of argument is not provided");
    }
    try {
      return left->toString() + " + " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class Minus : public Term {
public:
  Minus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Minus: one of argument is not provided");
    }
    try {
      return left->calculate() - right->calculate();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Minus: one of argument is not provided");
    }
    try {
      return left->toString() + " - " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class Mult : public Term {
public:
  Mult(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right) {
    this->left = std::move(left);
    this->right = std::move(right);
  };
  int calculate() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Multiplication: one of argument is not provided");
    }
    try {
      return left->calculate() * right->calculate();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
  std::string toString() override {
    if (left == nullptr || right == nullptr) {
      throw std::runtime_error("Term|Multiplication: one of argument is not provided");
    }
    try {
      return left->toString() + " * " + right->toString();
    } catch (const std::runtime_error &e) {
      throw;
    }
  }
};

class Primary : public Expression {};

class Integer : public Primary {
  int value;
  std::string toString() override { return std::to_string(value); }

public:
  int calculate() override { return value; }
  Integer(int value) : value(value){};
};

class Parenthesized : public Primary {
public:
  Parenthesized(std::unique_ptr<Expression> expression)
      : expression(std::move(expression)) {}
  int calculate() override { return expression->calculate(); }
  std::string toString() override { return "(" + expression->toString() + ")"; }

private:
  std::unique_ptr<Expression> expression;
};

class Parser {
public:
  static std::unique_ptr<Expression> parse(const std::string &input) {
    std::string str = removeSpaces(input);
    std::unique_ptr<Expression> result = parseRelation(str);
    if (!str.empty()) {
      std::cerr << "WARNING: Not parsed " << str << std::endl;
    }
    return result;
  }

private:
  static std::string removeSpaces(const std::string &input) {
    std::string inputWithoutSpaces;
    for (char ch : input) {
      if (ch != ' ') {
        inputWithoutSpaces.push_back(ch);
      }
    }
    return inputWithoutSpaces;
  }

  static std::unique_ptr<Expression> parseRelation(std::string &str) {
    std::unique_ptr<Expression> result = parseTerm(str);
    if (!str.empty()) {
      char ch = str[0];
      if (ch == '<') {
        str.erase(0, 1);
        result = std::make_unique<Less>(std::move(result), parseTerm(str));
      } else if (ch == '>') {
        str.erase(0, 1);
        result = std::make_unique<More>(std::move(result), parseTerm(str));
      } else if (ch == '=') {
        str.erase(0, 1);
        result = std::make_unique<Equal>(std::move(result), parseTerm(str));
      }
    }
    return result;
  }

  static std::unique_ptr<Expression> parseTerm(std::string &str) {
    std::unique_ptr<Expression> result = parseFactor(str);
    while (!str.empty()) {
      char ch = str[0];
      if (ch == '+') {
        str.erase(0, 1);
        result = std::make_unique<Plus>(std::move(result), parseFactor(str));
      } else if (ch == '-') {
        str.erase(0, 1);
        result = std::make_unique<Minus>(std::move(result), parseFactor(str));
      } else {
        break;
      }
    }
    return result;
  };

  static std::unique_ptr<Expression> parseFactor(std::string &str) {
    std::unique_ptr<Expression> result = parsePrimary(str);
    while (!str.empty()) {
      char ch = str[0];
      if (ch == '*') {
        str.erase(0, 1);
        result = std::make_unique<Mult>(std::move(result), parsePrimary(str));
      } else {
        break;
      }
    }
    return result;
  };

  static std::unique_ptr<Expression> parsePrimary(std::string &str) {
    int i = 0;
    if (str.empty()) {
      std::cerr << "Parsing error: Expected primary, but input is empty"
                << std::endl;
      return nullptr;
    }
    char ch = str[i];
    if (ch == '-' || ('0' <= ch && ch <= '9')) {
      bool isNegative = false;
      if (ch == '-') {
        isNegative = true;
        i++;
        ch = str[i];
      }
      int integer = 0;
      while ('0' <= ch && ch <= '9' && i < str.size()) {
        integer *= 10;
        integer += (ch - '0');
        i++;
        if (i < str.size()) {
          ch = str[i];
        }
      }
      if (isNegative) {
        if (i == 1) {
          std::cerr
              << "Parsing error: expected negative integer, found only sign \""
              << str << "\"" << std::endl;
          return nullptr;
        }
        integer *= -1;
      } else if (i == 0) {
        std::cerr << "Parsing error: Expected primary, found \"" << str << "\""
                  << std::endl;
        return nullptr;
      }
      str.erase(0, i);
      return std::make_unique<Integer>(integer);
    } else if ('(' == ch) {
      int brackets_counter = 1;
      std::string expression;
      while (i < str.size() && brackets_counter != 0) {
        i++;
        if (i < str.size()) {
          ch = str[i];
          if (ch == '(') {
            brackets_counter++;
            expression.push_back(ch);
          } else if (ch == ')') {
            brackets_counter--;
            if (brackets_counter != 0) {
              expression.push_back(ch);
            }
          } else {
            expression.push_back(ch);
          }
        }
      }
      if (brackets_counter != 0) {
        std::cerr << "Parsing error: no closed bracket \"" << str << "\""
                  << std::endl;
        return nullptr;
      }
      str.erase(0, i + 1);
      return std::make_unique<Parenthesized>(parseRelation(expression));
    }
    std::cerr << "Parsing error: expected primary, found \"" << str << "\""
              << std::endl;
    return nullptr;
  }
};

int main() {
  std::string input;
  std::getline(std::cin, input);
  try {
    std::unique_ptr<Expression> expressionTree = Parser::parse(input);
    std::string expressionStr = expressionTree->toString();
    std::cout << "Expression: " << expressionStr << std::endl;
    int result = expressionTree->calculate();
    std::cout << "Result: " << result << std::endl;
  } catch (const std::runtime_error &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }
  return 0;
}
