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

static const char *const ADMIN_MENU_NEW_CAR = "1.Create new Car object/s";
static const char *const ADMIN_MENU_NEW_ROUTE = "2.Create new Route object/s";
static const char *const ADMIN_MENU_NEW_TAXI = "3.Create new Taxi object/s";
static const char *const ADMIN_MENU_NEW_TAXI_NO_ROUTES = "4.Create new Taxi object/s without routes";
static const char *const ADMIN_MENU_CHANGE_TAXI_ROUTE = "5.Change Taxi route";
static const char *const ADMIN_MENU_ADD_REMOVE_POINTS_OF_ROUTE = "6.Add point/s to existing Route";

static const char *const CAR_MENU_ADD_REG_NUMBER = "1. Add car's Registration Number";
static const char *const CAR_MENU_ADD_MODEL = "2. Add car's model";
static const char *const CAR_MENU_NUM_SEATS = "3. Add number of Seats";
static const char *const CAR_MENU_BRAND = "4. Add Car's brand";
static const char *const CAR_MENU_YEARS_OF_USE = "5. Add car's years of use";
static const char *const CAR_MENU_FUEL_PER_KM = "6. Add car's fuel per km usage";
static const char *const CAR_MENU_SAVE_CAR = "7. Save current car specifications";

static const char *const TAXI_MENU_ADD_DRIVER_NAME = "1. Add Taxi's Driver Name";


static const char *const FILE_PATH_ROOT = "/home/bzahov/Documents/gitRepos/TU-University-Tasks/Programming Languages(PE) C++/Tasks/TaxiRouteSystem(CourseWork)/car_database.txt";
static const char *const FILE_PATH_CAR = "car_database.txt";
static const char *const FILE_PATH_POINT = "point_database.txt";
static const char *const FILE_PATH_ROUTE = "route_database.txt";
static const char *const FILE_PATH_TAXI = "taxi_database.txt";
static const char *const FILE_PATH_DEBUG = "debug_database.txt";


static const char *const REGEX_DESERIALIZATION_OF_DATA = "([,|>](\\w+)[:])";

static const char *const SERIALIZATION_STR_DEFAULT = "";

static const string PREFIX_POINT = ">PointName";
static const string PREFIX_CAR = ">CarName";
static const string PREFIX_ROUTE = ">RouteName";
static const string PREFIX_TAXI = ">TaxiName";

static const char *const DELIMITER_START_LINE = ">";

// ---Class declarations--------------

class Serializable {
public:
    virtual void saveToFile(string filename) = 0;

    virtual void isObjectInFileDatabase(string filename, string parameter) = 0;

    //TODO: add toString() and print() to class
};

class InvalidInputException;

class Car;

class Point;

class Taxi;

class Route;

class InvalidInputException : public std::exception {
public:
    string generalInfo = "InvalidInputException in: ";
    string info;

    InvalidInputException(string info) : info(std::move(info)) {}

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

static void removeWhiteSpacesOfString(string &stringWithData);

static void deserializeStrData(string &stringWithData);

static void adminMenu() throw(InvalidInputException);

static bool addNewCars();

static void loadDataVectorsFromFiles();

static vector<string> split(const string &str, const string &delimiter);

//---------------------

void testTaxiFunctionality();

template<class T>
void extractNestedData(T dataObject, vector<string> &splitData);

static vector<Car> carsVector;
static vector<Route> routesVector;
static vector<Taxi> taxiVector;
static vector<Point> pointVector;

//********


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
        setBrand(SERIALIZATION_STR_DEFAULT);
        setModel(SERIALIZATION_STR_DEFAULT);
        setYearsOfUse(0);
        setNumSeats(0);
        setFuelPerKm(0);
        setRegistrationNumber(SERIALIZATION_STR_DEFAULT);
    }

    Car(const string &brand, const string &model, int yearsOfUse, int numSeats, double fuelPerKm,
        const string &registrationNumber) {
        setBrand(brand);
        setModel(model);
        setYearsOfUse(yearsOfUse);
        setNumSeats(numSeats);
        setFuelPerKm(fuelPerKm);
        setRegistrationNumber(registrationNumber);
    }

    const string &getRegistrationNumber() const;

    void setRegistrationNumber(const string &regNumber) throw(InvalidInputException);

    // TODO: make input check
    const string &getBrand() const {
        return brand;
    }

    void setBrand(const string &newBrand) {
        Car::brand = newBrand;

    }

    const string &getModel() const {
        return model;
    }

    void setModel(const string &newModel) {
        this->model = newModel;
    }

    int getYearsOfUse() const {
        return yearsOfUse;
    }

    void setYearsOfUse(int newYearsOfUse) {
        Car::yearsOfUse = newYearsOfUse;
    }

    int getNumSeats() const {
        return numSeats;
    }

    void setNumSeats(int newNumSeats) {
        Car::numSeats = newNumSeats;
    }

    double getFuelPerKm() const {
        return fuelPerKm;
    }

    void setFuelPerKm(double newFuelPerKm) {
        Car::fuelPerKm = newFuelPerKm;
    }

    virtual void print();

    virtual string toString() const;

    virtual void saveToFile(string str);

    virtual void isObjectInFileDatabase(string str, string regNum);

    static std::vector<Car> readAllFromFile(string str);

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
};

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

