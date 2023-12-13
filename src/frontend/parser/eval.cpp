//
// Created by zc175 on 2023/9/30.
//
#include "frontend/ast.h"
#include "utils/conv.h"
#include "utils/error.h"

// Below defines arithmetic operations for variant<int, float>
std::variant<int, float> operator-(const std::variant<int, float> &left) {
  if (left.index() == 0) {
    return -std::get<int>(left);
  } else {
    return -std::get<float>(left);
  }
}

std::variant<int, float> operator+(const std::variant<int, float> &left,
                                   const std::variant<int, float> &right) {
  float float_result = 0;
  int int_result = 0;
  bool is_float = false;

  if (left.index() == 0) {
    int_result += std::get<int>(left);
  } else {
    is_float = true;
    float_result += std::get<float>(left);
  }

  if (right.index() == 0) {
    if (is_float) {
      return float_result + (float)std::get<int>(right);
    } else {
      return int_result + std::get<int>(right);
    }
  } else {
    if (is_float) {
      return float_result + std::get<float>(right);
    } else {
      return (float)int_result + std::get<float>(right);
    }
  }
}

std::variant<int, float> operator-(const std::variant<int, float> &left,
                                   const std::variant<int, float> &right) {
  float float_result = 0;
  int int_result = 0;
  bool is_float = false;

  if (left.index() == 0) {
    int_result += std::get<int>(left);
  } else {
    is_float = true;
    float_result += std::get<float>(left);
  }

  if (right.index() == 0) {
    if (is_float) {
      return float_result - (float)std::get<int>(right);
    } else {
      return int_result - std::get<int>(right);
    }
  } else {
    if (is_float) {
      return float_result - std::get<float>(right);
    } else {
      return (float)int_result - std::get<float>(right);
    }
  }
}

std::variant<int, float> operator*(const std::variant<int, float> &left,
                                   const std::variant<int, float> &right) {
  float float_result = 0;
  int int_result = 0;
  bool is_float = false;

  if (left.index() == 0) {
    int_result += std::get<int>(left);
  } else {
    is_float = true;
    float_result += std::get<float>(left);
  }

  if (right.index() == 0) {
    if (is_float) {
      return float_result * (float)std::get<int>(right);
    } else {
      return int_result * std::get<int>(right);
    }
  } else {
    if (is_float) {
      return float_result * std::get<float>(right);
    } else {
      return (float)int_result * std::get<float>(right);
    }
  }
}

std::variant<int, float> operator/(const std::variant<int, float> &left,
                                   const std::variant<int, float> &right) {
  float float_result = 0;
  int int_result = 0;
  bool is_float = false;

  if (left.index() == 0) {
    int_result += std::get<int>(left);
  } else {
    is_float = true;
    float_result += std::get<float>(left);
  }

  if (right.index() == 0) {
    if (is_float) {
      return float_result / (float)std::get<int>(right);
    } else {
      return int_result / std::get<int>(right);
    }
  } else {
    if (is_float) {
      return float_result / std::get<float>(right);
    } else {
      return (float)int_result / std::get<float>(right);
    }
  }
}

std::variant<int, float> operator%(const std::variant<int, float> &left,
                                   const std::variant<int, float> &right) {
  int int_result = 0;

  if (left.index() == 0) {
    int_result += std::get<int>(left);
  } else {
    critical_error("Mod operation for non-int value");
  }

  if (right.index() == 0) {
    return int_result % std::get<int>(right);
  } else {
    critical_error("Mod operation for non-int value");
  }
  return 1;
}

// Below defines compile time evaluation functions for expressions
