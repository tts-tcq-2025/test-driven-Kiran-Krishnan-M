#include <gtest/gtest.h>
#include "StringCalculator.h"

TEST(StringCalculatorTDD, EmptyStringReturnsZero) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add(""), 0);
}

TEST(StringCalculatorTDD, SingleNumberReturnsValue) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("7"), 7);
    EXPECT_EQ(calc.Add("42"), 42);
}

TEST(StringCalculatorTDD, TwoNumbersComma) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("1,2"), 3);
}

TEST(StringCalculatorTDD, UnknownAmountOfNumbers) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("1,2,3,4,5"), 15);
}

TEST(StringCalculatorTDD, NewlinesAsDelimiters) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("1\n2,3"), 6);
}

TEST(StringCalculatorTDD, CustomSingleCharDelimiter) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("//;\n1;2"), 3);
}

TEST(StringCalculatorTDD, CustomAnyLengthDelimiter) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("//[***]\n1***2***3"), 6);
}

TEST(StringCalculatorTDD, ThrowsOnSingleNegative) {
    StringCalculator calc;
    try {
        (void)calc.Add("1,-2,3");
        FAIL() << "Expected exception for negative numbers";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "negatives not allowed: -2");
    }
}

TEST(StringCalculatorTDD, ThrowsOnMultipleNegativesListsAll) {
    StringCalculator calc;
    try {
        (void)calc.Add("-1,-2,3,-4");
        FAIL() << "Expected exception for negatives";
    } catch (const std::invalid_argument& ex) {
        EXPECT_STREQ(ex.what(), "negatives not allowed: -1,-2,-4");
    }
}

TEST(StringCalculatorTDD, IgnoresNumbersGreaterThan1000) {
    StringCalculator calc;
    EXPECT_EQ(calc.Add("2,1001"), 2);
    EXPECT_EQ(calc.Add("1000,2"), 1002);
}

TEST(StringCalculatorTDD, CustomDelimiterWithRegexSpecialChars) {
    StringCalculator calc;
    // Delimiter is ".*+" (would be tricky for regex; our non-regex replace handles it as literal)
    EXPECT_EQ(calc.Add("//[.*+]\n1.*+2.*+3"), 6);
}

// Clarification test from spec (not OK): we don't need to prove invalid, but ensure we don't crash
TEST(StringCalculatorTDD, ClarificationInvalidInputNotRequired) {
    StringCalculator calc;
    EXPECT_NO_THROW(calc.Add("1,\n")); // We don't enforce invalid format rejection per spec
}
