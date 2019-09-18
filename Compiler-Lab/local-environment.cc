#include <iostream>
#include <utility>
#include "local-environment.hh"
#include "symbol-table.hh"


/////////////////
// Symbol_Table::create
/////////////////


void Symbol_Table::create(Local_Environment & local_global_variables_table){
	Local_Environment * localenv = (Local_Environment *) & local_global_variables_table;
	if (this->get_table_scope() == global)
	{
		for (list<Symbol_Table_Entry *>::iterator it = variable_table.begin(); it != variable_table.end(); ++it)
		{
			Symbol_Table_Entry * sentry = *it;
			if (sentry->get_data_type() == int_data_type)
			{
				Eval_Result_Value_Int * val = new Eval_Result_Value_Int();
				val->set_value(0);
				localenv->put_variable_value(*val, sentry->get_variable_name());
			}
			else if(sentry->get_data_type() == double_data_type)
			{
				Eval_Result_Value_Double * val = new Eval_Result_Value_Double();
				val->set_value(0.0);
				localenv->put_variable_value(*val, sentry->get_variable_name());
			}
			else{
				printf("cs316: Error: in Symbol_Table::create non-int/double data_type\n");
			}
		}
	}
	else if (this->get_table_scope() == local)
	{
		for (list<Symbol_Table_Entry *>::iterator it = variable_table.begin(); it != variable_table.end(); ++it)
		{
			Symbol_Table_Entry * sentry = *it;
			if (sentry->get_data_type() == int_data_type)
			{
				Eval_Result_Value_Int * val = new Eval_Result_Value_Int();
				localenv->put_variable_value(*val, sentry->get_variable_name());
			}
			else if(sentry->get_data_type() == double_data_type)
			{
				Eval_Result_Value_Double * val = new Eval_Result_Value_Double();
				localenv->put_variable_value(*val, sentry->get_variable_name());
			}
			else{
				printf("cs316: Error: in Symbol_Table::create non-int/double data_type\n");
			}
		}
	}
}

/////////////////
// Eval_Result
/////////////////

// Eval_Result::Eval_Result(){}
// Eval_Result::~Eval_Result(){}

int Eval_Result::get_int_value(){
	printf("%s\n", "cs316: Error: This function (get_int_value) is not defined for the parent Eval_Result class");
	exit(0);
}

void Eval_Result::set_value(int value){
	printf("%s\n", "cs316: Error: This function (set_value(i)) is not defined for the parent Eval_Result class");
	exit(0);
}

double Eval_Result::get_double_value(){
	printf("%s\n", "cs316: Error: This function (get_double_value) is not defined for the parent Eval_Result class");
	exit(0);	
}

void Eval_Result::set_value(double value){
	printf("%s\n", "cs316: Error: This function (set_value(d)) is not defined for the parent Eval_Result class");
	exit(0);
}

bool Eval_Result::is_variable_defined(){
	printf("%s\n", "cs316: Error: This function (is_variable_defined) is not defined for the parent Eval_Result class");
	exit(0);
}

void Eval_Result::set_variable_status(bool def){
	printf("%s\n", "cs316: Error: This function (set_variable_status) is not defined for the parent Eval_Result class");
	exit(0);	
}


///////////////////////
// Eval_Result_Value
///////////////////////

// Eval_Result_Value::Eval_Result_Value(){}
// Eval_Result_Value::~Eval_Result_Value(){}

int Eval_Result_Value::get_int_value(){
	printf("%s\n", "cs316: Error: This function (get_int_value) is not defined for the parent Eval_Result_Value class");
	exit(0);
}

void Eval_Result_Value::set_value(int value){
	printf("%s\n", "cs316: Error: This function (set_value(i)) is not defined for the parent Eval_Result_Value class");
	exit(0);
}

double Eval_Result_Value::get_double_value(){
	printf("%s\n", "cs316: Error: This function (get_double_value) is not defined for the parent Eval_Result_Value class");
	exit(0);	
}

