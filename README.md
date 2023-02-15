# Windowing Function - How to use it

## Hash Table

### C

Run the file called *main.c*,  automatically the windowing function will be called with already set parameters. <br> 
If you want to change those parameters you just have to manipulate that function by setting the windowing function in the way you prefer.

### Python
First of all you have to create a file python in the same place where the *.c are located. <br>
Then you have to write in the terminal: `gcc -o library.so -shared -fPIC Windowing.c Buffer.c` <br>
That line will create a windowing library called *library.so* that we can call from the python file.
Here an exemple of how to call it: <br>
```
import ctypes
clibrary = ctypes.CDLL(library_path)
string = "string"
a_string = string.encode('utf-8')
for i in range(1,50):
    clibrary.windowing(ctypes.create_string_buffer(a_string),i)
```
It's important to set the python machine with the same bit as the C one, so if you used Win32 to debug the C code, you should use python with 32 bit.
## List
### C

Run the file called *main.c*,  automatically the windowing function will be called with already set parameters. <br> 
If you want to change those parameters you just have to manipulate that function by setting the windowing function in the way you prefer, but you have to remember that this version works only with integer value.

### Python

First of all you have to create a file python in the same place where the *.c are located. <br>
Then you have to write in the terminal: `gcc -o library.so -shared -fPIC Windowing.c Buffer.c` <br>
That line will create a windowing library called *library.so* that we can call from the python file.
Here an exemple of how to call it: <br>
```
import ctypes
clibrary = ctypes.CDLL(library_path)
for i in range(1,50):
    clibrary.windowing(i,i)
```
It's important to set the python machine with the same bit as the C one, so if you used Win32 to debug the C code, you should use python with 32 bit.
