#Звіт з лабораторної роботи №1<br/>за дисципліною "Комп'ютерна графіка"<br/>студента групи ПА-17-1<br/>Панасенка Єгора Сергійовича<br/>Кафедра комп'ютерних технологій<br/>ФПМ, ДНУ, 2017-2018 навч.р.<br/>Тема: робота з OpenGL за допомогою<br/>бібліотеки freeglut,<br/>графічні об’єкти на площині,<br/>перетворення плоских об’єктів.

## Постановка задачі

### Частина 1

1. Ознайомитись з можливостями графічної бібліотеки OpenGL. Розібратися з особливостями підключення бібліотеки Glut (freeglut, glfw)
2. Розробити програму, що виводить на екран вікно, в якому будуть розміщатися графічні об’єкти. Фон вікна зробити світлого кольору. Заголовок вікна "Лабораторна робота No 1. Виконав ..." (вказати прізвище, ім’я студента та групу).
Передбачити можливість закриття вікна при натисненні клавіши Esc.
3. Вивести у вікно графічне зображення "логотипу" студента, який складається з двох літер – перших літер імені та прізвища. Вимоги:
    1. Зображення повинно бути створено за допомогою простих графічних елементів (із бібліотеки OpenGL): точки, лінії, трикутники, чотирикутники, багатокутники та ін.
    2. Для відображення кола або його фрагментів у складі літер розробити підпрограму малювання кола з використанням інкрементного алгоритму.
    3. Кожну літеру зафарбувати своїм кольором.
    4. Літери повинні мати зовнішні контури, колір яких відрізняється від
кольору зафарбування.
    5. Літери мають перетинатися.
4. Організувати зміну кольору букв при натисненні клавіш:  
    перша клавіша – змінюється колір однієї літери,  
    друга клавіша – змінюється колір іншої літери,  
    третя клавіша – відтворюється вихідний колір букв.  
    Колір при зміні обирається випадковим чином. Клавіши керування обрати самостійно.

### Частина 2

1. Організувати пересування вправо, вліво, вгору, вниз двох літер одночасно по натисненню клавіш керування (відповідні стрілки).
2. Організувати пересування вправо, вліво, вгору, вниз кожної літери окремо.  
    Клавіши керування обрати самостійно.
3. Організувати повернення букв у вихідне положення по натисненню деякої клавіши.
4. Організувати дискретне обертання двох літер одночасно за / проти годинникової стрілки навколо початку координат (обертання навколо осі Oz) на деякий фіксований невеликий кут по натисненню клавіш PgUp / PgDn відповідно.
5. Організувати дискретне обертання кожної літери окремо за / проти годинникової стрілки навколо свого центру на фіксований невеликий кут по натисненню клавіш F1 / F2 – для однієї літери, F11 / F12 – для іншої.
6. Організувати неперервне обертання двох літер одночасно: F6 – за годинниковою стрілкою, F7 – проти годинникової стрілки. Натискання на клавішу вмикає обертання, інше натискання на цю ж клавішу зупиняє
обертання.
7. Організувати неперервне обертання таким чином: одна із літер – за годинниковою стрілкою, інша – проти годинникової стрілки. Увімкнути/вимкнути таке обертання – клавіша F8.

Примітка 1. Будь-які маніпуляції з вікном (зміна розмірів, пропорцій і т.п.) не повинні спотворювати зображення. Пропорції об'єктів повинні зберігатися.

Примітка 2. Матрицю проекцій задавати таким чином, щоб точка (0, 0) знаходилась в центрі вікна.

## Опис роботи програми

Програма вміє малювати криві за стандартом svg файлів, тобто програма малює криву Безьє перетворенням спеціальних команд у масив точок. Далі вийшла крива (масив точок) тріангулюється за допомогою вушного методу, який має складність n<sup>2</sup>, але прораховуються тільки один раз при запуску програми і також зберігається у масив точок, а під час передрукуванні малюнку перетворюється тільки масив точок.
При зміні розміру вікна програми зображення не спотворюється.
Існують такі комбінації клавіатури:

1. TAB - перехід між вибраними об'єктами по циклу
    1. Всі букви
    2. Буква Е
    3. Буква П
2. W, A, S, D - Пересування активного об'єкта вгору, вліво, вниз, вправо відповідно
3. Q, R - Поворот активного об'єкта проти годинникової стрілки і за годинниковою стрілкою відповідно
4. R - Повернути букви у початковий стан
5. Z, X - Змінити колір букви Е, букви П
6. C - Повернути колір у початковий стан
7. F6, F7, F8 - Анімація згідно з завданням
8. Page Up, Page Down - Поворот навколо центра проти годинникової стрілки і за годинниковою стрілкою відповідно.

Знімок малюнку при запуску програми:

![Пример](Screenshot.png)

## Опис структури коду і самого коду

### Структура файлів

У проекті присутні такі файли:

1. CMakeLists.txt - файл для компіляції через cmake
2. COPYING - файл ліцензії
3. Readme.md - звіт
4. Screenshot.png - знімок малюнку при запуску програми
5. classes.h   - заголовний файл з усіма класами, які використовуються у головній програмі
6. classes.cpp - файл з реалізаціями усіх класів
7. main.cpp - головна програма
