## SREC Parser
this repo contains source code for srec-parser library <br>

The code snippet is a C program that defines functions and structures for parsing and extracting data from SREC files. It includes a function for initializing the SREC parser and a function for parsing the SREC file and extracting the relevant data.

### **Inputs** 

`__fpath`: A pointer to the file path of the SREC file to parse.<br>
`file_name`: The name of the SREC file to parse. 
### **Flow**
The init_parser function initializes the SREC parser by allocating memory for the `srec_t` structure, opening the SREC file, and setting the initial values for the structure members.<br>
The `parse_srec_fn` function opens the SREC file, reads each line, and extracts the relevant data from the SREC records. It stores the extracted data in an array and prints the data to the standard output. <br>

### **Outputs**
`srec_t*`: A pointer to the initialized srec_t structure.
`int`: Returns `SREC_PARSE_SUCCESS` if the parsing is successful, otherwise returns `SREC_PARSE_FAILED`.


### **Usage example**

include `srec.h` in your code and pass the `file_path` as an argument

```cpp
#include <stdio.h>
#include "lib/srec.h"

int main() {
    const char* file_name = "example.srec";
    int result = parse_srec_fn(file_name);
    if (result == SREC_PARSE_SUCCESS) {
        printf("SREC parsing successful.\n");
    } else {
        printf("SREC parsing failed.\n");
    }
    return 0;
}
```


### **example**:
`main.cpp` contains example code. <br>
If you are using ***Linux*** or ***MacOS***, run `'make run'` command from root dir of repo to see the output. <br><br>
for windows, run `'gcc main.cpp -o srec.o && ./srec.o && rm srec.o'` command. <br><br>
*Note*: logs are off by-default, to enable logs define `ENABLE_LOGS` in the example code


### **Credits**:
**Inintial Development**: Tushar Gaikwad <br>
**Maintainer**: Tushar Gaikwad

