//
// Created by bzahov on 24.11.19 г..
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <list>
#include <exception>
#include <utility>
#include <vector>
#include <regex>
#include <functional>
#include <bits/stdc++.h>

using namespace std;

// ----Const strings
static const char *const MENU_EXIT_OPTIONS = "0.For exit(0 or 'q' or 'Q')";
static const char *const MENU_EXITING = "Exiting menu";
static const char *const MENU_EXITING_WITHOUT_SAVING = "Exiting menu without saving...";
static const char *const MENU_CHOSEN = "Chosen > ";
static const char *const MENU_WRONG_OPTION_ERROR = "Please select right option";

static const char *const ADMIN_MENU_NEW_POINT = "1.Create new Point object/s";
static const char *const ADMIN_MENU_NEW_ROUTE = "2.Create new Route object/s";
static const char *const ADMIN_MENU_NEW_CAR = "3.Create new Car object/s";
static const char *const ADMIN_MENU_NEW_TAXI = "4.Create new Taxi object/s";

static const char *const ADMIN_MENU_NEW_TAXI_NO_ROUTES = "4.Create new Taxi object/s without routes";
static const char *const ADMIN_MENU_CALCULATE_ROUTE_TOTAL_DISTANCE = "5. Calculate total distance for all Routes";
static const char *const ADMIN_MENU_CALCULATE_TAXI_FUEL_USAGE = "6. Calculate fuel per km for all Taxi routes";

static const char *const CAR_MENU_ADD_REG_NUMBER = "1. Add car's Registration Number";
static const char *const CAR_MENU_ADD_MODEL = "2. Add car's model";
static const char *const CAR_MENU_NUM_SEATS = "3. Add number of Seats";
static const char *const CAR_MENU_BRAND = "4. Add Car's brand";
static const char *const CAR_MENU_YEARS_OF_USE = "5. Add car's years of use";
static const char *const CAR_MENU_FUEL_PER_KM = "6. Add car's fuel per km usage";
static const char *const CAR_MENU_SAVE_CAR = "7. Save current car specifications";

static const char *const ROUTE_MENU_ADD_NAME = "1. Add Route's Name";
static const char *const ROUTE_MENU_ADD_DISTANCE = "2. Add Route's distance";
static const char *const ROUTE_MENU_NUM_DAILY_REPEAT = "3. Add number of daily repeats";
static const char *const ROUTE_MENU_ADD_POINT = "4. Add to the Route new point";
static const char *const ROUTE_MENU_CALC_TOTAL_DISTANCE = "5. Calculate Route's total distance";
//static const char *const ROUTE_MENU_FUEL_PER_KM = "6. Add Route's fuel per km usage";
static const char *const ROUTE_MENU_SAVE = "6. Save current Route specifications";

static const char *const POINT_MENU_ADD = "1. Add Point attributes";
static const char *const POINT_MENU_SAVE = "2. Save current Point specifications";

static const char *const TAXI_MENU_ADD_DRIVER_NAME = "1. Add Taxi's Driver Name";
static const char *const TAXI_MENU_ADD_ROUTE = "2. Add Route's information";
static const char *const TAXI_MENU_ADD_CAR = "3. Add Car's information";
static const char *const TAXI_MENU_ADD_NONE = "4. NONE Add to Car new Route";
static const char *const TAXI_MENU_CALC_TOTAL_DISTANCE = "5.Calculate Route's total distance";
//static const char *const TAXI_MENU_FUEL_PER_KM = "6. Add Route's fuel per km usage";
static const char *const TAXI_MENU_SAVE = "6.Save current Taxi's specifications";

//static const char *const TAXI_MENU_ADD_DRIVER_NAME = "1. Add Taxi's Driver Name";


static const char *const FILE_PATH_ROOT = "/home/bzahov/Documents/gitRepos/TU-University-Tasks/Programming Languages(PE) C++/Tasks/TaxiRouteSystem(CourseWork)/car_database.txt";
static const char *const FILE_PATH_CAR = "car_database.txt";
static const char *const FILE_PATH_POINT = "point_database.txt";
static const char *const FILE_PATH_ROUTE = "route_database.txt";
static const char *const FILE_PATH_TAXI = "taxi_database.txt";
static const char *const FILE_PATH_DEBUG = "debug_database.txt";

static const char *const REGEX_DESERIALIZATION_OF_DATA = "([,|>](\\w+)[:])";
static const char *const REGEX_NAME = "([[A-Z a-z])+";
static const char *const REGEX_CAR_NUMBER = "^([A-Z]{2}[0-9]{4}[A-Z]{2})";
static const char *const REGEX_INFO = "([A-Z a-z0-9])+";

static regex regexName(REGEX_NAME);
static regex regexInfo(REGEX_INFO);
static regex regexRegNumb(REGEX_CAR_NUMBER);


static const char *const SERIALIZATION_STR_DEFAULT = "";

static const string PREFIX_POINT = ">PointName";
static const string PREFIX_CAR = ">CarName";
static const string PREFIX_ROUTE = ">RouteName";
static const string PREFIX_TAXI = ">TaxiName";
static const bool DEBUG = false;

static const char *const DELIMITER_START_LINE = ">";

// ---Class declarations--------------

class Serializable;

class InvalidInputException;

class Point;

class Car;

class Taxi;

class Route;

class InvalidInputException : public std::exception {
public:
    string generalInfo = "InvalidInputException in: ";
    string info;

    explicit InvalidInputException(string info) : info(std::move(info)) {}

    string getMessage() const {
        string result = generalInfo + info;
        return result;
    }

    const char *what() const noexcept override {
        return getMessage().c_str();
    }
};


//--------Declaration of Methods---------
template<class T>
static bool getAndIterateEachLineOfFileWithCallbackSaveIntoVector(const string &fileName, vector<T> &dataVector,
                                                                  void (*callback)(const string &, vector<T> &));

template<class T>
static void extractDataFromLineIntoGenericsList(const string &lineOfData, list<T> &list);

template<class T>
static void extractDataFromLineIntoGenericsObj(const string &lineOfData, T &dataObject);

template<class T>
static bool getAndPutFileContentIntoGenericsVector(const string &fileName, vector<T> &vector);

template<class T>
static void extractDataFromLineIntoGenericsVector(const string &lineOfData, vector<T> &vectorWithData);

static void extractDataFromLineIntoPointVector(const string &lineOfData, vector<Point> &pointsVector);

static void extractDataFromLineIntoCarVector(const string &lineOfData, vector<Car> &carsVector);

template<class T>
static void listToString(list<T> list, string &result);

template<class T>
static void stringToList(const string &stringWithData, list<T> &);

static string insertDelimiterAtBeginnig(string &b, const string &delimiter);

static void removeWhiteSpacesOfString(string &stringWithData);

static void deserializeStrData(string &stringWithData);

static void adminMenu() noexcept(false);

static bool pointsMenu(bool singlePick = false);

static bool carsMenu(/*vector<Car> &resultVector,*/ bool singlePick = false);

static bool routesMenu(/*vector<Route> &resultVector,*/ bool singlePick = false);

static bool taxiMenu();

static void loadDataVectorsFromFiles();

static vector<string> split(const string &str, const string &delimiter);

//---------------------

void testTaxiFunctionality();

