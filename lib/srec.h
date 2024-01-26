
/*
 * MIT License
 * 
 * Copyright (c) 2024 Tushar Gaikwad
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef __SREC_H__
#define __SREC_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


/**
 * Logs a debug message with a formatted string.
 *
 * This function takes a formatted string and variable arguments, and prints the formatted string to the standard output.
 * The formatted string can contain format specifiers, which will be replaced by the corresponding arguments.
 *
 * @param fmt The format string.
 * @param ... The variable arguments.
 * @return None.
 */
void log_debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

#ifdef ENABLE_LOGS
    #define log_debug(...) printf(__VA_ARGS__)
#else 
    #define log_debug(...) 
#endif


/**
 * @brief Defines a structure for SREC data.
 *
 * This structure represents the SREC data and contains the following members:
 * - handle: A pointer to a FILE object.
 * - write_address: A pointer to a uint8_t object.
 * - ADDRESS_SIZE: A uint8_t value representing the size of the address.
 * - checksum: A uint16_t value representing the checksum.
 */
typedef struct srec_struct 
{
    FILE *handle;
    uint8_t *write_address;
    uint8_t ADDRESS_SIZE;
    uint16_t checksum;
}srec_t;

typedef enum {
    SREC_PARSE_SUCCESS = 0,
    SREC_PARSE_FAILED,
}parse_srec_status;


srec_t* init_parser(uint8_t* __fpath);
parse_srec_status parse_srec(uint8_t* rec_type, uint8_t* byte_count, uint8_t* data_address, uint8_t **rec_data, uint8_t* file_name);
parse_srec_status parse_srec_fn(const char *file_name);



/**
 * Initializes the SREC parser.
 *
 * @param __fpath The file path of the SREC file to parse.
 * @return A pointer to the initialized srec_t structure.
 */
srec_t* init_parser(uint8_t* __fpath) 
{
    srec_t* srec = (srec_t*)malloc(sizeof(srec_t));
    if (srec == NULL) 
    {
        return NULL;
    }
    srec->handle = fopen(reinterpret_cast<const char*>(__fpath), "r");
    srec->ADDRESS_SIZE = 0;
    srec->checksum = 0;
    srec->write_address = 0x00;
    return srec;
}


/**
 * Parses an SREC file and extracts the relevant data.
 *
 * @param file_name The name of the SREC file to parse.
 * @return Returns SREC_PARSE_SUCCESS if the parsing is successful, otherwise returns SREC_PARSE_FAILED.
 */
parse_srec_status parse_srec_fn(const char *__fpath) 
{
    FILE *file = NULL;
    char *rec_type, *byte_count, *data_address;
    char **rec_data;
    long f_size = 0;
    int IDX = 0;
    // const char *file_name = "example.srec";

    if((file = fopen(__fpath, "r")) == NULL)
    {
        log_debug("\r\nUNABLE to open a file....");
        return SREC_PARSE_FAILED;
    }

    fseek(file, 0L, SEEK_END);
    f_size = ftell(file);
    log_debug("file_size: %d\n",ftell(file));
    fseek(file, 0L, SEEK_SET);

    char *line = (char *) malloc(sizeof(char) * UINT16_MAX);
    rec_data = (char**)malloc(sizeof(char)*f_size);
    *rec_data = (char*)malloc(sizeof(char)*UINT16_MAX);

#if 1
    while (fgets(line, UINT16_MAX, file)) {
        uint8_t type;
        uint8_t byteCount;
        uint8_t *address = (uint8_t *)malloc(sizeof(uint8_t)*6);
        uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t)*UINT8_MAX);
        memset(data,0,sizeof(uint8_t)*UINT8_MAX);
        memset(address,0,sizeof(uint8_t)*6);

        unsigned int checksum;
        log_debug("\n\rLine: %s\r\n", line);
        
        
        if (sscanf(line, "S%c%2X%6X", &type, &byteCount, &address) == 3) {
            int dataSize = (byteCount - 4); 

            if (type == '0') {
                log_debug("\n\rHeader frame...skipping\n\r");
                continue;
            } else if (type >= '4') {
                log_debug("\n\rTail frame...skipping\n\r");
                continue;
            }
            int i;
            for (i = 0; i < dataSize; i++) {
                sscanf(line + 10 + i * 2, "%2X", &data[i]);
                // log_debug("%02X\n", data[i]);
            }
            rec_data[IDX] = (char*)data;
            IDX++;
            sscanf(line + 10 + dataSize * 2, "%2X", &checksum); 
            
            log_debug("\n\rType: S%c, Byte Count: %d, Address: 0x%06X, Data:",type, byteCount, address);

            for (i = 0; i < dataSize; i++) log_debug(" 0x%02X", data[i]);
            
            log_debug(", Checksum: 0x%02X\n", checksum);
        }
    }
#endif
    log_debug("\r\nprinting data \r\n");
    for(int i = 0; i < IDX; i++) log_debug("\r\ndata: %X \r\n", rec_data[i]);
        
    fclose(file);
    return SREC_PARSE_SUCCESS;
}

#ifdef __cplusplus
    }
#endif

#endif