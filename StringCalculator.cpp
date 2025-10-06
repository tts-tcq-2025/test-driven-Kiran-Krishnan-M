#include "StringCalculator.h"

#include <algorithm>
#include <iterator>   // for std::back_inserter
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Replace all occurrences of 'delimiter' in 'text' with 'with'.
// Avoids regex so delimiters with regex meta characters are treated literally.
static std::string replace_all(std::string text,
                               const std::string& delimiter,
                               const std::string& with) {
  if (delimiter.empty()) return text;
  std::size_t pos = 0;
  while ((pos = text.find(delimiter, pos)) != std::string::npos) {
    text.replace(pos, delimiter.size(), with);
    pos += with.size();
  }
  return text;
}

int StringCalculator::Add(const std::string& numbers) {
  if (numbers.empty()) {
    return 0;
  }

  std::string delimiter = parseDelimiter(numbers);
  std::string numbers_part = extractNumbers(numbers);
  std::string normalized = normalizeDelimiters(numbers_part, delimiter);
  std::vector<int> nums = parseNumbers(normalized);

  validateNumbers(nums);

  return calculateSum(nums);
}

std::string StringCalculator::parseDelimiter(const std::string& numbers) {
  if (!hasCustomDelimiterFormat(numbers)) {
    return ",";
  }

  std::size_t newline_pos = numbers.find('\n');
  if (newline_pos == std::string::npos) {
    // Malformed header: fall back to default delimiter
    return ",";
  }

  // Extract content between '//' and the first newline.
  std::string header = numbers.substr(2, newline_pos - 2);
  return extractDelimiterFromHeader(header);
}

bool StringCalculator::hasCustomDelimiterFormat(const std::string& numbers) {
  return numbers.length() >= 4 && numbers.rfind("//", 0) == 0;
}

std::string StringCalculator::extractDelimiterFromHeader(
    const std::string& delimiter_part) {
  if (hasBracketFormat(delimiter_part)) {
    // Strip leading '[' and trailing ']'.
    return delimiter_part.substr(1, delimiter_part.length() - 2);
  }
  return delimiter_part;
}

bool StringCalculator::hasBracketFormat(const std::string& delimiter_part) {
  return delimiter_part.length() >= 2 &&
         delimiter_part.front() == '[' &&
         delimiter_part.back() == ']';
}

std::string StringCalculator::extractNumbers(const std::string& numbers) {
  if (!hasCustomDelimiterFormat(numbers)) {
    return numbers;
  }
  return extractNumbersAfterDelimiterHeader(numbers);
}

std::string StringCalculator::extractNumbersAfterDelimiterHeader(
    const std::string& numbers) {
  std::size_t newline_pos = numbers.find('\n');
  if (newline_pos == std::string::npos || newline_pos + 1 >= numbers.length()) {
    // If malformed, return original; spec does not require rejecting it.
    return numbers;
  }
  return numbers.substr(newline_pos + 1);
}

std::string StringCalculator::normalizeDelimiters(const std::string& numbers_part,
                                                  const std::string& delimiter) {
  std::string result = numbers_part;

  if (delimiter != ",") {
    result = replaceDelimiterWithComma(result, delimiter);
  }

  std::replace(result.begin(), result.end(), '\n', ',');

  return result;
}

std::string StringCalculator::replaceDelimiterWithComma(
    const std::string& text, const std::string& delimiter) {
  return replace_all(text, delimiter, ",");
}

std::vector<int> StringCalculator::parseNumbers(
    const std::string& normalized_numbers) {
  std::vector<int> nums;
  std::stringstream ss(normalized_numbers);
  std::string token;

  while (std::getline(ss, token, ',')) {
    if (!token.empty()) {
      nums.push_back(std::stoi(token));
    }
  }
  return nums;
}

int StringCalculator::calculateSum(const std::vector<int>& nums) {
  return std::accumulate(
      nums.begin(), nums.end(), 0,
       { return (n <= 1000) ? (sum + n) : sum; });
}

void StringCalculator::validateNumbers(const std::vector<int>& nums) {
  std::vector<int> negatives = findNegativeNumbers(nums);
  if (!negatives.empty()) {
    std::string message = buildNegativeNumbersMessage(negatives);
    throw std::invalid_argument(message);
  }
}

std::vector<int> StringCalculator::findNegativeNumbers(
    const std::vector<int>& nums) {
  std::vector<int> negatives;
  std::copy_if(nums.begin(), nums.end(), std::back_inserter(negatives),
                { return num < 0; });
  return negatives;
}

std::string StringCalculator::buildNegativeNumbersMessage(
    const std::vector<int>& negatives) {
  std::string message = "negatives not allowed: ";
  for (std::size_t i = 0; i < negatives.size(); ++i) {
    if (i > 0) message += ",";
    message += std::to_string(negatives[i]);
  }
  return message;
}
