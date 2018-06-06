autos_data <- read.table("mem.dat", header=T, sep="\t")

barplot(as.matrix(autos_data), ylab= "IPC",
   beside=TRUE, col=rainbow(4))

legend("topright", c("2","4","8","16"), cex=1,
  bty="n", fill=rainbow(5));

save.image()
