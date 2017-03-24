#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;


class Student{
private:
    string fullName;
    string id;
    string year;
    string gender;
    string validYear[] = {"freshman", "sophomore", "junior", "senior"};
    string validGender[] = {"male", "female"};
    /**
     * TODO To-lower is to be defined
     * @brief isValidYear
     * @param year
     * @return
     */
    bool isValidYear(const string& year){
        for(int i = 0; i < 4; i++){
            if (validYear[i]  == year){
                return true;
            }
        }
        return false;
    }

    bool isValidGender(const string& gender){
        for(int i = 0; i < 2; i++){
            if (validGender[i] == gender){
                return true;
            }
        }
        return false;
    }

public:
    Student(string& fullName, string& id, string& year, string& gender){
        setFullName(fullName);
        if (!((setId(id) && setYear(year) && setGender(gender)))){
            cout << "creation failed!" << endl;
        }
    }

    void setFullName(String& fullName){
        this->fullName = fullName;

    }

    /**
     * @brief setId
     * @param id
     * @return true if id is set, false otherwise
     */
    bool setId(String& id){
        for(int i = 0; i < id.length(); i++){
            char c = id[i];
            if (!(id[i] >= '0' && id[i] =< '9')){
                cout << "Invalid Id" << endl;
                return false;
            }
        }
        this->id = id;
        return true;
    }

    bool setYear(const string& year){
        if (isValidYear(year)){
            this->year = year;
            return true;
        }
        return false;
    }

    bool setGender(const string& gender){
        if (isValidGender(gender)){
            this->gender = gender;
            return true;
        }
        return false;
    }

    string getYear(){
        return this->year;
    }

    string getId(){
        return this->id;
    }

};


class StudentsManager{

private:
    map<string, Student> studentMap;

    //student save format
    // id,name,year,gender
    string& formatToCsv(const Student& student){

    }

public:

    void addStudent(Student student){
        studentMap.insert(std::pair<string, Student>(student.getId(), student));
    }


    void removeStudent(const string& id){
        studentMap.erase(id);
    }

    void updateStudent(Student student){
        studentMap.insert(std::pair<string, Student>(student.getId(), student));
    }

    Student& getStudent(const string& id){
        return studentMap.find(id);
    }

    void printByFiler(const string& filter="all"){
        for(pair<string, Student>& each : studentMap){
            Student& Student = each.second;
            if(filter == "all"){
                print(student);
                continue;
            }else{
                if (Student.getYear() == filter){
                    print(student);
                }
            }
        }
    }


    void saveToDatabase(){
        ofstream file;
        file.open("database.txt", fstream::app);

        for(pair<string, Student>& each : studentMap){
            Student& Student = each.second;
            file << formatToCsv(student);

        }
        file.close();
    }

    void loadFromFile(){

    }



};



int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    return 0;
}