void Car::setRegistrationNumber(const string &regNum) throw(InvalidInputException) {
    if (true) { // TODO VALIDATE REGISTRATION NUMBER
        Car::registrationNumber = regNum;
    } else throw InvalidInputException("Wrong registration Number");
}

void Car::saveToFile(const string fileName) {
    cout << "saving car: " << Car::toString() << " to file";
    std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);
    ofs << this->toString();
    ofs.close();
}

void Car::isObjectInFileDatabase(string str, string regNum) {
    cerr << "reading car's info: (NOT IMPLEMENTED)" << Car::toString() << " from file";
    // TODO:
    //ifs.close();
}


std::vector<Car> Car::readAllFromFile(string str) {
    std::vector<Car> result;
    /*char *line= "";
    size_t len = 0;
    ssize_t nread;
    cout << "\n-----g3g---\n";
/*

    stream = fopen(FILE_PATH_CAR, "r");
    if (stream == NULL) {
        perror("fopen");
        throw (InvalidInputException("wrong file path"));
    }


    while ((nread = getline(&line, &len, stream)) != -1) {
        printf("Retrieved line of length %zu:\n", nread);
        cout << line;
        //fwrite(line, nread, 1, stdout);
    }

    free(line);
    fclose(stream);
//-----
    std::ifstream ifs(FILE_PATH_CAR,fstream::in);
    cout<< ifs.get();
    ifs.getline(line,256);
    cout << line;
    ifs.close();*/

    std::ifstream infile(FILE_PATH_CAR, fstream::in);
    int a, b;
    while (infile >> a >> b) {
        // process pair (a,b)
    }
    return result;
}



//-------------

class Point : Serializable {
private:
    string name;
    double x{};
    double y{};
public:
    Point() {
        setXY(0, 0);
        setName(SERIALIZATION_STR_DEFAULT);
    }

    Point(const string &name, double xx, double yy) {
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

    void setX(double x) throw() {
        if (x >= 0) {
            Point::x = x;
        } else throw InvalidInputException("Point -> setX");
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        Point::y = y;
    }

    const string &getName() const;

    void setName(const string &name);

    void setXY(const int a, const int b) {
        x = a;
        y = b;
    };

    static bool isValid(const Point &p);

    void print();

    string toString() const;

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
    void saveToFile(string filename) override;

    void isObjectInFileDatabase(string filename, string parameter) override;

    bool isEmpty();
};

const string &Point::getName() const {
    return name;
}

void Point::setName(const string &name2) { // promenih go
    Point::name = name2;
}

bool Point::isValid(const Point &p) {
    // TODO: implement if needed
    //return p.getX()>0 && p.getY()>0;
    return true;
}

void Point::print() {
    cout << toString();
}

string Point::toString() const {
    return PREFIX_POINT + ":" + getName() + ",X: " + to_string(getX()) + ",Y: " + to_string(getY());
}

void Point::saveToFile(string filename) {
    cout << "saving Point: " << Point::toString() << " to file";
    std::ofstream ofs(filename, fstream::in | fstream::out | fstream::app);
    ofs << this->toString();
    ofs.close();
}

void Point::isObjectInFileDatabase(string filename, string parameter) {
    // TODO:
    cerr << "reading point's info: (NOT IMPLEMENTED)" << Point::toString() << " from file";
}

bool Point::isEmpty() {
    return name.empty() && x == 0 && y == 0;
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

    void setName(const string &name) {
        Route::name = name;
    }

    double getDistance() const {
        return distance;
    }

    void setDistance(double dist) { // RENAMED param
        Route::distance = dist;
    }

    int getDailyRepeat() const {
        return dailyRepeat;
    }

    void setDailyRepeat(int newDailyRepeat) {
        Route::dailyRepeat = newDailyRepeat;
    }

    void addNewRoutePoint(Point p);

    string toString() const;

    void print();

    void saveToFile(string filename) override;

    void isObjectInFileDatabase(string filename, string parameter) override;

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
            cerr << endl << point.toString() << endl;
            if (point.isEmpty()) {
                break;
            }
            route.addNewRoutePoint(point);
        } while (true);
        return is;
    }
};


