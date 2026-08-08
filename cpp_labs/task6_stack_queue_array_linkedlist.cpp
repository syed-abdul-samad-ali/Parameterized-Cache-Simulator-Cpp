

#include <iostream>

const int MAX_SIZE = 100;


struct ArrayStack {
    int data[MAX_SIZE];
    int top = -1;

    bool isEmpty() { return top == -1; }
    bool isFull() { return top == MAX_SIZE - 1; }

    void push(int value) {
        if (isFull()) { std::cout << "ArrayStack overflow\n"; return; }
        data[++top] = value;
    }

    int pop() {
        if (isEmpty()) { std::cout << "ArrayStack underflow\n"; return -1; }
        return data[top--];
    }

    int peek() {
        if (isEmpty()) { std::cout << "ArrayStack is empty\n"; return -1; }
        return data[top];
    }
};


struct ArrayQueue {
    int data[MAX_SIZE];
    int front = 0, rear = -1, count = 0;

    bool isEmpty() { return count == 0; }
    bool isFull() { return count == MAX_SIZE; }

    void enqueue(int value) {
        if (isFull()) { std::cout << "ArrayQueue overflow\n"; return; }
        rear = (rear + 1) % MAX_SIZE;
        data[rear] = value;
        count++;
    }

    int dequeue() {
        if (isEmpty()) { std::cout << "ArrayQueue underflow\n"; return -1; }
        int value = data[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return value;
    }
};


struct StackNode {
    int value;
    StackNode* next;
};

struct LinkedListStack {
    StackNode* top = nullptr;

    bool isEmpty() { return top == nullptr; }

    void push(int value) {
        StackNode* node = new StackNode{value, top};
        top = node;
    }

    int pop() {
        if (isEmpty()) { std::cout << "LinkedListStack underflow\n"; return -1; }
        StackNode* node = top;
        int value = node->value;
        top = top->next;
        delete node;
        return value;
    }
};


struct QueueNode {
    int value;
    QueueNode* next;
};

struct LinkedListQueue {
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;

    bool isEmpty() { return front == nullptr; }

    void enqueue(int value) {
        QueueNode* node = new QueueNode{value, nullptr};
        if (isEmpty()) {
            front = rear = node;
        } else {
            rear->next = node;
            rear = node;
        }
    }

    int dequeue() {
        if (isEmpty()) { std::cout << "LinkedListQueue underflow\n"; return -1; }
        QueueNode* node = front;
        int value = node->value;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete node;
        return value;
    }
};

int main() {
    std::cout << "-- Array Stack --\n";
    ArrayStack aStack;
    aStack.push(1); aStack.push(2); aStack.push(3);
    std::cout << aStack.pop() << " " << aStack.pop() << " " << aStack.pop() << std::endl;

    std::cout << "-- Array Queue --\n";
    ArrayQueue aQueue;
    aQueue.enqueue(1); aQueue.enqueue(2); aQueue.enqueue(3);
    std::cout << aQueue.dequeue() << " " << aQueue.dequeue() << " " << aQueue.dequeue() << std::endl;

    std::cout << "-- Linked List Stack --\n";
    LinkedListStack llStack;
    llStack.push(10); llStack.push(20); llStack.push(30);
    std::cout << llStack.pop() << " " << llStack.pop() << " " << llStack.pop() << std::endl;

    std::cout << "-- Linked List Queue --\n";
    LinkedListQueue llQueue;
    llQueue.enqueue(10); llQueue.enqueue(20); llQueue.enqueue(30);
    std::cout << llQueue.dequeue() << " " << llQueue.dequeue() << " " << llQueue.dequeue() << std::endl;

    return 0;
}
