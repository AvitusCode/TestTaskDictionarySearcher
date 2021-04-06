#include "dictionary.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dictionary w;
    w.setWindowTitle("Simple Dictionary Searcher");
    w.readDictionatyFile("D:/Downloads/8497/TextEditParsing/words.txt"); // Необходимо указать файл со словарем

    w.show();
    return a.exec();
}
