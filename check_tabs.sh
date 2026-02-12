#!/bin/bash
echo "=== Checking for tabs in commands ==="
# Ищем строки с командами (не пустые, не комментарии, не переменные)
grep -n "^[^#]" Makefile | grep -v "^[a-zA-Z_]" | grep -v "^$" | head -10

echo ""
echo "=== First 20 lines with visible whitespace ==="
cat -A Makefile | head -20
