// #include "ast.hh"
#include <stdlib.h>
#include <list>

template class Number_Ast<int>;
template class Number_Ast<double>;
template class Const_Opd<int>;
template class Const_Opd<double>;


/////////////////////////////////////////////////
// Ast - parent class
/////////////////////////////////////////////////

// Zilch
Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){
	printf("cs316: Error: create_store_stmt not defined for parent class Ast\n");
	exit(0);
}

Code_For_Ast & Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	printf("cs316: Error: compile_and_optimize_ast not defined for parent class Ast\n");
	exit(0);
}



/////////////////////////////////////
////// Assignment_Ast ///////////////
/////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast & code2 = (this->rhs)->compile();
	list<Icode_Stmt *> l2;
	l2 = code2.get_icode_list();
	list<Icode_Stmt *> stmtlist = l2;	

	Mem_Addr_Opd * opdreg1 = new Mem_Addr_Opd((this->lhs)->get_symbol_entry());
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());
	
	if (this->node_data_type == int_data_type){
		Move_IC_Stmt * stmt = new Move_IC_Stmt(store, opdreg2, opdreg1);
		stmtlist.push_back(stmt);
		(code2.get_reg())->reset_use_for_expr_result();
		Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
		return *code;
	}	
	else if (this->node_data_type == double_data_type){
		Move_IC_Stmt * stmt = new Move_IC_Stmt(store_d, opdreg2, opdreg1);
		stmtlist.push_back(stmt);
		(code2.get_reg())->reset_use_for_expr_result();
		Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
		return *code;
	} 
	else{
		printf("cs316: Error: in Assignment_Ast::compile unknown data type\n");
		exit(0);
	}
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////////////////
// Name_Ast
/////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile(){
	// cout << "Offset " << variable_symbol_entry->get_start_offset();
	Mem_Addr_Opd * opdmem = new Mem_Addr_Opd(*variable_symbol_entry);
	Move_IC_Stmt * movestmt;
	Register_Descriptor * macreg;
	// cout << variable_symbol_entry->get_symbol_scope()<< endl;

	if (get_data_type() == int_data_type)
	{
		macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		movestmt = new Move_IC_Stmt(load, opdmem, opdreg);
	}
	else if(get_data_type() == double_data_type){
		macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		movestmt = new Move_IC_Stmt(load_d, opdmem, opdreg);
	}
	else{
		printf("cs316: Error: in Name_Ast::compile non-int/double data type\n");
		exit(0);
	}

	list<Icode_Stmt *> stmtlist;
	stmtlist.push_back(movestmt);
	Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
	return *code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
	Mem_Addr_Opd * opdmem = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Addr_Opd * opdreg = new Register_Addr_Opd(store_register);		
	Move_IC_Stmt * movestmt;
	
	if (get_data_type() == int_data_type)
	{
		movestmt = new Move_IC_Stmt(load, opdmem, opdreg);
	}
	else if(get_data_type() == double_data_type){
		movestmt = new Move_IC_Stmt(load_d, opdmem, opdreg);
	}
	else{
		printf("cs316: Error: in Name_Ast::compile non-int/double data type\n");
		exit(0);
	}

	list<Icode_Stmt *> stmtlist;
	stmtlist.push_back(movestmt);
	Code_For_Ast * code = new Code_For_Ast(stmtlist, store_register);
	return *code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////
////// Number_Ast ///////////////////
/////////////////////////////////////

template<class T> 
Code_For_Ast & Number_Ast<T>::compile(){
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Const_Opd<T> * opdconst = new Const_Opd<T>(this->constant);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(imm_load, opdconst, opdreg);

		list<Icode_Stmt *> stmtlist;
		stmtlist.push_back(stmt);

		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Const_Opd<T> * opdconst = new Const_Opd<T>(this->constant);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(imm_load_d, opdconst, opdreg);

		list<Icode_Stmt *> stmtlist;
		stmtlist.push_back(stmt);

		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		return *code;
	}
	else{
		printf("cs316: Error: unknown data type in Number_Ast\n");
		exit(0);
	}

}

