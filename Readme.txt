Tema1 APD -- Bratescu Stefan-Gabriel

Partea 1:
	In prima faza am implementat I/O al imaginilor. Am folosit fread si fwrite pentru binary si
	fprintf si fscanf pentru human-readable. Acolo mai fac si alocarea si eliberarea memoriei.
	Pentru resize initial am facut secvential. Se observa foarte usor ca se poate paraleliza
	dupa i. Am lasat alocarile in resize si am paralelizat dupa i in functial resizeThread.

Partea 2:
	Am alocat si intializat imaginea cu alb intial. Am luat fiecare pixel in ordinea lor normala din
	matrice. Pentru fiecare i si j am calculat x si y corespunzator. Am luat formula distantei de la punct
	la o dreapta si am aplicat-o. Intial era totul cu double pentru precizie. Pentru ca checker-ul pica pentru
	ele am tot schimbat pentru a nimeri combinatia reusita. 
	La paralelizare din nou era usor sa paralelizezi dupa i similar cu prima parte.