template<class T>
static bool isItemExistInVector(basic_string<char> identificationName, const vector<T> &dataVector);

static vector<Car> carsVector;
static vector<Route> routesVector;
static vector<Taxi> taxiVector;
static vector<Point> pointVector;

//********
class Serializable {
public:
    virtual void saveToFile(string fileName) = 0;

    virtual bool isObjectInFileDatabase(string filename) = 0;

    virtual string getIdentification() const = 0;

    virtual string toString() const = 0;

    virtual void print() = 0;

    virtual string getFilePath() const = 0;
};

//-------------
static bool isValidCoords(double a, double b);

class Point : Serializable {
private:
    string name;
    double x{};
    double y{};
public:
    Point() {
        setXY(0, 0);
        name = SERIALIZATION_STR_DEFAULT;
    }

    Point(const string &newName, double xx, double yy) {
        setXY(xx, yy);
        setName(name);
    }

    Point(double x, double y) : x(x), y(y) {}

    virtual ~Point() {
        setXY(0, 0);
    }

    double getX() const {
        return x;
    }

    void setX(double newX) noexcept(false) {
        if (newX >= 0) {
            Point::x = newX;
        } else throw InvalidInputException("Point -> setX");
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        Point::y = y;
    }

    const string &getName() const;

    bool isValid() const;

    void setName(const string &newName) noexcept(false);

    void setXY(const double a, const double b) {
        if (isValidCoords(a, b)) {
            x = a;
            y = b;
        } else {
            throw InvalidInputException("Invalid Coords");
        }
    };

    void print() override;

    string toString() const override;

    friend ostream &operator<<(ostream &os, const Point &point) {
        os << point.toString();
        return os;
    }

    friend istream &operator>>(istream &is, Point &point) {
        is >> point.name;
        is >> point.x;
        is >> point.y;
        return is;
    }

public:
    void saveToFile(string fileName) override;

    bool isObjectInFileDatabase(string filename) override;

    bool isEmpty();

    string getIdentification() const override;

    string getFilePath() const override {
        return FILE_PATH_POINT;
    }
};


string Point::getIdentification() const {
    return getName();
}

const string &Point::getName() const {
    return name;
}

void Point::setName(const string &newName) noexcept(false) {
    if (regex_match(newName, regexName)) {
        Point::name = newName;
    } else { throw InvalidInputException("Point: Invalid Name: " + newName); }
}

bool Point::isValid() const {
    return getX() > 0 && getY() > 0;
}

void Point::print() {
    cout << toString();
}

string Point::toString() const {
    return PREFIX_POINT + ":" + getName() + ",X: " + to_string(getX()) + ",Y: " + to_string(getY());
}

void Point::saveToFile(string fileName) {
    if (!isObjectInFileDatabase(fileName)) {
        cout << "saving Point: " << Point::toString() << " to file: " << fileName << endl;;
        std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);
        ofs << this->toString() << endl;
        ofs.close();
    } else {
        cout << "Item Point with Identification Name: " << getIdentification() << " - already in the file" << endl;
    }
}

bool Point::isObjectInFileDatabase(string filename) {
    // TODO: make one more check inside file database
    return isItemExistInVector(getIdentification(), pointVector);
}

bool Point::isEmpty() {
    return name.empty() && x == 0 && y == 0;
}

bool isValidCoords(double a, double b) {
    return a >= 0 && b >= 0;
}

// ---------
class Car : Serializable {
protected:
    string registrationNumber;
    string brand;
    string model;
    int numSeats;
    double fuelPerKm;
    int yearsOfUse;
public:
    Car() {
        brand = SERIALIZATION_STR_DEFAULT;
        model = SERIALIZATION_STR_DEFAULT;
        yearsOfUse = 0;
        numSeats = 0;
        fuelPerKm = 0;
        registrationNumber = SERIALIZATION_STR_DEFAULT;
    }

    Car(const string &brand, const string &model, int yearsOfUse, int numSeats, double fuelPerKm,
        const string &registrationNumber) noexcept(false) {
        setBrand(brand);
        setModel(model);
        setYearsOfUse(yearsOfUse);
        setNumSeats(numSeats);
        setFuelPerKm(fuelPerKm);
        setRegistrationNumber(registrationNumber);
    }

    const string &getRegistrationNumber() const;

    bool setRegistrationNumber(const string &regNumber) noexcept(false);

    const string &getBrand() const {
        return brand;
    }

    // TODO: make input check
    void setBrand(const string &newBrand) noexcept(false) {
        if (regex_match(newBrand, regexInfo)) {
            Car::brand = newBrand;
        } else {
            throw InvalidInputException("Invalid Brand: " + newBrand);
        }
    }

    const string &getModel() const {
        return model;
    }

    void setModel(const string &newModel) {
        if (regex_match(newModel, regexInfo)) {
            this->model = newModel;
        } else { throw InvalidInputException("Car: Invalid Model: " + newModel); }
    }

    int getYearsOfUse() const {
        return yearsOfUse;
    }

    void setYearsOfUse(int newYearsOfUse) noexcept(false) {
        if (newYearsOfUse >= 0) {
            Car::yearsOfUse = newYearsOfUse;
        } else { throw InvalidInputException("Invalid newYearsOfUse"); }

    }

    int getNumSeats() const {
        return numSeats;
    }

    void setNumSeats(int newNumSeats) noexcept(false) {
        if (newNumSeats >= 0) {
            Car::numSeats = newNumSeats;
        } else { throw InvalidInputException("Invalid newNumSeats"); }

    }

    double getFuelPerKm() const {
        return fuelPerKm;
    }

    void setFuelPerKm(double newFuelPerKm) noexcept(false) {
        if (newFuelPerKm >= 0.1) {
            Car::fuelPerKm = newFuelPerKm;
        } else { throw InvalidInputException("Invalid newFuelPerKm"); }

    }

    void print()
    override;

    string toString() const override;

    void saveToFile(string fileName) override;

    bool isObjectInFileDatabase(string filename) override;

//    static std::vector<Car> readAllFromFile(const string& str);

    friend ostream &operator<<(ostream &os, const Car &car) {
        os << car.toString() << endl;
        return os;
    }

    friend istream &operator>>(istream &is, Car &car) {
        //cout<< CAR_MENU_ADD_REG_NUMBER;
        is >> car.registrationNumber;
        //cout << CAR_MENU_BRAND;
        is >> car.brand;
        //cout << CAR_MENU_ADD_MODEL;
        is >> car.model;
        //cout << CAR_MENU_NUM_SEATS;
        is >> car.numSeats;
        is >> car.fuelPerKm;
        //cout << CAR_MENU_YEARS_OF_USE;
        is >> car.yearsOfUse;
        return is;
    }

    string getIdentification() const
    override;

    bool isCarDataValid() const;

    string getFilePath() const override {
        return FILE_PATH_CAR;
    }
};

string Car::getIdentification() const {
    return registrationNumber;
}

string Car::toString() const {
    string result =
            PREFIX_CAR + ": " + getRegistrationNumber() +
            ",brand: " + getBrand() +
            ",model: " + getModel() +
            ",seats: " + to_string(getNumSeats()) +
            ",fuelPerKm: " + to_string(getFuelPerKm()) +
            ",yearsOfUse:" + to_string(getYearsOfUse());
    return result;
}

