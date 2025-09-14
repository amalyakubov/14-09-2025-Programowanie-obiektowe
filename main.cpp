#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

class Address {
private:
  std::string city;
  std::string street;

public:
  std::string toString() const {
    return std::format("city={}, street={}", this->city, this->street);
  }

  Address(std::string city, std::string street) : city(city), street(street) {}
};

class Employee {
private:
  std::string name;
  Address address;

public:
  virtual double calculateSalary() = 0;

  virtual std::string toString() = 0;

  std::string getName() { return this->name; }

  Address getAddress() { return this->address; }

  Employee(std::string name, std::string city, std::string street)
      : name(name), address(Address(city, street)) {}
};

class Driver : public Employee {
private:
  std::string licenseCategory;

public:
  double calculateSalary() override { return 4000; }

  std::string toString() override {
    return std::format("Employee{{name={},Address={}, licenseCategory={}}}",
                       this->getName(), this->getAddress().toString(),
                       this->licenseCategory);
  }
  Driver(std::string name, std::string city, std::string street,
         std::string licenseCategory)
      : Employee(name, city, street), licenseCategory(licenseCategory) {}
};

class Mechanic : public Employee {
private:
  int repairedVehhicles;

public:
  double calculateSalary() override { return 6000; }
  std::string toString() override {
    return std::format("Employee{{name={},Address={}, repairedVehicles={}}}",
                       this->getName(), this->getAddress().toString(),
                       this->repairedVehhicles);
  }
  Mechanic(std::string name, std::string city, std::string street,
           int repairedVehicles)
      : Employee(name, city, street), repairedVehhicles(repairedVehhicles) {}
};

class Manager : public Employee {
private:
  double bonus;

public:
  double calculateSalary() override { return 9000; }
  std::string toString() override {
    return std::format("Employee{{name={},Address={}, bonus={}}}",
                       this->getName(), this->getAddress().toString(),
                       this->bonus);
  }
  Manager(std::string name, std::string city, std::string street, double bonus)
      : Employee(name, city, street), bonus(bonus) {}
};

class Loadable {
  virtual void loadCargo(std::string cargo) = 0;
};

class Serviceable {
  virtual void service(std::shared_ptr<Mechanic> m) = 0;
};

class Vehicle : Loadable, Serviceable {
private:
  std::string model;
  int capacity;

public:
  virtual void drive() = 0;
  virtual std::string toString() = 0;
  void info() {
    std::cout << std::format("Pojazd: {} ({} miejsca/ladownosci)", this->model,
                             this->capacity);
  }
  void service(std::shared_ptr<Mechanic> m) override {
    std::cout << std::format("{} serwisuje {}\n", m->getName(), this->model);
  }

  std::string getModel() { return this->model; }

  int getCapacity() { return this->capacity; }

  Vehicle(std::string model, int capacity)
      : model(model), capacity(capacity) {};
};

class Car : public Vehicle {
public:
  void drive() override { std::cout << "Samochód jedzie\n"; }
  std::string toString() override {
    return std::format("Model={}, capacity={}", this->getModel(),
                       this->getCapacity());
  }

  void loadCargo(std::string cargo) override {
    std::cout << std::format("{} załadowano w samochód {}", cargo,
                             this->getModel());
  }

  Car(std::string model, int capacity) : Vehicle(model, capacity) {};
};

class Truck : public Vehicle {
  void drive() override { std::cout << "Ciężarówka jedzie\n"; }

  std::string toString() override {
    return std::format("Model={}, capacity={}", this->getModel(),
                       this->getCapacity());
  }

  void loadCargo(std::string cargo) override {
    std::cout << std::format("{}  załadaowano w ciężarówkę {}", cargo,
                             this->getModel());
  }
};

class Bus : public Vehicle {
public:
  void drive() override { std::cout << "Autobus wiezie pasażerów\n"; }

  std::string toString() override {
    return std::format("Model={}, capacity={}", this->getModel(),
                       this->getCapacity());
  }

  void loadCargo(std::string cargo) override {
    std::cout << std::format("{} załadowano w ciężarówkę {}\n", cargo,
                             this->getModel());
  }

  Bus(std::string model, int capacity) : Vehicle(model, capacity) {};
};

class Branch {
private:
  std::string branchName;
  Address address;
  std::vector<std::shared_ptr<Employee>> listaPracownikow;
  std::vector<std::shared_ptr<Vehicle>> listaPojazdow;

public:
  void addEmployee(std::shared_ptr<Employee> e) {
    this->listaPracownikow.push_back(std::move(e));
  }

  void addVehicle(std::shared_ptr<Vehicle> v) {
    this->listaPojazdow.push_back(std::move(v));
  }

