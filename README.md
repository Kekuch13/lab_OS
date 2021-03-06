# Лабораторные работы по дисциплине "Операционные системы"

## [Лаба 1](./lab1): Создание и уничтожение потоков
Программа, содержащая два потока (в дополнение к основному потоку). 
Каждый из потоков выводит определенное число на экран.

## [Лаба 2](./lab2): Синхронизация потоков с помощью мьютексов
Программа, содержащая два потока, осуществляющих координированный доступ к разделяемому ресурсу. 
В качестве разделяемого ресурса в данной работе используется экран.

Потоки выводят символы в определенном порядке, например:<br/>
111112222211111222221111122222.

Реализация трех экземпляров программы:
1. С блокирующей операцией захвата мьютекса `pthread_mutex_lock()`;
2. С операцией проверки захвата мьютекса без блокировки `pthread_mutex_trylock()`;
3. С блокировкой на время операции захвата мьютекса `pthread_mutex_timedlock()`.

## [Лаба 3](./lab3): Взаимодействие потоков через неименованные каналы
Программа, содержащая два потока,
один из которых пишет данные в неименованный канал (pipe),
а другой читает данные из канала.

Реализация трех экземпляров программы:
1. Использование `read()`/`write()` с блокировкой, т.е.
при чтении из пустого канала, или при записи в полный канал 
потоки чтения и записи блокируются 
и программа не завершается при нажатии клавиши `Enter`
2. Блокировка снимается с использованием `pipe2()`, 
программа может завершиться при нажатии `Enter`.
3. Блокировка снимается с использованием `fcntl()`,
программа может завершиться при нажатии `Enter`.

## [Лаба 4](./lab4): Создание и уничтожение процессов
Две программы:
1. Программа 1 *(дочерний процесс)*:<br/>
   * При запуске принимает аргументы командной строки, 
а затем в цикле выводит каждый аргумент на экран с задержкой в одну секунду.
   * Должна выводить на экран свой идентификатор и идентификатор процесса-родителя.
   * Должна формировать код завершения.
2. Программу 2 *(родительский процесс)*:<br/>
   * Запускает программу 1 в качестве дочернего процесса с помощью вызовов `fork()` и `execv()`.
   * Должна вывести на экран идентификатор процесса-родителя, свой идентификатор и идентификатор дочернего процесса.
   * Должна сформировать набор параметров для передачи в дочерний процесс аргументов командной строки.
   * Должна ожидать завершения дочернего процесса, проверяя событие завершения каждую половину секунды, а по завершению дочернего процесса вывести на экран код завершения.

## [Лаба 5](./lab5): Cинхронизация процессов с помощью именованных семафоров 
Обе программы используют общий ресурс – файл и осуществляют запись данных в него.

Каждая программа запускается в отдельном терминале. 
То есть экран в данном случае не является общим ресурсом. 
Общим ресурсом будет только файл.

Для наблюдения за работой программ в каждой программе сделан вывод символа не только в файл, 
но и на экран своего терминала.

Обе программы должны создать (или открыть, если создан) один и тот же именованный семафор, 
должны создать (или открыть, если создан) один и тот же файл, но записывать в файл разные символы.

В случае использования общего именованного семафора процессы выводят символы в файл 
в определенном порядке, например:<br/>
111112222211111222221111122222.

## [Лаба 6](./lab6): Взаимодействие процессов через разделяемую память
Комплект из двух программ, одна из которых посылает данные в разделяемую память, 
а вторая – читает эти данные.

Поскольку механизм разделяемой памяти не содержит средств синхронизации записи и чтения, 
для синхронизации используется механизм именованных семафоров.

Для получения сегмента разделяемой памяти используется стандарт SVID.

## [Лаба 7](./lab7): Взаимодействие процессов через именованные каналы
Написать комплект из двух программ, одна из которых записывает данные в именованный канал, 
а вторая – считывает эти данные. 

## [Лаба 8](./lab8): Сетевое взаимодействие процессов через сокеты
Написать комплект из двух программ, одна из которых выполняет функции сервера, а вторая выполняет функции клиента.

Клиент, после установления соединения с сервером, посылает серверу запросы с периодом 1 сек.
Сервер принимает запросы, обрабатывает их и отправляет ответы клиенту. Под обработкой будем понимать выполнение функции `stat()`
для получения ID устройства содержащего файл с программой-сервером.
Клиент принимает ответы и выводит их на экран.

## [Лаба 9](./lab9): Управление доступом с «posix-возможностями»
Написать программу 1, в которой выполняется функция `chmod()`, требующая привилегий. Написать программу 2 просмотра и установки «возможностей».

При выполнении программы 1 с правами непривилегированного пользователя функция возвращает ошибку, связанную с отсутствием прав на её выполнение.
Программа 2 просматривает «возможности» программы 1 с помощью функции `cap_get_file()` и устанавливает «возможности» программы 2 с помощью функции `cap_set_file()`.
Программа просмотра и установки «возможностей» выполняется только с правами администратора или с «возможностью» `CAP_SETFCAP`.

## [Лаба 10](./lab10): Событийное программирование на уровне ОС
В [лабораторной работе 4](./lab4) организовать цикл ожидания события `ev_child`(завершение дочернего процесса). 
В обработчике события вывести код завершения дочернего процесса.
