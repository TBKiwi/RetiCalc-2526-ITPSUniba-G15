# RetiCalc-2526-ITPSUniba-G15
Buzzerio Simone


Assegnazione 1: Socket TCP Progettare ed implementare un’applicazione TCP client/server conforme al seguente protocollo:

1. Il client, invia il messaggio iniziale “Hello” al server.
2. Ricevuti i dati client, il server visualizza sullo std output un messaggio contenente l’indirizzo IP dell’host del client (Esempio: “ricevuti dati dal client con indirizzo: xx”).
3. Il client legge una stringa di caratteri dallo standard input e la invia al server.
4. Il server legge la stringa inviata dal client e la visualizza sullo standard output; dopodiché, elimina tutte le vocali e la invia nuovamente al client.
5. Il client legge la risposta inviata dal server e visualizza la risposta sullo standard output, dopodiché termina il processo e chiude la connessione
6. Il server resta in attesa di richieste di connessione da parte di altri client.


Assegnazione 2: Socket UDP e DNS in C Progettare ed implementare un’applicazione UDP client/server conforme al seguente protocollo:

1. Il client legge da tastiera il nome dell’host e il numero di porta del server da contattare.
2. Il client, invia il messaggio iniziale “Hello” al server.
3. Ricevuti i dati client, il server visualizza sullo std output un messaggio contenente il nome e l’indirizzo IP dell’host del client (Esempio: “ricevuti dati dal client nome: xxxxx indirizzo:yyyyyy”).
4. Il client legge una stringa di caratteri dallo standard input e la invia al server.
5. Il server legge la stringa inviata dal client e la visualizza sullo standard output; dopodiché, elimina tutte le vocali e la invia nuovamente al client.
6. Il server resta in attesa di altri dati.
7. Il client legge la risposta inviata dal server e visualizza la risposta sullo standard output (“Stringa zzzzzz ricevuta dal server nome:xxxxxx inidirizzo:yyyyy”); dopodiché termina il processo
