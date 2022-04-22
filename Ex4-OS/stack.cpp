#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "malloc.cpp"

using namespace std;

struct word
{
    string word; // word
    struct word *next; // pointer to next word
};

class Stack
{
private:
    struct word *peek; // pointer to top of stack
    size_t size; // size of stack

public:
    Stack()
    { 
        peek = NULL;
        size = 0;
    };

    ~Stack()
    {
        while (peek != NULL)
        {
            word *tmp = peek;
            peek = peek->next;
            free(tmp);
        }
    };

    void push(string value)
    { // push a value into the stack
        if (value == "")
        {
            cout << "ERROR: invalid input" << endl;
        }
    
        if (peek == NULL) // if the stack is empty.
        {
            peek = (struct word *)Mycalloc(1, sizeof(struct word));
            peek->word = value;
            peek->next = NULL;
            size++;
        }
        else // if the stack is not empty the value will be pushed and the top will be next value.
        {
            struct word *tmp = (struct word *)Mycalloc(1, sizeof(struct word));
            tmp->word = value;
            tmp->next = peek;
            peek = tmp;
            size++;
        }
    };
    string pop()
    { // pop a value from the stack
        if (size > 0)
        {
            word *tmp = peek;
            string value = peek->word;
            if (peek->next != NULL)
            {
                peek = peek->next;
            }
            else
            {
                peek = NULL;
            }
            Myfree(tmp);
            size--;
            return "OUTPUT: " + value;
        }
        else if (size == 0 || peek == NULL)
        {
            cout << "Stack is empty" << endl;
            return "";
        }
    }

    string top()
    { // return the top value of the stack
        if (size > 0)
        {
            return "OUTPUT: " + peek->word;
        }
        else if (size == 0)
        {
            cout << "ERROR : Stack is empty" << endl;
            return "";
        }
    };
    bool isEmpty()
    { // check if the stack is empty
        return (size == 0);
    };
};