# Introducere in Robotica - Tema2

Această temă reprezintă un joc de tip TypeRacer.

## Componente:

Arduino UNO (ATmega328P microcontroller);
    
1x LED RGB (pentru a semnaliza dacă cuvântul corect e scris greșit sau nu);
    
2x Butoane (pentru start/stop rundă și pentru selectarea dificultății);
    
5x Rezistoare (3x 220/330 ohm, 2x 1000 ohm);
    
Breadbord;
    
Fire de legătură.

## Schema:

![Schema](https://github.com/user-attachments/assets/03e93894-b0d4-4618-82ab-8f6e429111c2)

## Descriere:

Inițial, jocul este în repaus. LED-ul RGB are culoarea albă. Se alege dificultatea jocului folosind butonul de dificultate, iar în terminal va apărea “Easy/Medium/Hard mode on!”. Se apasă butonul de start/stop. LED-ul clipește timp de 3 secunde, iar în terminal se va afișa numărătoarea înversă: 3, 2, 1. LED-ul devine verde și încep să apară cuvinte de tastat. La tastarea corectă, următorul cuvânt apare imediat. Dacă nu se tasteaza cuvântul în timpul stabilit de dificultate, va apărea un nou cuvânt. O greșeală face LED-ul să devină roșu. Pentru a corecta cuvântul, se va folosi tasta BackSpace. Dupa 30 de secunde, runda se termină, iar în terminal se va afișa scorul: numărul total de cuvinte scrise corect. Jocul se poate opri oricând cu butonul de start/stop. 

## Montaj:

![IMG_20241028_014049](https://github.com/user-attachments/assets/31301d84-be02-424f-8803-637c08c27651)

## Video

[<img src="https://img.youtube.com/vi/HkYYlN9PhC8/maxresdefault.jpg">](https://youtu.be/HkYYlN9PhC8)
