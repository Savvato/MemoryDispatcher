# MemoryDispatcher

Это учебный проект по дисциплине "Разработка компонентов операционных систем".

Реализуется очень примитивная симуляция диспетчера памяти, как это устроено в ОС.

Этот проект заключается в написании программы, которая переводит логические адреса в физические для виртуального адресного пространства размера 2^16 = 65,536 байтов. Программа должна читать из файла (`page1.dat`) логические адреса и, используя таблицу страниц, переводить каждый логический адрес в соответствующий ему физический адрес символа файла (`Karamzin_page.txt`) и выводить символ в выходной файл, чтобы получался осмысленный текст. Цель  этого проекта состоит в том, чтобы моделировать шаги, включенные в перевод логических адресов в физические.