template<class T> 
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////
////// Plus_Ast ///////////////////
//////////////////////////////////////


Code_For_Ast & Plus_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs)->compile();
	Code_For_Ast & code2 = (this->rhs)->compile();
	list<Icode_Stmt *> l1,l2, stmtlist;
	l1 = code1.get_icode_list();
	l2 = code2.get_icode_list();

	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	for (it=l2.begin();it!=l2.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(add, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(add_d, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}
	
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////
////// Minus_Ast ///////////////////
//////////////////////////////////////


Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs)->compile();
	Code_For_Ast & code2 = (this->rhs)->compile();
	list<Icode_Stmt *> l1,l2, stmtlist;
	l1 = code1.get_icode_list();
	l2 = code2.get_icode_list();

	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	for (it=l2.begin();it!=l2.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sub, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sub_d, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}
	
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////
////// Mult_Ast ///////////////////
//////////////////////////////////////


Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs)->compile();
	Code_For_Ast & code2 = (this->rhs)->compile();
	list<Icode_Stmt *> l1,l2, stmtlist;
	l1 = code1.get_icode_list();
	l2 = code2.get_icode_list();

	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	for (it=l2.begin();it!=l2.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(mult, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(mult_d, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}
	
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////
////// Divide_Ast ///////////////////
//////////////////////////////////////


Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs)->compile();
	Code_For_Ast & code2 = (this->rhs)->compile();
	list<Icode_Stmt *> l1,l2, stmtlist;
	l1 = code1.get_icode_list();
	l2 = code2.get_icode_list();

	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	for (it=l2.begin();it!=l2.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(divd, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(div_d, opdreg1, opdreg2, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}
	
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


/////////////////////////////////////
////// UMinus_Ast ///////////////////
//////////////////////////////////////


Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs)->compile();
	list<Icode_Stmt *> l1, stmtlist;
	l1 = code1.get_icode_list();
	
	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(uminus, opdreg1, NULL, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Compute_IC_Stmt * stmt = new Compute_IC_Stmt(uminus_d, opdreg1, NULL, opdreg);
		stmtlist.push_back(stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}
	
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

/////////////////////////////////////////////////
// Conditional_Expression_Ast
/////////////////////////////////////////////////

Code_For_Ast & Conditional_Expression_Ast::compile(){

	if(cond == NULL || lhs == NULL || rhs == NULL){
		printf("cs316: Error: in Conditional_Expression_Ast::compile NULL sub-Ast found\n");
		exit(0);
	}

	Code_For_Ast & condcode = cond->compile();
	Code_For_Ast & code1 = lhs->compile();
	Code_For_Ast & code2 = rhs->compile();

	Register_Descriptor * condreg = condcode.get_reg();
	Register_Descriptor * reg1 = code1.get_reg();
	Register_Descriptor * reg2 = code2.get_reg();
	if (reg1 == NULL || reg2 == NULL){
		printf("cs316: Error: in Conditional_Expression_Ast::compile, register found NULL\n");
		exit(0);
	}

	Register_Addr_Opd * opdcond;
	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(reg1);
	Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(reg2);

	string label1 = get_new_label();
	string label2 = get_new_label();

	
	Compute_IC_Stmt * movestmt1;
	Compute_IC_Stmt * movestmt2;
	Register_Descriptor * macreg;
	Register_Descriptor * zeroreg = machine_desc_object.spim_register_table[zero];
	Register_Addr_Opd * zeroopd = new Register_Addr_Opd(zeroreg);
	if (get_data_type() == int_data_type){
		macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		movestmt1 = new Compute_IC_Stmt(or_t, opdreg1, zeroopd, opdreg);
		movestmt2 = new Compute_IC_Stmt(or_t, opdreg2, zeroopd, opdreg);
	}
	else if(get_data_type() == double_data_type){
		macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		movestmt1 = new Compute_IC_Stmt(or_t, opdreg1, zeroopd, opdreg);
		movestmt2 = new Compute_IC_Stmt(or_t, opdreg2, zeroopd, opdreg);
	}
	else{
		printf("cs316: Error: in Conditional_Expression_Ast::compile non-int/double data type\n");
		exit(0);
	}


	Control_Flow_IC_Stmt * condstmt;
	Control_Flow_IC_Stmt * jumpstmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);
	Label_IC_Stmt * labelstmt1 = new Label_IC_Stmt(label, label1);
	Label_IC_Stmt * labelstmt2 = new Label_IC_Stmt(label, label2);

	if (condreg != NULL){
		opdcond  = new Register_Addr_Opd(condreg);
		condstmt  = new Control_Flow_IC_Stmt(beq, opdcond, NULL, label1);
	}
	else{
		list<Icode_Stmt*> & list_new = condcode.get_icode_list();
		if ((list_new.back())->get_op().get_op() == seq_d
		 || (list_new.back())->get_op().get_op() == slt_d
		 || (list_new.back())->get_op().get_op() == sle_d)
			condstmt  = new Control_Flow_IC_Stmt(bc1f, NULL,NULL, label1);
		else
			condstmt = new Control_Flow_IC_Stmt(bc1t, NULL,NULL, label1);
	}


	// check
	list<Icode_Stmt *> stmtlist;
	stmtlist.splice(stmtlist.end(), condcode.get_icode_list());
	stmtlist.push_back(condstmt);
	stmtlist.splice(stmtlist.end(), code1.get_icode_list());
	stmtlist.push_back(movestmt1);
	stmtlist.push_back(jumpstmt);
	stmtlist.push_back(labelstmt1);
	stmtlist.splice(stmtlist.end(), code2.get_icode_list());
	stmtlist.push_back(movestmt2);
	stmtlist.push_back(labelstmt2);

	if (condreg != NULL) condreg->reset_use_for_expr_result();
	reg1->reset_use_for_expr_result();
	reg2->reset_use_for_expr_result();

	Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);

	return *code;
}


/////////////////////////////////////
////// Relational_Expr_Ast ///////////////////
//////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile(){
	Code_For_Ast & code1 = (this->lhs_condition)->compile();
	Code_For_Ast & code2 = (this->rhs_condition)->compile();
	list<Icode_Stmt *> l1,l2, stmtlist;
	l1 = code1.get_icode_list();
	l2 = code2.get_icode_list();

	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	for (it=l2.begin();it!=l2.end();++it){
		stmtlist.push_back(*it);
	}

	if (this->get_data_type() == int_data_type){

		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
		Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());

		if (this->rel_op == less_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sle, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == less_than){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(slt, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == greater_than){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sgt, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == greater_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sge, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(seq, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == not_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sne, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else{
			printf("cs316: Error: Unknown Relation\n");
			exit(0);
		}

		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;	
	}
	else if (this->get_data_type() == double_data_type){
		// Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		// Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
		Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());

		if (this->rel_op == less_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sle_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == less_than){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(slt_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == greater_than){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sgt_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == greater_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sge_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(seq_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else if (this->rel_op == not_equalto){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(sne_d, opdreg1, opdreg2, NULL);
			stmtlist.push_back(stmt);
		}
		else{
			printf("cs316: Error: Unknown Relation\n");
			exit(0);
		}

		Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;	
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}


}

/////////////////////////////////////
////// Logical_Expr_Ast ///////////////////
//////////////////////////////////////

Code_For_Ast & Logical_Expr_Ast::compile(){

	if (this->bool_op == _logical_not){
		list<Icode_Stmt *> l2, stmtlist;
		Register_Descriptor * macreg1 = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(macreg1);
		Const_Opd<int> * opdconst = new Const_Opd<int>(1);
		Move_IC_Stmt * stmt1 = new Move_IC_Stmt(imm_load, (Ics_Opd *) opdconst, (Ics_Opd *) opdreg1);
		stmtlist.push_back(stmt1);

		Code_For_Ast & code2 = (this->rhs_op)->compile();
		if (code2.get_reg() == NULL){
			printf("cs316: Error: Logical Stmt not allowed for float\n");
			exit(0);
		}
		l2 = code2.get_icode_list();

		list<Icode_Stmt *>::iterator it;
		for (it=l2.begin();it!=l2.end();++it){
			stmtlist.push_back(*it);
		}

		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());

		if (this->bool_op == _logical_not){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(not_t, opdreg2, opdreg1, opdreg);
			stmtlist.push_back(stmt);
		}
		else{
			printf("cs316: Error: Unknown bool format\n");
			exit(0);
		}
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code2.get_reg())->reset_use_for_expr_result();
		macreg1->reset_use_for_expr_result();
		return *code;
	}
	else{

		Code_For_Ast & code1 = (this->lhs_op)->compile();
		Code_For_Ast & code2 = (this->rhs_op)->compile();
		if (code1.get_reg() == NULL){
			printf("cs316: Error: Logical Stmt not allowed for float\n");
			exit(0);
		}
		if (code2.get_reg() == NULL){
			printf("cs316: Error: Logical Stmt not allowed for float\n");
			exit(0);
		}
		list<Icode_Stmt *> l1, l2, stmtlist;
		l1 = code1.get_icode_list();
		l2 = code2.get_icode_list();

		list<Icode_Stmt *>::iterator it;

		for (it=l1.begin();it!=l1.end();++it){
			stmtlist.push_back(*it);
		}

		for (it=l2.begin();it!=l2.end();++it){
			stmtlist.push_back(*it);
		}

		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
		Register_Addr_Opd * opdreg2 = new Register_Addr_Opd(code2.get_reg());

		if (this->bool_op == _logical_and){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(and_t, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else if (this->bool_op == _logical_or){
			Compute_IC_Stmt * stmt = new Compute_IC_Stmt(or_t, opdreg1, opdreg2, opdreg);
			stmtlist.push_back(stmt);
		}
		else{
			printf("cs316: Error: Unknown bool format\n");
			exit(0);
		}
		Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
		(code1.get_reg())->reset_use_for_expr_result();
		(code2.get_reg())->reset_use_for_expr_result();
		return *code;
	}

}

/////////////////////////////////////////////////
// Selection_Statement_Ast
/////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile(){
	bool hasElse = true;
	if(cond == NULL || then_part == NULL){
		printf("cs316: Error: in Selection_Statement_Ast::compile NULL sub-Ast found\n");
		exit(0);
	}

	if (else_part == NULL){
		hasElse = false;
	}

	Code_For_Ast & condcode = cond->compile();
	Code_For_Ast & code1 = then_part->compile();
	// Code_For_Ast & code2;
	// if (hasElse) code2 = else_part->compile();

	Register_Descriptor * condreg = condcode.get_reg();
	
	

	Register_Addr_Opd * opdcond;
	
	string label1 = get_new_label();
	string label2 = get_new_label();

	Control_Flow_IC_Stmt * condstmt;
	Control_Flow_IC_Stmt * jumpstmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);
	Label_IC_Stmt * labelstmt1 = new Label_IC_Stmt(label, label1);
	Label_IC_Stmt * labelstmt2;
	if(hasElse) labelstmt2 = new Label_IC_Stmt(label, label2);

	if (condreg != NULL){
		opdcond  = new Register_Addr_Opd(condreg);
		condstmt  = new Control_Flow_IC_Stmt(beq, opdcond, NULL, label1);
	}
	else{
		list<Icode_Stmt*> & list_new = condcode.get_icode_list();
		if ((list_new.back())->get_op().get_op() == seq_d
		 || (list_new.back())->get_op().get_op() == slt_d
		 || (list_new.back())->get_op().get_op() == sle_d)
			condstmt  = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1);
		else
			condstmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label1);
	}


	// check
	list<Icode_Stmt *> stmtlist;
	stmtlist.splice(stmtlist.end(), condcode.get_icode_list());
	stmtlist.push_back(condstmt);
	stmtlist.splice(stmtlist.end(), code1.get_icode_list());
	if (hasElse) stmtlist.push_back(jumpstmt);
	stmtlist.push_back(labelstmt1);
	if(hasElse) stmtlist.splice(stmtlist.end(), (else_part->compile()).get_icode_list());
	if(hasElse) stmtlist.push_back(labelstmt2);

	if (condreg != NULL) condreg->reset_use_for_expr_result();

	Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);

	return *code;
}


