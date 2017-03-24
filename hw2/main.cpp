#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cstdio>
using namespace std;


class Student{
private:
    string fullName;
    string id;
    string year;
    string gender;

public:
    Student(string& fullName, string& id, string& year, string& gender){
        setFullName(fullName);
        setId(id);
        setYear(year);
        setGender(gender);
    }

    void setFullName(string& fullName){
        this->fullName = fullName;

    }

    /**
     * @brief setId
     * @param id
     * @return true if id is set, false otherwise
     */
    bool setId(string& id){
        for(unsigned int i = 0; i < id.length(); i++){
            char c = id[i];
            if(!(c  >= '0' && c <= '9')){
                cout << "Invalid Id" << endl;
                return false;
            }
        }
        this->id = id;
        return true;
    }

    string getName() const {
        return this->fullName;
    }

    void setYear(const string& year){
        this->year = year;
     }

    void setGender(const string& gender){
        this->gender = gender;
    }

    string getYear() const {
        return this->year;
    }

    string getId() const {
        return this->id;
    }

    string getGender() const {
        return this->gender;
    }

};


class StudentsManager{

private:
    map<string, Student&> studentMap;
    const char* path = "/home/rockink/cs410/hw2/test.txt";

    //student save format
    // id,name,year,gender
    string formatToCsv(const Student& student){

        string temp = student.getName() +"," +
                student.getId() + "," +
                student.getYear() + "," +
                student.getGender() +"\n";
        return temp;
    }

public:

    void addStudent(Student& student){
        studentMap.insert(std::pair<string, Student&>(student.getId(), student));
    }


    void removeStudent(const string& id){
        studentMap.erase(id);
    }



    void updateStudent(Student& student){
        studentMap.insert(std::pair<string, Student&>(student.getId(), student));
    }


    /**
     * @brief getStudent should also handle the end case, which I haven't done it.
     * @param id
     * @return
     */
    Student& getStudent(const string& id){
        map<string, Student&>::iterator pos = studentMap.find(id);
        Student& student = pos->second;
        cout << "getid " << student.getId() << endl;
        return student;
    }


    void print(Student& student) {
        cout << "Student Information:" << endl;
        cout << "Name Of Student: " << student.getName() <<  endl;
        cout << "ID: " << student.getId() << endl;
        cout << "Year: " << student.getYear() << endl;
        cout << "Gender: " << student.getGender() << endl;
    }

    void printByFiler(const string& filter="all"){
        for (map<string, Student&>::const_iterator it = studentMap.begin();
                it != studentMap.end(); ++it){
            Student student = it->second;
            if(filter == "all"){
                print(student);
                continue;
            }else{
                if (student.getYear() == filter){
                    print(student);
                }
            }
        }
    }


    void saveToDatabase(string& in){
        ofstream file;
        file.open(in.c_str(), fstream::out);

        for (map<string, Student&>::const_iterator it = studentMap.begin();
                it != studentMap.end(); ++it){
            Student& student = it->second;
            file << formatToCsv(student);
        }

        file.close();
    }

    void readFromDatabase(string& path){

        ifstream file(path.c_str(), ifstream::in);
        string test;

        for(string fullName; getline(file, fullName, ',');){
            string id;
            string year;
            string gender;

            getline(file, id, ',');
            getline(file, year, ',');
            getline(file, gender, '\n');
            Student *student = new Student(fullName, id, year, gender);
            addStudent(*student);
        }

        file.close();

    }



    /**
     * @brief isIdAlreadyDefined, if only it is not the end
     * @param id
     * @return
     */
    bool isIdAlreadyDefined(string& id){
        return studentMap.find(id) != studentMap.end();
    }


};


class Controller{
private:
    string array[7] = {
        "1. Add Records",
        "2. List Records",
        "3. Modify Records",
        "4. Delete Records",
        "5. Read to Database",
        "6. Write to Database",
        "7. Exit Program"
    };

    string modifyStudentOptions[4] = {
      "1. Modify Name",
      "2. Modify Year",
      "3. Modify Gender"
      "4. Goto Main Menu"
    };

    string validYear[4]={"freshman", "sophomore", "junior", "senior"};
    string validGender[2] = {"male", "female"};


    StudentsManager *studentsManager;

    //helper function
    string readInput(const string& inputInfo){
        cout << inputInfo << "==>";
        string input;
        cin >> input;
        return input;
    }

    /**
     * @brief readUniqueId reads only the unique id
     * @param inputInfo
     * @return the unique id
     */
    string readUniqueId(const string& inputInfo){
        string id;
        do {
            id = readInput(inputInfo);
        } while (studentsManager->isIdAlreadyDefined(id));
        return id;
    }


