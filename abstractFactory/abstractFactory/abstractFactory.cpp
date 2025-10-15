#include <iostream>
#include <string>
#include <memory>
#include <map>

class AudioTrack {
public:
  virtual ~AudioTrack() = default;
  virtual void play() const = 0;
  virtual std::string getLanguage() const = 0;
};

class Subtitles {
public:
  virtual ~Subtitles() = default;
  virtual void display() const = 0;
  virtual std::string getLanguage() const = 0;
};

class RussianAudio : public AudioTrack {
public:
  void play() const override {
    std::cout << "Воспроизведение русской звуковой дорожки" << std::endl;
  }

  std::string getLanguage() const override {
    return "Russian";
  }
};

class RussianSubtitles : public Subtitles {
public:
  void display() const override {
    std::cout << "[Русские субтитры: Привет, мир!]" << std::endl;
  }

  std::string getLanguage() const override {
    return "Russian";
  }
};

class EnglishAudio : public AudioTrack {
public:
  void play() const override {
    std::cout << "Playing English audio track" << std::endl;
  }

  std::string getLanguage() const override {
    return "English";
  }
};

class EnglishSubtitles : public Subtitles {
public:
  void display() const override {
    std::cout << "[English subtitles: Hello, world!]" << std::endl;
  }

  std::string getLanguage() const override {
    return "English";
  }
};

class FilmFactory {
public:
  virtual ~FilmFactory() = default;
  virtual std::unique_ptr<AudioTrack> createAudioTrack() const = 0;
  virtual std::unique_ptr<Subtitles> createSubtitles() const = 0;
  virtual std::string getLanguage() const = 0;
};

class RussianFilmFactory : public FilmFactory {
public:
  std::unique_ptr<AudioTrack> createAudioTrack() const override {
    return std::make_unique<RussianAudio>();
  }

  std::unique_ptr<Subtitles> createSubtitles() const override {
    return std::make_unique<RussianSubtitles>();
  }

  std::string getLanguage() const override {
    return "Russian";
  }
};

class EnglishFilmFactory : public FilmFactory {
public:
  std::unique_ptr<AudioTrack> createAudioTrack() const override {
    return std::make_unique<EnglishAudio>();
  }

  std::unique_ptr<Subtitles> createSubtitles() const override {
    return std::make_unique<EnglishSubtitles>();
  }

  std::string getLanguage() const override {
    return "English";
  }
};

class Movie {
private:
  std::string title;
  std::unique_ptr<AudioTrack> audio_track;
  std::unique_ptr<Subtitles> subtitles;
  std::string language;

public:
  Movie(const std::string& title_, std::unique_ptr<FilmFactory> factory_)
    : title(title_), language(factory_->getLanguage()) {
    audio_track = factory_->createAudioTrack();
    subtitles = factory_->createSubtitles();
  }

  void play() const {
    std::cout << std::endl << "=== Воспроизведение фильма: " << title << " ===" << std::endl;
    std::cout << "Язык: " << language << std::endl;
    audio_track->play();
    subtitles->display();
    std::cout << "==================================" << std::endl;
  }

  void changeLanguage(std::unique_ptr<FilmFactory> factory_) {
    language = factory_->getLanguage();
    audio_track = factory_->createAudioTrack();
    subtitles = factory_->createSubtitles();
    std::cout << "Язык фильма изменен на: " << language << std::endl;
  }

  std::string getCurrentLanguage() const {
    return language;
  }

  std::string getTitle() const {
    return title;
  }
};

class CinemaRentalSystem {
private:
  std::map<std::string, std::unique_ptr<FilmFactory>> factories;
  std::map<std::string, std::string> available_movies;

public:
  CinemaRentalSystem() {
    registerFactory("russian", std::make_unique<RussianFilmFactory>());
    registerFactory("english", std::make_unique<EnglishFilmFactory>());

    available_movies = {
        {"1", "Матрица"},
        {"2", "Властелин колец"},
        {"3", "Хоббиты"},
        {"4", "Гарри Поттер"},
        {"5", "Нарния"}
    };
  }