/////////////////////////////////////////////////
// Iteration_Statement_Ast
/////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile(){
	// if(cond == NULL || body == NULL){
	// 	printf("cs316: Error: in Selection_Statement_Ast::compile NULL sub-Ast found\n");
	// }

	Code_For_Ast & condcode = cond->compile();
	Code_For_Ast & bodycode = body->compile();
	
	Register_Descriptor * condreg = condcode.get_reg();

	

	Register_Addr_Opd * opdcond;
	
	string label1 = get_new_label();
	string label2 = get_new_label();

	Control_Flow_IC_Stmt * condstmt;
	Control_Flow_IC_Stmt * jumpstmt;
	if(!is_do_form) jumpstmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2);

	if (condreg != NULL){
		opdcond = new Register_Addr_Opd(condreg);
		condstmt = new Control_Flow_IC_Stmt(bne, opdcond, NULL, label1);
	}
	else{
		list<Icode_Stmt*> & list_new = condcode.get_icode_list();
		if ((list_new.back())->get_op().get_op() == seq_d
		 || (list_new.back())->get_op().get_op() == slt_d
		 || (list_new.back())->get_op().get_op() == sle_d)
			condstmt  = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label1);
		else
			condstmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1);
	}
	
	Label_IC_Stmt * labelstmt1 = new Label_IC_Stmt(label, label1);
	Label_IC_Stmt * labelstmt2 = new Label_IC_Stmt(label, label2);

	// check
	list<Icode_Stmt *> stmtlist;	
	if(!is_do_form) stmtlist.push_back(jumpstmt);
	stmtlist.push_back(labelstmt1);
	stmtlist.splice(stmtlist.end(), bodycode.get_icode_list());
	stmtlist.push_back(labelstmt2);
	stmtlist.splice(stmtlist.end(), condcode.get_icode_list());
	stmtlist.push_back(condstmt);

	if (condreg != NULL) condreg->reset_use_for_expr_result();
	Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);

	return *code;
}


