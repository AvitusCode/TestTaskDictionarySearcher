#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTimerEvent>

#include "AhoCore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dictionary; }
QT_END_NAMESPACE

class Dictionary : public QWidget
{
    Q_OBJECT

public:
    Dictionary(QWidget *parent = nullptr);
    int readDictionatyFile(const char* filename);
    void findMatches();
    ~Dictionary();
protected:
   void timerEvent(QTimerEvent *e);
private slots:
    void slotText();
private:
    QTextEdit *edit1;
    QPlainTextEdit *edit2;
    char *text_buffer;
    AhoCore ac;

    int timerId;
    bool paused, text_changed;
    static const int DELAY = 1000; // a little delay for the user.

    Ui::Dictionary *ui;
};
#endif // DICTIONARY_H