void Car::print() {
    cout << "Car info:\n" << Car::toString() << endl;
}

const string &Car::getRegistrationNumber() const {
    return registrationNumber;
}

bool Car::setRegistrationNumber(const string &regNum) noexcept(false) {
    if (regex_match(regNum, regexRegNumb)) {
        Car::registrationNumber = regNum;
        return true;
    } else throw InvalidInputException("Wrong registration Number");
    return false;
}

void Car::saveToFile(const string fileName) {
    if (!isObjectInFileDatabase(fileName)) {

        cout << "saving car: " << Car::toString() << " to file: " << fileName << endl;;
        std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);
        ofs << this->toString() << endl;
        ofs.close();
    } else {
        cout << "Item Car with Identification Name: " << getIdentification() << " - already in the file" << endl;
    }
}

bool Car::isObjectInFileDatabase(string filename) {
    // TODO: make one more check inside file database
    return isItemExistInVector(getIdentification(), carsVector);
}

//std::vector<Car> Car::readAllFromFile(const string& str) {
//    std::vector<Car> result;
//    /*char *line= "";
//    size_t len = 0;
//    ssize_t nread;
//    cout << "\n-----g3g---\n";
///*
//
//    stream = fopen(FILE_PATH_CAR, "r");
//    if (stream == NULL) {
//        perror("fopen");
//        throw (InvalidInputException("wrong file path"));
//    }
//
//
//    while ((nread = getline(&line, &len, stream)) != -1) {
//        printf("Retrieved line of length %zu:\n", nread);
//        cout << line;
//        //fwrite(line, nread, 1, stdout);
//    }
//
//    free(line);
//    fclose(stream);
////-----
//    std::ifstream ifs(FILE_PATH_CAR,fstream::in);
//    cout<< ifs.get();
//    ifs.getline(line,256);
//    cout << line;
//    ifs.close();*/
//
//    std::ifstream infile(FILE_PATH_CAR, fstream::in);
//    int a, b;
//    while (infile >> a >> b) {
//        // process pair (a,b)
//    }
//    return result;
//}

bool Car::isCarDataValid() const {
    try {
        Car(brand, model, yearsOfUse, numSeats, fuelPerKm, registrationNumber);
    } catch (InvalidInputException &e) {
        cerr << "Car: " << e.getMessage() << endl;
        return false;
    }
    return true;
}


//------------
class Route : Serializable {
private:
    string name;
    double distance;
    int dailyRepeat;
    list<Point> routePointsList;
public:
    Route() {
        name = "";
        distance = 0;
        dailyRepeat = 0;
    }

    Route(const string &name, double distance, int dailyRepeat) {
        setName(name);
        setDistance(distance);
        setDailyRepeat(dailyRepeat);
    }

    const list<Point> &getRoutePointsList() const;

    void setRoutePointsList(const list<Point> &newRoutePointsList);

    const string &getName() const {
        return name;
    }

    void setName(const string &newName) noexcept(false) {
        if (regex_match(newName, regexName)) {
            Route::name = newName;
        } else throw InvalidInputException("Route: Wrong Name");

    }

    double getDistance() const {
        return distance;
    }

    void setDistance(double dist) { // RENAMED param
        if (dist > 0) {
            Route::distance = dist;
        } else throw InvalidInputException("Route: Distance is too short");
    }

    int getDailyRepeat() const {
        return dailyRepeat;
    }

    void setDailyRepeat(int newDailyRepeat) {
        if (newDailyRepeat > 0) {
            Route::dailyRepeat = newDailyRepeat;
        } else throw InvalidInputException("Route: newDailyRepeat is invalid");

    }

    void addNewRoutePoint(const Point &p);

    string toString() const override;

    void print() override;

    void saveToFile(string fileName) override;

    bool isObjectInFileDatabase(string filename) override;

    friend ostream &operator<<(ostream &os, const Route &route) {
        os << route.toString();
        return os;
    }

    friend istream &operator>>(istream &is, Route &route) {
        is >> route.name;
        is >> route.distance;
        is >> route.dailyRepeat;
        // add inner points data
        do {
            Point point;
            is >> point;
//            cerr << endl << point.toString() << endl;
            if (point.isEmpty()) {
                break;
            }
            route.addNewRoutePoint(point);
        } while (true);
        return is;
    }

    double calcRouteTotalDistance() const {
        return dailyRepeat * distance;
    }

    string getIdentification() const override;

    bool isValid() const;

    string getFilePath() const override {
        return FILE_PATH_ROUTE;
    }
};

string Route::getIdentification() const {
    return name;
}

void Route::addNewRoutePoint(const Point &p) noexcept(false) {
    if (p.isValid()) {
        routePointsList.push_back(p);
    } else
        throw InvalidInputException(
                "Route -> Point " + p.getName() + " with coords(x,y): " + to_string(p.getX()) + "," +
                to_string(p.getY()) + " is invalid ");
}

string Route::toString() const {
    string pointsStr;
    listToString(routePointsList, pointsStr);
    return PREFIX_ROUTE + ":" + name + ",distance: " + to_string(distance) + ",dailyRepeat: " +
           to_string(dailyRepeat) + "" + pointsStr;
}

void Route::print() {
    cout << Route::toString();
}

void Route::saveToFile(string fileName) {

    // check does nested objects are in the database, if not, they will be added
    for (auto point : getRoutePointsList()) {
        point.saveToFile(FILE_PATH_POINT);
    }
    if (!isObjectInFileDatabase(fileName)) {
        cout << "saving Route: " << Route::toString() << " to file: " << fileName << endl;
        std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);

        ofs << this->toString() << endl;
        ofs.close();
    } else {
        cout << "Item Route with Identification Name: " << getIdentification() << " - already in the file" << endl;
    }
}

bool Route::isObjectInFileDatabase(string filename) {
    // TODO: make one more check inside file database
    return isItemExistInVector(getIdentification(), routesVector);
}

const list<Point> &Route::getRoutePointsList() const {
    return routePointsList;
}

void Route::setRoutePointsList(const list<Point> &newRoutePointsList) {
    routePointsList = newRoutePointsList;
}

bool Route::isValid() const {
    try {
        Route route(name, distance, dailyRepeat);
        route.setRoutePointsList(this->getRoutePointsList());
    } catch (InvalidInputException &e) {
        cerr << e.getMessage() << endl;
        return false;
    }
    return true;
}

// -----------
class Taxi : public Car {
protected:
    string driverName;
    Route currentRoute;
public:
    Taxi() : Car() {}

    Taxi(const string &newDriverName, const string &brand, const string &model, int yearsOfUse,
         int numSeats, double fuelPerKm, const string &driverName, const string &regNum)
            : Car(brand, model, yearsOfUse, numSeats, fuelPerKm, regNum) {
        setDriverName(newDriverName);
    }

    Taxi(const string &newDriverName, const Car &car) : Car(car.getBrand(), car.getModel(), car.getYearsOfUse(),
                                                            car.getNumSeats(), car.getFuelPerKm(),
                                                            car.getRegistrationNumber()) {
        setDriverName(newDriverName);
    }

