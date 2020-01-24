// Implementation file for the LongInt class. 

#include "longint.h"

// Helper to convert char to int
int convertCharToInt(char c) { 
    return c - '0';
}

// Helper to convert int to char
char convertIntToChar(int i) { 
    return (char)('0' + i);
}

// Overloaded >>: reads characters '0' through '9'from keyboard input
// Skips all over characters
// Takes an istream object and the LongInt input as parameters
istream& operator>>(istream& in, LongInt& rhs) {
    string input;
    in >> input;
    rhs.initializeFromString(input, rhs);
    return in;
}

// Overloaded <<: prints a LongInt with a negative sign if appropriate
// Takes an ostream object and the LongInt to be printed as parameters
// If the LongInt is empty, prints out 0
// If the LongInt includes characters outside other than '0' through '9'
// and '-', they are omitted
// Leading 0's are omitted as well
ostream& operator<<(ostream& out, const LongInt& rhs) {
    if (rhs.digits.isEmpty()) {
        out << "0";
    }

    if (rhs.negative) {
        out << "-";
    }

    LongInt rhsCopyInt = LongInt(rhs);
    //Deque<char> rhsCopy = Deque<char>(rhsCopyInt.digits);
    while (!rhsCopyInt.digits.isEmpty()) {
        char c = rhsCopyInt.digits.removeFront();
        out << c;
    }
    
    return out;
}

// Constructor
// Takes a very long integer in the form of a string as parameter, 
// copies it into current LongInt
LongInt::LongInt(const string str) {
    negative = false;
    initializeFromString(str, *this);
}

// Constructor
// Takes a LongInt rhs as parameter, copies it into current LongInt
LongInt::LongInt(const LongInt& rhs) {
    negative = false;
    *this = rhs;
}

// Default constructor: 
// Creates a LongInt with empty digits queue and false negative
LongInt::LongInt() {    
    negative = false;
}

// Destructor
// Deletes the current object
// Digits does not require destruction as it does not exist outside of this object (not a pointer)
LongInt::~LongInt() {}

// Overloaded operator+
// Takes a LongInt rhs as parameter
// Adds rhs to the current object, and returns a new LongInt object with this value
LongInt LongInt::operator+(const LongInt& rhs) const {
    LongInt lhsCopy = LongInt(*this);
    LongInt rhsCopy = LongInt(rhs);
    
    // If exactly one LongInt is negative, then use subtraction
    bool requiresSubtraction = false;
    if (!lhsCopy.negative && rhsCopy.negative) {
        rhsCopy.negative = false;
        requiresSubtraction = true;
    } else if (lhsCopy.negative && !rhsCopy.negative) {
        lhsCopy.negative = false;
        requiresSubtraction = true;
    }

    // If requires subtraction, subtract smaller number from bigger number
    if (requiresSubtraction) {
        LongInt result;
        // If rhs is greater than lhs, its sign is dominant, and vice versa
        if (lhsCopy < rhsCopy) {
            result = rhsCopy - lhsCopy;
            result.negative = rhs.negative; // Retain the sign of the dominant term
        } else {
            result = lhsCopy - rhsCopy;
            result.negative = this->negative;
        }

        return result;
    }

    // Only if both positive or both negative
    LongInt result;
    result.negative = negative && rhs.negative;
    int carry = 0;
    while (!rhsCopy.digits.isEmpty() || !lhsCopy.digits.isEmpty()) {
        int lhsInt = 0;
        int rhsInt = 0;    
        
        // If not empty, read least significant digit
        if (!rhsCopy.digits.isEmpty()) {
            char rhsChar = rhsCopy.digits.removeBack();
            rhsInt = convertCharToInt(rhsChar);
        }
        
        if (!lhsCopy.digits.isEmpty()) {
            char lhsChar = lhsCopy.digits.removeBack();
            lhsInt = convertCharToInt(lhsChar);
        }

        int sum = (lhsInt + rhsInt + carry) % 10;
        carry = (lhsInt + rhsInt + carry) / 10;
        result.digits.addFront(convertIntToChar(sum));
    }

    // If we have a left-over carry after we're done summing each digit,
    // we have to add it to the sum
    if (carry > 0) {
        result.digits.addFront(convertIntToChar(carry));
    }

    result.remove0s();
    return result;
}

