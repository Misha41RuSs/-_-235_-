#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h>

#define WINDOW_HEIGHT 1.2
#define WINDOW_WIDTH 1.1
#define DOOR_HEIGHT 1.9
#define DOOR_WIDTH 0.7

// Размеры окна
typedef struct {
    int x;
    int y;
} Window;

// Размеры двери
typedef struct {
    int x;
    int y;
} Door;

// Структура для представления комнаты
typedef struct {
    double length;
    double width;
    double height;
    int door_numb;
    int window_numb;
    double room_area;
    double numb_film;
    double window_area;
    double door_area;
    Window* windows;
    Door* doors;
} Room;

// Обьявление функций
Room inputRoomData(Room* room); // Функция для ввода данных о комнате
void outputRoomData(Room* room, int roomCount); // Функция для вывода данных о комнате
double roomArea(Room* room, int roomCount); // Функция для подсчета площади комнаты
Room* windowArea(Room* room, int roomCount); // Функция для расчета площади окон
Room* doorArea(Room* room, int roomCount); // Функция для расчета площади дверей
double waterproofingFilmArea(); // Функция для ввода разммеров пленки
double numbFilm(double room_area, double waterproofing_film_area, Room* room, int roomCount); // Функция для подсчеита количества рулонов пленки
int writeRoomDataToFileFilm(Room* room, int roomCount, double numb_film, double waterproofing_film_area); // Запись данных в файл
int writeRoomDataToFile(Room* room, int roomCount); // Запись данных в файл для считывания (без кол-ва пленки)
int readRoomDataFromFile(Room* room, int roomCount); // Считывание данных из файла

void inputCoordinateData(Room* room, int roomCount); // Задаем координаты для окон и дверей
void displayRoom(Room room); // Рисуем графическое изображение
void displayRooms(Room* rooms, int roomCount); // Рисуем графическое изображение для каждой комнаты