    Taxi(const string &newDriverName, const Car &car, const Route &route)
            : Car(car.getBrand(), car.getModel(), car.getYearsOfUse(), car.getNumSeats(), car.getFuelPerKm(),
                  car.getRegistrationNumber()) {
        setDriverName(newDriverName);
        setCurrentRoute(route);
    }

    Route getCurrentRoute() const {
        return currentRoute;
    }

    void setCurrentRoute(const Route &newRoute) noexcept(false) {
        if (newRoute.isValid()) {
            Taxi::currentRoute = newRoute;
        } else throw InvalidInputException("Taxi: setCurrentRoute -> route is invalid\"");
    }

    const string &getDriverName() const {
        return driverName;
    }

    void setDriverName(const string &newDriverName) noexcept(false) {
        if (regex_match(newDriverName, regexName)) {
            Taxi::driverName = newDriverName;
        } else throw InvalidInputException("Taxi: newDriverName -> is invalid\"");

    }

    void print() override;

    double calculatePerDayFuelUse() const;

    string toString() const override;

    void saveToFile(string fileName) override;

    bool isObjectInFileDatabase(string filename) override;

    void addNewRoutePoint(const Point &p);

    void addNewCarInfo(const Car &car);

    friend ostream &operator<<(ostream &os, const Taxi &taxi);

    friend istream &operator>>(istream &is, Taxi &taxi);

    string getIdentification() const override;

//    Car getCurrentCarInfo();

    void setCurrentCarInfo(const Car &car);


    bool isTaxiDataValid() const;

    Car getCurrentCarInfo() const;

    string getFilePath() const override {
        return FILE_PATH_TAXI;
    }
};

bool Taxi::isTaxiDataValid() const {
    try {
        bool isValid = regex_match(driverName, regexName) && getCurrentCarInfo().isCarDataValid() &&
                       getCurrentRoute().isValid();
        if (!isValid) {
            cerr << "Taxi: is not valid" << endl;
        }
        return isValid;
//        Car c(brand, model, yearsOfUse, numSeats, fuelPerKm, registrationNumber);
//        Route();
    } catch (InvalidInputException &e) {
        cerr << "Taxi: " << e.getMessage() << endl;
        return false;
    }
    return true;
}

string Taxi::getIdentification() const {
    return driverName;
}

ostream &operator<<(ostream &os, const Taxi &taxi) {
    os << taxi.toString() << endl;
    return os;
}

istream &operator>>(istream &is, Taxi &taxi) {
//       (Car) taxi;
    is >> taxi.driverName;
    Car car;
    Route route;
//        cout << TAXI_MENU_ADD_DRIVER_NAME;
    is >> car;
    is >> route;
    taxi.addNewCarInfo(car);
    taxi.setCurrentRoute(route);

    return is;
}

void Taxi::print() {
    cout << "Taxi info: \n" + this->toString() << endl;
}

double Taxi::calculatePerDayFuelUse() const {
    //извежда информация колко гориво да се зареди за извършване нa дневната обиколка
    // (repeats * distance) * fuelPerKM
    double result = this->getCurrentRoute().calcRouteTotalDistance() * fuelPerKm;

    cout << endl << "CalculatePerDayFuelUse: " << result << " liters of fuel for Taxi with: daily Repeats: "
         << getCurrentRoute().getDailyRepeat() << " distance: " << getCurrentRoute().getDistance() << " fuelPerKm: "
         << fuelPerKm << endl;
    return result;
}

string Taxi::toString() const {
    return PREFIX_TAXI + ":" + driverName + Car::toString().append(currentRoute.toString());
}

void Taxi::saveToFile(string fileName) {
    // check does nested objects are in the database, if not, they will be added
    getCurrentRoute().saveToFile(FILE_PATH_ROUTE);
    getCurrentCarInfo().saveToFile(FILE_PATH_CAR);
    if (!isObjectInFileDatabase(fileName)) {
        cout << "saving taxi: " << Taxi::toString() << " to file: " << fileName << endl;
        std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);
        ofs << Taxi::toString() << endl;
        ofs.close();
    } else {
        cout << "Item Taxi with Identification Name: " << getIdentification() << " - already in the file" << endl;
    }
}

bool Taxi::isObjectInFileDatabase(string filename) {
    // TODO: make one more check inside file database
    return isItemExistInVector(getIdentification(), taxiVector);
}

void Taxi::addNewRoutePoint(const Point &p) {
    if (p.isValid()) {
        this->getCurrentRoute().addNewRoutePoint(p);
    } else cerr << "Taxi addNewRoutePoint -> Point is invalid" << endl;
}

void Taxi::addNewCarInfo(const Car &car) {
    this->setModel(car.getModel());
    this->setBrand(car.getBrand());
    this->setRegistrationNumber(car.getRegistrationNumber());
    this->setNumSeats(car.getNumSeats());
    this->setFuelPerKm(car.getFuelPerKm());
    this->setYearsOfUse(car.getYearsOfUse());
}

Car Taxi::getCurrentCarInfo() const {
    return Car(brand, model, yearsOfUse, numSeats, fuelPerKm,
               registrationNumber);
}

void Taxi::setCurrentCarInfo(const Car &car) {
    setBrand(car.getBrand());
    setModel(car.getModel());
    setYearsOfUse(car.getYearsOfUse());
    setNumSeats(car.getNumSeats());
    setFuelPerKm(car.getFuelPerKm());
    setRegistrationNumber(car.getRegistrationNumber());
}

// --------Implementation of static methods--------------------

void getCarFromMenu(Car &newCar);

void getRouteFromMenu(Route &newRoute);

//void getTaxiFromMenu(Taxi &newTaxi);

void getPointFromMenu(Point &point);

static void adminMenu() noexcept(false) {
    bool isLooping = true;

    cout << endl << "------------Main Menu-------------------------------------------" << endl
         << "Hello to taxi route system" << endl
         << "Hello taxi manager ";
    do {
        cout << endl << "----------------------------------------------------------------" << endl
             << "Please choose from the following options:" << endl
             << ADMIN_MENU_NEW_POINT << endl
             << ADMIN_MENU_NEW_ROUTE << endl
             << ADMIN_MENU_NEW_CAR << endl
             << ADMIN_MENU_NEW_TAXI << endl
             //         << ADMIN_MENU_NEW_TAXI_NO_ROUTES << endl
             << ADMIN_MENU_CALCULATE_ROUTE_TOTAL_DISTANCE << endl
             << ADMIN_MENU_CALCULATE_TAXI_FUEL_USAGE << endl
             << MENU_EXIT_OPTIONS << endl
             << endl;
        cin.clear();
        cout.clear();

        char task = 0;
        cin >> task;
        switch (task) {
            case '1' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_POINT << endl << endl;
                isLooping = pointsMenu();
                break;
            case '2' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_ROUTE << endl << endl;
                isLooping = routesMenu();
                break;
            case '3' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_CAR << endl << endl;
                isLooping = carsMenu();
                break;
            case '4' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_TAXI << endl << endl;
                isLooping = taxiMenu();
                break;
            case '5' :
                cout << MENU_CHOSEN << ADMIN_MENU_CALCULATE_ROUTE_TOTAL_DISTANCE << endl << endl;
                for (const Route &r : routesVector) {
                    cout << "For Route: " << r.toString() << endl <<
                         "Total distance is: " << r.calcRouteTotalDistance() << endl << endl;
                }
                break;
            case '6' :
                cout << MENU_CHOSEN << ADMIN_MENU_CALCULATE_TAXI_FUEL_USAGE << endl << endl;
                for (const Taxi &taxi : taxiVector) {
                    cout << "For Taxi " << taxi.toString() << endl <<
                         "Total distance for current Route is: " << taxi.getCurrentRoute().calcRouteTotalDistance()
                         << endl <<
                         "And Fuel Usage Per km  is: ";
                    taxi.calculatePerDayFuelUse();
                    cout << endl;
                }
                break;
            case '7' :
                cout << MENU_CHOSEN << endl;
                break;
            case 'q' :
            case 'Q' :
            case '0' :
                cout << MENU_EXITING << endl;
                isLooping = false;
            default:
                cout << MENU_WRONG_OPTION_ERROR << endl << endl;
                break;
        }
    } while (isLooping);
}

