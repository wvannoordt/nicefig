main:
	g++ -std=c++20 -I${NICEFIG}/src main.cc -o ./a.x

pdf: main
	./a.x && pdflatex dv.tex && rm *.log *.aux
