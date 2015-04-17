#FileTools makefile

#compiler
CC=g++

#out dir
OUTPUT_DIR=../output/

#sources
SRCS = FileTools.cpp main.cpp libs/CommandLineParser/CommandLineParser.cpp libs/DebugLogger/debug_logger.cpp libs/Exceptions/Exceptions.cpp


all: filetools

filetools:
	mkdir -p $(OUTPUT_DIR)
	$(CC) ${SRCS} -o $(OUTPUT_DIR)FileTools


clean: $(OUTPUT_DIR)FileTools
	rm $(OUTPUT_DIR)*
	