  void showBranchInfo() const {
    std::cout << "Branch Name: " << branchName << "\n";
    std::cout << "Address: " << address.toString() << "\n";

    std::cout << "Employees:\n";
    for (const auto &emp : listaPracownikow)
      std::cout << "  - " << emp->toString() << "\n";

    std::cout << "Vehicles:\n";
    for (const auto &v : listaPojazdow)
      std::cout << "  - " << v->toString() << "\n";
  }

  std::vector<Vehicle *> getVehicles() {
    std::vector<Vehicle *> vehicles;
    for (auto &v : listaPojazdow)
      vehicles.push_back(v.get());
    return vehicles;
  }

  Branch(std::string branchName, std::string city, std::string street)
      : branchName(branchName), address(city, street) {}
};

class Cargo {
private:
  std::string description;
  double weight;
  Address destination;
  std::shared_ptr<Vehicle> pojazd;
  std::shared_ptr<Driver> kierowca;

public:
  std::string toString() const {
    return std::format("Cargo={{description={}, weight={}, destination={}}}",
                       this->description, this->weight,
                       this->destination.toString());
  }
  void assignVehicle(std::shared_ptr<Vehicle> v, std::shared_ptr<Driver> d) {
    this->pojazd = std::move(v);
    this->kierowca = std::move(d);
  }

  Cargo(std::string description, double weight, std::string city,
        std::string street)
      : description(description), weight(weight), destination(city, street),
        kierowca(nullptr) {};
};

class LogisticsComapny {
private:
  std::string companyName;
  std::vector<Branch> branchList;
  std::vector<Cargo> cargoList;

public:
  void addBranch(Branch b) { this->branchList.push_back(b); }
  void addCargo(Cargo &c) { this->cargoList.push_back(std::move(c)); }
  void showCompanyInfo() const {
    std::cout << companyName << "\n";
    for (auto branch : branchList) {
      branch.showBranchInfo();
    }
    for (auto &cargo : cargoList) {
      std::cout << cargo.toString() << "\n";
    }
  }

  std::vector<Branch> getBranhches() { return this->branchList; }

  LogisticsComapny(std::string companyName) : companyName(companyName) {}
};

int main() {
  auto Andrzej = std::make_shared<Driver>("Andrzej", "Warszawa",
                                          "Aleja Rzeczypospolitej 3A", "B");
  auto Mikolaj = std::make_shared<Mechanic>("Mikołaj", "Warszawa",
                                            "Typowe Warszawaskie Za#upie", 12);
  auto Jacek = std::make_shared<Manager>(
      "Jacek", "Warszawa", "Wilanów (no a gdzie indziej)", 90000.0);

  auto id3 = std::make_shared<Car>("Volkswagen Id3", 4);
  auto solaris = std::make_shared<Bus>("Solaris", 40);

  Branch warszawa("Warszawa", "Warszawa", "ul. Długa");

  // if addEmployee takes ownership:
  warszawa.addEmployee(Andrzej);
  warszawa.addEmployee(Mikolaj);
  warszawa.addEmployee(Jacek);

  warszawa.addVehicle(id3);
  warszawa.addVehicle(solaris);

  auto AndrzejZKrakowa = std::make_shared<Driver>(
      "Andrzej Z Krakowa", "Kraków", "Aleja Rzeczypospolitej 3A", "B");
  auto MikolajZKrakowa = std::make_shared<Mechanic>(
      "Mikołaj z Krakowa", "Kraków", "Typowe Krakowskie Za#upie", 12);
  auto JacekZKrakowa = std::make_shared<Manager>(
      "Jacek", "Kraków", "nwm jakieś bogate osiedle", 100000.0);

  auto id4 = std::make_shared<Car>("Volkswagen Id4", 4);
  auto solaris2 = std::make_shared<Bus>("Solaris model nwm", 40);
  Bus &solaris2ref = *solaris2;

  solaris2ref.loadCargo("Pszenica");

  Branch krakow("Kraków", "Kraków", "ul. Wawelska");

  // if addEmployee takes ownership:
  krakow.addEmployee(AndrzejZKrakowa);
  krakow.addEmployee(MikolajZKrakowa);
  krakow.addEmployee(JacekZKrakowa);

  krakow.addVehicle(id4);
  krakow.addVehicle(solaris2);

  LogisticsComapny Transmax("Transmax");
  Transmax.addBranch(krakow);
  Transmax.addBranch(warszawa);

  Transmax.showCompanyInfo();

  auto cargo1 = Cargo("Pszenica I guess", 90.0, "Olsztyn", "Spacerowa");
  cargo1.assignVehicle(solaris2, AndrzejZKrakowa);

  for (auto branch : Transmax.getBranhches()) {
    for (auto &v : branch.getVehicles()) {
      v->drive();
      v->service(Mikolaj);
    }
  }
}

// wiem że ten kod jest okropny ale to wina c++ a nie moja.