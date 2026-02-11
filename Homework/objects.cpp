#include <iostream>
#include <string>

using namespace std;

/*

HELLO EVERYONE! THIS IS A SIMPLE C++ PROGRAM TO DEMONSTRATE THE CONCEPT OF OBJECTS AND CLASSES.

IF YOU THINK ABOUT THIS IS SOME KIND OF CALCULATOR, BUT INSTEAD OF CALCULATING NUMBERS, IT CALCULATES THE ENERGY LEVEL OF A DOG NAMED MAX.

IT CAN BE MORE PLAYFULL THAN A CALCULATOR, BECAUSE IT CAN BARK, EAT, REST, PLAY AND WALK. ALL THESE ACTIONS AFFECT THE ENERGY LEVEL OF MAX.

YOU CAN CUSTOMIZE THE NAME, CREATE NEW FUNCTIONS, OR EVEN CREATE NEW ANIMAL CLASSES LIKE CAT, BIRD, ETC. THE POSSIBILITIES ARE ENDLESS!

*/

class Animal {
    private:
        int id;
    protected:
        int energy = 100;
    public:
        string name;

    Animal(int id, string name, int energy) {
        this->id = id;
        this->name = name;
        this->energy = energy;
    }

    void showId() {
        cout << name << " has id = " << id << endl;
    }
};

class Dog : public Animal {
    public:
        Dog(int id, string name, int energy) : Animal(id, name, energy) {}

        void bark() {
            cout << name << " Woof!" << endl;
        }

        void eat() {
            energy += 10;
            cout << name << " ate and now has energy = " << energy << endl;
        }

        void rest() {
            energy += 50;
            cout << name << " rested and now has energy = " << energy << endl;
        }

        void play() {
            energy -= 40;
            cout << name << " played and now has energy = " << energy << endl;
        }

        void walk() {
            energy -= 20;
            cout << name << " walked and now has energy = " << energy << endl;
        }
};

int main() {

    cout << "Creating a dog named Max with id 1 and energy 100" << endl;

    /*
    
    HERE WE COULD HAVE A WHILE LOOP TO INTERACT WITH THE USER 

    HE CAN CHOOSE TO BARK, EAT, REST, PLAY OR WALK, AND THE PROGRAM WILL UPDATE MAX'S ENERGY LEVEL ACCORDINGLY.

    GREETIGS FROM YOUR FAVORITE PROFESSOR! HOPE YOU ENJOYED THIS SIMPLE DEMONSTRATION OF OBJECTS AND CLASSES IN C++.

    */

    return 0;
}