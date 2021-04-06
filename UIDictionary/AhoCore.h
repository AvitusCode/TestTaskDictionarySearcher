#pragma once
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <functional>

//QT
#include <QString>
/*

 Совпадения по эталонной строке ищутся при помощи алгоритма Ахо-Корасика.
 @data: Node - узел для структуры данных Бор (Формально это древо, у которого
 каждое ребро имеет индекс конкретного символа. Построить Бор - это
 означает задать отношение между вершинами древа и эталонными строками).
 Также Бор можно интерпретировать, как конечный автомат, который строится за за
 O(n + t) времени, где n - набор символов из эталонных строк, t - построение
 суффиксных ссылок (операция init()).

void addString(const QString& str); - Добавить эталонную строку
void clear(); - очистить Бор
void step(const QChar c); - переключение состояния автомата
void search(const QString& str, Callback callback); - поиск вхождений подстроки в строку str
В целом можно ожидать примерно линейную скорость (от входных данных) поиска нужных вхождений.

*/
class AhoCore
{
    using Callback = const std::function<void(const QString& str)> &;

public:
    AhoCore() {}
    void addString(const QString& str);
    void init();
    void clear();
    void step(const QChar c);
    void search(const QString& str, Callback callback);

private:
    AhoCore(const AhoCore&) = delete;
    AhoCore& operator=(const AhoCore&) = delete;

    class Node
    {

    public:
        using LinksMap = std::map<const QChar, Node*>;

        LinksMap links;
        Node* prev;  // суффиксная ссылка.
        int out;    //Метка : -1, если ключ не является концом слова

    public:
        Node(Node* fail_node = NULL) : prev(fail_node), out(-1) { }

        Node* getLink(const QChar c) const
        {
            auto iter = links.find(c);

            if (iter != links.cend())
                return iter->second;
            else
                return NULL;

        }

        bool isTerminal() const {
            return (out >= 0);
        }

        void clear()
        {
            for (auto& [key, value] : links)
                value->clear();
            freeMap();
            links.clear();
            prev = NULL;
            out = -1;
        }

        ~Node(){
            freeMap();
        }

    private:
        void freeMap()
        {
            for (auto& [key, value] : links)
                delete value;
        }
    };

    // Data
    AhoCore::Node root; // Корень древа Бор
    AhoCore::Node* current_state; // Состояние автомата
    std::vector<QString> words; // Для проверки можно хранить эталонные строки
};
