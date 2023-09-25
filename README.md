# Game
Приветсвтую!
В этом репозитории лежит код моей игры.
На данный момент игра запускается только на ОС Linux, но в дальнейшем я собираюсь адаптировать её под Windows.

Как запустить игру?
Чтобы запустить игру скачайте файл MyGame (это бинарный файл и как раз по этой причине он не запускается на Windows),
затем скачайте карты FirstMap.txt и SecondMap.txt, после этого перейдите в терминал,
перейдите в папку со скачанными файлами (при помощи cd), пропишите команду chmod +x MyGame для того чтобы сделать файл исполняемым.
Наконец запустите игру! (при помощи команды ./MyGame)
Периодически я буду вносить изменения, т.к игра еще не закончена.

Краткое описание файлов игры:

-main.cpp:
Отрисовывает окно, задает с какой скоростью будет идти игра (параметр FPS), отслеживает нажимаемые кнопки на клавиатуре.

-game.cpp:
Основной файл, в котором прописана вся логика игры, отрисовывает все объекты, содержит все классы и функции, считывает карты.

-FirstMap.txt, SecondMap.txt, ThirdMap.txt:
Текстовые файлы, которые содержат матрицу, описывающую местонахождение начальных объектов,
которые считываются функцией из файла game.cpp.
Эти файлы нужны чтобы можно было в любой момент переделать/подправить карту не залезая в основной код.

-classes.h:
Заголовочный файл содержащий макросы и общие функции.

-Makefile:
Файл, в котором описаны правила сборки проекта.
Выдает конечный файл MyGame формата .bin.

-MyGame:
Бинарный файл самой игры, которые появляется в результате выполнения инструкций в Makefile.

По всем вопросам пишите в Telegram
@ArtemS101
Буду рад пообщаться!
