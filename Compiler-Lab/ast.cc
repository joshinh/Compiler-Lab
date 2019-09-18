// #include "ast.hh"
#include <stdlib.h>

template class Number_Ast<int>;
template class Number_Ast<double>;

int Ast::labelCounter;

/////////////////////////////////////////////////
// Ast - parent class
/////////////////////////////////////////////////
Ast::Ast(){}

Ast::~Ast(){}

Data_Type Ast::get_data_type(){
	printf("%s\n", "cs316: This function (get_data_type) is not defined for the parent Ast class");
	exit(0);
}

void Ast::set_data_type(Data_Type dt){
	printf("%s\n", "cs316: This function (set_data_type) is not defined for the parent Ast class");
	exit(0);
}

bool Ast::is_value_zero(){
	return false;
}

bool Ast::check_ast(){
	printf("%s\n", "cs316: This function (check_ast) is not defined for the parent Ast class");
	exit(0);
}

Symbol_Table_Entry & Ast::get_symbol_entry(){
	printf("%s\n", "cs316: This func5tion (get_symbol_entry) is not defined for the parent Ast class");
	exit(0);
}

/////////////////////////////////////////////////
// Assignment_Ast
/////////////////////////////////////////////////
Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
  	if(temp_lhs == NULL || temp_rhs == NULL){
      	printf("%s\n", "cs316: Error: lhs or rhs is NULL in Assignment_Ast");
      	exit(0);
    }
  	this->lhs = temp_lhs;
 	this->rhs = temp_rhs;
  	this->lineno = line;
  	this->ast_num_child = binary_arity;
  	this->node_data_type = temp_lhs->get_data_type();
  	
}

Assignment_Ast::~Assignment_Ast(){
	if(this->lhs != NULL)
		delete this->lhs;
  	if (this->rhs != NULL)
  		delete this->rhs;
}

bool Assignment_Ast::check_ast(){
	if (this->lhs == NULL || this->rhs == NULL){
      	printf("%s\n", "cs316: Error: lhs or rhs is NULL in Assignment_Ast");
    	return false;
    }
    if (this->ast_num_child != binary_arity)
    {
    	printf("%s\n", "cs316: Error: arity is not 2 in Assignment_Ast");
    	return false;
    }
	// later work
	// if(typeid(*lhs).name() != Name_Ast)
	return true;
}

void Assignment_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "Asgn:\n";
	file_buffer << AST_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////
// Name_Ast
/////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	this->variable_symbol_entry = &var_entry;
	this->lineno = line;
	this->ast_num_child = unary_arity;
	this->node_data_type = var_entry.get_data_type();
}

Name_Ast::~Name_Ast(){
	if (this->variable_symbol_entry != NULL)
	{
		delete this->variable_symbol_entry;
	}
}

Data_Type Name_Ast::get_data_type(){
	return this->node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *(this->variable_symbol_entry);
}

void Name_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
	file_buffer << "Name : " << (this->variable_symbol_entry)->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){

}

/////////////////////////////////////////////////
// Number_Ast
/////////////////////////////////////////////////

template<class T> Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	this->constant = number;
	this->lineno = line;
	this->node_data_type = constant_data_type;
	this->ast_num_child = unary_arity;
}

template<class T> Number_Ast<T>::~Number_Ast(){}

template<class T> Data_Type  Number_Ast<T>::get_data_type(){
	return this->node_data_type;
}

template<class T> void Number_Ast<T>::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

template<class T> bool Number_Ast<T>::is_value_zero(){
	if (constant == 0)
		return true;
	else
		return false;	
}

template<class T> void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer << "Num : " << (this->constant);
}

/////////////////////////////////////////////////
// Arithmetic_Expr_Ast
/////////////////////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return this->node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
	if (this->ast_num_child == binary_arity)
	{
		if (lhs == NULL || rhs == NULL || lhs->get_data_type() != rhs->get_data_type())
		{
			printf("%s\n", "cs316: Error: lhs/rhs are NULL or data_type doesn't match in binary_arity Arithmetic_Expr_Ast");
			return false;
		}
	}
	else if (this->ast_num_child == unary_arity)
	{
		if (this->lhs == NULL)
		{
			printf("%s\n", "cs316: Error: lhs is NULL in unary_arity Arithmetic_Expr_Ast");
			return false;
		}
	}
	// later work
	// if(typeid(*lhs).name() != Name_Ast)
	return true;
}