bool carsMenu(bool singlePick/*vector<Car> &resultVector*/) {
    bool addMore = true;
    do {
        Car newCar = Car();
        getCarFromMenu(newCar);
        if (singlePick) addMore = false;
        else {
            string str;
            cout << "> Add new Car: or close with q Q 0 for exit)\n" << endl;
            cin >> str;
            if (str == "q" || str == "Q" || str == "0") addMore = false;
        }
    } while (addMore);
    return true;
}

static bool routesMenu(bool singlePick) { // def false
    bool addMore = !singlePick;
    do {
        Route newRoute = Route();
        getRouteFromMenu(newRoute);
        if (!singlePick) {
            string str;
            cout << "> Add new Route: or close with q Q 0 for exit)\n" << endl;
            cin >> str;
            if (str == "q" || str == "Q" || str == "0") addMore = false;
        } else {
            addMore = false;
            break;
        }
    } while (addMore);
    return true;
}

void getCarFromMenu(Car &newCar) {
    bool isLooping = true;
    do {
        try {
            cout << endl << "````````Cars Menu``````````````" << endl
                 << "Car Menu -> add Car's specifications" << endl
                 << "Current Car: " << newCar.toString() << endl
                 << "Please choose from the following options:" << endl
                 << MENU_EXIT_OPTIONS << " without saving" << endl
                 << CAR_MENU_ADD_REG_NUMBER << endl
                 << CAR_MENU_ADD_MODEL << endl
                 << CAR_MENU_NUM_SEATS << endl
                 << CAR_MENU_BRAND << endl
                 << CAR_MENU_YEARS_OF_USE << endl
                 << CAR_MENU_FUEL_PER_KM << endl
                 << CAR_MENU_SAVE_CAR << endl
                 << endl;
            char option = 0;
            cin >> option;
            string str;
            switch (option) {
                case '1' :
                    cout << MENU_CHOSEN << CAR_MENU_ADD_REG_NUMBER << endl;
                    cin >> str;
                    newCar.setRegistrationNumber(str);
                    break;
                case '2' :

                    cout << MENU_CHOSEN << CAR_MENU_ADD_MODEL << endl;
                    cin >> str;
                    newCar.setModel(str);
                    break;
                case '3' :
                    cout << MENU_CHOSEN << CAR_MENU_NUM_SEATS << endl;
                    int seats;
                    cin >> seats;
                    newCar.setNumSeats(seats);
                    break;
                case '4' :
                    cout << MENU_CHOSEN << CAR_MENU_BRAND << endl;
                    cin >> str;
                    newCar.setBrand(str);
                    break;
                case '5' :
                    cout << MENU_CHOSEN << CAR_MENU_YEARS_OF_USE << endl;
                    int years;
                    cin >> years;
                    newCar.setYearsOfUse(years);
                    break;
                case '6' :
                    cout << MENU_CHOSEN << CAR_MENU_FUEL_PER_KM << endl;
                    double fuel;
                    cin >> fuel;
                    newCar.setFuelPerKm(fuel);
                    break;
                case '7' :
                    if (newCar.isCarDataValid()) {
                        cout << MENU_CHOSEN << "Save this car to program" << endl;
                        newCar.saveToFile(FILE_PATH_CAR);
                        carsVector.push_back(newCar);
                    } else {
                        cout << endl << MENU_CHOSEN << "Don't save car" << endl;
                    }
                    isLooping = false;
                    break;
                case 'q' :
                case 'Q' :
                case '0' :
                    cout << MENU_EXITING_WITHOUT_SAVING << endl;
                    isLooping = false;
                default:
                    cout << MENU_WRONG_OPTION_ERROR << endl;
                    break;
            }
        } catch (InvalidInputException &e) {
            cerr << e.getMessage() << endl;
            continue;
        }
    } while (isLooping);
//    cin.clear();
//    cout.clear();
}

void getRouteFromMenu(Route &newRoute) {
    bool isLooping = true;
    do {
        try {
            cout << endl << "````````Routes Menu``````````````" << endl
                 << "Route Menu -> add Route's specifications" << endl
                 << "Current Route: " << newRoute.toString() << endl
                 << "Please choose from the following options:" << endl
                 << MENU_EXIT_OPTIONS << " without saving" << endl
                 << ROUTE_MENU_ADD_NAME << endl
                 << ROUTE_MENU_ADD_DISTANCE << endl
                 << ROUTE_MENU_NUM_DAILY_REPEAT << endl
                 << ROUTE_MENU_ADD_POINT << endl
                 << ROUTE_MENU_CALC_TOTAL_DISTANCE << endl
                 //                     << ROUTE_MENU_FUEL_PER_KM << endl
                 << ROUTE_MENU_SAVE << endl
                 << endl;
            char option = 0;
            cin >> option;
            string str;
            Point point;
            switch (option) {
                case '1' :
                    cout << MENU_CHOSEN << ROUTE_MENU_ADD_NAME << endl;
                    cin >> str;
                    newRoute.setName(str);
                    break;
                case '2' :

                    cout << MENU_CHOSEN << ROUTE_MENU_ADD_DISTANCE << endl;
                    double distance;
                    cin >> distance;
                    newRoute.setDistance(distance);
                    break;
                case '3' :
                    cout << MENU_CHOSEN << ROUTE_MENU_NUM_DAILY_REPEAT << endl;
                    int dailyRepeats;
                    cin >> dailyRepeats;
                    newRoute.setDailyRepeat(dailyRepeats);
                    break;
                case '4' :
                    cout << MENU_CHOSEN << ROUTE_MENU_ADD_POINT << endl;
                    getPointFromMenu(point);
                    newRoute.addNewRoutePoint(point);
                    break;
                case '5' :
                    cout << MENU_CHOSEN << ROUTE_MENU_CALC_TOTAL_DISTANCE << endl;
                    cout << "Route total distance is: " << newRoute.calcRouteTotalDistance();
                    break;
                case '6' :
//                        cout << MENU_CHOSEN << ROUTE_MENU_FUEL_PER_KM << endl;
//                        double fuel;
//                        cin >> fuel;
//                        newRoute.setFuelPerKm(fuel);
//                        break;
//                    case '7' :
                    if (newRoute.isValid()) {
                        cout << MENU_CHOSEN << "Save this Route to program" << endl;
                        newRoute.saveToFile(FILE_PATH_ROUTE);
                        routesVector.push_back(newRoute);
                    } else {
                        cout << endl << MENU_CHOSEN << "Didn't saved Route" << endl;
                    }
                    isLooping = false;
                    break;
                case 'q' :
                case 'Q' :
                case '0' :
                    cout << MENU_EXITING_WITHOUT_SAVING << endl;
                    isLooping = false;
                default:
                    cout << MENU_WRONG_OPTION_ERROR << endl;
                    break;
            }
        } catch (InvalidInputException &e) {
            cerr << e.getMessage() << endl;
            isLooping = false;
            continue;
        }
    } while (isLooping);
    cin.clear();
    cout.clear();
}

