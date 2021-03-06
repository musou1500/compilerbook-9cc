#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -c tmp.s
  gcc rt/lib.o tmp.o -o tmp
  ./tmp
  actual="$?"

  if [ "$actual" != "$expected" ]; then
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 0
try 42 42
try 21 '5+20-4'
try 41 ' 12 + 34 - 5 '
try 47 "5+6*7"
try 5 "3+6/3"
try 15 "5*(9-6)"
try 4 "(3+5)/2"
try 2 "a = b = 2;"
try 4 "a = b = 2; a + b;"
try 1 "2 == 2"
try 0 "3 == 2"
try 1 "2 * 2 == 4"
try 1 "2 + 3 == 5"
try 1 "2 != 5"
try 5 "add_int(2, 3)"
try 25 "5 * add_int(2, 3)"
try 8 "{ a = 2; { b = 4; a * b; } }"
try 1 "1 < 2"
try 1 "1 <= 2"
try 1 "2 > 1"
try 1 "3 >= 2"
try 1 "1 && 1"
try 0 "1 && 0"
try 1 "0 || 1"
try 1 "1 || 0 && 1"
try 1 "if(1) { 1; } else { 2; }"
try 2 "if(0) { 1; } else { 2; }"
try 2 "if(0) { 1; } else if(1) { 2; } else { 3; }"
try 3 "if(0) { 1; } else if(0) { 2; } else { 3; }"
try 55 "i = 0; sum = 0; while(i < 10) { i = i + 1; sum = sum + i; }"
try 2 "i = 0; while(i < 10) { if (i == 2) { return i; } i = i + 1; }"
echo OK
