/*
 * company.cpp
 *
 *  Created on: Sep 8, 2022
 *      Author: aseel
 */
#include "../headers/company.h"
#include <thread>
#include <algorithm>
#include <set>
#include <unordered_map>
using namespace std;

 Company * Company::companyObject;
 vector<Employee> Company::allEmployeesOfDepartments  = {};//ALL EMPS FILLED BY A FUNCTION IN COMPANY-with duplication
 vector<Employee> Company::empsOfAllCompany = {};
 set<string> Company::childrenOfDepartments={};
 vector<Employee> Company::listOfFloatingEmps={};
 vector<Employee> Company::employeesOfMultiDeps_Results={};
 vector<Employee> Company::employeesOfDepartments={};//all employees in company without multiplication
 int Company::flag=0;
 Department *Company::foundDepartment=nullptr;
Company::Company(){
	this->ceoEmployee=new Employee("Tareq",35,CEO,50000);
	empsOfAllCompany.push_back(*ceoEmployee);
     cout<<"company created successfully!"<<endl;
}

Employee Company::getCeoEmployee(){
     return *ceoEmployee;
}
void Company::setCeoEmployee(Employee ceo_employee){
     this->ceoEmployee=&ceo_employee;
     empsOfAllCompany[0]=ceo_employee;

}
void Company::setMainDepartments(vector<Department> main_department){
     this->mainDepartments=main_department;
}
vector<Department> *Company::getMainDepartments(){
     return &mainDepartments;
}
Department* Company::addMainDepartmentToCompany(Department department){
	auto department_iterator=find(getMainDepartments()->begin(),getMainDepartments()->end(), department) ;
	if(department_iterator== getMainDepartments()->end()){
		mainDepartments.push_back(department);
		cout<<"Main-Department "<<department.getDepartmentName()<<" has added successfully!"<<endl;
	}else{
		cout<<department.getDepartmentName()<<" is already a main department !"<<endl;
		return &(*department_iterator);
	}
}

void Company::removeMainDepartmentFromCompany(Department department){
	auto i=find(getMainDepartments()->begin(),getMainDepartments()->end(), department);
	if(i != this->getMainDepartments()->end()){
		mainDepartments.erase(i);
          cout<<"department has bee deleted successfully"<<endl;
	 }else{
		 cout<<"Department with  "<< department.getDepartmentName() <<"  dose not exists in company"<<endl;
	     return;
	 }
}
void Company::removeMainDepartmentFromCompany(string department_name){
	auto i=find(getMainDepartments()->begin(),getMainDepartments()->end(), department_name);
	if(i != this->getMainDepartments()->end()){
		mainDepartments.erase(i);
          cout<<"Main_Department has been deleted successfully"<<endl;
	 }else{
		 cout<<"Department with  "<<department_name<<"  is NOT a MAIN department of the company"<<endl;
	     return;
	 }
}