void getPointFromMenu(Point &newPoint) {
    bool isLooping = true;
    do {
        try {
            cout << endl << "````````POINTS Menu``````````````" << endl
                 << "Point Menu -> add Route's specifications" << endl
                 << "Current Point: " << newPoint.toString() << endl
                 << "Please choose from the following options:" << endl
                 << MENU_EXIT_OPTIONS << " without saving" << endl
                 << POINT_MENU_ADD << endl
                 << POINT_MENU_SAVE << endl
                 << endl;
            char option = 0;
            cin >> option;
            switch (option) {
                case '1' :
                    cout << MENU_CHOSEN << ROUTE_MENU_ADD_NAME << endl;
                    cout << endl << "Note!!! Add Point data in sequence: \"PointName X-coord Y-coord\"" << endl;
                    cin >> newPoint;
                    break;
                case '2' :
                    if (newPoint.isValid()) {
                        cout << MENU_CHOSEN << "Save this Route to program" << endl;
                        newPoint.saveToFile(FILE_PATH_POINT);
                        pointVector.push_back(newPoint);
                    } else {
                        cout << endl << MENU_CHOSEN << "Didn't saved the Point" << endl;
                    }
                    isLooping = false;
                    break;
                case 'q' :
                case 'Q' :
                case '0' :
                    cout << MENU_EXITING_WITHOUT_SAVING << endl;
                    isLooping = false;
                    break;
                default:
                    cout << MENU_WRONG_OPTION_ERROR << endl;
                    break;
            }
        } catch (InvalidInputException &e) {
            cerr << e.getMessage() << endl;
            isLooping = true;
            continue;
        }
    } while (isLooping);
}
//void getPointFromMenu(const Point &point) {
//    cout << endl << "Note!!! Add Point data in sequence: \"PointName X-coord Y-coord\"" << endl;
//    cin >> (Point &) point;
//}

static bool taxiMenu() {
    bool addMore = true;
    do {
        Taxi newTaxi = Taxi();
        bool isLooping = true;
        do {
            try {
                cout << endl << "````````Taxi Menu``````````````" << endl
                     << "Taxi Menu -> add Taxi's specifications" << endl
                     << "Is current Taxi valid?: " << newTaxi.isTaxiDataValid() << " " << endl
                     << "With Data: " << newTaxi.toString() << endl
                     << "Please choose from the following options:" << endl
                     << MENU_EXIT_OPTIONS << " without saving" << endl
                     << TAXI_MENU_ADD_DRIVER_NAME << endl
                     << TAXI_MENU_ADD_ROUTE << endl
                     << TAXI_MENU_ADD_CAR << endl
                     //                     << TAXI_MENU_ADD_NONE << endl
                     << TAXI_MENU_CALC_TOTAL_DISTANCE << endl
                     //                     << TAXI_MENU_FUEL_PER_KM << endl
                     << TAXI_MENU_SAVE << endl
                     << endl;
                char option = 0;
                cin >> option;
                string str;
                Point point;
                Route r;
                Car car;
                switch (option) {
                    case '1' :
                        cout << MENU_CHOSEN << TAXI_MENU_ADD_DRIVER_NAME << endl;
                        cin >> str;
                        newTaxi.setDriverName(str);
                        break;
                    case '2' :

                        cout << MENU_CHOSEN << TAXI_MENU_ADD_ROUTE << endl;
                        getRouteFromMenu(r);
                        newTaxi.setCurrentRoute(r);
                        break;
                    case '3' :
                        cout << MENU_CHOSEN << TAXI_MENU_ADD_CAR << endl;
                        getCarFromMenu(car);
                        newTaxi.setCurrentCarInfo(car);
                        break;
                    case '4' :
                        cout << MENU_CHOSEN << TAXI_MENU_ADD_NONE << endl
                             << "Note!!! Add Point data in sequence: \"PointName X-coord Y-coord\"" << endl;
                        cin >> point;
                        newTaxi.addNewRoutePoint(point);
                        break;
                    case '5' :
                        cout << MENU_CHOSEN << TAXI_MENU_CALC_TOTAL_DISTANCE << endl;
                        cout << "Taxi total distance is: " << newTaxi.calculatePerDayFuelUse();
                        break;
                    case '6' :
//                        cout << MENU_CHOSEN << TAXI_MENU_FUEL_PER_KM << endl;
//                        double fuel;
//                        cin >> fuel;
//                        newTaxi.setFuelPerKm(fuel);
//                        break;
//                    case '7' :
                        if (newTaxi.isCarDataValid()) {
                            cout << MENU_CHOSEN << "Save this Taxi to program" << endl;
                            newTaxi.saveToFile(FILE_PATH_TAXI);
                            taxiVector.push_back(newTaxi);
                        } else {
                            cout << endl << MENU_CHOSEN << "Didn't saved Taxi to file!" << endl;
                        }
                        isLooping = false;
                        break;
                    case 'q' :
                    case 'Q' :
                    case '0' :
                        cout << MENU_EXITING_WITHOUT_SAVING << endl;
                        isLooping = false;
                    default:
                        cout << MENU_WRONG_OPTION_ERROR << endl;
                        break;
                }
            } catch (InvalidInputException &e) {
                cerr << endl << e.getMessage() << endl;
                continue;
            }
        } while (isLooping);
        string str;
        cout << "> Add new Taxi: or close with q Q 0 for exit)\n" << endl;
        cin >> str;
        if (str == "q" || str == "Q" || str == "0") addMore = false;
    } while (addMore);
    return true;
}


static bool pointsMenu(bool singlePick) {

    bool addMore = true;
    do {
        Point point;
        getPointFromMenu(point);
        if (singlePick) { addMore = false; }
        else {
            string str;
            cout << ">click any key to continue to Add new Point: or close with q Q 0 for exit)\n" << endl;
            cin >> str;
            if (str == "q" || str == "Q" || str == "0") addMore = false;
        }
    } while (addMore);
    return true;
}

static void clientMenu() noexcept(false) {
    cout << "Hello to taxi route system" << endl
         << "Hello client " << endl
         << "Please choose from the following options:" << endl
         << endl;

}

/*
 * It will iterate through all the lines in file and
 * call the given callback on each line.
 */