void Route::addNewRoutePoint(Point p) {
    if (Point::isValid(p)) {
        routePointsList.push_back(p);
    } else cerr << "addNewRoutePoint -> Point is invalid" << endl; // TODO
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

void Route::saveToFile(string filename) {
    cout << "saving Route: " << Route::toString() << " to file";
    std::ofstream ofs(filename, fstream::in | fstream::out | fstream::app);
    ofs << this->toString() << endl;
    ofs.close();

}

void Route::isObjectInFileDatabase(string filename, string parameter) {
    // TODO
    cerr << "reading Route's info: (NOT IMPLEMENTED)" << Route::toString() << " from file";

}

const list<Point> &Route::getRoutePointsList() const {
    return routePointsList;
}

void Route::setRoutePointsList(const list<Point> &newRoutePointsList) {
    routePointsList = newRoutePointsList;
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

    Taxi(const string &newDriverName, const Car& car, const Route &route)
            : Car(car.getBrand(), car.getModel(), car.getYearsOfUse(), car.getNumSeats(), car.getFuelPerKm(),
                  car.getRegistrationNumber()) {
        setDriverName(newDriverName);
        setCurrentRoute(route);
    }

    Route getCurrentRoute() const {
        return currentRoute;
    }

    void setCurrentRoute(const Route &newRoute) {
        Taxi::currentRoute = newRoute;
    }

    const string &getDriverName() const {
        return driverName;
    }

    void setDriverName(const string &newDriverName) {
        Taxi::driverName = newDriverName;
    }

    void print();

    double calculatePerDayFuelUse();

    string toString() const override;

    void saveToFile(string filename) override;

    void isObjectInFileDatabase(string str, string regNum) override;

    void addNewRoutePoint(const Point &p);

    void addNewCarInfo(const Car &car);

    friend ostream &operator<<(ostream &os, const Taxi &taxi);

    friend istream &operator>>(istream &is, Taxi &taxi);
};

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

double Taxi::calculatePerDayFuelUse() {
    // TODO: да извежда информация колко гориво да се зареди за извършване нa дневната обиколка
    return 0;
}

string Taxi::toString() const {
    return PREFIX_TAXI + ":" + driverName + Car::toString().append(currentRoute.toString());
}

void Taxi::saveToFile(string fileName) {
    cout << "saving taxi: " << Taxi::toString() << " to file:";
    std::ofstream ofs(fileName, fstream::in | fstream::out | fstream::app);
    ofs << Taxi::toString() << endl;
    ofs.close();
}

void Taxi::isObjectInFileDatabase(string filename, string regNum) {
    vector<Taxi> &resultVector();
    // getAndIterateEachLineOfFileWithCallbackSaveIntoVector(filename, resultVector,)
}

void Taxi::addNewRoutePoint(const Point &p) {
    if (Point::isValid(p)) {
        this->getCurrentRoute().addNewRoutePoint(p);
    } else cerr << "addNewRoutePoint -> Point is invalid" << endl; // TODO
}

void Taxi::addNewCarInfo(const Car &car) {
    this->setModel(car.getModel());
    this->setBrand(car.getBrand());
    this->setRegistrationNumber(car.getRegistrationNumber());
    this->setNumSeats(car.getNumSeats());
    this->setFuelPerKm(car.getFuelPerKm());
    this->setYearsOfUse(car.getYearsOfUse());
}

// --------Implementation of static methods--------------------
static void adminMenu() throw(InvalidInputException) {
    cout << "Hello to taxi route system" << endl
         << "Hello taxi manager " << endl
         << "Please choose from the following options:" << endl
         << ADMIN_MENU_NEW_CAR << endl
         << ADMIN_MENU_NEW_ROUTE << endl
         << ADMIN_MENU_NEW_TAXI << endl
         << ADMIN_MENU_NEW_TAXI_NO_ROUTES << endl
         << ADMIN_MENU_CHANGE_TAXI_ROUTE << endl
         << ADMIN_MENU_ADD_REMOVE_POINTS_OF_ROUTE << endl
         << MENU_EXIT_OPTIONS << endl
         << endl;
    bool isLooping = true;
    do {
        char task = 0;
        cin >> task;
        switch (task) {
            case '1' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_CAR << endl;
                isLooping = addNewCars();
                break;
            case '2' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_ROUTE << endl;
                break;
            case '3' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_TAXI << endl;
                break;
            case '4' :
                cout << MENU_CHOSEN << ADMIN_MENU_NEW_TAXI_NO_ROUTES << endl;
                break;
            case '5' :
                cout << MENU_CHOSEN << ADMIN_MENU_CHANGE_TAXI_ROUTE << endl;
                break;
            case '6' :
                cout << MENU_CHOSEN << ADMIN_MENU_ADD_REMOVE_POINTS_OF_ROUTE << endl;
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
                cout << MENU_WRONG_OPTION_ERROR << endl;
                break;
        }
    } while (isLooping);
}

static bool addNewCars() {
    bool addMore = true;
    do {
        Car newCar = Car();
        bool isLooping = true;
        do {
            cout << "Car Menu -> add Car's specifications" << endl
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
                    cout << MENU_CHOSEN << "Save this car to program" << endl;
                    newCar.saveToFile(FILE_PATH_CAR);
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
        } while (isLooping);
        string str;
        cout << "> Add new Car:\n(write q for exit)\n"
             << "Write regNumber:"
             << endl;
        cin >> str;
        cout << str;
        newCar.setRegistrationNumber(str);
        if (str == "q") addMore = false;
    } while (addMore);
    return false;
}

static void clientMenu() throw(InvalidInputException) {
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
    ifstream in(fileName.c_str());
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

// TODO MAKE IT WITH GENERICS !!!!
void extractDataFromLineIntoPointVector(const string &lineOfData, vector<Point> &pointsVector) {

    Point newPoint;

    // regex_replace() for removing the match
    deserializeStrData(lineOfData);
    stringstream streamWithCarData(lineOfData);
    streamWithCarData >> newPoint;
    cout << newPoint.toString(); // DEBUG
    pointsVector.push_back(newPoint);
}*/

template<class T>
static void extractDataFromLineIntoGenericsVector(const string &lineOfData, vector<T> &vectorWithData) {
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

}

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

    cerr << "GenericsList - after saved  " << typeid(T).name() << "  -> " << dataObject.toString() << endl; // DEBUG
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

    cerr << endl << "GenericsObj - after saved  " << typeid(T).name() << "  -> " << dataObject.toString()
         << endl; // DEBUG
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
//   getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_CAR, carsVector,
//                                                         extractDataFromLineIntoGenericsVector);
//    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_POINT, pointVector,
//                                                          extractDataFromLineIntoGenericsVector);
    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_ROUTE, routesVector,
                                                          extractDataFromLineIntoGenericsVector);
//    getAndIterateEachLineOfFileWithCallbackSaveIntoVector(FILE_PATH_TAXI , taxiVector  , extractDataFromLineIntoGenericsVector);
}

void testExtractFunct() {
    string testLine;
    Point point;
    Point p1("aa", 1, 2);
    Point p2("bb", 2, 3);

    extractDataFromLineIntoGenericsObj(p1.toString(), point);
    // Car
    Car car;
    Car c1("newBrand1", "newModel1", 10, 20, 30, "CB1234CB");
    extractDataFromLineIntoGenericsObj(c1.toString(), car);
    // Route
    Route route;
    Route r("route69", 69, 2);
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

////Debug
//template<class T>
//void bla(T clas){
//    cout << (typeid(Taxi).name() == typeid(T).name()) << endl;
//
//}
int main(int argc, const char **argv) {
    testExtractFunct();

    // adminMenu();
    //loadDataVectorsFromFiles(/*carsVector,pointVector,routesVector,taxiVector*/);
//    Car car;
//    Taxi taxi;
//    bla(taxi);
//    testRouteFunctionality();
    //testTaxiFunctionality();


//    if (result) {
//        // Print the vector contents
//        // for(Car car : carsVector){}
//        //cout<< car.toString() << endl;
//    }


    return 0;
}


