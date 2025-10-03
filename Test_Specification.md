| Test ID | Test Name                     | Input             | Expected Output                                           | Notes                                             |
| ------- | ----------------------------- | ----------------- | --------------------------------------------------------- | ------------------------------------------------- |
| TC-01   | ReturnZeroForEmptyString      | `""`              | `0`                                                       | Empty string returns 0                            |
| TC-02   | ReturnSingleNumber            | `"1"`             | `1`                                                       | Single number                                     |
| TC-03   | SumTwoCommaSeparatedNumbers   | `"1,2"`           | `3`                                                       | Two numbers separated by comma                    |
| TC-04   | SumMultipleNumbers            | `"1,2,3,4"`       | `10`                                                      | Handles unknown number of numbers                 |
| TC-05   | HandleNewLineAsDelimiter      | `"1\n2,3"`        | `6`                                                       | Supports newline as delimiter                     |
| TC-06   | InvalidNewLineFormat          | `"1,\n"`          | Exception                                                 | Invalid format (you said “not required to prove”) |
| TC-07   | CustomSingleCharDelimiter     | `"//;\n1;2"`      | `3`                                                       | Custom delimiter `;`                              |
| TC-08   | NegativeNumberThrowsException | `"1,-2"`          | Exception <br/>Message: `"negatives not allowed: -2"`     | Negative numbers cause exception                  |
| TC-09   | MultipleNegativesException    | `"1,-2,-3"`       | Exception <br/>Message: `"negatives not allowed: -2, -3"` | Multiple negatives listed                         |
| TC-10   | IgnoreNumbersGreaterThan1000  | `"2,1001"`        | `2`                                                       | Numbers > 1000 are ignored                        |
| TC-11   | CustomDelimiterAnyLength      | `"//[]\n12***3"`  | `6`                                                       | Delimiter of arbitrary length                     |
| TC-12   | MultipleCustomDelimiters      | `"//[][%]\n12%3"` | `6`                                                       | Multiple delimiters (if supported)                |
