#!/bin/bash

# Проверка наличия аргумента (пути к каталогу)
if [ "$#" -ne 1 ]; then
  echo "Формат вызова: wordcount <путь>"
  exit 1
fi

# Путь к целевому каталогу
directory="$1"

# Проверка существования каталога
if [ ! -d "$directory" ]; then
  echo "Указанный каталог не существует."
  exit 1
fi

# Переменная, чтобы отслеживать наличие файлов *.txt
found_txt_files=false

# Поиск текстовых файлов и подсчет слов
for file in "$directory"/*.txt; do
  if [ -f "$file" ]; then
    # Имя файла
    filename=$(basename "$file")
    # Подсчет количества слов в файле (только буквенных слов)
    word_count=$(cat "$file" | tr -s '[:space:]' '\n' | grep -o -E '\w+' | wc -l)
    # Вывод результата (имя файла и количество слов)
    echo "$filename - $word_count"
    found_txt_files=true
  fi
done

# Проверка, были ли найдены файлы *.txt
if [ "$found_txt_files" = false ]; then
  echo "В указанной директории нет файлов с расширением *.txt."
fi