/////////////////////////////////////////////////
// Plus_Ast
/////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast *l, Ast *r, int line){
	if (l == NULL || r == NULL){
		printf("cs316: Error: Null argument in Plus_Ast\n");
		exit(0);
	}
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
}

// Plus_Ast::~Plus_Ast(){
// 	if (this->lhs != NULL)
// 		delete this->lhs;
// 	if (this->rhs != NULL)
// 		delete this->rhs;
// }

void Plus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: PLUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";  
}

/////////////////////////////////////////////////
// Minus_Ast
/////////////////////////////////////////////////


Minus_Ast::Minus_Ast(Ast *l, Ast *r, int line){
	if (l == NULL || r == NULL){
		printf("cs316: Error: Null argument in Minus_Ast\n");
		exit(0);
	}
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();

}

// Minus_Ast::~Minus_Ast(){
// 	if (this->lhs != NULL)
// 		delete this->lhs;
// 	if (this->rhs != NULL)
// 		delete this->rhs;
// }

void Minus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";  
}

/////////////////////////////////////////////////
//Divide_Ast
/////////////////////////////////////////////////


Divide_Ast::Divide_Ast(Ast *l, Ast *r, int line){
	if (l == NULL || r == NULL){
		printf("cs316: Error: Null argument in Divide_Ast\n");
		exit(0);
	}
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
}

// Divide_Ast::~Divide_Ast(){
// 	if (this->lhs != NULL)
// 		delete this->lhs;
// 	if (this->rhs != NULL)
// 		delete this->rhs;
// }

void Divide_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: DIV\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";  
}


/////////////////////////////////////////////////
// Mult_Ast
/////////////////////////////////////////////////


Mult_Ast::Mult_Ast(Ast *l, Ast *r, int line){
	if (l == NULL || r == NULL){
		printf("cs316: Error: Null argument in Mult_Ast\n");
		exit(0);
	}
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
	this->ast_num_child = binary_arity;
	this->node_data_type = l->get_data_type();
}

// Mult_Ast::~Mult_Ast(){
// 	if (this->lhs != NULL)
// 		delete this->lhs;
// 	if (this->rhs != NULL)
// 		delete this->rhs;
// }

void Mult_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MULT\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";  
}


/////////////////////////////////////////////////
// UMinus_Ast
/////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	if (l == NULL)
	{
		printf("%s\n", "cs316: Error: first argument NULL in UMinus_Ast");
		exit(0);
	}
	this->lhs = l;
	this->rhs = NULL;
	this->node_data_type = l->get_data_type();
	this->ast_num_child = unary_arity;
	this->lineno = line;
}

// UMinus_Ast::~UMinus_Ast(){
// 	if (this->lhs != NULL)
// 	{
// 		delete this->lhs;
// 	}
// }

void UMinus_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: UMINUS\n";
	file_buffer << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")";
}


/////////////////////////////////////////////////
// Conditional_Expression_Ast
/////////////////////////////////////////////////

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
	if (l == NULL || r == NULL || cond == NULL){
		printf("Exit : Null argument in Conditional_Expression_Ast\n");
		exit(0);
	}
	this->cond = cond;
	this->lhs = l;
	this->rhs = r;
	this->lineno = line;
	this->ast_num_child = ternary_arity;
	this->node_data_type = l->get_data_type();
	
}

Conditional_Expression_Ast::~Conditional_Expression_Ast(){
	if (this->cond != NULL)
	{
		delete this->cond;
	}
	if (this->lhs != NULL)
	{
		delete this->lhs;
	}
	if (this->rhs != NULL)
	{
		delete this->rhs;
	}

}

void Conditional_Expression_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "Cond:";
	file_buffer << "\n" << AST_NODE_SPACE << "IF_ELSE";
	(this->cond)->print(file_buffer);
	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	(this->lhs)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_NODE_SPACE << "RHS (";
	(this->rhs)->print(file_buffer);
	file_buffer << ")";
}


