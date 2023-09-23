main:
	g++ -std=c++17 -I${NICEFIG}/src main.cc -o ./a.x

pdf: main
	./a.x && pdflatex dv.tex && rm *.log *.aux
