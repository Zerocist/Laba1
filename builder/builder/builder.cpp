#include <iostream>
#include <string>
#include <memory>

class Computer {
private:
  std::string cpu;
  std::string ram;
  std::string storage;
  std::string gpu;
  bool hasBluetooth;
  bool hasWiFi;

public:
  Computer() : hasBluetooth(false), hasWiFi(false) {}

  void setCPU(const std::string& cpu_) { cpu = cpu_; }
  void setRAM(const std::string& ram_) { ram = ram_; }
  void setStorage(const std::string& storage_) { storage = storage_; }
  void setGPU(const std::string& gpu_) { gpu = gpu_; }
  void setBluetooth(bool hasBluetooth_) { hasBluetooth = hasBluetooth_; }
  void setWiFi(bool hasWiFi_) { hasWiFi = hasWiFi_; }

  void display() const {
    std::cout << "Компьютер конфигурации:" << std::endl;
    std::cout << "CPU: " << (cpu.empty() ? "Не указано" : cpu) << std::endl;
    std::cout << "RAM: " << (ram.empty() ? "Не указано" : ram) << std::endl;
    std::cout << "Storage: " << (storage.empty() ? "Не указано" : storage) << std::endl;
    std::cout << "GPU: " << (gpu.empty() ? "Не указано/Интегрированная" : gpu) << std::endl;
    std::cout << "Bluetooth: " << (hasBluetooth ? "Да" : "Нет") << std::endl;
    std::cout << "WiFi: " << (hasWiFi ? "Да" : "Нет") << std::endl << std::endl;
  }
};

class ComputerBuilder {
private:
  std::unique_ptr<Computer> computer;

public:
  ComputerBuilder() : computer(std::make_unique<Computer>()) {}

  ComputerBuilder& setCPU(const std::string& cpu_) {
    computer->setCPU(cpu_);
    return *this;
  }

  ComputerBuilder& setRAM(const std::string& ram_) {
    computer->setRAM(ram_);
    return *this;
  }

  ComputerBuilder& setStorage(const std::string& storage_) {
    computer->setStorage(storage_);
    return *this;
  }

  ComputerBuilder& setGPU(const std::string& gpu_) {
    computer->setGPU(gpu_);
    return *this;
  }

  ComputerBuilder& setBluetooth(bool hasBluetooth_) {
    computer->setBluetooth(hasBluetooth_);
    return *this;
  }

  ComputerBuilder& setWiFi(bool hasWiFi_) {
    computer->setWiFi(hasWiFi_);
    return *this;
  }

  std::unique_ptr<Computer> build() {
    return std::move(computer);
  }
};

int main() {
  setlocale(LC_ALL, "Russian");
  
  std::unique_ptr<Computer> gamingPC = ComputerBuilder()
    .setCPU("AMD Ryzen 9 7950X")
    .setRAM("64GB DDR5")
    .setStorage("4TB NVMe SSD")
    .setGPU("NVIDIA RTX 4090")
    .setBluetooth(true)
    .setWiFi(true)
    .build();

  std::unique_ptr<Computer> officePC = ComputerBuilder()
    .setCPU("Intel Core i3-12100")
    .setRAM("8GB DDR4")
    .setStorage("256GB SSD")
    .setBluetooth(true)
    .build();

  gamingPC->display();
  officePC->display();

  return 0;
}