/////////////////////////////////////////////////
// Relational_Expr_Ast
/////////////////////////////////////////////////


Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
	if (lhs == NULL || rhs == NULL){
		printf("cs316: Error: Null argument in Relational_Expression_Ast\n");
		exit(0);
	}
	if (rhs->get_data_type() != lhs->get_data_type())
	{
		printf("cs316: Error: non-compatible data_type in Relational_Expression_Ast\n");
		exit(0);
	}
	this->lhs_condition = lhs;
	this->rel_op = rop;
	this->rhs_condition = rhs;
	this->lineno = line;
	this->ast_num_child = ternary_arity;
	this->node_data_type = lhs->get_data_type();
}

Relational_Expr_Ast::~Relational_Expr_Ast(){

	// if (this->rel_op != NULL)
	// {
	// 	delete this->rel_op;
	// }
	if (this->lhs_condition != NULL)
	{
		delete this->lhs_condition;
	}
	if (this->rhs_condition != NULL)
	{
		delete this->rhs_condition;
	}
}

Data_Type Relational_Expr_Ast::get_data_type(){
	return this->node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}


bool Relational_Expr_Ast::check_ast(){
	if (this->lhs_condition == NULL || this->rhs_condition == NULL){
      	printf("%s\n", "cs316: Error: lhs or rhs is NULL in Relational_Expr_Ast");
    	return false;
    }

    // if (this->rel_op == NULL){
    //   	printf("%s\n", "cs316: Error: rel_op is NULL in Relational_Expr_Ast");
    // 	return false;
    // }


    if (this->ast_num_child != ternary_arity)
    {
    	printf("%s\n", "cs316: Error: arity is not 3 in Relational_Expr_Ast");
    	return false;
    }
	// later work
	// if(typeid(*lhs).name() != Name_Ast)
	return true;
}


void Relational_Expr_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: ";
	//Find what is the relational op
	if (this->rel_op == less_equalto){
		file_buffer << "LE";
	}
	else if (this->rel_op == less_than){
		file_buffer << "LT";
	}
	else if (this->rel_op == greater_than){
		file_buffer << "GT";
	}
	else if (this->rel_op == greater_equalto){
		file_buffer << "GE";
	}
	else if (this->rel_op == equalto){
		file_buffer << "EQ";
	}
	else if (this->rel_op == not_equalto){
		file_buffer << "NE";
	}
	else{
		printf("%s", "cs316: Error: Unknown rel_op in Relational_Expr_Ast");
	}

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	(this->lhs_condition)->print(file_buffer);
	file_buffer << ")\n";
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	(this->rhs_condition)->print(file_buffer);
	file_buffer << ")";
}



/////////////////////////////////////////////////
// Logical_Expr_Ast
/////////////////////////////////////////////////


Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	if (rhs == NULL){
		printf("cs316: Error: Null argument in Logical_Expr_Ast\n");
		exit(0);
	}
	this->lhs_op = lhs;
	this->rhs_op = rhs;
	this->bool_op = bop;
	this->lineno = line;
	this->ast_num_child = ternary_arity;
	this->node_data_type = void_data_type;
}

Logical_Expr_Ast::~Logical_Expr_Ast(){
	// if (this->bool_op != NULL)
	// {
	// 	delete this->rel_op;
	// }
	if (this->lhs_op != NULL)
	{
		delete this->lhs_op;
	}
	if (this->rhs_op != NULL)
	{
		delete this->rhs_op;
	}
}


Data_Type Logical_Expr_Ast::get_data_type(){
	return this->node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){
	if (this->bool_op == _logical_not){
		if (this->rhs_op == NULL){
      		printf("%s\n", "cs316: Error: lhs is NULL in Logical_Expr_Ast");
    		return false;
    	}	
	}
	else if (this->lhs_op == NULL || this->rhs_op == NULL){
      	printf("%s\n", "cs316: Error: lhs or rhs is NULL in Logical_Expr_Ast");
    	return false;
    }

    // if (this->bool_op == NULL){
    //   	printf("%s\n", "cs316: Error: bool_op is NULL in Logical_Expr_Ast");
    // 	return false;
    // }


    if (this->ast_num_child != ternary_arity)
    {
    	printf("%s\n", "cs316: Error: arity is not 3 in Logical_Expr_Ast");
    	return false;
    }
	// later work
	// if(typeid(*lhs).name() != Name_Ast)
	return true;
}

