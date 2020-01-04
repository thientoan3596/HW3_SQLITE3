.PHONY: all
all: myapp

myapp: main.o libmysqlite3.o
	gcc main.o libmysqlite3.o -lsqlite3 -o myapp

libmysqlite3.o: libmysqlite3.c libmysqlite3.h
	gcc -c libmysqlite3.c

main.o: main.c libmysqlite3.h
	gcc -c main.c

.PHONY: rollback
rollback:
	cp ./backup_db/stockexchange_5A.db ./stockexchange_5A.db
.PHONY: clean
clean:
	rm -rf *.o myapp
