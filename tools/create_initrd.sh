#!/bin/bash

# Определяем абсолютный путь к корневой директории проекта
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR" || { echo "Error: Cannot cd to $ROOT_DIR"; exit 1; }

echo "Current directory: $(pwd)"
echo "Creating binary initrd in: $ROOT_DIR"

# Создаем простой бинарный образ initrd с заголовком
echo "Creating binary module image..."

# Заголовок: магическое число и количество модулей
printf "SVTM" > initrd.img  # Магическое число

# Подсчитываем количество доступных модулей
MODULE_COUNT=0
MODULE_LIST=""

if [ -f "modules/ata.kmod" ]; then
    MODULE_LIST="$MODULE_LIST modules/ata.kmod"
    MODULE_COUNT=$((MODULE_COUNT + 1))
fi

if [ -f "modules/initrd_fs.kmod" ]; then
    MODULE_LIST="$MODULE_LIST modules/initrd_fs.kmod"
    MODULE_COUNT=$((MODULE_COUNT + 1))
fi

if [ -f "modules/test.kmod" ]; then
    MODULE_LIST="$MODULE_LIST modules/test.kmod"
    MODULE_COUNT=$((MODULE_COUNT + 1))
fi

# Записываем количество модулей (4 байта, little-endian)
printf "$(printf '\\x%02x\\x%02x\\x%02x\\x%02x' $((MODULE_COUNT & 0xFF)) $((MODULE_COUNT >> 8 & 0xFF)) $((MODULE_COUNT >> 16 & 0xFF)) $((MODULE_COUNT >> 24 & 0xFF)))" >> initrd.img

# Добавляем модули с информацией о размере
for module in $MODULE_LIST; do
    # Получаем размер модуля
    size=$(stat -c%s "$module")
    # Записываем размер (4 байта, little-endian)
    printf "$(printf '\\x%02x\\x%02x\\x%02x\\x%02x' $((size & 0xFF)) $((size >> 8 & 0xFF)) $((size >> 16 & 0xFF)) $((size >> 24 & 0xFF)))" >> initrd.img
    # Добавляем сам модуль
    cat "$module" >> initrd.img
    echo "Added module: $(basename $module) (size: $size bytes)"
done

echo "Binary initrd created successfully!"
echo "Initrd size: $(du -h initrd.img | cut -f1)"
echo "Modules included: $MODULE_COUNT"
echo "Done!"
