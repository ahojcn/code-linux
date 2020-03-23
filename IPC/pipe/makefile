.PHONY:all
all: reader writer

reader:read_myfifo.c
	gcc $^ -o $@

writer:write_myfifo.c
	gcc $^ -o $@

.PHONY:clean
clean:
	rm reader writer
