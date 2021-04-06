#include "dictionary.h"
#include "ui_dictionary.h"

#include <QString>
#include <QHBoxLayout>
#include <fstream>
#include <functional>

Dictionary::Dictionary(QWidget *parent) : QWidget(parent), paused(true), text_changed(false), ui(new Ui::Dictionary)
{
    ui->setupUi(this);

    edit1 = new QLineEdit(this);
    edit2 = new QPlainTextEdit("",this);

    edit1->setMaxLength(MAX_WORD);
    edit2->setReadOnly(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(edit1);
    layout->setAlignment(edit1, Qt::AlignTop);
    layout->addWidget(edit2);

    setLayout(layout);

    connect(edit1, &QLineEdit::textChanged, this, &Dictionary::slotText);

    timerId = startTimer(DELAY);
}

void Dictionary::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if (!paused && text_changed) // Временно приостанавливаем вычисления, чтобы дать пользователю ввести данные и избежать артефактов
    {
        findMatches();
        paused = true;
        text_changed = false;
    }
    else{
        paused = false;
    }
}

void Dictionary::findMatches()
{
    QString newtext = "\n";
    QString matches = "Matches found: ";
    size_t count = 0;

    auto callback = [&newtext, &count](const QString& substr)
    {
        newtext += substr + '\n'; count++;
    };

    // Обращаемся к функции search(), чтобы найти совпадения.
    ac.search(text_buffer, std::ref(callback));

    matches += QString::number(count) + "\n" + newtext;
    edit2->setPlainText(matches);
}

void Dictionary::slotText()
{
    paused = true;
    text_changed = true;

    QString text = edit1->text(); // Получаем и приводим текст из поля ввода к более удобному представлению
    QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;]+");
    QStringList list = text.split(reg, Qt::SkipEmptyParts);

    list.removeDuplicates();
    list.sort();

    // Вводим новые данные в структуру данных Бор и строим конечный автомат
   ac.clear();
   foreach (const QString &str, list)
   {
       ac.addString(str);
   }
   ac.init();
}


int Dictionary::readDictionatyFile(const char* filename)
{
    std::ifstream input(filename);

    if (input.is_open())
    {
        size_t sz = input.seekg(0, std::ios::end).tellg();
        input.seekg(0, std::ios::beg);
        text_buffer = new char[sz + 1];

        if (text_buffer)
        {
            input.read(text_buffer, sz);
            text_buffer[sz] = '\0';

            for (size_t i = 0; i < sz; i++)
                if (text_buffer[i] == '\n')
                    text_buffer[i] = ' ';
        }

        input.close();
    }

    return 0;
}

Dictionary::~Dictionary()
{
    delete edit1;
    delete edit2;
    delete[] text_buffer;
    delete ui;
}

