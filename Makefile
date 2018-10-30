all :demo background profile 
profile:
	touch profile
pipe:
	rm *_pipe
background:background.o message.o pipe.o
	gcc background.o message.o pipe.o -o background  
demo:main.o message.o pipe.o
	gcc main.o message.o pipe.o -o a.out  
clean:
	rm *.o a.out background


