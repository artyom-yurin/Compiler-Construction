#include <iostream>
#include <memory>

class Expression {
public:
  virtual std::string toString() { return "expression"; };
  virtual ~Expression() = default;
};

class Binary : public Expression {
  std::string toString() { return "Binary"; };

protected:
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
};

class Relation : public Binary {
  std::string toString() { return "Relation"; };
};

class Less : public Relation {
  std::string toString() override {
    return left->toString() + " < " + right->toString();
  }
};

class More : public Relation {
  std::string toString() override {
    return left->toString() + " > " + right->toString();
  }
};

class Equal : public Relation {
  std::string toString() override {
    return left->toString() + " = " + right->toString();
  }
};

class Term : public Binary {
  std::string toString() { return "Term"; };
};

class Plus : public Term {
  std::string toString() override {
    return left->toString() + " + " + right->toString();
  }
};

class Minus : public Term {
  std::string toString() override {
    return left->toString() + " - " + right->toString();
  }
};

class Mult : public Term {
  std::string toString() override {
    return left->toString() + " * " + right->toString();
  }
};

class Primary : public Expression {};

class Integer : public Primary {
  int value;
  std::string toString() override { return std::to_string(value); }

public:
  Integer(int value) : value(value){};
};

class Parenthesized : public Primary {
public:
  Parenthesized(std::unique_ptr<Expression> expression)
      : expression(std::move(expression)) {}
  std::string toString() override { return "(" + expression->toString() + ")"; }

private:
  std::unique_ptr<Expression> expression;
};

class Parser {
public:
  static std::unique_ptr<Expression> parse(const std::string &input) {
    std::string str = removeSpaces(input);
    return parseRelation(str);
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
    std::unique_ptr<Expression> left = parseTerm(str);
    // while true
    // op
    // another one Term
    return std::make_unique<Relation>();
  }

  static std::unique_ptr<Expression> parseTerm(std::string &str) {
    int i = 0;
    if (str.size() == i) {
      return nullptr;
    }
    char ch = str[i];
    if ('0' <= ch && ch <= '9') {
      int integer = 0;
      while ('0' <= ch && ch <= '9' && i < str.size()) {
        integer *= 10;
        integer += (ch - '0');
        i++;
        if (i < str.size()) {
          ch = str[i];
        }
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
        return nullptr;
      }
      str.erase(0, i + 1);
      return std::make_unique<Parenthesized>(parseRelation(expression));
    }
  }
};

int main() {
  std::string input;
  std::getline(std::cin, input);
  std::unique_ptr<Expression> expressionTree = Parser::parse(input);
  std::cout << expressionTree->toString() << std::endl;
  // int result = expressionTree.calculate();
  return 0;
}
