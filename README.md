# Surveillance-Platform-IIoT

Μπορείτε να επιλέξετε την τοποθεσία στην οποία θα εκτελείται η πλατφόρμα του node-red δηλαδή
να επιλέξετε είτε εκτέλεση της πλατφόρμας στον υπολογιστής σας (local PC) είτε σε raspberry pi
(οδηγίες: https://nodered.org/docs/getting-started/raspberrypi) είτε εκτέλεσή της στο
νεφοϋπολογιστικό περιβάλλον του flowforge. Επισημαίνουμε ότι στην περίπτωση που
ακολουθήσετε τη χρήση του cloud, για να αποκτήστε πρόσβαση στο node-red θα πρέπει να
δημιουργείστε δωρεάν λογαριασμό στο νεφοϋπολογιστικό περιβάλλον flowforge στην ιστοσελίδα
https://app.flowforge.com/account/create . Στο flowforge μπορείτε για 30 μέρες να ενεργοποιήσετε
δωρεάν 1 instance του node-red στο οποίο θα εγκαταστήσετε το node-red dashboard (Προσοχή:
δεν θα ρυθμίσετε billing details).
Επίσης, για τον MQTT broker μπορείτε είτε να χρησιμοποιήσετε mosquitto broker εγκατεστημένο
στον τοπικό σας Η/Υ, είτε στο raspberry pi είτε κάποιον από τους διαθέσιμους online (π.χ.
broker.hivemq.com, test.mosquitto.org, …).
Ο μικροελεγκτής του IIoT node μπορεί να είτε Arduino με Ethernet Shield είτε WeMos είτε Arduino
Wi-Fi board. Οι αισθητήρες/ενεργοποιητές είναι αυτοί που φαίνονται στο σχήμα 1 παραπάνω.
Σχήμα 1. Αρχιτεκτονική συστήματος ΙIοΤ

Προδιαγραφές:
Το σύστημα που θα αναπτύξετε θα επιτελεί την παρακάτω διαδικασία πάνω στον μικροελεγκτή του
IIoT node, αφού πρώτα συνδέσετε τους αισθητήρες και τους ενεργοποιητές χρησιμοποιώντας το
breadboard.

2)Αλγόριθμος του ΙΙοΤ
i. Αρχικοποίηση αισθητήρων/ενεργοποιητών συνδέσεων δικτύου & ΜQTT broker.
ii. Σε κάθε επανάληψη του ατέρμονα βρόχου (loop) θα λαμβάνετε δεδομένα από τον
αισθητήρα DHT11 ή DHT22 (θερμοκρασία, υγρασία, heat-index). Tα δεδομένα θα
αποστέλλονται μέσω MQTT publish στο Node-Red κάθε 10 δευτερόλεπτα.
iii. Σε περίπτωση που ανιχνευτεί φλόγα από τον αισθητήρα ανίχνευσης φλόγας (κατάσταση
υψηλής προτεραιότητας) το σύστημα εκτελεί άμεση απόκριση και περιστρέφει το πτερύγιο
τον σερβοκινητήρα SG90 κατά 145ο αντίθετα της φοράς του ρολογιού και μετά την πάροδο
30 δευτερολέπτων μετακινεί το πτερύγιο κατά 1 μοίρα (αντίθετα της φοράς του ρολογιού)
έως να φτάσει τις 180ο. Στη συνέχεια μετά την πάροδο 30 δευτερολέπτων επαναφέρει το
πτερύγιο στη θέση 0ο. [Σε περίπτωση που δε διαθέτετε αισθητήρα ανίχνευσης φλόγας
χρησιμοποιήστε τον αισθητήρα ανίχνευσης φωτός LDR. Θα τοποθετήσετε ένα καπάκι πάνω
στον αισθητήρα (συνθήκες συσκότισης) και όταν βγάζετε το καπάκι θα προσομοιώνει την
παρουσία φλόγας].
iv. Τη στιγμή που θα ανιχνευτεί φλόγα ο ΙΙοΤ node θα αποστείλει μέσω του topic
iiotnode\flamedetection την τιμή 1 ή true μέσω του MQTT στο Node-red ενώ ταυτόχρονα
θα ηχήσει για η σειρήνα (buzzer) ή θα ανάψει το LED μέχρι να ολοκληρωθεί η διαδικασία
του βήματος iii.
v. Όταν o IIoT node λάβει σήμα από το node-red μέσω του ΜQTT με subscribe στο topic
iiotnode\conditions την τιμή 1 ή true (χαμηλότερης προτεραιότητας από τις συνθήκες του
βήματος iii), τότε ηχεί τη σειρήνα (buzzer) ή ανάβει το LED και μετακινεί ανά 5ο το πτερύγιο
του σερβοκινητήρα αντίθετα της φοράς του ρολογιού έως τη θέση των 90ο. Στη συνέχεια
μετά από 45 δευτερόλεπτα εκτελεί άμεση κίνηση του πτερυγίου στη θέση των 45ο
(αντίθετα της φοράς του ρολογιου) και μετά από 10 δευτερόλεπτα επαναφέρει το πτερύγιο
του σερβοκινητήρα στη θέση 0ο με βήμα 9ο και παύει τη σειρήνα ή σβήνει το LED.
Σχεδίαση στο Node-Red
i. Απεικονίστε σε πραγματικό χρόνο στο node-red τις γραφικές αναπαραστάσεις
(θερμοκρασίας vs. χρόνου), (υγρασίας vs. χρόνου), (headindex vs. χρόνου).
ii. Αποθηκεύστε σε βάση δεδομένων (SQLite) ανά τακτά διαστήματα των 30 δευτερολέπτων
τα δεδομένα της θερμοκρασίας στο περιβάλλον του node-red με τη χρήση του κόμβου
«litedb».
iii. Με χρήση των κόμβων «function» και «litedb» του node-red να αναπτύξετε πρόγραμμα σε
javascript (node function) που να λαμβάνει τις τιμές της θερμοκρασίας από τη βάση
δεδομένων για διάστημα 2 λεπτών και να υπολογίζει το μέσο όρο τους να τους αποθηκεύει
σε ξεχωριστό πίνακα στην SQLlite DB. Στη συνέχεια, να αναγράφετε την τελευταία τιμή του
μέσου όρου στο UI του node-red (παρατήρηση: όχι γραφική παράσταση).
iv. Έλεγχος συνθηκών χαμηλής προτεραιότητας: Ελέγχετε τον τρέχοντα μέσο όρο με την
προηγουμένη τιμή του μέσου όρου που υπολογίζεται και αποθηκεύεται με το προηγούμενο