void Company::getAllEmployeesUsingThreads(Department department,Company* company_object){
	company_object->allEmployeesOfDepartments.insert(company_object->allEmployeesOfDepartments.end(), department.getEmployeesOfDepartment()->begin(),department.getEmployeesOfDepartment()->end() );//allowing duplicating) for further use in employees of multiple departments API
     for(auto it =department.getEmployeesOfDepartment()->begin();it !=department.getEmployeesOfDepartment()->end();it++){
    	 if(find(company_object->employeesOfDepartments.begin(),company_object->employeesOfDepartments.end(),(*it))==company_object->employeesOfDepartments.end()){//Checking of this employee is already in empsOfDeps--if found don't add it
    		 company_object->employeesOfDepartments.push_back((*it));//if not found --> add it
    	 }
     }
    if(department.isAnySubDeps()==1){//checking if department has sub department or not ---if it has,it calls allEmpsFun function on each sub department of that department
    	for(auto f=department.getSubDepartments()->begin();f!=department.getSubDepartments()->end();f++){
    		getAllEmployeesUsingThreads((*f),company_object);
    	}
    }
    else{
       return ;
    }
}
vector<Employee> Company::getAllEmployees(){
	allEmployeesOfDepartments.clear();// a vector of employees in departments (allowing duplicating) for further use in employees of multiple departments API
	employeesOfDepartments.clear();// a vector of employees in departments(without allowing duplicating)
    int size=(int) mainDepartments.size();
    for(int i=0;i<size;i++){
    	 thread th(getAllEmployeesUsingThreads,mainDepartments.at(i),this);//a thread for each Main Department in company
    	 th.join();
    }
    return this->employeesOfDepartments;
}
int Company::getEmployeesWithSameSalary(){
	int numberOfEmploeesWithSameSalaries=0;//number of all the employees getting the same salaries all together
	unordered_map<int, int> sameSalayMap;//unordered_map to store <salary,number of employees getting this salary>
	for(auto i=empsOfAllCompany.begin();i!=empsOfAllCompany.end();i++){//loop through the employees in company to fill the unordered_map
		sameSalayMap[i->getSalary()]++;
	}
    for (auto i = sameSalayMap.begin(); i != sameSalayMap.end(); i++){//loop to print the salaries taken by multiple employees and the number of employees getting this salary
    	if(i->second>1){
		 cout << i->first << "   :   " << i->second<< endl;
		 numberOfEmploeesWithSameSalaries+=i->second;
    	}
    }
	return numberOfEmploeesWithSameSalaries;
}
void Company::addEmployeeToCompany(Employee emp){//employees who are in company but not in department
	auto i=find(this->empsOfAllCompany.begin(), this->empsOfAllCompany.end(), emp);
	if(i != this->empsOfAllCompany.end()){
		cout<<"employee already exists in Company"<<endl;
		return;
	}else{
		this->empsOfAllCompany.push_back(emp);
		cout<<"Employee added successfully to the Company!"<<endl;
		return;
	}
}
void Company::removeEmployeeFromCompany(Employee emp){//employees who are in company but not in department
	auto i=find(this->empsOfAllCompany.begin(), this->empsOfAllCompany.end(), emp);
	if(i != this->empsOfAllCompany.end()){
		this->empsOfAllCompany.erase(i);
	}
	else{
		cout<<"employee with empID number: "<<emp.getEmployeeId()<<" is not part of the company"<<endl;
	}
}
vector<Employee> Company::getEmployeesOfMultipleDepartments(){ //finding employees of multiple departments
	getAllEmployees();
	vector<Employee>  EmpsOfDeps=this->allEmployeesOfDepartments;//getting employees of department (with multiplication)
	employeesOfMultiDeps_Results={};//vector to store the employees of multiple departments
	set<int> SetOfEmps;//set of employees in department without Repetition
	for(auto i=EmpsOfDeps.begin();i!=EmpsOfDeps.end();i++){
		if(SetOfEmps.find((*i).getEmployeeId())==SetOfEmps.end()){// if not found in setOfEmps add it in SetOfEmps
			SetOfEmps.insert((*i).getEmployeeId());
		}else{//if found -then it is an employee of multiDep--we add it to empsOfMultiDeps_Results
			employeesOfMultiDeps_Results.push_back((*i));
		}
	}
	cout<<"Employees of multi-Departments : "<<endl;//printing Employees of multi-Departments
	for(auto i=employeesOfMultiDeps_Results.begin();i!=employeesOfMultiDeps_Results.end();i++){
		cout<<"Employee name & ID : "<<	i->getName()<<"  |  "<<i->getEmployeeId()<<endl;;
	}
	return employeesOfMultiDeps_Results;
}
bool Company::isThereAnyLoopsInDepartments(){//returns true if we have a department has two parent departments
	childrenOfDepartments={}; //clear set<string> ChildsOfDeps since it is a static
	bool result;//to return the result
	for(auto it1=this->getMainDepartments()->begin();it1!=this->getMainDepartments()->end();it1++){//searching in Main Departments of the company
		for(auto it2=(*it1).getSubDepartments()->begin();it2!=(*it1).getSubDepartments()->end();it2++){//children of main deps
			if(childrenOfDepartments.find((*it2).getDepartmentName())==childrenOfDepartments.end()){//if department is already in ChildsOfDeps then returns true --if not add it to ChildsOfDeps
				childrenOfDepartments.insert((*it2).getDepartmentName());
			}else{
				return true;
			}
			if((*it2).isAnySubDeps()==1){ //checking if department has subDepartments ---if yes we call loop_IN_Deps_hand on that department
				result=isThereAnyLoopsInDepartmentsHand((*it2));
				if(result==1){
					return true;
				}
			}

		}
	}
	return false;
}
bool Company::isThereAnyLoopsInDepartmentsHand(Department parent_department){//recursion function to search for loops in subDepartments
	bool result;
	for(auto it1=parent_department.getSubDepartments()->begin();it1!=parent_department.getSubDepartments()->end();it1++){
		if(childrenOfDepartments.find((*it1).getDepartmentName())==childrenOfDepartments.end()){
			childrenOfDepartments.insert((*it1).getDepartmentName());
		}else{
			return true;//child already exists
		}
		if((*it1).isAnySubDeps()==1){
			result=isThereAnyLoopsInDepartmentsHand((*it1));
			if(result==1){
				return true;
			}
		}
	}
	return false;
}
bool Company::getfloatingEmployees(){
	 listOfFloatingEmps={};// clearing vector<Empolyee> listOfFloatingEmps since it is static data member
	vector<Employee> getAllEmpsOfDeps=getAllEmployees();//getting Employees of all departments
	for(auto i=empsOfAllCompany.begin(); i!=empsOfAllCompany.end();i++){//loop in EmpsOfAllCompany (contains employees of departments and employees with no department)
		if(find(getAllEmpsOfDeps.begin(),getAllEmpsOfDeps.end(),(*i))==getAllEmpsOfDeps.end()){//cheacking if employee in EmpsOfAllCompany is also an employee in  department --if no
			if((*i).getRole(this)!="CEO"){//if this employee(with no department) is NOT the CEO then add it to listOfFloatingEmps
				listOfFloatingEmps.push_back((*i));
			}
		}
	}
	if(!listOfFloatingEmps.empty()){//printing the floating employees of company
		cout<<"-----------------------------------------------------------------------"<<endl;
		cout<<"Floating employees in company are: "<<endl;
		for(auto i=listOfFloatingEmps.begin();i!=listOfFloatingEmps.end();i++){
			cout<<"Employee name : "<<(*i).getName()<<"  |  Employee ID : "<<(*i).getEmployeeId()<<endl;
		}
		cout<<"-----------------------------------------------------------------------"<<endl;
		return true;
	}
	cout<<"Company doesn't have any floating employees "<<endl;
	return false;
}

