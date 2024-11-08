all:
	cd System && make && cd .. && \
	cd programs/Text_counter && make && cd .. && \
	cd Text_counter_Threads && make && cd .. && \
	cd Executer && make && cd .. && \
	cd process_planificator && make && cd .. && \
	cd Cache && make && cd .. &&\
	cd Buscador && make && cd ..

