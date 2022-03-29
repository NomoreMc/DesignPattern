#include <iostream>
#include <list>
#include <string>
using namespace std;

class ObserverBase {
public:
    virtual ~ObserverBase() {};
    virtual void Update(string messageFromPublisher) = 0;
};

class SubjectBase {
public:
    virtual ~SubjectBase() {};
    virtual void Attach(ObserverBase *ob) = 0;
    virtual void Detach(ObserverBase *ob) = 0;
    virtual void Notify() = 0;
};

class Subject: public SubjectBase {
public:
    virtual ~Subject() {
        this->observers.clear();
    }
    void Attach(ObserverBase *ob) override {
        observers.push_back(ob);
    }
    void Detach(ObserverBase *ob) override {
        observers.remove(ob);
    }
    void Notify() override {
        int obSize = observers.size();
        cout<<"Currently, We have "<<obSize<<" observers!"<<endl;
        list<ObserverBase *>::iterator iter = observers.begin();
        while (iter != observers.end()) {
            (*iter)->Update(this->messageToObserver);
            ++iter;
        }
    }
    void SetNews(string message) {
        this->messageToObserver = message;
        this->Notify();
    }
private:
    list<ObserverBase *> observers;
    string messageToObserver;
};

class Observer: public ObserverBase {
public:
    Observer() {
        this->publisher = nullptr;
        cout<<"Observer #"<<++Observer::obNum<<" created but not registered!"<<endl;
        this->selfNum = Observer::obNum;
    }
    Observer(Subject *publisher) : publisher(publisher) {
        this->publisher->Attach(this);
        cout<<"Observer #"<<++Observer::obNum<<" registered!"<<endl;
        this->selfNum = Observer::obNum;
    };
    virtual ~Observer() {
        if (this->publisher != nullptr) {
            this->publisher->Detach(this);
            this->publisher = nullptr;
        }
    }
    void Update(string messageFromPublisher) override {
        this->messageFromPublisher = messageFromPublisher;
        cout<<"Observer #"<<this->selfNum<<" received message from publisher: "<<this->messageFromPublisher<<endl;
    }
    void Subscribe(Subject *publisher) {
        this->publisher = publisher;
        this->publisher->Attach(this);
        cout<<"Observer #"<<this->selfNum<<" subscribed!"<<endl;
    }
    void UnSubscribe() {
        if (this->publisher != nullptr) {
            this->publisher->Detach(this);
            this->publisher = nullptr;
        }
    }
private:
    static int obNum;
    Subject *publisher;
    int selfNum;
    string messageFromPublisher;
};

int Observer::obNum = 0;

void test() {
    Subject *subject = new Subject();

    Observer *ob1 = new Observer(subject);
    Observer *ob2 = new Observer(subject);
    subject->SetNews("hello1");
    Observer *ob3 = new Observer(subject);
    Observer *ob4 = new Observer(subject);
    subject->SetNews("hello1");
    Observer *ob5 = new Observer();
    ob5->Subscribe(subject);
    ob3->UnSubscribe();
    ob4->UnSubscribe();
    subject->SetNews("hello3");

    delete ob1;
    delete ob2;
    delete ob3;
    delete ob4;
    delete ob5;
    delete subject;
}

int main(int argc, char **argv) {
    test();
    return 0;
}