void Logical_Expr_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: ";
	//Find what is the relational op
	if (this->bool_op == _logical_not){
		file_buffer << "NOT";
	}
	else if (this->bool_op == _logical_or){
		file_buffer << "OR";
	}
	else if (this->bool_op == _logical_and){
		file_buffer << "AND";
	}
	
	if (this->lhs_op != NULL){
		file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
		(this->lhs_op)->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SUB_NODE_SPACE << "RHS (";
		(this->rhs_op)->print(file_buffer);
		file_buffer << ")";
	}
	else{
		file_buffer << "\n";	
		file_buffer << AST_SUB_NODE_SPACE << "RHS (";
		(this->rhs_op)->print(file_buffer);
		file_buffer << ")";	
	}

	
}


/////////////////////////////////////////////////
// Selection_Statement_Ast
/////////////////////////////////////////////////


Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line){
	if (cond == NULL || then_part == NULL){
		printf("Exit : Null argument in Selection_Statement_Ast\n");
		exit(0);
	}
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	this->lineno = line;
	this->ast_num_child = ternary_arity;
	this->node_data_type = void_data_type;
}


Selection_Statement_Ast::~Selection_Statement_Ast(){
	if (this->cond != NULL)
	{
		delete this->cond;
	}
	if (this->then_part != NULL)
	{
		delete this->then_part;
	}
	if (this->else_part != NULL)
	{
		delete this->else_part;
	}
}

Data_Type Selection_Statement_Ast::get_data_type(){
	return this->node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){
	if (this->then_part == NULL || this->cond == NULL){
      	printf("%s\n", "cs316: Error: Arg is NULL in Selection_Statement_Ast");
    	return false;
    }

    if (this->ast_num_child != ternary_arity)
    {
    	printf("%s\n", "cs316: Error: arity is not 3 in Selection_Statement_Ast");
    	return false;
    }
	// later work
	// if(typeid(*lhs).name() != Name_Ast)
	return true;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "IF : \n";

	file_buffer << AST_SPACE << "CONDITION (";
	(this->cond)->print(file_buffer);
	file_buffer << ")\n";

	file_buffer << AST_SPACE << "THEN (";
	(this->then_part)->print(file_buffer);
	file_buffer << ")";

	if (this->else_part != NULL){
		file_buffer << "\n";
		file_buffer << AST_SPACE << "ELSE (";
		(this->else_part)->print(file_buffer);
		file_buffer << ")";
	}
	
}

/////////////////////////////////////////////////
// Iteration_Statement_Ast
/////////////////////////////////////////////////



Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
	if (cond == NULL || body == NULL){
		printf("Exit : Null argument in Iteration_Statement_Ast\n");
		exit(0);
	}
	this->cond = cond;
	this->body = body;
	this->is_do_form = do_form;
	this->lineno = line;
	this->node_data_type = void_data_type;
	this->ast_num_child = ternary_arity;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast(){
	if (this->cond != NULL)
	{
		delete this->cond;
	}
	if (this->body != NULL)
	{
		delete this->body;
	}
}

