// Header file for the LongInt class.
// A very long integer is a data type whose range is greater than that of the standard integer.
// It can only include digits, and the character "-" if the value is negative.
// The first digit corresponds to the most significant digit.
// The last digit corresponds to the least significant digit.

#ifndef LONGINT_H
#define LONGINT_H
#include <iostream>
#include "deque.h"

using namespace std;

class LongInt {
  // Overloaded <<: prints a LongInt
  friend istream &operator>>( istream &in, LongInt &rhs );
  // Overloaded <<: takes a LongInt from user input
  friend ostream &operator<<( ostream &out, const LongInt &rhs );

 public:
  // Constructors
  LongInt( const string str ); // Constructor that copies given string str
  LongInt( const LongInt &rhs ); // Constructor that copies given LongInt rhs
  LongInt( ); // Default constructor

  // Destructor
  ~LongInt( ); //

  // Arithmetic binary operators
  LongInt operator+( const LongInt &rhs ) const; // Adds two LongInt objects
  LongInt operator-( const LongInt &rhs ) const; // Subtracts two LongInt objects

  // Assignment operators
  const LongInt &operator=( const LongInt &rhs ); // Sets current object equal to parameter

  // Logical binary operators
  bool operator< ( const LongInt &rhs ) const; // Is object < parameter?
  bool operator<=( const LongInt &rhs ) const; // Is object <= parameter?
  bool operator> ( const LongInt &rhs ) const; // Is object > parameter?
  bool operator>=( const LongInt &rhs ) const; // Is object >= parameter?
  bool operator==( const LongInt &rhs ) const; // Is object == parameter?
  bool operator!=( const LongInt &rhs ) const; // Is object != parameter?

 private:
  Deque<char> digits; // The digits of the LongInt
  bool negative;  // LongInt can be positive or negative
  void remove0s( ); // Removes leading 0s
  void initializeFromString(string input, LongInt &rhs); // Initializes a LongInt from
                                                         // the given string and LongInt object
  void clear(LongInt &rhs); // Clears digits
};

#endif