int main() {
    setlocale(LC_ALL, "RUS");

    //Флаги
    int validInput = 0; //Проверка корректности количества комнат(целое положит. число)
    int dataEntered = 0; //Проверка введённости данных о помещении(ях) (чтобы при отсутствии не запускались кейсы 2, 3, 5)
    int dataFilm = 0; //Проверка введённости данных о плёнке для комнаты, чтобы в файл записывалась информация с плёнкой или без неё

    int choice = 0;
    int roomCount;
    double waterproofing_film_area, window_area, door_area, room_area, numb_film;
    char continued;
    Room* rooms;

    while (!validInput) {
        printf("Введите количество комнат: ");
        if (scanf("%d", &roomCount) != 1 || roomCount <= 0) {
            printf("Некорректный ввод! Введите положительное целое число.\n\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        else {
            validInput = 1;
        }
    }

    printf("Данные успешно записаны!\n");
    system("pause");

    rooms = (Room*)malloc(roomCount * sizeof(Room));

    while (choice != 7)
    {
        system("cls");
        printf("****************** Главное меню *****************\n");
        printf("1. Ввести параметры помещения;\n2. Вывести данные о помещении(ях);\n3. Посчитать количество рулонов гидроизоляционной пленки;\n");
        printf("4. Считать данные из файла;\n5. Записать информацию о размерах помещения и количестве пленки в файл;\n");
        printf("6. Вывести графическое изображение помещения;\n7. Завершение работы программы;\n");

        printf("Выберите действие: \n");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            if (rooms == NULL) {
                printf("Ошибка выделения памяти\n");
                return 1; // В случае ошибки выделения памяти завершаем программу
            }
            else
            {
                for (int i = 0; i < roomCount; i++) {
                    printf("Данные для комнаты %d:\n", i + 1);
                    inputRoomData(&rooms[i]);
                    rooms[i].room_area = 2 * (rooms[i].length * rooms[i].width) + 2 * (rooms[i].height * rooms[i].width) + 2 * (rooms[i].height * rooms[i].length);
                }
                printf("Данные усешно записаны!");
                dataEntered = 1;
            }
            break;
        }
        case 2:
        {
            if (dataEntered == 0) {
                printf("Нет данных о помещении(ях), введите их вручную или считайте из файла.\n");
            }
            else {
                outputRoomData(rooms, roomCount);
            }
            break;
        }
        case 3:
        {
            if (dataEntered == 0) {
                printf("Нет данных о помещении(ях), введите их вручную или считайте из файла.\n");
            }
            else {

                room_area = roomArea(rooms, roomCount); // Площадь комнат

                windowArea(rooms, roomCount); // Площадь окон

                doorArea(rooms, roomCount); // Площадь дверей

                waterproofing_film_area = waterproofingFilmArea(); // Площадь пленки

                numb_film = round(numbFilm(room_area, waterproofing_film_area, rooms, roomCount)); // Кол-во рулонов

                for (int i = 0; i < roomCount; i++)
                {
                    printf("Количество пленки для %d комнаты: %lf\n", i + 1, rooms[i].numb_film);
                }

                printf("\nВсего понадобится рулона(ов) гидроизоляционной пленки: %.0lf\n\n", numb_film);
                dataFilm = 1;
            }
            break;
        }
        case 4:
        {
            int roomsRead = readRoomDataFromFile(rooms, roomCount); // Считываем данные из файла

            if (roomsRead > 0) {
                printf("Успешно считаны данные о %d комнатах из файла.\n", roomsRead);
                dataEntered = 1;

            }
            else {
                printf("Нет данных для считывания из файла.\n");
            }

            break;
        }
        case 5:
        {
            if (dataEntered == 0) {
                printf("Нет данных о помещении(ях), введите их вручную или считайте из файла.\n");
            }

            if (dataFilm == 0) {
                writeRoomDataToFile(rooms, roomCount);
            }
            else {
                writeRoomDataToFileFilm(rooms, roomCount, numb_film, waterproofing_film_area);
            }
            break;
        }
        case 6:
        {
            if (dataEntered == 0) {
                printf("Нет данных о помещении(ях), введите их вручную или считайте из файла.\n");
            }
            else {
                // Ввод данных о комнате
                inputCoordinateData(rooms, roomCount);

                // Вывод графического изображения помещения
                //displayRoom(*rooms);

                displayRooms(rooms, roomCount);

                // Освобождение памяти
                free(rooms->windows);
                free(rooms->doors);
            }

            break;
        }
        case 7:
        {
            printf("Завершение работы программы, До свидания!\n");
            break;
        }
        default:
        {
            printf("Вы ввели некорректные данные, повторите попытку!\n");
            break;
        }

        }

        printf("\n");
        printf("\tПродолжить? (Да-введите любой символ, нет - n)\n");
        scanf("%c", &continued);
        printf("\n");
        if (continued == getchar() == 'n')
        {
            printf("\n");
            puts("   *********************************************************");
            break;
        }

    }

    free(rooms);
    return 0;
}



// Функция для ввода данных о комнате
Room inputRoomData(Room* room) {

    printf("Введите длину комнаты (в метрах): ");
    scanf("%lf", &room->length);

    printf("Введите ширину комнаты (в метрах): ");
    scanf("%lf", &room->width);

    printf("Введите высоту комнаты (в метрах): ");
    scanf("%lf", &room->height);

    printf("Сколько окон в комнате?\n");
    scanf("%d", &room->window_numb);

    printf("Сколько дверей в комнате?\n");
    scanf("%d", &room->door_numb);

    return *room;
}

// Функция для вывода данных о комнатах
void outputRoomData(Room* room, int roomCount)
{
    printf("Вы ввели %d комнаты\n", roomCount);
    for (int i = 0; i < roomCount; i++)
    {
        printf("Информация о комнате № %d\n", i + 1);
        printf("Длинна комнаты: %lf\n", room[i].length);
        printf("Ширина комнаты: %lf\n", room[i].width);
        printf("Высота комнаты: %lf\n", room[i].height);
        printf("Площадь комнаты: %lf\n", room[i].room_area);
        printf("Количество окон: %d\n", room[i].window_numb);
        printf("Количество дверей: %d\n", room[i].door_numb);
    }
}

