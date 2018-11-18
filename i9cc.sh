#!/bin/bash
while read -p "> " line
do
  if [ "$line" = "exit" ]; then
    echo "bye"
    break
  fi

  ./9cc "$line" > tmp.s
  gcc -c tmp.s
  gcc rt/lib.o tmp.o -o tmp
  ./tmp
  echo "$?"
done
