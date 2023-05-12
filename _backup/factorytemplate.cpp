#include <iostream>
#include <map>
#include <memory>

// Base class
class MyBaseClass {
public:
    virtual void doSomething() = 0;
    virtual ~MyBaseClass() {}
};

// Derived class 1
class MyDerivedClass1 : public MyBaseClass {
public:
    void doSomething() override {
        std::cout << "Doing something in MyDerivedClass1" << std::endl;
    }
};

// Derived class 2
class MyDerivedClass2 : public MyBaseClass {
public:
    void doSomething() override {
        std::cout << "Doing something in MyDerivedClass2" << std::endl;
    }
};

// Factory class
class MyFactory {
public:
    // Function pointer type for creating objects
    using CreateFunc = std::unique_ptr<MyBaseClass> (*)();

    // Register a class with the factory
    void registerClass(const std::string& className, CreateFunc createFunc) {
        classRegistry_[className] = createFunc;
    }

    // Create an object based on the specified class name
    std::unique_ptr<MyBaseClass> createObject(const std::string& className) {
        auto it = classRegistry_.find(className);
        if (it != classRegistry_.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    std::map<std::string, CreateFunc> classRegistry_;
};

int main() {
    MyFactory factory;

    // Register the classes with the factory
    factory.registerClass("DerivedClass1", []() {
        return std::make_unique<MyDerivedClass1>();
    });

    factory.registerClass("DerivedClass2", []() {
        return std::make_unique<MyDerivedClass2>();
    });

    // Create objects based on the specified class names
    std::string className1 = "DerivedClass1";
    std::unique_ptr<MyBaseClass> object1 = factory.createObject(className1);
    if (object1) {
        object1->doSomething();
    }

    std::string className2 = "DerivedClass2";
    std::unique_ptr<MyBaseClass> object2 = factory.createObject(className2);
    if (object2) {
        object2->doSomething();
    }

    return 0;
}