double roomArea(Room* room, int roomCount)
{
    double room_area = 0;

    for (int i = 0; i < roomCount; i++) {
        room_area += 2 * (room[i].length * room[i].width) + 2 * (room[i].height * room[i].width) + 2 * (room[i].height * room[i].length);
    }

    return room_area;
}

// Функция для расчета площади окон
Room* windowArea(Room* room, int roomCount)
{
    for (int i = 0; i < roomCount; i++)
    {
        room[i].window_area = (WINDOW_HEIGHT * WINDOW_WIDTH) * room[i].window_numb;
    }
    return room;
}

// Функция для расчета площади дверей
Room* doorArea(Room* room, int roomCount)
{
    for (int i = 0; i < roomCount; i++)
    {
        room[i].door_area = (DOOR_HEIGHT * DOOR_WIDTH) * room[i].door_numb;
    }
    return room;
}

double waterproofingFilmArea()
{
    double waterproofing_film_length, waterproofing_film_width;

    printf("Введите длинну гидроизоляционной пленки (в метрах): ");
    scanf("%lf", &waterproofing_film_length);
    printf("Введите ширину гидроизоляционной пленки (в метрах): ");
    scanf("%lf", &waterproofing_film_width);
    return (waterproofing_film_length * waterproofing_film_width);
}

double numbFilm(double room_area, double waterproofing_film_area, Room* room, int roomCount)
{
    double numbFilm = 0;
    for (int i = 0; i < roomCount; i++)
    {
        if (waterproofing_film_area < room[i].room_area)
        {
            room[i].numb_film = round((room[i].room_area - room[i].door_area - room[i].window_area) / waterproofing_film_area);
            numbFilm += room[i].numb_film;
        }
        else {
            room[i].numb_film = 1;
            numbFilm += room[i].numb_film;
        }
    }

    return numbFilm;
}

int writeRoomDataToFile(Room* room, int roomCount) {
    FILE* file = fopen("room_data.txt", "w"); // Открываем файл для записи

    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        return 0;
    }
    // Записываем информацию о каждой комнате и о плёнке в файл
    for (int i = 0; i < roomCount; i++) {
        fprintf(file, "Комната №%d:\n", i + 1);
        fprintf(file, "Длина: %.2lf м\n", room[i].length);
        fprintf(file, "Ширина: %.2lf м\n", room[i].width);
        fprintf(file, "Высота: %.2lf м\n", room[i].height);
        fprintf(file, "Площадь комнаты: %.2lf\n", room[i].room_area);
        fprintf(file, "Количество окон: %d\n", room[i].window_numb);
        fprintf(file, "Количество дверей: %d\n", room[i].door_numb);

        fprintf(file, "\n");
    }


    fclose(file); // Закрываем файл

    printf("Информация успешно записана в файл 'room_data.txt'.\n");
    return 1;
}

int writeRoomDataToFileFilm(Room* room, int roomCount, double numb_film, double waterproofing_film_area) {
    FILE* file = fopen("room_data_out.txt", "w"); // Открываем файл для записи

    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        return 0;
    }
    // Записываем информацию о каждой комнате и о плёнке в файл
    for (int i = 0; i < roomCount; i++) {
        fprintf(file, "Комната №%d:\n", i + 1);
        fprintf(file, "Длина: %.2lf м\n", room[i].length);
        fprintf(file, "Ширина: %.2lf м\n", room[i].width);
        fprintf(file, "Высота: %.2lf м\n", room[i].height);
        fprintf(file, "Площадь комнаты: %.2lf\n", room[i].room_area);
        fprintf(file, "Количество окон: %d\n", room[i].window_numb);
        fprintf(file, "Количество дверей: %d\n", room[i].door_numb);
        fprintf(file, "Количество пленки: %lf\n", room[i].numb_film);
        fprintf(file, "\n");
    }
    if (waterproofing_film_area != 0)
    {
        fprintf(file, "Необходимое количество рулонов пленки для всех помещений: %lf\n", numb_film);
    }

    fclose(file); // Закрываем файл

    printf("Информация успешно записана в файл 'room_data_out.txt'.\n");
    return 1;
}





