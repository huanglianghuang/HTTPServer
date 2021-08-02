server: main.c ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./timer/client_data.h ./timer/time_wheel.h ./timer/tw_timer.h  ./log/log.cpp ./log/log.h ./log/block_queue.h ./CGImysql/sql_connection_pool.cpp ./CGImysql/sql_connection_pool.h
	g++ -o server main.c ./threadpool/threadpool.h ./timer/time_wheel.cpp ./timer/tw_timer.cpp ./http/http_conn.cpp ./http/http_conn.h ./lock/locker.h ./log/log.cpp ./log/log.h ./CGImysql/sql_connection_pool.cpp ./CGImysql/sql_connection_pool.h -lpthread -lmysqlclient -std=c++11


clean:
	rm  -r server