// Overloaded operator-
// Takes a LongInt rhs as parameter
// Subtracts rhs from the current object, and returns a new LongInt object with this value
LongInt LongInt::operator-(const LongInt& rhs) const {
    LongInt lhsCopy = LongInt(*this);
    LongInt rhsCopy = LongInt(rhs);
    
    // If exactly one LongInt is negative, then use addition
    if (!lhsCopy.negative && rhsCopy.negative) {
        rhsCopy.negative = false; 
        return lhsCopy + rhsCopy;
    } else if (lhsCopy.negative && !rhsCopy.negative) {
        rhsCopy.negative = true; // Set both to negative, add
        return rhsCopy + lhsCopy;
    }

    LongInt result;
    result.negative = negative && rhs.negative;
    int borrow = 0;
    while (!rhsCopy.digits.isEmpty() || !lhsCopy.digits.isEmpty()) {
        int lhsInt = 0;
        int rhsInt = 0;    
        
        // If not empty, read least significant digit of both
        if (!rhsCopy.digits.isEmpty()) {
            char rhsChar = rhsCopy.digits.removeBack();
            rhsInt = convertCharToInt(rhsChar);
        }
        
        if (!lhsCopy.digits.isEmpty()) {
            char lhsChar = lhsCopy.digits.removeBack();
            lhsInt = convertCharToInt(lhsChar);
        }

        int difference = lhsInt - rhsInt - borrow;

        // If only one digit left and difference is negative
        if (rhsCopy.digits.isEmpty() && lhsCopy.digits.isEmpty() && difference < 0) {
            result.negative = !result.negative; // Nothing left to borrow, so go into negatives
            difference *= -1; // Make sure difference is always positive
        } else if (difference < 0) {
            borrow = 1;
            difference += 10;
        } else {
            borrow = 0;
        }
        
        result.digits.addFront(convertIntToChar(difference));
    }

    result.remove0s();
    return result;
}

// Overloaded operator=
// Takes a LongInt rhs as parameter
// Sets the current object equal to the parameter, and returns the current object
const LongInt& LongInt::operator=(const LongInt& rhs) {
    clear(*this);
    negative = rhs.negative;
    digits = rhs.digits;

    this->remove0s();
    return *this;
}

// Overloaded operator<
// Returns true if the current object is less than the parameter, false otherwise
bool LongInt::operator<(const LongInt& rhs) const {
    return rhs > *this;
}

// Overloaded operator<=
// Returns true if the current object is less or equal to the parameter, false otherwise
bool LongInt::operator<=(const LongInt& rhs) const {
    return !(*this > rhs);
}

// Overloaded operator>
// Returns true if the current object is greater than the parameter, false otherwise
bool LongInt::operator>(const LongInt& rhs) const {
    // If different signs, return true if current is the positive one
    if (this->negative != rhs.negative) {
        return rhs.negative;
    }

    // If different sizes, return true if we have larger size
    if (this->digits.size() != rhs.digits.size()) {
        return this->digits.size() > rhs.digits.size();
    }

    // Compare digit by digit starting with the most significant
    LongInt lhsCopy = LongInt(*this);
    LongInt rhsCopy = LongInt(rhs);
    while (!lhsCopy.digits.isEmpty() || !rhsCopy.digits.isEmpty()) {
        char mostSignificantLhsDigit = lhsCopy.digits.removeFront();
        char mostSignificantRhsDigit = rhsCopy.digits.removeFront();

        if (mostSignificantLhsDigit != mostSignificantRhsDigit) {
            return mostSignificantLhsDigit > mostSignificantRhsDigit;
        }
    }

    return false;
}

// Overloaded operator>=
// Returns true if the current object is greater than or equal to the parameter, false otherwise
bool LongInt::operator>=(const LongInt& rhs) const {
    return !(*this < rhs);
}

// Overloaded operator==
// Returns true if the current object is equal to the parameter, false otherwise
bool LongInt::operator==(const LongInt& rhs) const {
    LongInt lhsCopy = LongInt(*this);
    LongInt rhsCopy = LongInt(rhs);
    if (lhsCopy.negative != rhsCopy.negative) {
        return false;
    }

    while (!rhsCopy.digits.isEmpty() || !lhsCopy.digits.isEmpty()) {
        if (rhsCopy.digits.isEmpty()) {
            return false;
        }

        if (lhsCopy.digits.isEmpty()) {
            return false;
        }

        // So far, same size and all digits have been equal
        char rhsDigit = rhsCopy.digits.removeBack();
        char lhsDigit = lhsCopy.digits.removeBack();
        if (rhsDigit != lhsDigit) {
            return false;
        }
    }

    return true;
  }

// Overloaded operator!=
// Returns true if the current object is not equal to the parameter, false otherwise
bool LongInt::operator!=(const LongInt& rhs) const {
    return !(*this == rhs);
}

// Removes leading zeros from the current LongInt
void LongInt::remove0s() {
    while(!digits.isEmpty() && digits.getFront() == '0') {
        digits.removeFront();
    }

    if(digits.isEmpty()) {
        negative = false;
    }
}

// Takes a very long integer in the form of a string, and a LongInt object as parameters
// Copies the integer into the LongInt object
void LongInt::initializeFromString(string input, LongInt& rhs) {
    clear(rhs);
    for (int i = 0; i < input.length(); i++) {
        char charAtI = input[i];
        bool isNumber = '0' <= charAtI && charAtI <= '9';

        if (i == 0) {
            rhs.negative = (charAtI == '-');
        }

        if (isNumber) {
            rhs.digits.addBack(charAtI);
        }
    }

    rhs.remove0s();
}

// Takes a LongInt object as parameter
// Clears its digits
void LongInt::clear(LongInt& rhs) {
    while (!rhs.digits.isEmpty()) {
        rhs.digits.removeFront();
    }
}
