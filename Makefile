SOURCES=main.cpp
OUT_DIR=bin/

CFLAGS= -Wall -ggdb

all: $(SOURCES) clean smsread

clean:
	@echo "Cleaning..."
	@rm -rf $(OUT_DIR)smsread

smsread:
	@echo "Making $@..."
	@mkdir -p $(OUT_DIR)
	g++ $(CFLAGS) -o $(OUT_DIR)smsread $(SOURCES)
