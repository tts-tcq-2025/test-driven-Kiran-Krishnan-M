#pragma once
#include <string>
#include <vector>

class StringCalculator {
public:
    int Add(const std::string& numbers);

private:
    // Keep functions tiny to satisfy CCN ≤ 3
    static bool hasCustomDelimiterFormat(const std::string& numbers);
    static bool hasBracketFormat(const std::string& delimiterPart);

    static std::string parseDelimiter(const std::string& numbers);
    static std::string extractNumbers(const std::string& numbers);
    static std::string extractNumbersAfterDelimiterHeader(const std::string& numbers);

    static std::string normalizeDelimiters(const std::string& numbersPart,
                                           const std::string& delimiter);
    static std::string replaceDelimiterWithComma(const std::string& text,
                                                 const std::string& delimiter);

    static std::vector<int> parseNumbers(const std::string& normalizedNumbers);
    static void validateNumbers(const std::vector<int>& nums);
    static std::vector<int> findNegativeNumbers(const std::vector<int>& nums);
    static std::string buildNegativeNumbersMessage(const std::vector<int>& negatives);
    static int calculateSum(const std::vector<int>& nums);
};
