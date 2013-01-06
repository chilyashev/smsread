SOURCES=main.cpp
OUT_DIR=dist/

CFLAGS= -Wall

all: $(SOURCES) clean smsread

clean:
	@echo "Cleaning..."
	@rm -rf $(OUT_DIR)smsread

smsread:
	@echo "Making $@..."
	g++ $(CFLAGS) -o $(OUT_DIR)smsread $(SOURCES)