template<class T>
static bool getAndIterateEachLineOfFileWithCallbackSaveIntoVector(const string &fileName, vector<T> &dataVector,
                                                                  void (*callback)(const string &, vector<T> &)
) {// Open the File
    ifstream in(fileName.c_str(), std::ios_base::in | std::ios_base::out | std::ios::app);
    //in.open(fileName.c_str(), std::ios_base::in | std::ios_base::out);
    if (!in) {
        cerr << "Cannot open the File : " << fileName << std::endl;
        return false;
    }

    string lineOfData;
    // Read the next line from File untill it reaches the end.
    while (getline(in, lineOfData)) {
        // Call the given callback
        callback(lineOfData, dataVector);
    }
    //Close The File
    in.close();
    return true;
}

template<class T>
bool isItemExistInVector(string identificationName, const vector<T> &dataVector) {
    auto foundItem = findInVector(identificationName, dataVector);
    bool isExisting = foundItem.first >= 0;
    if (DEBUG) {
        if (isExisting) {
            cerr << "item with identName: " << identificationName << " exist at index in vector: " << foundItem.first
                 << ", "
                 << typeid(T).name() << ":\n" << foundItem.second.toString() << endl;
        } else
            cerr << "item with identName: " << identificationName << " does not exist in vector:" << typeid(T).name()
                 << endl;
    }
    return isExisting;
}

template<class T>
pair<int, T> findInVector(string identificationName, vector<T> v) {
    pair<int, T> result;
    result.first = -1;
    auto it = find_if(v.begin(), v.end(), [&identificationName](const T &obj) {
        return obj.getIdentification() == identificationName;
    });
    if (it != v.end()) {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        result.first = std::distance(v.begin(), it);
        result.second = v.at(result.first);
    }

    return result;
}

template<class T>
static bool getAndPutFileContentIntoGenericsVector(const string &fileName, vector <T> &vector) {
    // Open the File
    ifstream in(fileName.c_str());

    // Check if object is valid
    if (!in) {
        cerr << "Cannot open the File : " << fileName << endl;
        return false;
    }

    string lineOfData;
    // Read the next line from File untill it reaches the end.
    while (getline(in, lineOfData)) {
        // remove spaces of line
        //cout << "!:" << lineOfData << endl;
        removeWhiteSpacesOfString(lineOfData);
        // cout << "?:" << lineOfData << endl;

        if (!lineOfData.empty()) {
            extractDataFromLineIntoGenericsVector(lineOfData, vector);
            /*if (typeid(T) == typeid(Car) ){
                extractDataFromLineIntoCarVector(lineOfData, carsVector);
            }if (typeid(T) == typeid(Point) ){

                cout<< "worl";
                extractDataFromLineIntoPointVector(lineOfData, pointVector);
            }*/


        }
    }
    //Close The File
    in.close();
    return true;
}

// TODO MAKE IT WITH GENERICS !!!!

/*void extractDataFromLineIntoCarVector(const string &lineOfData, vector<Car> &carsVector) {
    Car newCar;

    // regex_replace() for removing the match
    deserializeStrData(lineOfData);
    stringstream streamWithCarData(lineOfData);
    streamWithCarData >> newCar;
    cout << newCar.toString(); // DEBUG
    carsVector.push_back(newCar);
}

 void extractDataFromLineIntoPointVector(const string &lineOfData, vector<Point> &pointsVector) {

    Point newPoint;

    // regex_replace() for removing the match
    deserializeStrData(lineOfData);
    stringstream streamWithCarData(lineOfData);
    streamWithCarData >> newPoint;
    cout << newPoint.toString(); // DEBUG
    pointsVector.push_back(newPoint);
}*/

//template<class T>
//static void extractDataFromLineIntoGenericsVector(const string &lineOfData, vector<T> &vectorWithData) {

//    extractDataFromLineIntoGenericsObj(lineOfData, point);

//    vectorWithData.push_back(dataObject);
//    dataObject.saveToFile(FILE_PATH_DEBUG);
//
//    T dataObject;
//    cerr << endl << "Deserialized: " << typeid(T).name() << endl;
//    //string test = ">name:route1,distance: 21.000000,dailyRepeat: 1|>name:a,X: 1.000000,Y: 2.000000>name:b,X: 3.000000,Y: 4.000000>name:d,X: 1.000000,Y: 1.000000";//|>Car: bla,asd:sad> sadasd asd>sadasd";
//    const string &copyOfLineOfData = lineOfData;
//    //vector < string > a = split(copyOfLineOfData, "|");
//    vector<string> splitData = split(copyOfLineOfData, DELIMITER_START_LINE);
//    //list<Point> pointList = dataObject.getRoutePointsList();
//
//    string prefixTag = lineOfData.substr(0,PREFIX_ROUTE.size());
//
//    if (splitData.size() > 1) {
//        // if (lineOfData.substr(0,PREFIX_ROUTE.size()) == PREFIX_ROUTE) {
//        for (int i = 1; i < splitData.size(); i++) {
//            vector<string> splitDataW = split(splitData[0], ":");
//
//            newPoint;
//            string pointDataStr = insertDelimiterAtBeginnig(splitData.at(i), DELIMITER_START_LINE);
//            deserializeStrData(pointDataStr);
//            extractDataFromLineIntoGenericsObj(pointDataStr, newPoint);
//            //if (dataObject type == T type)
//            dataObject.addNewRoutePoint(newPoint);
//        }
// }else if (lineOfData.substr(0,PREFIX_TAXI.size()) == PREFIX_TAXI) {
/*for (int i = 1; i < splitData.size(); i++) {
    Point newPoint;
    string pointDataStr = splitData.at(i);
    deserializeStrData(pointDataStr);
    extractDataFromLineIntoGenericsList(pointDataStr, pointList);
    //dataObject.addNewRoutePoint(newPoint);
}*/
// }
//    } else cerr << endl << "?" << endl;
//    string routeBasicInfo = insertDelimiterAtBeginnig(splitData.at(0), DELIMITER_START_LINE);
//    deserializeStrData(routeBasicInfo);
//    stringstream streamWithObjectData(routeBasicInfo);
//
//    streamWithObjectData >> dataObject;
//    vectorWithData.push_back(dataObject);
//    dataObject.saveToFile(FILE_PATH_DEBUG);
//
//    cerr << endl << "Final: " << dataObject.toString() << endl; // DEBUG

//}

static string insertDelimiterAtBeginnig(string &b, const string &delimiter) { return b.insert(0, delimiter); }

template<class T>
static void extractDataFromLineIntoGenericsList(const string &lineOfData, list <T> &list) {

    T dataObject;
    string copyOfLineOfData = lineOfData;

    deserializeStrData(copyOfLineOfData);
    //cerr << "GenericsList - before to save " << typeid(T).name() << "-> " << copyOfLineOfData << endl; // DEBUG

    stringstream streamWithObjectData(copyOfLineOfData);

    streamWithObjectData >> dataObject;
    list.push_back(dataObject);
    if (DEBUG) {
        cerr << "GenericsList - after saved  " << typeid(T).name() << "  -> " << dataObject.toString()
             << endl; // DEBUG
    }
    streamWithObjectData.clear();

}

