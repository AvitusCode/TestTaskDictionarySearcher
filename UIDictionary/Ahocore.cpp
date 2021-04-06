#include "AhoCore.h"
#include <QStringRef>

using std::string;
using std::vector;
using std::queue;


void AhoCore::addString(const QString& str)
{
    Node* current_node = &root;
    for (const QChar* ptr = str.cbegin(); ptr < str.cend(); ptr++) // разбираем строку и создаем 'древо'
    {
        Node* child_node = current_node->getLink(*ptr);

        if (!child_node)
        {
            child_node = new Node(&root);
            current_node->links[*ptr] = child_node;
        }
        current_node = child_node;
    }

    current_node->out = words.size();
    words.push_back(str);
}

void AhoCore::init()
{
    std::queue<Node*> q;
    q.push(&root);

    while (!q.empty())
    {
        Node* current_node = q.front();
        q.pop();

        for (const auto& [key, value] : current_node->links)
        {
            const QChar symbol = key;
            Node* child = value;

            // Defining .prev for the childnode

            Node* temp_node = current_node->prev;
            while (temp_node)
            {
                Node* fail_candidate = temp_node->getLink(symbol);
                if (fail_candidate)
                {
                    child->prev = fail_candidate;
                    break;
                }
                temp_node = temp_node->prev;
            }

            q.push(child);
        }
    }
}

void AhoCore::step(const QChar c)
{
    while (current_state)
    {
        Node* candidate = current_state->getLink(c);
        if (candidate)
        {
            current_state = candidate;
            return;
        }
        current_state = current_state->prev;
    }
    current_state = &root;
}

void AhoCore::search(const QString& str, Callback callback)
{
    long int prev_position = 0;

    for (long int position = str.indexOf(' '), cur_pos = 0; cur_pos < str.size(); cur_pos = position + 1, position = str.indexOf(' ', cur_pos))
    {
        position = position == -1 ? str.size() : position;
        current_state = &root;

        for (long int i = cur_pos; i < position; i++)
        {
            step(str.at(i));
            if (current_state->isTerminal())
                callback(str.mid(cur_pos, position - prev_position));
        }

        prev_position = position;
    }
}

void AhoCore::clear()
{
    words.clear();
    current_state = &root;
    current_state->clear();
}