    bool isInTheList(const string& input, string* inputList, int& lengthOfList){
        for(int i = 0; i < lengthOfList; i++){
            if (input == *(inputList + i)){
                return true;
            }
        }
        return false;
    }

    string readFilteredInput(const string& inputInfo, string* inputList, int lengthOfList){
        string input;
        do {
            input = readInput(inputInfo);
        } while (!isInTheList(input, inputList, lengthOfList));
        return input;
    }

    void addStudent(){
        string name = readInput("Enter Student's Name");
        string id = readUniqueId("Enter Unique Student's id");
        string yearOfStudy = readFilteredInput("Enter your year of study", validYear, 4);
        string gender = readFilteredInput("Enter Your Gender", validGender, 2);
        Student *student = new Student(name, id, yearOfStudy, gender);
        studentsManager->addStudent(*student);
    }


    void removeStudent(){
        string id = readInput("Enter Student's id");
        if (studentsManager->isIdAlreadyDefined(id)){
            Student student = studentsManager->getStudent(id);
            cout << "Found the student. Removing..." << endl;
            studentsManager->print(student);
            studentsManager->removeStudent(id);
            cout << "Done!\n";
        }else{
            cout << "Whooaa! No Student of that id found!!" << endl;
        }
    }

    void listStudents(){
        studentsManager->printByFiler();
    }

    /**
     * @brief executeStudentEditOption
     * @param option
     * @param student
     *
     * option is selected in this manner: Copied from optionEditArray
     *
        string modifyStudentOptions[4] = {
          "1. Modify Name",
          "2. Modify Year",
          "3. Modify Gender"

        };
     *
     */
    bool executeStudentEditOption(int& option, Student& student){
        switch (option) {
        case 1:{
             string newName = readInput("Enter New Name");
             student.setFullName(newName);
             return false;
        }
        case 2:{
            string newYear = readFilteredInput("Enter New Year", validYear, 4);
            student.setYear(newYear);
            return false;
        }
        case 3:{
            string newGender = readFilteredInput("Enter New Gender", validGender, 2);
            student.setGender(newGender);
            return false;
        }
        case 4:
            return true;
        default:
            return false;
        }
    }

    void printEditStudentOption(){
        for(int i = 0; i < 5; i++){
            cout << modifyStudentOptions[i] <<endl;
        }
    }

    void editStudent(){
        string id = readInput("Input ID for student retrieval");
        if(studentsManager->isIdAlreadyDefined(id)){
            Student& student = studentsManager->getStudent(id);
            studentsManager->print(student);

            int option;
            do{

                this->printEditStudentOption();
                cout << "Select the option : ";
                cin >> option;
            }while(!this->executeStudentEditOption(option, student));

        }else{
            cout << "No Student Defined by that id " << endl;
        }
    }


    void readFromDatabase(){
        string in = readInput("Select the ABSOLUTE PATH");
        studentsManager->readFromDatabase(in);
    }

    void writeToDatabase(){
        string in = readInput("Select the ABSOLUTE PATH");
        studentsManager->saveToDatabase(in);
    }

public:
    Controller(StudentsManager* studentManager) {
        this->studentsManager = studentManager;
    }

    void printMenu(){
        for(int i = 0; i < 7; i++){
            cout << array[i] <<endl;
        }
    }


    /**
     * @brief executeOption
     * @param option
     * This option is based on the string up there called array
     */
    bool executeOption(int& option){
        switch (option) {
        case 1:{ //add student
            //show student
            addStudent();
            return true;
        }
        case 2:{ //list students
            listStudents();
            return true;
        }
        case 3:{
            editStudent();
            return true;
        }
        case 4:{
            //remove student by Id
            removeStudent();
            return true;
        }
        case 5: {//read to database
            readFromDatabase();
            return true;
        }
        case 6:{ //write to database
            writeToDatabase();
            return true;
        }
        case 7:{
            return false;
        }
        default:
            break;
        }
    }

};



/**
 * @brief main
 * @param argc
 * @param argv
 * @return main for the program.
 *
 */
int main(int argc, char *argv[]){

    /**
     * @brief studentsManager holds the map of students, uniquely identified by id,
     * and it is a class that provides the logic of the transaction of the students,
     * such as add, remove, display.
     *
     */
    StudentsManager *studentsManager = new StudentsManager;


    /**
     * @brief controller provides an interface that interacts with the user, and deisgnates
     * where the control should go to. It interacts with the user.
     */
    Controller *controller = new Controller(studentsManager);


    int option;
    do {
        controller->printMenu();
        cout << "Select the option : ";
        cin >> option;
    }while (controller->executeOption(option));
    return 0;
}