3) βήμα iii και εάν υπάρχει διαφορά μεγαλύτερη ή ίση των 2οC τότε με publish μέσω του
ΜQTTστο topic iiotnode\conditions στείλτε την τιμή 1 ή true στον IIoT node και
αποθηκεύστε σε διαφορετικό πίνακα της SQLlite DB τη χρονική στιγμή που
πραγματοποιήθηκε το συγκεκριμένο συμβάν. Στο UI του node-red να αναγράφετε τη
χρονική στιγμή του συμβάντος (παρατήρηση: όχι γραφική παράσταση).
ΠΑΡΑΤΗΡΗΣΗ: Να αποθηκεύσετε δεδομένα τουλάχιστον 3 ημερών. Αλλάζοντας τις συνθήκες του
περιβάλλοντος χώρου μπορείτε να προσομοιώσετε τις αλλαγές τις θερμοκρασίας ή τις αλλαγές των
μέσων όρων ή την ύπαρξη φλόγας. Σύνολο βαθμολογίας 30% του τελικού Βαθμού.
Επιπλέον bonus 10% του τελικού βαθμού εάν τον Αλγόριθμο του ΙΙοΤ τον υλοποιήσετε επιπλέον
είτε με FreeRTOS και MQTT broker είτε με χρήση του Modbus-TCP πρωτοκόλλου και κατάργηση
του ΜQTT broker.
Η εργασία προβλέπεται να εκπονηθεί από ομάδα τριών φοιτητών. Το παραδοτέο θα αποτελείται
από:
Αναφορά με τον κώδικα της εφαρμογής (ο κώδικας του node-red να είναι σε μορφή JSON
όπως εξάγεται από το node-red export to clipboard, κώδικας Arduino και φωτογραφία της
διάταξης)
Επίδειξη λειτουργίας της εφαρμογής
