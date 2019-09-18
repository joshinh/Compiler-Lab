// #include "ast.hh"
#include <stdlib.h>

template class Number_Ast<int>;
template class Number_Ast<double>;

/////////////////////////////////////////////////
// Ast - parent class
/////////////////////////////////////////////////

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	printf("%s\n", "cs316: Error: This function (print_value) is not defined for the parent Ast class");
	exit(0);	
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){
	printf("%s\n", "cs316: Error: This function (get_value_of_evaluation) is not defined for the parent Ast class");
	exit(0);	
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	printf("%s\n", "cs316: Error: This function (set_value_of_evaluation) is not defined for the parent Ast class");
	exit(0);	
}


/////////////////////////////////////////////////
// Name_Ast
/////////////////////////////////////////////////

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	string var_name = variable_symbol_entry->get_variable_name();
	if (!eval_env.does_variable_exist(var_name) || !eval_env.is_variable_defined(var_name)){
		if (!interpreter_global_table.does_variable_exist(var_name) || !interpreter_global_table.is_variable_defined(var_name)){
			printf("cs316: Error: In Name_Ast:evaluate, vriable does not exist or is undefined\n");
			exit(0);
		}
		else{
			return *(interpreter_global_table.get_variable_value(var_name));	
		}		
	}
	else{
		return *(eval_env.get_variable_value(var_name));
	}
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	string var_name = variable_symbol_entry->get_variable_name();
	eval_env.put_variable_value(result, var_name);	
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	string var_name = variable_symbol_entry->get_variable_name();
	if (!eval_env.does_variable_exist(var_name) || !eval_env.is_variable_defined(var_name)){
		if (!interpreter_global_table.does_variable_exist(var_name) || !interpreter_global_table.is_variable_defined(var_name)){
			printf("cs316: Error: In Name_Ast:evaluate, vriable does not exist or is undefined\n");
			exit(0);
		}
		else{
			return *(interpreter_global_table.get_variable_value(var_name));	
		}		
	}
	else{
		return *(eval_env.get_variable_value(var_name));
	}
}



