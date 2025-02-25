/**
 * \file
 * \brief Непосредственно решение лабораторной работы
 */


#include <iostream>
#include <fstream>
#include "lib.h"

/**
 * Основной алгоритм заключается в том, что при считывании текста создается двумерный массив dots типа int,
 * в каждом элементе которого хранится длина одного предложения, номера его первого и последнего элемента.
 * Затем этот массив сортируется по длине предложений с помощью функции sort,
 * после чего на его основе формируется вывод.
 *
 * Решение задачи на 9.
 * Задача решается на основе уже отсрортированного массива dots.
 * Цикл ищет пробел и знаки препинания, по которым можно выделить слова в предложении,
 * с конца (то есть справа налево) самого длиннго предложения, границы которого указаны в последнем элементе dots.
 * При нахождении одного из разделительных символов слово выводиться слева направо.
 *
 * \param argc количество флагов при вызовы функции из консоли
 * \param argv массив этих флагов
 * \return при выполнении без ошибок вернет 0, при ошибки ввода -1.
 */
int main(int argc, char** argv) {
    int args = check_args(argc, argv);
    if(args == -1){
        std::cerr << "Wrong arguments.";
        return -1;
    }

    noskipws(std::cin);

    //считывание названий файлов при наличие флагов
    char* input_file = new char[50];
    if(args == 1 || args == 3){
        std::cout << "Please, type a filename to read from.\n";
        int i = 0;
        while(std::cin >> input_file[i]){
            ++i;
            if(std::cin.peek() == '\n'){
                break;
            }
        }
    }

    char* output_file = new char[50];
    if(args == 2 || args == 3){
        std::cout << "Please, type a filename to write in.\n";
        int i = 0;
        while(std::cin >> output_file[i]){
            if(output_file[i] == '\n'){
                continue;
            }
            ++i;
            if(std::cin.peek() == '\n'){
                break;
            }
        }
    }


    //считывание предложений
    char* str = new char[1000];
    int** dots = new int*[200];
    int i = 0, dots_count = 0;
    int pre_start = 0;
    if(args == 1 || args == 3){
        std::ifstream input(input_file);
        if(!input.is_open()){
            std::cerr << "Input file doesn't exist.\n";
            return -1;
        }
        noskipws(input);
        while(input >> str[i]){
            if(str[i] == '\n'){
                str[i] = ' ';
            }
            if(str[i] == '.'){
                dots[dots_count] = new int[3];
                while(str[pre_start] == ' '){++pre_start;}
                dots[dots_count][0] = i - pre_start + 1;
                dots[dots_count][1] = pre_start; dots[dots_count][2] = i;
                pre_start = i + 1;
                ++dots_count;
            }
            ++i;
        }

        input.close();
    }
    else{
        std::cout << "Please, type an expression. Use ctrl + D at the end of it.\n";
        while(std::cin >> str[i]){
            if(str[i] == '\n'){
                str[i] = ' ';
            }
            if(str[i] == '.'){
                dots[dots_count] = new int[3];
                while(str[pre_start] == ' '){++pre_start;}
                dots[dots_count][0] = i - pre_start + 1;
                dots[dots_count][1] = pre_start; dots[dots_count][2] = i;
                pre_start = i + 1;
                ++dots_count;
            }
            ++i;
        }
    }

    //учет неправильного ввода, то есть если предложений нет
    if(dots_count == 0){
        std::cerr << "Nothing found.";
        return -1;
    }

    //сортировка и вывод
    sort(dots, dots_count);

    if(args == 2 || args == 3){
        std::ofstream output(output_file);
        for (int j = 0; j < dots_count; ++j) {
            for (int jj = dots[j][1]; jj <= dots[j][2]; ++jj) {
                output << str[jj];
            }
            output << "\n";
        }
        output.close();

    } else {
        std::cout << "\nAnswer:\n";
        for (int j = 0; j < dots_count; ++j) {
            for (int jj = dots[j][1]; jj <= dots[j][2]; ++jj) {
                std::cout << str[jj];
            }
            std::cout << "\n";
        }
    }

    //доп задание на 9
    int end = dots[dots_count - 1][2];
    std::ofstream nine_out("NINE.txt");
    for(int j = dots[dots_count - 1][2]; j >= dots[dots_count - 1][1]; --j){
        if(str[j] == ' ' || str[j] == ',' || str[j] == ':' || str[j] == ';'){
            for(int jj = j + 1; jj < end; ++jj){
                nine_out << str[jj];
            }
            if(end - (j + 1) >= 1) {nine_out << ' ';}
            end = j;
        }
        if(j == dots[dots_count - 1][1]){
            for(int jj = j; jj < end; ++jj){
                nine_out << str[jj];
            }
        }
    }
    delete []str;
    delete []input_file; delete []output_file;
    for(int j = 0; j < dots_count; ++j){
        delete []dots[j];
    }
    delete []dots;
    return 0;
}
