#!/bin/bash

Rscript sgrafico1.r
mv Rplots.pdf grafico1.pdf
Rscript sgrafico2.r
mv Rplots.pdf grafico2.pdf
Rscript sgrafico3.r
mv Rplots.pdf grafico3.pdf
Rscript sgrafico4.r
mv Rplots.pdf grafico4.pdf

