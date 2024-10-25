all: compile exe clean 

compile:
	gcc -fopenmp -o main main.c

exe: 
	time ./all.sh  # Execute the all.sh script

clean:
	rm -f main *.bin* # images/*  # Clean up the images directory as well
#	rm -rf images  # Remove the images directory
