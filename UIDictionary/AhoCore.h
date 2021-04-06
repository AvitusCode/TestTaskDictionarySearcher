#pragma once
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <functional>

//QT
#include <QString>
/*

 ���������� �� ��������� ������ ������ ��� ������ ��������� ���-��������.
 @data: Node - ���� ��� ��������� ������ ��� (��������� ��� �����, � ��������
 ������ ����� ����� ������ ����������� �������. ��������� ��� - ���
 �������� ������ ��������� ����� ��������� ����� � ���������� ��������).
 ����� ��� ����� ����������������, ��� �������� �������, ������� �������� �� ��
 O(n + t) �������, ��� n - ����� �������� �� ��������� �����, t - ����������
 ���������� ������ (�������� init()).

void addString(const QString& str); - �������� ��������� ������
void clear(); - �������� ���
void step(const QChar c); - ������������ ��������� ��������
void search(const QString& str, Callback callback); - ����� ��������� ��������� � ������ str
� ����� ����� ������� �������� �������� �������� (�� ������� ������) ������ ������ ���������.

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
        Node* prev;  // ���������� ������.
        int out;    //����� : -1, ���� ���� �� �������� ������ �����

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
    AhoCore::Node root; // ������ ����� ���
    AhoCore::Node* current_state; // ��������� ��������
    std::vector<QString> words; // ��� �������� ����� ������� ��������� ������
};