Department * Company::findDepartment(Department *needed_department){
	foundDepartment=nullptr;
	flag=0;//department not found
	int size=(int) this->getMainDepartments()->size();
	    for(int i=0;i<size;i++){
	    	Company::findDepartmenthand(needed_department,&(this->getMainDepartments()->at(i)),flag);
	    	if(foundDepartment!=nullptr && flag==1){
	    		return foundDepartment;
	    		break;
	    	}else if(foundDepartment==nullptr && flag==0){
	    		continue;
	    	}
	    }
	    return nullptr;
}
Department *Company::findDepartmenthand(Department *needed_department,Department *parent_department,int &flag){
	if (flag==1){
		return foundDepartment;
	}
	if((*needed_department)==(*parent_department)){
		flag=1;
		foundDepartment = parent_department;
		return foundDepartment;
	}if(flag==0){
	if(parent_department->isAnySubDeps()==1){
	    	for(auto f=parent_department->getSubDepartments()->begin();f!=parent_department->getSubDepartments()->end();f++){
	    		Company::findDepartmenthand(needed_department,&(*f),flag);
	    	}
	  }
	return nullptr;
	}


}
Employee * Company::findEmployeeInDepartment(Employee needed_employee,vector<Department>* range_of_departments){//for hirarichy
	for( auto it1=range_of_departments->begin();it1!=range_of_departments->end();it1++){
		auto it2=find((*it1).getEmployeesOfDepartment()->begin(),(*it1).getEmployeesOfDepartment()->end(),needed_employee);
		if(it2 != (*it1).getEmployeesOfDepartment()->end()){
			return &(*it2);
		}
		return findEmployeeInDepartment(needed_employee,(*it1).getSubDepartments());//for hirarichy
	}
}


Employee *Company::findEmployeeInCompany(Employee employee){
	auto i=find(this->empsOfAllCompany.begin(),this->empsOfAllCompany.end(),employee);
	if(i != this->empsOfAllCompany.end()){
		cout<<"employee found in EmpsOfAllCompany"<<endl;
		return &(*i);
	}
	else{
		cout<<"employee not found in company"<<endl;
		return nullptr;
	}
}
Company::~Company(){
	cout<<"Company has deleted successfully, in Company destructor"<<endl;
}