Data_Type Iteration_Statement_Ast::get_data_type(){
	return this->node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
	this->node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast(){
	if (this->cond == NULL || this->body == NULL){
      	printf("%s\n", "cs316: Error: Arg is NULL in Iteration_Statement_Ast");
    	return false;
    }

    if (this->ast_num_child != ternary_arity)
    {
    	printf("%s\n", "cs316: Error: arity is not 3 in Iteration_Statement_Ast");
    	return false;
    }
    return true;
}


void Iteration_Statement_Ast::print(ostream & file_buffer){
	if (this->is_do_form){
		file_buffer << "\n" << AST_SPACE << "DO (";
		(this->body)->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "WHILE CONDITION (";
		(this->cond)->print(file_buffer);
		file_buffer << ")";
	}
	else{
		file_buffer << "\n" << AST_SPACE << "WHILE : ";
		file_buffer << "\n" << AST_SPACE << "CONDITION (";
		(this->cond)->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "BODY (";
		(this->body)->print(file_buffer);	
		file_buffer << ")";
	}
}


/////////////////////////////////////////////////
// Sequence_Ast
/////////////////////////////////////////////////


Sequence_Ast::Sequence_Ast(int line){
	this->lineno = line;
	this->node_data_type = void_data_type;

	//TO DO: arity set
}

Sequence_Ast::~Sequence_Ast(){
	//TO DO: Fill if required
}


void Sequence_Ast::ast_push_back(Ast * ast){
	(this->statement_list).push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
	list<Ast *>::iterator it;
	for(it=(this->statement_list).begin();it!=(this->statement_list).end();it++){
		file_buffer << "\n" << AST_NODE_SPACE;
		(*it)->print(file_buffer);
		
	}
}


/////////////////////////////////////////////////
// Print_Ast
/////////////////////////////////////////////////

Print_Ast::Print_Ast(Ast* v, int line){
	this->lineno = line;
	this->var = v;
	this->ast_num_child =  unary_arity;
	this->node_data_type = void_data_type;
}

Print_Ast::~Print_Ast(){
	if (this->var != NULL)
	{
		delete this->var;
	}
}

void Print_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "Print :\n";
	file_buffer << AST_SUB_NODE_SPACE << "(Name : ";
	file_buffer << (((Name_Ast *) this->var)->get_symbol_entry()).get_variable_name();
	file_buffer << ")";
}


/////////////////////////////////////////////////
// Call_Ast
/////////////////////////////////////////////////

Call_Ast::Call_Ast(string name, int line){
	this->procedure_name = name;
	this->lineno = line;
	this->ast_num_child = zero_arity;
	//Not sure if below should be in constructor
	this->node_data_type = (program_object.get_procedure_prototype(name))->get_return_type();

}

Call_Ast::~Call_Ast(){
	list<Ast *>::iterator it;
	for(it=(this->actual_param_list).begin();it!=(this->actual_param_list).end();it++){
		if (*it != NULL){
			delete *it;
		}		
	}
}


Data_Type Call_Ast::get_data_type(){
	return this->node_data_type;
}

void Call_Ast::set_register(Register_Descriptor * reg){
	this->return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
	list<Symbol_Table_Entry *>::iterator it1;
	list<Ast *>::iterator it2;
	it1 = (formal_param_list.get_table()).begin();
	it2 = (this->actual_param_list).begin();
	while (true){
		if (it1 == (formal_param_list.get_table()).end() && it2 == (this->actual_param_list).end()) return;
		if (it1 == (formal_param_list.get_table()).end()){
			printf("cs316: Error: in Call_Ast:check_actual_formal_param - actual parameters provided greater than formal params\n");
			exit(0);
		}
		if (it2 == (this->actual_param_list).end()){
			printf("cs316: Error: in Call_Ast:check_actual_formal_param - actual parameters provided lesser than formal params\n");
			exit(0);	
		}
		if ((*it1)->get_data_type() != (*it2)->get_data_type()){
			printf("cs316: Error: in Call_Ast:check_actual_formal_param - type of arguments does not match\n");
			exit(0);	
		}
		++it1;
		++it2;
	}
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	this->actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer){
	list<Ast *>::iterator it;
	file_buffer << "\n" << AST_SPACE << "FN CALL: ";
	file_buffer << this->procedure_name << "(\n";
	for(it=(this->actual_param_list).begin();it!=(this->actual_param_list).end();it++){
		(*it)->print(file_buffer);		
	}
	file_buffer << ")";

}


/////////////////////////////////////////////////
// Return_Ast
/////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * ret_val, string name, int line){
	if (ret_val != NULL) {
		this->return_value = ret_val;
		this->node_data_type = ret_val->get_data_type();
	}
	else {
		this->return_value = NULL;
		this->node_data_type = void_data_type;
	}
	this->proc_name = name;
	this->lineno = line;
	this->ast_num_child = unary_arity;
}

Return_Ast::~Return_Ast(){
	if (this->return_value != NULL){
		delete this->return_value;
	}
}

Data_Type Return_Ast::get_data_type(){
	return this->node_data_type;
}

void Return_Ast::print(ostream & file_buffer){
	file_buffer << "\n" << AST_SPACE << "RETURN ";
	if (this->return_value != NULL)	(this->return_value)->print(file_buffer);
}









