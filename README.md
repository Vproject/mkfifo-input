mkfifo-input
============

Creates a named pipe on win32 and forwards stdin

Based on UnZipPipe from http://jcarlossaez1.wordpress.com/2005/10/24/ii-on-the-fly-compression-is-easy-on-unix-but-it-is-also-easy-on-windows/

##usage
... | mkfifo-input out_pipe_name

Reads stdin and writes it to a pipe. Msys tools seem to have trouble open the pipe. Other mingw compiled tools don't seem to suffer from that. Note: backslashes may require escaping; \\\\.\\pipe\\ vs \\\\\\\\.\\\\pipe\\\\

###Example
```
$ ffmpeg.exe -i example.y4m -f yuv4mpegpipe -v -1 - | ./mkfifo-input.exe your_pipename
```
####output
```
Reading from stdin and writing to pipe \\\\.\\pipe\\your_pipename
```

####start another process in a different shell that uses the pipe
```
$ ffmpeg.exe -i \\\\\\\\.\\\\pipe\\\\your_pipename exampleout.y4m
```
