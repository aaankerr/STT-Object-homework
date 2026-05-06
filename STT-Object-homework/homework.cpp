#include <iostream>
#include <string>

using namespace std;

class Animal {
    private:
        int id;
    protected:
        int energy;
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
    int setEnergyDefaults(){
        if(energy > 100){
            energy = 100;
        }else if(energy < 0){
            energy = 0;
        }
        return energy;
    }
    int getEnergy() {
        return energy;
    }
};
class Dog : public Animal {
    public:
        Dog(int id, string name, int energy) : Animal(id, name, energy) {}

        void bark() {
            cout << name << "barked, 'Woof!'" << endl;
        }

        void eat() {
            energy += 10;
            setEnergyDefaults();
            cout << name << " ate and now has energy = " << energy << endl;
        }

        void rest() {
            energy += 50;
            setEnergyDefaults();
            cout << name << " rested and now has energy = " << energy << endl;
        }

        void play() {
            energy -= 40;
            setEnergyDefaults();
            cout << name << " played and now has energy = " << energy << endl;
        }

        void walk() {
            energy -= 20;
            setEnergyDefaults();
            cout << name << " walked and now has energy = " << energy << endl;
        }
};
class Iguana : public Animal {
    public:
        Iguana(int id, string name, int energy) : Animal(id, name, energy) {}

    void dropTail(){
        energy -= 100;
        setEnergyDefaults();
        cout << name << " lost his tail! He lost all of his energy." << endl;
    }
    void rest() {
        energy += 50;
        setEnergyDefaults();
        cout << name << " rested and now has energy = " << energy << endl;
    }
    void walk() {
        energy -= 20;
        setEnergyDefaults();
        cout << name << " walked and now has energy = " << energy << endl;
    }
    void takeSun(){
        energy += 10;
        setEnergyDefaults();
        cout << name << " took the sun and now has energy = " << energy << endl;
    }
};
class Cat : public Animal {
    public:
        Cat(int id, string name, int energy) : Animal(id, name, energy) {}

        void meow() {
            cout << name << ": Meow! " << endl;
        }

        void eat() {
            energy += 10;
            setEnergyDefaults();
            cout << name << " Yummy! "<<name<< " ate, and now has energy:  " << energy << endl;
        }

        void rest() {
            energy += 50;
            setEnergyDefaults();
            cout << name << " rested and, now has energy:  " << energy << endl;
        }

        void clean() {
            energy -= 40;
            setEnergyDefaults();
            cout << name << " is clean, and now has energy: " << energy << endl;
        }

        void scratch() {
            energy -= 20;
            setEnergyDefaults();
            cout << "Look at this mess! " << name << " scratched everything, and now has energy: " << energy << endl;
        }
};

void dogChosen(){
    int action = 0;
    char rest;
    Dog d(1, "a", 100);
    cout << "Give a name to your dog! " << endl;
    cin >> d.name;
    while(true){
        cout << "What will " << d.name << " do? (1 - 5)" << endl;
        cout << "1. Bark \n2. Eat \n3. Rest \n4. Play \n5. Walk \n6. Exit game" << endl;
        cin >> action;
        if(action == 6){
            break;
        }        
        switch(action){
            case 1: d.bark(); break;
            case 2: d.eat(); break;
            case 3: d.rest(); break;
            case 4: d.play(); break;
            case 5: d.walk(); break;
            default: cout << d.name << " didn't like that!" << endl; break;
        }
        
        if(d.getEnergy() <= 0){
            cout << d.name << " is tired, will they rest? (Y/N)" << endl;
            cin >> rest;
            if(rest == 'Y'){
                d.rest();
            } else if(rest == 'N'){
                cout << d.name << " died" << endl;
                cout << R"(
                .-------.
              /   R.I.P   \
             /             \
            |    2026-2026  |
            |               |
            |               |
            |               |
            |      _   _    |
            |     ( )_( )   |
            |      \_ _/    |
            |        V      |
            |               |
          __|_______________|__
        /                      \
       /________________________\
    )" <<endl;

                break;
            }
        }

    }
}
void catChosen(){
    int action = 0;
    char rest;
    Cat d(1, "a", 100);
    cout << "Give a name to your cat! " << endl;
    cin >> d.name;
    while(true){
        cout << "What will " << d.name << " do? (1 - 5)" << endl;
        cout << "1. Meow \n2. Eat \n3. Rest \n4. Clean \n5. Scratch \n6. Exit game" << endl;
        cin >> action;
        if(action == 6){
            break;
        }        
        switch(action){
            case 1: d.meow(); break;
            case 2: d.eat(); break;
            case 3: d.rest(); break;
            case 4: d.clean(); break;
            case 5: d.scratch(); break;
            default: cout << d.name << " didn't like that!" << endl; break;
        }
     if(d.getEnergy() <= 0){
            cout << d.name << " is tired, will they rest? (Y/N)" << endl;
            cin >> rest;
            if(rest == 'Y'){
                d.rest();
            } else if(rest == 'N'){
                cout << d.name << " died" << endl;
                cout << R"(
                .-------.
              /   R.I.P   \
             /             \
            |    2026-2026  |
            |               |
            |     "GG's"    |
            |               |
          __|_______________|__
        /                      \
       /________________________\
    )" <<endl;

                break;
            }
        }
    }

}
void iguanaChosen(){
    int action = 0;
    char rest;
    Iguana d(1, "a", 100);
    cout << "Give a name to your iguana! " << endl;
    cin >> d.name;
    while(true){
        cout << "What will " << d.name << " do? (1 - 5)" << endl;
        cout << "1. Take the sun \n2. Walk \n3. Rest \n4. Drop Tail \n5. Exit game" << endl;
        cin >> action;
        if(action == 5){
            break;
        }        
        switch(action){
            case 1: d.takeSun(); break;
            case 2: d.walk(); break;
            case 3: d.rest(); break;
            case 4: d.dropTail(); break;
            default: cout << d.name << " didn't like that!" << endl; break;
        }
if(d.getEnergy() <= 0){
            cout << d.name << " is tired, will they rest? (Y/N)" << endl;
            cin >> rest;
            if(rest == 'Y'){
                d.rest();
            } else if(rest == 'N'){
                cout << d.name << " died" << endl;
                cout << R"(
                .-------.
              /   R.I.P   \
             /             \
            |    2026-2026  |
            |               |
            |     ^,,,^     |
            |    ( O_O )    |
            |     \ = /     |
            |      `-'      |
            |               |
          __|_______________|__
        /                      \
       /________________________\
    )" <<endl;

                break;
            }
        }

    }
}

int main() {
    char animal;
    cout << "Choose your animal: Dog, Cat, or Iguana (D, C, I)" << endl;
    cin >> animal;
    switch(animal){
        case 'D': dogChosen(); break; 
        case 'C': catChosen(); break;
        case 'I': iguanaChosen(); break;
        default: cout << "Invalid animal" << endl;
    }
    cout << "Thanks for playing!" << endl;
    return 0;
}