/////////////////////////////////////////////////
// Sequence_Ast
/////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile(){

	for (list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end(); ++it){
		Code_For_Ast & currcode = (*it)->compile();
		(this->sa_icode_list).splice((this->sa_icode_list).end(), currcode.get_icode_list());
	}

	Code_For_Ast * code = new Code_For_Ast(sa_icode_list, NULL);
	return *code;
}

void Sequence_Ast::print_assembly(ostream & file_buffer){
	for (list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
	{
		(*it)->print_assembly(file_buffer);				
	}
}

void Sequence_Ast::print_icode(ostream & file_buffer){
	for (list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
	{
		(*it)->print_icode(file_buffer);				
	}
}

/////////////////////////////////////////////////
// Print_Ast
/////////////////////////////////////////////////

Code_For_Ast & Print_Ast::compile(){

	if (var == NULL)
	{
		printf("cs316: Error: in Print_Ast::compile var is NULL\n");
		exit(0);
	}

	Register_Descriptor * v0reg = machine_desc_object.spim_register_table[v0];
	Register_Descriptor * a0reg = machine_desc_object.spim_register_table[a0];
	Register_Descriptor * f12reg = machine_desc_object.spim_register_table[f12];

	Register_Addr_Opd * opdv0 = new Register_Addr_Opd(v0reg);
	Register_Addr_Opd * opdprintarg;

	Const_Opd<int> * opdconst;
	
	Mem_Addr_Opd * opdmem = new Mem_Addr_Opd(((Name_Ast *) var)->get_symbol_entry());

	Move_IC_Stmt * move1;
	Move_IC_Stmt * move2;
	Print_IC_Stmt * pstmt =  new Print_IC_Stmt();

	if ((((Name_Ast *) var)->get_symbol_entry()).get_data_type() == int_data_type){
		opdconst = new Const_Opd<int>(1);
		opdprintarg = new Register_Addr_Opd(a0reg);
		move1 = new Move_IC_Stmt(imm_load, opdconst, opdv0); 
		move2 = new Move_IC_Stmt(load, opdmem, opdprintarg);
	}
	else if((((Name_Ast *) var)->get_symbol_entry()).get_data_type() == double_data_type){
		opdconst = new Const_Opd<int>(3);
		opdprintarg = new Register_Addr_Opd(f12reg);
		move1 = new Move_IC_Stmt(imm_load, opdconst, opdv0);
		move2 = new Move_IC_Stmt(load_d, opdmem, opdprintarg);
	}
		 
	list<Icode_Stmt *> stmtlist;	
	
	stmtlist.push_back(move1);
	stmtlist.push_back(move2);
	stmtlist.push_back(pstmt);

	Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);

	return *code;
}



/////////////////////////////////////////////////
// Return_Ast
/////////////////////////////////////////////////

Code_For_Ast & Return_Ast::compile(){
	if (this->return_value == NULL){
		list<Icode_Stmt *> stmtlist;
		Control_Flow_IC_Stmt * ret_stmt = new Control_Flow_IC_Stmt(ret_inst, NULL, NULL, "epilogue_" + this->proc_name);
		stmtlist.push_back(ret_stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
		return *code;
	}

	Code_For_Ast & code1 = (this->return_value)->compile();
	list<Icode_Stmt *> l1, stmtlist;
	l1 = code1.get_icode_list();
	list<Icode_Stmt *>::iterator it;

	for (it=l1.begin();it!=l1.end();++it){
		stmtlist.push_back(*it);
	}

	Register_Addr_Opd * opdreg1 = new Register_Addr_Opd(code1.get_reg());
	
	if (this->get_data_type() == int_data_type){
		Register_Descriptor * v1reg = machine_desc_object.spim_register_table[v1];
		Register_Addr_Opd * opdv1 = new Register_Addr_Opd(v1reg);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(mov, opdreg1, opdv1);
		stmtlist.push_back(stmt);
		Control_Flow_IC_Stmt * ret_stmt = new Control_Flow_IC_Stmt(ret_inst, NULL, NULL, "epilogue_" + this->proc_name);
		stmtlist.push_back(ret_stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, v1reg);
		(code1.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * f0reg = machine_desc_object.spim_register_table[f0];
		Register_Addr_Opd * opdf0 = new Register_Addr_Opd(f0reg);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(move_d, opdreg1, opdf0);
		stmtlist.push_back(stmt);
		Control_Flow_IC_Stmt * ret_stmt = new Control_Flow_IC_Stmt(ret_inst, NULL, NULL, "epilogue_" + this->proc_name);
		stmtlist.push_back(ret_stmt);
		Code_For_Ast * code = new Code_For_Ast(stmtlist, f0reg);
		(code1.get_reg())->reset_use_for_expr_result();
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}	
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


/////////////////////////////////////////////////
// Call_Ast
/////////////////////////////////////////////////

Code_For_Ast & Call_Ast::compile(){

	if (!program_object.get_procedure_prototype(this->procedure_name)->is_proc_defined())
	{
		printf("cs316: Error: in Call_Ast::compile call to undefined procedure\n");
		exit(0);
	}

	list<Ast *>::iterator it1;
	list<Symbol_Table_Entry *>::iterator it2;
	list<Icode_Stmt *> l_param, stmtlist;	
	list<Icode_Stmt *>::iterator it3;
	Code_For_Ast code_param;
	Register_Addr_Opd * opdreg1;
	Move_IC_Stmt * stmt_arg;
	Symbol_Table & formal_param_list = (program_object.get_procedure_prototype(this->procedure_name))->get_formal_param_list();
	Mem_Addr_Opd * opdmem;
	int param_size = 0;

	this->check_actual_formal_param(formal_param_list);

	list<Ast *> reverse_actual = (this->actual_param_list);
	list<Symbol_Table_Entry *> reverse_formal = (formal_param_list.get_table());

	reverse_actual.reverse();
	reverse_formal.reverse();

	it1 = reverse_actual.begin();
	it2 = reverse_formal.begin();

	while(true){
		if (it1 == reverse_actual.end() && it2 == reverse_formal.end()) break;
		
		code_param = (*it1)->compile();
		l_param = code_param.get_icode_list();
		for (it3=l_param.begin();it3!=l_param.end();++it3){
			stmtlist.push_back(*it3);
		}
		opdreg1 = new Register_Addr_Opd(code_param.get_reg());
		(*it2)->set_start_offset(-1 * param_size);
		opdmem = new Mem_Addr_Opd(**it2);

		if ((*it1)->get_data_type() == int_data_type) {
			stmt_arg = new Move_IC_Stmt(store, opdreg1, opdmem);
			param_size += 4;
		}
		else if ((*it1)->get_data_type() == double_data_type) {
			stmt_arg = new Move_IC_Stmt(store_d, opdreg1, opdmem);
			param_size += 8;
		}
		else{
			printf("cs316: Error: No matching data type\n");
			exit(0);
		}
		stmtlist.push_back(stmt_arg);
		(code_param.get_reg())->reset_use_for_expr_result();
		++it1;
		++it2;

	}

	//Now add jal while changinng $sp before and after call
	Control_Flow_IC_Stmt * cstmt = new Control_Flow_IC_Stmt(jal, NULL, NULL, this->procedure_name, param_size);
	stmtlist.push_back(cstmt);

	//printf("Testing %d\n", return_value_reg == 0);
	

	if (this->get_data_type() == int_data_type){
		Register_Descriptor * v1reg = machine_desc_object.spim_register_table[v1];
		Register_Addr_Opd * opdv1 = new Register_Addr_Opd(v1reg);
		Register_Descriptor * macreg = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(mov, opdv1, opdreg);
		stmtlist.push_back(stmt);
		if (return_value_reg == 0){
			Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
			return *code;	
		}
		else{
			Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
			macreg->reset_use_for_expr_result();
			return *code;
		}
	}
	else if (this->get_data_type() == double_data_type){
		Register_Descriptor * f0reg = machine_desc_object.spim_register_table[f0];
		Register_Addr_Opd * opdf0 = new Register_Addr_Opd(f0reg);
		Register_Descriptor * macreg = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd * opdreg = new Register_Addr_Opd(macreg);
		Move_IC_Stmt * stmt = new Move_IC_Stmt(move_d, opdf0, opdreg);
		stmtlist.push_back(stmt);
		if (return_value_reg == 0){
			Code_For_Ast * code = new Code_For_Ast(stmtlist, macreg);
			return *code;	
		}
		else{
			Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
			macreg->reset_use_for_expr_result();
			return *code;
		}
	}
	else if (this->get_data_type() == void_data_type){
		Code_For_Ast * code = new Code_For_Ast(stmtlist, NULL);
		return *code;
	}
	else{
		printf("cs316: Error: Unknown Data type\n");
		exit(0);
	}

}


Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

