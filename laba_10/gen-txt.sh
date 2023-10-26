#!/bin/bash

# Проверяем, что передано два аргумента
if [ "$#" -ne 2 ]; then
    echo "Использование: $0 <путь_к_каталогу> <количество_файлов>"
    exit 1
fi

# Извлекаем аргументы
directory="$1"
file_count="$2"

# Проверяем, что каталог существует
if [ ! -d "$directory" ]; then
    echo "Каталог '$directory' не существует."
    exit 1
fi

# Переходим в указанный каталог
cd "$directory" || exit 1

# Генерируем и записываем файлы
for ((i = 1; i <= file_count; i++)); do
    # Генерируем случайное количество (от 10 до 20) слов
    word_count=$((RANDOM % 11 + 10))
    
    # Генерируем случайные слова и записываем их в файл
    words=""
    for ((j = 1; j <= word_count; j++)); do
        word=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 5 | head -n 1)
        words="$words $word"
    done

    # Создаем файл и записываем в него сгенерированные слова
    filename="file_$i.txt"
    echo "$words" > "$filename"
    echo "Создан файл $filename с $word_count словами."
done

echo "Готово! Всего создано $file_count файлов в каталоге '$directory'."