int readRoomDataFromFile(Room* room, int roomCount) {
    FILE* file = fopen("room_data.txt", "r"); // Открываем файл для чтения

    if (file == NULL) {
        printf("Ошибка открытия файла для чтения.\n");
        return 0;
    }

    int i = 0;
    while (i < roomCount) {
        // Считываем информацию о каждой комнате из файла
        if (fscanf(file, "Комната №%*d:\n") == EOF) {
            break; // Достигнут конец файла
        }

        fscanf(file, "Длина: %lf м\n", &room[i].length);
        fscanf(file, "Ширина: %lf м\n", &room[i].width);
        fscanf(file, "Высота: %lf м\n", &room[i].height);
        fscanf(file, "Площадь комнаты: %lf\n", &room[i].room_area);
        fscanf(file, "Количество окон: %d\n", &room[i].window_numb);
        fscanf(file, "Количество дверей: %d\n", &room[i].door_numb);


        // Переходим к следующей комнате
        i++;

        if (fscanf(file, "\n") == EOF) {
            break; // Достигнут конец файла
        }
    }

    fclose(file); // Закрываем файл

    return i; // Возвращаем количество успешно считанных комнат
}

void inputCoordinateData(Room* rooms, int roomCount) {
    for (int i = 0; i < roomCount; i++) {
        rooms[i].windows = (Window*)malloc(rooms[i].window_numb * sizeof(Window));
        if (rooms[i].windows == NULL) {
            printf("Не удалось выделить память для окон.\n");
        }
        for (int j = 0; j < rooms[i].window_numb; j++) {
            rooms[i].windows[j].x = 0; // Случайная координата x в пределах комнаты
            rooms[i].windows[j].y = 1 + rand() % ((int)rooms[i].width - 2);   // Случайная координата y в пределах комнаты
        }

        rooms[i].doors = (Door*)malloc(rooms[i].door_numb * sizeof(Door));
        if (rooms[i].doors == NULL) {
            printf("Не удалось выделить память для дверей.\n");
        }
        for (int j = 0; j < rooms[i].door_numb; j++) {
            rooms[i].doors[j].x = 0; // Случайная координата x в пределах комнаты
            rooms[i].doors[j].y = 1 + rand() % ((int)rooms[i].width - 2);   // Случайная координата y в пределах комнаты
        }
    }
}

void displayRoom(Room room) {
    // Определение размеров графического изображения
    int width = room.length + 2;
    int height = room.width + 2;

    // Выделение памяти для графического изображения
    char** image = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        image[i] = (char*)malloc(width * sizeof(char));
    }

    // Заполнение графического изображения символами пробела
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = ' ';
        }
    }

    // Рисование стен помещения
    for (int i = 0; i < height; i++) {
        image[i][0] = '|';
        image[i][width - 1] = '|';
    }
    for (int j = 0; j < width; j++) {
        image[0][j] = '-';
        image[height - 1][j] = '-';
    }

    // Рисование окон
    for (int i = 0; i < room.window_numb; i++) {
        int x = room.windows[i].x;
        int y = room.windows[i].y;
        image[y][x] = 'W';
    }

    // Рисование дверей
    for (int i = 0; i < room.door_numb; i++) {
        int x = room.doors[i].x;
        int y = room.doors[i].y;
        image[y][x] = 'D';
    }

    // Вывод графического изображения
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%c", image[i][j]);
        }
        printf("\n");
    }

    // Освобождение памяти
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
}

void displayRooms(Room* rooms, int roomCount) {
    for (int i = 0; i < roomCount; i++) {
        printf("Изображение для комнаты %d:\n", i + 1);
        displayRoom(rooms[i]);
    }
}