template<class T>
static void extractDataFromLineIntoGenericsVector(const string &lineOfData, vector<T> &vector) {

    T dataObject;
    string copyOfLineOfData = lineOfData;

    deserializeStrData(copyOfLineOfData);
    //cerr << "GenericsList - before to save " << typeid(T).name() << "-> " << copyOfLineOfData << endl; // DEBUG

    stringstream streamWithObjectData(copyOfLineOfData);

    streamWithObjectData >> dataObject;
    vector.push_back(dataObject);
    dataObject.saveToFile(dataObject.getFilePath());
    if (DEBUG) {
        cerr << "GenericsVector - after save " << typeid(T).name() << "->\n" << dataObject.toString()
             << endl; // DEBUG
    }
    streamWithObjectData.clear();
}

template<class T>
static void extractDataFromLineIntoGenericsObj(const string &lineOfData, T &dataObject) {

    string copyOfLineOfData = lineOfData;

    deserializeStrData(copyOfLineOfData);
    //cerr << "GenericsObj - before to save " << typeid(T).name() << "-> " << copyOfLineOfData << endl; // DEBUG

    stringstream streamWithObjectData(copyOfLineOfData);

    streamWithObjectData >> dataObject;
    //list.push_back(dataObject);

    if (DEBUG) {
        cerr << endl << "GenericsObj - after saved  " << typeid(T).name() << "  -> " << dataObject.toString()
             << endl; // DEBUG
    }
    streamWithObjectData.clear();
}

template<class T>
static void listToString(list<T> list, string &result) {
    for (T &t: list) {
        result.append(t.toString());
    }
    //result.pop_back();
    //cerr<< result;
}

static void removeWhiteSpacesOfString(string &stringWithData) {
    //cerr<< stringWithData;
    stringWithData.erase
            (
                    remove_if(stringWithData.begin(), stringWithData.end(), ::isspace),
                    stringWithData.end()
            );
    //cerr<<endl << stringWithData;
}

static void deserializeStrData(string &stringWithData) {
    regex r(REGEX_DESERIALIZATION_OF_DATA);
    //cerr<< stringWithData<<endl;
    //removeWhiteSpacesOfString(stringWithData);
    stringWithData = regex_replace(stringWithData, r, "\n");

    //cerr << stringWithData;
}

static vector<string> split(const string &str, const string &delimiter) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delimiter, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delimiter.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

template<class T>
static void stringToList(const string &stringWithData, list<T> &listOfData) {

    vector<string> vs = split(stringWithData, DELIMITER_START_LINE);
    for (const string &s : vs) {
        cerr << endl << "stringToList(single object): " << s << endl;
        extractDataFromLineIntoGenericsList(s, listOfData);
    }
}

static void
loadDataVectorsFromFiles(/*vector<Car> carsVector,vector<Point> pointVector,vector<Route> routesVector,vector<Taxi> taxiVector*/) {
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_POINT, pointVector,
                                                          extractDataFromLineIntoGenericsVector);
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_CAR, carsVector,
                                                          extractDataFromLineIntoGenericsVector);
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_ROUTE, routesVector,
                                                          extractDataFromLineIntoGenericsVector);
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_TAXI, taxiVector,
                                                          extractDataFromLineIntoGenericsVector);
}

void testExtractFunct() {
    string testLine;
    Point point;
    Point p1("aa", 1, 2);
    Point p2("bb", 2, 3);

    extractDataFromLineIntoGenericsObj(p1.toString(), point);
    // Car
    Car car;
    Car c1("newBrandC", "newModelC", 10, 20, 30, "CB1234CB");
    extractDataFromLineIntoGenericsObj(c1.toString(), car);
    // Route
    Route route;
    Route r("routeC", 69, 2);
    r.addNewRoutePoint(p1);
    r.addNewRoutePoint(p2);

    extractDataFromLineIntoGenericsObj(r.toString(), route);
    // Taxi
    Taxi t;
    Taxi taxi1("Gosh", c1, r);
    extractDataFromLineIntoGenericsObj(taxi1.toString(), t);

}

void testRouteFunctionality() {

    Point p1("a", 1, 2);
    Point p2("b", 3, 4);
//    list<Point> p;
//    p.emplace_back("a", 1, 1.3);
//    p.emplace_back("b", 2.1, 2.2);
//    p.emplace_back("c", 3.3, 3.1);


    /*string string1;
    listToString(p,string1);
    cout << string1;

    //deserializeStrData(string1);
    cout<< "\n info : \n" ;
    list<Point> a ;
    stringToList(string1, a);
    //cout << a.back(); // bad alloc
    cout << "\n-----1-----\n";*/
    //cout << "----------\n";
    Route r("route1", 21, 1);
//    Point p1(1, 2);
//    Point p2(3, 4);
//    p1.print();
    cout << "----------\n";
    r.addNewRoutePoint(p1);
    r.addNewRoutePoint(p2);
    r.saveToFile(FILE_PATH_ROUTE);
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_ROUTE, routesVector,
                                                          extractDataFromLineIntoGenericsVector);
    cout << "\n----computed result----\n";
    routesVector.back().print();
    cout << "\n--- correct  result----\n";
    r.print();
}

void testTaxiFunctionality() {

    cout << "-----testTaxi-----\n";
    list<Point> p;
    Point p1("a", 1, 2);
    Point p2("b", 3, 4);
    Point p3("c", 5, 6);
    Route r("route1", 21, 1);
    r.addNewRoutePoint(p1);
    r.addNewRoutePoint(p2);
    //const string &brand, const string &model, int yearsOfUse, int numSeats, double fuelPerKm,string registrationNumber) {
    Car c1("myBrand1", "myModel1", 1, 2, 3, "CA1234CA");
    Car c2("myBrand2", "myModel2", 4, 5, 6, "CB5678CB");

    Taxi taxi1("Pesho", c1, r);

    taxi1.addNewRoutePoint(p3);
    //taxi1.saveToFile(FILE_PATH_TAXI);

    //taxi1.isObjectInFileDatabase(FILE_PATH_TAXI, "");

    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_TAXI, taxiVector,
                                                          extractDataFromLineIntoGenericsVector);
//    cout << "---result----\n";
//    routesVector.back().print();
    cout << "\n--- correct result----\n";
    taxi1.print();

}

//void getCarFromMenu(Car &newCar) {
//
//}

void testLoadFromFileFunct() {
    cout << "-----------------------------------------------" << endl
         << "Load Data from the Database:" << endl;
    try {
        loadDataVectorsFromFiles();

    } catch (InvalidInputException &e) {
        cerr << e.getMessage() << endl;
    }

    if (DEBUG) {
        cout << endl << "PointVector" << endl;
        for (const auto &x: pointVector) cout << x.toString() << "\n";

        cout << endl << "CarVector" << endl;
        for (const auto &x: carsVector) cout << x.toString() << "\n";

        cout << endl << "RouteVector" << endl;
        for (const auto &x: routesVector) cout << x.toString() << "\n";

        cout << endl << "TaxiVector" << endl;
        for (const auto &x: taxiVector) cout << x.toString() << "\n";

        isItemExistInVector("Pesho", taxiVector);
        auto taxi = findInVector("Pesho", taxiVector).second;
        taxi.calculatePerDayFuelUse();
        isItemExistInVector("Pekjsho", taxiVector);
    }

}

int main(int argc, const char **argv) {
    //  testExtractFunct();
    testLoadFromFileFunct();

    adminMenu();
    //loadDataVectorsFromFiles();
    //testRouteFunctionality();
    //testTaxiFunctionality();

    return 0;
}