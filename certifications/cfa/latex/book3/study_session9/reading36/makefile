
all: reading.pdf questions.pdf

reading.pdf: reading.latex
	pdflatex reading.latex

questions.pdf: questions.latex
	pdflatex questions.latex


.CLEAN:clean
clean:
	rm *.log *.aux *.pdf

.PHONY:open
open:
	open reading.pdf

.PHONY:ques
ques:
	open questions.pdf
