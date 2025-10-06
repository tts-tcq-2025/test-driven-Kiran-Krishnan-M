#include "StringCalculator.h"

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator> // for std::back_inserter

// Utility: replace all occurrences of 'delimiter' in 'text' with ','
static std::string replace_all(std::string text, const std::string& delimiter, const std::string& with) {
    if (delimiter.empty()) return text; // defensive
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
    std::string numbersPart = extractNumbers(numbers);
    std::string normalizedNumbers = normalizeDelimiters(numbersPart, delimiter);
    std::vector<int> nums = parseNumbers(normalizedNumbers);

    validateNumbers(nums);

    return calculateSum(nums);
}

std::string StringCalculator::parseDelimiter(const std::string& numbers) {
    if (!hasCustomDelimiterFormat(numbers)) {
        return ",";
    }

    std::size_t newlinePos = numbers.find('\n');
    if (newlinePos == std::string::npos) {
        // Malformed header: treat as default
        return ",";
    }

    std::string header = numbers.substr(2, newlinePos - 2); // after "//" up to '\n'
    return extractDelimiterFromHeader(header); // helper below
}

bool StringCalculator::hasCustomDelimiterFormat(const std::string& numbers) {
    // CCN <= 3: one combined condition
    return numbers.length() >= 4 && numbers.rfind("//", 0) == 0;
}

std::string StringCalculator::extractDelimiterFromHeader(const std::string& delimiterPart) {
    if (hasBracketFormat(delimiterPart)) {
        // Strip leading '[' and trailing ']'
        return delimiterPart.substr(1, delimiterPart.length() - 2);
    }
    return delimiterPart; // single-character header like "//;\n"
}

bool StringCalculator::hasBracketFormat(const std::string& delimiterPart) {
    // e.g. "[***]"
    return delimiterPart.length() >= 2
        && delimiterPart.front() == '['
        && delimiterPart.back()  == ']';
}

std::string StringCalculator::extractNumbers(const std::string& numbers) {
    if (!hasCustomDelimiterFormat(numbers)) {
        return numbers;
    }
    return extractNumbersAfterDelimiterHeader(numbers);
}

std::string StringCalculator::extractNumbersAfterDelimiterHeader(const std::string& numbers) {
    std::size_t newlinePos = numbers.find('\n');
    if (newlinePos == std::string::npos || newlinePos + 1 >= numbers.length()) {
        // If malformed, return original (we’ll try to parse; tests don’t require rejecting)
        return numbers;
    }
    return numbers.substr(newlinePos + 1);
}

std::string StringCalculator::normalizeDelimiters(const std::string& numbersPart,
                                                  const std::string& delimiter) {
    std::string result = numbersPart;

    // Replace custom delimiter (any length) with comma
    if (delimiter != ",") {
        result = replaceDelimiterWithComma(result, delimiter);
    }

    // Treat newlines as delimiters per spec
    std::replace(result.begin(), result.end(), '\n', ',');

    return result;
}

std::string StringCalculator::replaceDelimiterWithComma(const std::string& text,
                                                        const std::string& delimiter) {
    // Non-regex, robust for any delimiter content.
    return replace_all(text, delimiter, ",");
}

std::vector<int> StringCalculator::parseNumbers(const std::string& normalizedNumbers) {
    std::vector<int> nums;
    std::stringstream ss(normalizedNumbers);
    std::string token;

    while (std::getline(ss, token, ',')) {
        if (!token.empty()) {
            nums.push_back(std::stoi(token));
        }
    }
    return nums;
}

int StringCalculator::calculateSum(const std::vector<int>& nums) {
    return std::accumulate(nums.begin(), nums.end(), 0,  {
        return (n <= 1000) ? (sum + n) : sum;
    });
}

void StringCalculator::validateNumbers(const std::vector<int>& nums) {
    std::vector<int> negatives = findNegativeNumbers(nums);
    if (!negatives.empty()) {
        std::string message = buildNegativeNumbersMessage(negatives);
        throw std::invalid_argument(message);
    }
}

std::vector<int> StringCalculator::findNegativeNumbers(const std::vector<int>& nums) {
    std::vector<int> negatives;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(negatives),
                  { return num < 0; });
    return negatives;
}

std::string StringCalculator::buildNegativeNumbersMessage(const std::vector<int>& negatives) {
    std::string message = "negatives not allowed: ";
    for (std::size_t i = 0; i < negatives.size(); ++i) {
        if (i > 0) message += ",";
        message += std::to_string(negatives[i]);
    }
    return message;
}
