

#include <iostream>
#include<thread>
#include <mutex>
#include<semaphore>

int cnt = 0;
void increment() {
    for (int i = 0;i < 10000000;i++) {
        cnt++;
    }
}
int cnt_mutex = 0;
std::mutex mtx;
void increment_mutex() {
    for (int i = 0;i < 10000000;i++) {
        mtx.lock();
        cnt_mutex++;
        mtx.unlock();

    }
}
int cnt_semaphore = 0;
std::binary_semaphore sem(1);
void increment_semaphore() {
    for (int i = 0;i < 10000000;i++) {
        sem.acquire();
        cnt_semaphore++;
        sem.release();

    }
}
class ThreadList {
private:
    struct Node
    {
        int value;
        Node* next;
        Node(int val) :value(val), next(nullptr) {}
    };
    Node* head;
    std::mutex mtx_list;
public:
    ThreadList():head(nullptr){}
    void addNode(int val, int pos) {
        mtx_list.lock();
        Node* newNode = new Node(val);
        if (pos <= 0||!head) {
            newNode->next = head;
            head = newNode;
            mtx_list.unlock();
            return;
        }
        
            Node* tmp = head;
            int counter = 0;
            while (tmp->next&&counter<pos-1) {
                tmp = tmp->next;
                counter++;
            }
            newNode->next = tmp->next;
            tmp->next = newNode;
        mtx_list.unlock();
    }
    void print() {
        mtx_list.lock();
        Node* tmp = head;
        while (tmp) {
        std::cout << tmp->value << " ,";
        tmp = tmp->next;
        }
        std::cout << std::endl;
        mtx_list.unlock();
    }


};

int main()
{
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    std::cout <<"unsafe thread: "<< cnt << std::endl;
    std::thread t3(increment_mutex);
    std::thread t4(increment_mutex);
    t3.join();
    t4.join();
    std::cout << "mutex thread: " << cnt_mutex << std::endl;
    std::thread t5(increment_semaphore);
    std::thread t6(increment_semaphore);
    t5.join();
    t6.join();
    std::cout << "semaphore thread: " << cnt_semaphore << std::endl;
    ThreadList list;
    list.addNode(-1, 0);
    list.addNode(54, 1);
    list.addNode(6, 2);
    list.addNode(12, 1);

    list.print();


}