/////////////////////////////////////////////////
// Plus_Ast
/////////////////////////////////////////////////

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// lhs->check_ast();
	// rhs->check_ast();	

	if (this->get_data_type() == int_data_type)
	{
		Eval_Result_Value_Int * left = (Eval_Result_Value_Int *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * right = (Eval_Result_Value_Int *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		retval->set_value(left->get_int_value() + right->get_int_value());
		return *(retval);
	}
	else if (this->get_data_type() == double_data_type)
	{
		Eval_Result_Value_Double * left = (Eval_Result_Value_Double *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * right = (Eval_Result_Value_Double *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		retval->set_value(left->get_double_value() + right->get_double_value());
		return *(retval);
	}
	else
	{
		printf("cs316: Error: in Plus_Ast::evaluate non-int/double Eval_Result\n");
	}	
	
}


/////////////////////////////////////////////////
// Minus_Ast
/////////////////////////////////////////////////

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// lhs->check_ast();
	// rhs->check_ast();

	if (this->get_data_type() == int_data_type)
	{
		Eval_Result_Value_Int * left = (Eval_Result_Value_Int *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * right = (Eval_Result_Value_Int *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		retval->set_value(left->get_int_value() - right->get_int_value());
		return *(retval);
	}
	else if (this->get_data_type() == double_data_type)
	{
		Eval_Result_Value_Double * left = (Eval_Result_Value_Double *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * right = (Eval_Result_Value_Double *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		retval->set_value(left->get_double_value() - right->get_double_value());
		return *(retval);
	}
	else
	{
		printf("cs316: Error: in Minus_Ast::evaluate non-int/double Eval_Result\n");
	}
}



/////////////////////////////////////////////////
// Mult_Ast
/////////////////////////////////////////////////

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	// lhs->check_ast();
	// rhs->check_ast();

	if (this->get_data_type() == int_data_type)
	{
		Eval_Result_Value_Int * left = (Eval_Result_Value_Int *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * right = (Eval_Result_Value_Int *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		retval->set_value(left->get_int_value() * right->get_int_value());
		return *(retval);
	}
	else if (this->get_data_type() == double_data_type)
	{
		Eval_Result_Value_Double * left = (Eval_Result_Value_Double *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * right = (Eval_Result_Value_Double *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		retval->set_value(left->get_double_value() * right->get_double_value());
		return *(retval);
	}
	else
	{
		printf("cs316: Error: in Mult_Ast::evaluate non-int/double Eval_Result\n");
	}
}


/////////////////////////////////////////////////
// Divide_Ast
/////////////////////////////////////////////////

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	if (this->get_data_type() == int_data_type)
	{
		Eval_Result_Value_Int * left = (Eval_Result_Value_Int *) &lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * right = (Eval_Result_Value_Int *) &rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		if (right->get_int_value() == 0){
			printf("cs316: Error: in Divide_Ast::Division by 0\n");		
		}
		retval->set_value(left->get_int_value() / right->get_int_value());
		return *(retval);
	}
	else if (this->get_data_type() == double_data_type)
	{
		Eval_Result_Value_Double * left = (Eval_Result_Value_Double *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * right = (Eval_Result_Value_Double *) & rhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		if (right->get_double_value() == 0){
			printf("cs316: Error: in Divide_Ast::Division by 0\n");		
		}
		retval->set_value(left->get_double_value() / right->get_double_value());
		return *(retval);
	}
	else
	{
		printf("cs316: Error: in Divide_Ast::evaluate non-int/double Eval_Result\n");
	}
}

/////////////////////////////////////////////////
// UMinus_Ast
/////////////////////////////////////////////////

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	if (this->get_data_type() == int_data_type)
	{
		Eval_Result_Value_Int * left = (Eval_Result_Value_Int *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		retval->set_value(-left->get_int_value());
		return *(retval);
	}
	else if (this->get_data_type() == double_data_type)
	{
		Eval_Result_Value_Double * left = (Eval_Result_Value_Double *) & lhs->evaluate(eval_env, file_buffer);
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		retval->set_value(-left->get_double_value());
		return *(retval);
	}
	else
	{
		printf("cs316: Error: in UMinus_Ast::evaluate non-int/double Eval_Result\n");
	}
}


/////////////////////////////////////////////////
// Selection_Statement_Ast
/////////////////////////////////////////////////

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Eval_Result_Value_Int * condb = (Eval_Result_Value_Int *) & cond->evaluate(eval_env, file_buffer);
	
	if (condb->get_result_enum() != int_result)
	{
		printf("cs316: Error: in Selection_Statement_Ast::evaluate non-int_result cond\n");
	}
	else
	{
		if (condb->get_int_value() == 1){
			return then_part->evaluate(eval_env, file_buffer);
		}
		else if (condb->get_int_value() == 0){
			if (else_part != NULL) return else_part->evaluate(eval_env, file_buffer);
			else{
					Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
					result->set_value(1);
					return *result;			
			}
		}
		else{
			printf("cs316: Error: in Selection_Statement_Ast::evaluate non-boolean cond-value received\n");
		}
	}
}

/////////////////////////////////////////////////
// Iteration_Statement_Ast
/////////////////////////////////////////////////

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Eval_Result_Value_Int * bod;
	Eval_Result_Value_Int * condb;
	if (this->is_do_form)
	{	
		
		bod = (Eval_Result_Value_Int *) & body->evaluate(eval_env, file_buffer);
		// if (bod->get_result_enum() != int_result || bod->get_int_value() != 1){
		// 	printf("%d\n",bod->get_int_value());
		// 	printf("cs316: Error: in Iteration_Statement_Ast::evaluate non-void_result body returned 0\n");
		// }
	}

	while(true){
		condb = (Eval_Result_Value_Int *) & cond->evaluate(eval_env, file_buffer);
		
		if (condb->get_result_enum() != int_result){
			printf("cs316: Error: in Iteration_Statement_Ast::evaluate non-int_result cond\n");
		}
		else
		{
			if (condb->get_int_value() == 1){
				bod = (Eval_Result_Value_Int *) & body->evaluate(eval_env, file_buffer);
				// if (bod->get_result_enum() != int_result){
				// 	printf("cs316: Error: in Iteration_Statement_Ast::evaluate non-void_result body returned 1\n");
				// }
			}
			else if (condb->get_int_value() == 0){
				break;
			}
			else{
				printf("cs316: Error: in Iteration_Statement_Ast::evaluate non-boolean cond-value received\n");
			}
		}
	}

	Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
	retval->set_value(1);
	return *(retval);
}



/////////////////////////////////////////////////
// Assignment_Ast
/////////////////////////////////////////////////


Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	string name;
	name = ((this->lhs)->get_symbol_entry()).get_variable_name();
	int flag = 0;
	if (!eval_env.does_variable_exist(name)){
		if (interpreter_global_table.does_variable_exist(name))
		{
			flag = 1;
		}
		else{
			printf("cs316: Error: The variable  was not declared\n");
			exit(0);
		}
	}

	this->print(file_buffer);

	if ((this->lhs)->get_data_type() == int_data_type){
		if(flag == 0){
			Eval_Result_Value_Int * rhs = (Eval_Result_Value_Int *) & (this->rhs)->evaluate(eval_env, file_buffer);
			Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
			result->set_value(rhs->get_int_value());
			file_buffer << "\n" << AST_SPACE << name << " : " << result->get_int_value()  << "\n\n";
			eval_env.put_variable_value(*result, name);
			return *(result);
		}
		else if (flag == 1)
		{
			Eval_Result_Value_Int * rhs = (Eval_Result_Value_Int *) & (this->rhs)->evaluate(eval_env, file_buffer);
			Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
			result->set_value(rhs->get_int_value());
			file_buffer << "\n" << AST_SPACE << name << " : " << result->get_int_value()  << "\n\n";
			interpreter_global_table.put_variable_value(*result, name);
			return *(result);	
		}
		else{
			printf("cs316: Error: hell is loose\n");
			exit(0);
		}
	}
	else if ((this->lhs)->get_data_type() == double_data_type){
		if (flag == 0)
		{
			Eval_Result_Value_Double * rhs = (Eval_Result_Value_Double *) & (this->rhs)->evaluate(eval_env, file_buffer);
			Eval_Result_Value_Double * result = new Eval_Result_Value_Double();
			result->set_value(rhs->get_double_value());
			file_buffer << "\n" << AST_SPACE << name << " : " << result->get_double_value()  << "\n\n";
			eval_env.put_variable_value(*result, name);
			return *(result);
		}	
		else if(flag == 1){
			Eval_Result_Value_Double * rhs = (Eval_Result_Value_Double *) & (this->rhs)->evaluate(eval_env, file_buffer);
			Eval_Result_Value_Double * result = new Eval_Result_Value_Double();
			result->set_value(rhs->get_double_value());
			file_buffer << "\n" << AST_SPACE << name << " : " << result->get_double_value()  << "\n\n";
			interpreter_global_table.put_variable_value(*result, name);
			return *(result);
		}
		else{
			printf("cs316: Error: hell is loose\n");
			exit(0);
		}
	}
	else{
		printf("cs316: Error: Unknown variable type\n");
		exit(0);	
	}

}



/////////////////////////////////////////////////
// Number_Ast
/////////////////////////////////////////////////


template<class T> Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if (this->get_data_type() == int_data_type){
		Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
		result->set_value(this->constant);
		return *(result);
	}
	else if (this->get_data_type() == double_data_type){
		Eval_Result_Value_Double * result = new Eval_Result_Value_Double();
		result->set_value(this->constant);
		return *(result);
	}
	else{
		printf("cs316: Error: in Number_Ast::evaluate Unknown data type\n");
		exit(0);		
	}
}


/////////////////////////////////////////////////
// Conditional_Expression_Ast
/////////////////////////////////////////////////

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int * cond_eval = (Eval_Result_Value_Int *) & (this->cond)->evaluate(eval_env, file_buffer);
	if (cond_eval->get_int_value() == 1){
		if (this->get_data_type() == int_data_type || this->get_data_type() == double_data_type){
			return (this->lhs)->evaluate(eval_env, file_buffer);
		}
		else{
			printf("cs316: Error: in Conditional_Expression_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (cond_eval->get_int_value() == 0){
		if (this->get_data_type() == int_data_type || this->get_data_type() == double_data_type){
			return (this->rhs)->evaluate(eval_env, file_buffer);
		}
		else{
			printf( "cs316: Error: in Conditional_Expression_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else{
		printf("cs316: Error: in Conditional_Expression_Ast::evaluate Unknown cond value\n");
		exit(0);	
	}
}




/////////////////////////////////////////////////
// Relational_Expr_Ast
/////////////////////////////////////////////////


Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
	if (this->rel_op == less_equalto){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() <= l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() <= l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (this->rel_op == less_than){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) &(this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) &(this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() < l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() < l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (this->rel_op == greater_than){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) &(this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) &(this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() > l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() > l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (this->rel_op == greater_equalto){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) &(this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) &(this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() >= l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() >= l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (this->rel_op == equalto){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) &(this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) &(this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() == l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() == l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else if (this->rel_op == not_equalto){
		if (this->get_data_type() == int_data_type){
			Eval_Result_Value_Int * l1, * l2;
			l1 = (Eval_Result_Value_Int *) &(this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Int *) &(this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_int_value() != l2->get_int_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else if (this->get_data_type() == double_data_type){
			Eval_Result_Value_Double * l1, * l2;
			l1 = (Eval_Result_Value_Double *) & (this->lhs_condition)->evaluate(eval_env, file_buffer);
			l2 = (Eval_Result_Value_Double *) & (this->rhs_condition)->evaluate(eval_env, file_buffer);
			if (l1->get_double_value() != l2->get_double_value()) result->set_value(1);
			else result->set_value(0);
			return *(result);
		}
		else{
			printf("%s\n", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown data type\n");
			exit(0);
		}
	}
	else{
		printf("%s", "cs316: Error: in Relational_Expr_Ast::evaluate Unknown rel_op in Relational_Expr_Ast");
		exit(0);
	}

}



/////////////////////////////////////////////////
// Logical_Expr_Ast
/////////////////////////////////////////////////



Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
	Eval_Result_Value_Int * l1, * l2;
	l1 = (Eval_Result_Value_Int *) &(this->rhs_op)->evaluate(eval_env, file_buffer);

	if (this->bool_op != _logical_not) l2 = (Eval_Result_Value_Int *) &(this->lhs_op)->evaluate(eval_env, file_buffer);

	if (this->bool_op == _logical_not){
		if (l1->get_int_value() == 1) result->set_value(0);
		else result->set_value(1);
		return *(result);
	}
	else if (this->bool_op == _logical_and){
		if (l1->get_int_value() == 1 && l2->get_int_value() == 1) result->set_value(1);
		else result->set_value(0);
		return *(result);
	}	
	else if (this->bool_op == _logical_or){
		if (l1->get_int_value() == 1 || l2->get_int_value() == 1) result->set_value(1);
		else result->set_value(0);
		return *(result);
	}
	else{
		printf("%s", "cs316: Error: Unknown bool_op in Logical_Expr_Ast");
		exit(0);
	}
	

}



/////////////////////////////////////////////////
// Sequence_Ast
/////////////////////////////////////////////////

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int * result = new Eval_Result_Value_Int();
	list<Ast *>::iterator it;
	for(it = (this->statement_list).begin(); it!=(this->statement_list).end(); ++it){
		(*it)->evaluate(eval_env,file_buffer);
	}
	result->set_value(1);
	return *(result);
}


/////////////////////////////////////////////////
// Return_Ast
/////////////////////////////////////////////////


Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if ((this->return_value)->get_data_type() == int_data_type){
		Eval_Result_Value_Int * retval = new Eval_Result_Value_Int();
		Eval_Result_Value_Int * l1;
		l1 = (Eval_Result_Value_Int *) &(this->return_value)->evaluate(eval_env, file_buffer);
		retval->set_value(l1->get_int_value());
		return *(retval);
	}
	else if ((this->return_value)->get_data_type() == double_data_type){
		Eval_Result_Value_Double * retval = new Eval_Result_Value_Double();
		Eval_Result_Value_Double * l1;
		l1 = (Eval_Result_Value_Double *) &(this->return_value)->evaluate(eval_env, file_buffer);
		retval->set_value(l1->get_double_value());
		return *(retval);	
	}
	else{
		printf("cs316: Error: No matching data type\n");
		exit(0);
	}	
}


/////////////////////////////////////////////////
// Call_Ast
/////////////////////////////////////////////////

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	return (program_object.get_procedure_prototype(this->procedure_name))->evaluate(eval_env, file_buffer);
}




