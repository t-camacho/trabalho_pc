colors  <- c('blue', 'red', 'black', 'orange', 'green', 'purple',
            'brown', 'yellow');

legends <- c("Ideal",
              "Processo & Semáforo",
              "Processo & Transações",
              "Thread & Mutex",
              "Thread & Semáforo",
              "Thread & Transações",
              "Thread & Spin",
              "TBB")

ideal <- c(0, 2, 4, 8, 16)
processador <- c(0, 2, 4, 8, 16)

speedup_proces_sem   <- c(0, 1.72289156627, 1.93243243243, 2.3064516129
                          ,1.90666666667)
speedup_proces_trans <- c(0, 1.90666666667, 2.13432835821, 1.7875,
                          1.64367816092)
speedup_thread_mutex <- c(0, 1.95890410959, 2.34426229508, 
                          2.34426229508,2.34426229508)
speedup_thread_sem   <- c(0, 1.95890410959, 2.34426229508, 
                          2.34426229508, 2.34426229508)
speedup_thread_spin  <- c(0, 1.93243243243, 2.34426229508, 
                          2.3064516129, 2.26984126984)
speedup_thread_trans <- c(0, 1.7875, 2.26984126984, 2.13432835821,
                          1.95890410959)
speedup_tbb          <- c(0, 1.85714285714, 2.3064516129, 2.3064516129,
                 2.3064516129)

plot(c(0, 2, 4, 8, 16),c(0, 2, 4, 8, 16), type='n', xlab=NA, ylab=NA, 
     axes=FALSE)
axis(1, las=2, at=2*0:16)
axis(2, las=2, at=2*0:16)
box()

lines(processador, ideal, col = colors[1], lwd = 2)
lines(processador, speedup_proces_sem, col = colors[2], lwd = 2)
lines(processador, speedup_proces_trans, col = colors[3], lwd = 2)
lines(processador, speedup_thread_mutex, col = colors[4], lwd = 2)
lines(processador, speedup_thread_sem, col = colors[5], lwd = 2)
lines(processador, speedup_thread_spin, col = colors[6], lwd = 2)
lines(processador, speedup_thread_trans, col = colors[7], lwd = 2)
lines(processador, speedup_tbb, col = colors[8], lwd = 2)

legend(0, 16, legends, col = colors, pch=rep(20,2))
title(xlab = "Processadores", ylab = "Speedup")

save.image()
