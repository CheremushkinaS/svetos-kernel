#!/bin/bash
echo "=== Simple Makefile check ==="

# Проверяем первые байты файла
echo "First 10 bytes in hex:"
head -c 10 Makefile | hexdump -C

echo ""
echo "File size:"
wc -c Makefile

echo ""
echo "First 5 lines:"
head -5 Makefile

echo ""
echo "Testing make syntax:"
if make -n all >/dev/null 2>&1; then
    echo "✓ Makefile syntax is correct"
else
    echo "✗ Makefile syntax error"
    make -n all
fi
