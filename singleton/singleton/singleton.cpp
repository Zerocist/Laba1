#include <iostream>
#include <vector>
#include <memory>
#include <mutex>

class Room {
private:
  int roomNumber;
public:
  Room(int number) : roomNumber(number) {}
  int getRoomNumber() const { return roomNumber; }
};

class Floor {
private:
  int floorNumber;
  std::vector<Room> rooms;
public:
  Floor(int number, int roomsPerFloor) : floorNumber(number) {
    for (int i = 1; i <= roomsPerFloor; ++i) {
      rooms.emplace_back(floorNumber * 100 + i);
    }
  }

  int getFloorNumber() const { return floorNumber; }
  const std::vector<Room>& getRooms() const { return rooms; }

  void display() const {
    std::cout << "Этаж " << floorNumber << " содержит " << rooms.size() << " помещений: ";
    for (const auto& room : rooms) {
      std::cout << room.getRoomNumber() << " ";
    }
    std::cout << std::endl;
  }
};

class Elevator {
private:
  static std::unique_ptr<Elevator> instance;
  static std::once_flag initFlag;

  int currentFloor;
  int maxFloors;

  Elevator(int maxFloors) : currentFloor(1), maxFloors(maxFloors) {}
  Elevator(const Elevator&) = delete;
  Elevator& operator=(const Elevator&) = delete;

public:
  static Elevator& getInstance(int maxFloors = 10) {
    std::call_once(initFlag, [maxFloors]() {
      instance.reset(new Elevator(maxFloors));
      });
    return *instance;
  }

  void moveToFloor(int floor) {
    if (floor >= 1 && floor <= maxFloors) {
      currentFloor = floor;
      std::cout << "Лифт переместился на этаж " << currentFloor << std::endl;
    }
    else {
      std::cout << "Неверный номер этажа!" << std::endl;
    }
  }

  void callToFloor(int floor) {
    std::cout << "Вызов лифта на этаж " << floor << std::endl;
    moveToFloor(floor);
  }

  int getCurrentFloor() const { return currentFloor; }
  int getMaxFloors() const { return maxFloors; }

  void displayStatus() const {
    std::cout << "Лифт находится на этаже: " << currentFloor << std::endl;
  }
};

std::unique_ptr<Elevator> Elevator::instance = nullptr;
std::once_flag Elevator::initFlag;

class Building {
private:
  static std::unique_ptr<Building> instance;
  static std::once_flag initFlag;

  std::vector<Floor> floors;
  int floorsCount;
  int roomsPerFloor;

  Building(int floors, int roomsPerFloor) : floorsCount(floors), roomsPerFloor(roomsPerFloor) {
    for (int i = 1; i <= floors; ++i) {
      this->floors.emplace_back(i, roomsPerFloor);
    }
  }
  Building(const Building&) = delete;
  Building& operator=(const Building&) = delete;

public:
  static Building& getInstance(int floors = 10, int roomsPerFloor = 5) {
    std::call_once(initFlag, [floors, roomsPerFloor]() {
      instance.reset(new Building(floors, roomsPerFloor));
      });
    return *instance;
  }

  void displayBuildingInfo() const {
    std::cout << "=== ИНФОРМАЦИЯ О ЗДАНИИ ===" << std::endl;
    std::cout << "Количество этажей: " << floorsCount << std::endl;
    std::cout << "Помещений на этаже: " << roomsPerFloor << std::endl;
    std::cout << "Всего помещений: " << floorsCount * roomsPerFloor << std::endl;

    for (const auto& floor : floors) {
      floor.display();
    }
  }

  const std::vector<Floor>& getFloors() const { return floors; }
  int getFloorsCount() const { return floorsCount; }
  int getRoomsPerFloor() const { return roomsPerFloor; }

  Elevator& getElevator() {
    return Elevator::getInstance(floorsCount);
  }
};

std::unique_ptr<Building> Building::instance = nullptr;
std::once_flag Building::initFlag;

int main() {
  setlocale(LC_ALL, "Russian");

  Building& building = Building::getInstance(5, 3);

  Elevator& elevator = building.getElevator();

  std::cout << "=== СИСТЕМА УПРАВЛЕНИЯ ЗДАНИЕМ И ЛИФТОМ ===" << std::endl;

  building.displayBuildingInfo();
  std::cout << std::endl;

  elevator.displayStatus();
  std::cout << std::endl;

  elevator.callToFloor(3);
  elevator.callToFloor(1);
  elevator.moveToFloor(5);

  std::cout << std::endl;

  Building& anotherBuilding = Building::getInstance(10, 5);
  std::cout << "Проверка единственности здания:" << std::endl;
  std::cout << "Количество этажей в 'новом' здании: " << anotherBuilding.getFloorsCount() << std::endl;
  std::cout << "(должно остаться 5 этажей из первоначальной настройки)" << std::endl;

  std::cout << std::endl;

  Elevator& anotherElevator = Elevator::getInstance(20);
  std::cout << "Проверка единственности лифта:" << std::endl;
  std::cout << "Максимальный этаж 'нового' лифта: " << anotherElevator.getMaxFloors() << std::endl;
  std::cout << "(должно остаться 5 этажей из первоначальной настройки здания)" << std::endl;

  return 0;
}