# Результаты работы программы main.c при вызове разных сигналов
### SIGINT 
Сигнал прерывания (Ctrl-C) с терминала
*(Завершение)*
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 7973
Program launched with PID: 7973
kill -s SIGKILL 7973
[1]+  Interupt                 ./main
```
После вызова данного сигнала программа прекратила своё выполнение и в файл перестал писаться текст.

### SIGQUIT 
Сигнал «Quit» с терминала (Ctrl-\\) 
*(Завершение с дампом памяти)*

```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 8557
Program launched with PID: 8557
kill -s SIGQUIT 8557
[1]+  Quit                    (core dumped) ./main
```
После вызова данного сигнала программа прекратила своё выполнение и в файл перестал писаться текст. Но помимо этого в программа при своем заверешении сохранила дамп памяти в **/var/lib/apport/coredump**

```Shell
zeuslinux@zeuslinux:/var/lib/apport/coredump$ ls -l
total 432
-r-------- 1 zeuslinux root 442368 апр 16 01:42 core._home_zeuslinux_Documents_ELTEX_Module3_lection2_2_5_main.1000.8776f92e-6a23-47cc-bd97-34ccb8b4a584.8557.477015
```

### SIGABRT
Сигнал, посылаемый функцией abort()
*(Завершение с дампом памяти)*

```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 9568
Program launched with PID: 9568
kill -s SIGABRT 9568
[1]+  Aborted                 (core dumped) ./main
```
После вызова данного сигнала программа прекратила своё выполнение и в файл перестал писаться текст. Но помимо этого в программа при своем заверешении сохранила дамп памяти в **/var/lib/apport/coredump**
```Shell
zeuslinux@zeuslinux:/var/lib/apport/coredump$ ls -l
total 432
-r-------- 1 zeuslinux root 442368 апр 16 01:58 core._home_zeuslinux_Documents_ELTEX_Module3_lection2_2_5_main.1000.8776f92e-6a23-47cc-bd97-34ccb8b4a584.9568.574515
```
### SIGKILL
Безусловное завершение
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 9786
Program launched with PID: 9786
kill -s SIGKILL 9786
[1]+  Killed                  ./main
```
После вызова данного сигнала программа прекратила своё выполнение и в файл перестал писаться текст.

### SIGTERM
Сигнал завершения (сигнал по умолчанию для утилиты kill)
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 9852
Program launched with PID: 9852
kill -s SIGTERM 9852
[1]+  Terminated              ./main
```

### SIGTSTP
Сигнал остановки с терминала (Ctrl-Z)
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[1] 9883
Program launched with PID: 9883
kill -s SIGTSTP 9883
[1]+  Stopped                 ./main
```
После вызова данного сигнала программа приостановила своё выполнение и в файл перестал писаться текст. 


### SIGSTOP
Остановка выполнения процесса
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ ./main &
[2] 10030
Program launched with PID: 10030
kill -s SIGSTOP 10030
[2]+  Stopped                 ./main
```

### SIGCONT
Продолжить выполнение ранее остановленного процесса
```Shell
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ kill -s SIGCONT 9883
zeuslinux@zeuslinux:~/Documents/ELTEX/Module3/lection2/2.5$ kill -s SIGCONT 10030
```
После вызова данного сигналы процессы из предыдущих заданий продолжают свое выполнение
