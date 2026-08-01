#!/usr/bin/env bash

order=(
    "comments.md"
    "variablesAndConstants.md"
    "datatypes.md"
    "type conversion.md"
    "type function.md"
    "io.md"
    "strings.md"
    "list.md"
    "operations.md"
    "conditional statements.md"
    "loops.md"
    "functions.md"
    "len.md"
    "imports.md"
    "math.md"
    "tester.md"
    "repl.md"
)

for f in "${order[@]}"; do
    cat "$f"
    echo
done > "complete guide.md"

