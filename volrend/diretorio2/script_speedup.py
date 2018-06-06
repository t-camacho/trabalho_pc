#!/usr/bin/python

nomes_f = [
"speedup_thread_sem.txt",
"speedup_proces_sem.txt",
"speedup_thread_spin.txt",
"speedup_proces_trans.txt",
"speedup_thread_trans.txt",
"speedup_thread_mutex.txt",
"speedup_tbb.txt",
]

nomes = [
"tempo_sequential.txt",

"tempo_thread_semaphore_2_threads.txt",
"tempo_thread_semaphore_4_threads.txt",
"tempo_thread_semaphore_8_threads.txt",
"tempo_thread_semaphore_16_threads.txt",

"tempo_p_semaphore_16_threads.txt",
"tempo_p_semaphore_2_threads.txt",
"tempo_p_semaphore_4_threads.txt",
"tempo_p_semaphore_8_threads.txt",

"tempo_thread_spin_2_threads.txt",
"tempo_thread_spin_4_threads.txt",
"tempo_thread_spin_8_threads.txt",
"tempo_thread_spin_16_threads.txt",

"tempo_p_trans_2_threads.txt",
"tempo_p_trans_4_threads.txt",
"tempo_p_trans_8_threads.txt",
"tempo_p_trans_16_threads.txt",

"tempo_thread_trans_2_threads.txt",
"tempo_thread_trans_4_threads.txt",
"tempo_thread_trans_8_threads.txt",
"tempo_thread_trans_16_threads.txt",

"tempo_thread_mutex_2_threads.txt",
"tempo_thread_mutex_4_threads.txt",
"tempo_thread_mutex_8_threads.txt",
"tempo_thread_mutex_16_threads.txt",

"tempo_tbb_2_threads.txt",
"tempo_tbb_4_threads.txt"]

medias = []

for k in range (len(nomes)):
    arquivo = open(nomes[k], 'r')
    texto = arquivo.readlines()
    i = 0
    array = []

    for linha in texto:
        array.insert(i, int(float(linha)))
        i = i + 1

    arquivo.close()
    array.pop(0)
    buff = array[0]
    
    #menor
    for j in range (len(array)):
        if(buff > array[j]):
            buff = array[j]

    array.remove(buff)

    buff = array[0]

    #maior
    for j in range (len(array)):
        if(buff < array[j]):
            buff = array[j]

    array.remove(buff)

    buff = 0
    for j in range (len(array)):
        buff += array[j]

    medias.insert(k, buff/(len(array)));

j = 1

for i in range (6):
    speedup = []
    arquivo = open(nomes_f[i], 'w')
    speedup.append(str(float(medias[0])/medias[j]))
    speedup.append("\n")
    j = j + 1
    speedup.append(str(float(medias[0])/medias[j]))
    speedup.append("\n")
    j = j + 1
    speedup.append(str(float(medias[0])/medias[j]))
    speedup.append("\n")
    j = j + 1
    speedup.append(str(float(medias[0])/medias[j]))
    speedup.append("\n")
    j = j + 1
    arquivo.writelines(speedup)
    arquivo.close()

speedup = []
arquivo = open(nomes_f[6], 'w')
speedup.append(str(float(medias[0])/medias[j]))
speedup.append("\n")
j = j + 1
speedup.append(str(float(medias[0])/medias[j]))
speedup.append("\n")
j = j + 1
arquivo.writelines(speedup)
arquivo.close()