  void registerFactory(const std::string& language_, std::unique_ptr<FilmFactory> factory_) {
    factories[language_] = std::move(factory_);
  }

  void displayAvailableMovies() const {
    std::cout << std::endl << "=== ДОСТУПНЫЕ ФИЛЬМЫ ===" << std::endl;
    for (const auto& [id, title] : available_movies) {
      std::cout << id << ". " << title << std::endl;
    }
    std::cout << "========================" << std::endl;
  }

  void displayAvailableLanguages() const {
    std::cout << std::endl << "=== ДОСТУПНЫЕ ЯЗЫКИ ===" << std::endl;
    for (const auto& [language, _] : factories) {
      std::cout << "- " << language << std::endl;
    }
    std::cout << "=======================" << std::endl;
  }

  std::unique_ptr<Movie> rentMovie(const std::string& movieId_, const std::string& language_) const {
    auto movieIt = available_movies.find(movieId_);
    auto factoryIt = factories.find(language_);

    if (movieIt == available_movies.end()) {
      std::cout << "Фильм с ID " << movieId_ << " не найден!" << std::endl;
      return nullptr;
    }

    if (factoryIt == factories.end()) {
      std::cout << "Язык " << language_ << " не поддерживается!" << std::endl;
      return nullptr;
    }

    std::string title = movieIt->second;

    if (language_ == "russian") {
      return std::make_unique<Movie>(title, std::make_unique<RussianFilmFactory>());
    }
    else if (language_ == "english") {
      return std::make_unique<Movie>(title, std::make_unique<EnglishFilmFactory>());
    }

    return nullptr;
  }

  bool isLanguageSupported(const std::string& language_) const {
    return factories.find(language_) != factories.end();
  }
};

int main() {
  setlocale(LC_ALL, "Russian");

  CinemaRentalSystem rentalSystem;

  std::cout << "=== СИСТЕМА КИНОПРОКАТА ===" << std::endl;

  rentalSystem.displayAvailableMovies();
  rentalSystem.displayAvailableLanguages();

  std::cout << std::endl << "--- Пример 1: Аренда 'Матрицы' на русском ---" << std::endl;
  auto movie1 = rentalSystem.rentMovie("1", "russian");
  if (movie1) {
    movie1->play();
  }

  std::cout << std::endl << "--- Пример 2: Аренда 'Властелина колец' на английском ---" << std::endl;
  auto movie2 = rentalSystem.rentMovie("2", "english");
  if (movie2) {
    movie2->play();
  }

  std::cout << std::endl << "--- Пример 3: Смена языка во время просмотра ---" << std::endl;
  auto movie3 = rentalSystem.rentMovie("3", "english");
  if (movie3) {
    movie3->play();

    std::cout << std::endl << ">>> Смена языка на русский..." << std::endl;
    movie3->changeLanguage(std::make_unique<RussianFilmFactory>());
    movie3->play();

    std::cout << std::endl << ">>> Смена языка на английский..." << std::endl;
    movie3->changeLanguage(std::make_unique<EnglishFilmFactory>());
    movie3->play();
  }

  std::cout << std::endl << "--- Пример 4: Попытка аренды с неподдерживаемым языком ---" << std::endl;
  auto movie4 = rentalSystem.rentMovie("1", "spanish");
  if (!movie4) {
    std::cout << "Не удалось арендовать фильм!" << std::endl;
  }

  std::cout << std::endl << "--- Проверка согласованности языка звука и субтитров ---" << std::endl;
  auto testMovie = rentalSystem.rentMovie("4", "russian");
  if (testMovie) {
    testMovie->play();
    std::cout << "Текущий язык: " << testMovie->getCurrentLanguage() << std::endl;
    std::cout << "✓ Язык звука и субтитров всегда согласованы!" << std::endl;
  }

  return 0;
}