void Eval_Result_Value::set_value(double value){
	printf("%s\n", "cs316: Error: This function (set_value(d)) is not defined for the parent Eval_Result_Value class");
	exit(0);
}


///////////////////////////
// Eval_Result_Value_Int
///////////////////////////

Eval_Result_Value_Int::Eval_Result_Value_Int(){
	value = 0;
	defined = false;
	result_type = int_result;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int(){}

int Eval_Result_Value_Int::get_int_value(){
	return value;
}

void Eval_Result_Value_Int::set_value(int number){
	value = number;
	defined = true;
}

void Eval_Result_Value_Int::set_value(double number){
	value = (int) number;
	defined = true;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	printf("cs316: Warning: overwriting 'defined' flag manually\n");
	defined = def;
}

bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	printf("cs316: Warning: overwriting 'result_type' flag manually\n");
	result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}


//////////////////////////////
// Eval_Result_Value_Double
//////////////////////////////
Eval_Result_Value_Double::Eval_Result_Value_Double(){
	value = 0;
	defined = false;
	result_type = double_result;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double(){}

double Eval_Result_Value_Double::get_double_value(){
	return value;
}

void Eval_Result_Value_Double::set_value(int number){
	value = (double) number;
	defined = true;
}

void Eval_Result_Value_Double::set_value(double number){
	value = number;
	defined = true;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	printf("cs316: Warning: overwriting 'defined' flag manually\n");
	defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	printf("cs316: Warning: overwriting 'result_type' flag manually\n");
	result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}


//////////////////////////////
// Local_Environment
//////////////////////////////

Local_Environment::Local_Environment(){}

Local_Environment::~Local_Environment(){}

void Local_Environment::print(ostream & file_buffer){
	for (map<string, Eval_Result*>::iterator it = variable_table.begin(); it != variable_table.end(); ++it)
	{
		file_buffer << VAR_SPACE << it->first << " : ";
		//printf("Hello: %d\n",(it->second) == NULL);
		if ((it->second)->is_variable_defined())
		{
			switch((it->second)->get_result_enum()){
				case int_result:
					file_buffer << (it->second)->get_int_value() << "\n";
					break;
				case double_result:
					file_buffer << (it->second)->get_double_value() << "\n";
					break;
				default	:
					file_buffer << "undefined\n";
			}
		}
		else{
			file_buffer << "undefined\n";
		}
	}
}

// assumes element with key is present
bool Local_Environment::is_variable_defined(string name){
	if(does_variable_exist(name)){
		return (variable_table.find(name)->second)->is_variable_defined();
	}
	else{
		return false;
	}
}

// assumes element with key is present
Eval_Result * Local_Environment::get_variable_value(string name){
	return variable_table.find(name)->second;
}

void Local_Environment::put_variable_value(Eval_Result & value, string name){
	pair<map<string, Eval_Result*>::iterator, bool> ret;
	if (value.get_result_enum() == int_result){
		Eval_Result_Value_Int * new_eval = new Eval_Result_Value_Int();
		if (value.is_variable_defined()){
			new_eval->set_value(value.get_int_value());	
		}	
		pair<string, Eval_Result*> ins_pair(name, new_eval);
		ret = variable_table.insert(ins_pair);	
	}
	else{
		Eval_Result_Value_Double * new_eval = new Eval_Result_Value_Double();
		if (value.is_variable_defined()){			
			new_eval->set_value(value.get_double_value());	
		}	
		pair<string, Eval_Result*> ins_pair(name, new_eval);
		ret = variable_table.insert(ins_pair);
	}

	
	
	if (ret.second == false)
	{
		if (value.get_result_enum() == int_result)
		{
			((ret.first)->second)->set_value(value.get_int_value());	
		}
		else{
			((ret.first)->second)->set_value(value.get_double_value());
		}		
	}
}

bool Local_Environment::does_variable_exist(string name){
	map<string, Eval_Result*>::iterator it;
	it = variable_table.find(name);
	if (it != variable_table.end())
	{
		return true;
	}
	else{
		return false;
	}
}
