#include "icode.hh"
#include <stdlib.h>

template class Number_Ast<int>;
template class Number_Ast<double>;
template class Const_Opd<int>;
template class Const_Opd<double>;


/////////////////////////////////////
////// Instruction_Descriptor////////
/////////////////////////////////////

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	this->inst_op = op;
	this->mnemonic = mnn;
	this->ic_symbol = ics;
	this->name = name;
	this->ic_format = icf;
	this->assem_format = af;
}

Instruction_Descriptor::Instruction_Descriptor(){}

Tgt_Op Instruction_Descriptor::get_op(){
	return this->inst_op;
}

string Instruction_Descriptor::get_name(){
	return this->name;
}

string Instruction_Descriptor::get_mnemonic(){
	return this->mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
	return this->ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return this->ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return this->assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){

}



///////////////
/// Ics_Opd : parent class
//////////////

Register_Descriptor * Ics_Opd::get_reg(){
	printf("cs316: Error: the function get_reg is not defined for the parent Ics_Opd class\n");
}

//////////////////////
/// Mem_Addr_Opd
//////////////////////


Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	this->symbol_entry = &(se);
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	if (symbol_entry->get_symbol_scope() == global){
		file_buffer << symbol_entry->get_variable_name();
	}
	else if(symbol_entry->get_symbol_scope() == local){
		// file_buffer << "-";
		file_buffer << symbol_entry->get_start_offset();
		file_buffer << "($fp)";
	}
	else if (symbol_entry->get_symbol_scope() == formal){
		if (symbol_entry->get_ref_offset() == sp_ref){
			file_buffer << symbol_entry->get_start_offset();
			file_buffer << "($sp)";
		}
		else{
			file_buffer << symbol_entry->get_end_offset() + 8;
			file_buffer << "($fp)";
		}
		
	}
	else{
		printf("cs316: Error: in Mem_Addr_Opd::print_asm_opd non global/local scope found\n");
	}
}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

//////////////////////
/// Register_Addr_Opd
//////////////////////


Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	this->register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return this->register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer << "$" << register_description->get_name();
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	this->register_description = rhs.register_description;
	return *this;
}


//////////////////////
/// Const_Opd
//////////////////////

template<class T>
Const_Opd<T>::Const_Opd(T num){
	this->num = num;
}

template<class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	file_buffer << num;
}	

template<class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer << num;
}

template<class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){
	this->num = rhs.num;
	return *this;
}

//////////////////////
/// Icode_Stmt : parent class
//////////////////////

Instruction_Descriptor & Icode_Stmt::get_op(){
	return this->op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){
	printf("cs316: Error: the function get_op1 is not defined for the parent Icode_Stmt class\n");
}

Ics_Opd * Icode_Stmt::get_opd2(){
	printf("cs316: Error: the function get_op2 is not defined for the parent Icode_Stmt class\n");
}

Ics_Opd * Icode_Stmt::get_result(){
	printf("cs316: Error: the function get_result is not defined for the parent Icode_Stmt class\n");
}

void Icode_Stmt::set_opd1(Ics_Opd * io){
	printf("cs316: Error: the function set_op1 is not defined for the parent Icode_Stmt class\n");
}

void Icode_Stmt::set_opd2(Ics_Opd * io){
	printf("cs316: Error: the function set_op2 is not defined for the parent Icode_Stmt class\n");
}

void Icode_Stmt::set_result(Ics_Opd * io){
	printf("cs316: Error: the function set_result is not defined for the parent Icode_Stmt class\n");
}


//////////////////////
/// Print_IC_Stmt
//////////////////////

Print_IC_Stmt::Print_IC_Stmt(){
	this->op_desc = *(machine_desc_object.spim_instruction_table[print]);
}

Print_IC_Stmt::~Print_IC_Stmt(){}

void Print_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "\t" << op_desc.get_name() << "\n";
}

void Print_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\t" << op_desc.get_mnemonic() << "\n";	
}

//////////////////////
/// Move_IC_Stmt
//////////////////////

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	this->opd1 = opd1;
	this->result = result;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->result = rhs.result;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return this->opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return this->result;
}

void Move_IC_Stmt::set_result(Ics_Opd * io){
	this->result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (result == NULL || opd1 == NULL)
	{
		printf("cs316: Error: in Move_IC_Stmt::print NULL found\n");
	}

	if (idesc.get_ic_format() == i_r_op_o1)
	{
		file_buffer << "\t";
		file_buffer << idesc.get_name();
		file_buffer << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		printf("cs316: Error: in Move_IC_Stmt::print_icode unknown format\n");	
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (result == NULL || opd1 == NULL)
	{
		printf("cs316: Error: in Move_IC_Stmt::print NULL found\n");
	}

	if (idesc.get_assembly_format() == a_op_o1_r)
	{
		file_buffer << "\t" << idesc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (idesc.get_assembly_format() == a_op_r_o1)
	{
		file_buffer << "\t" << idesc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		printf("cs316: Error: in Move_IC_Stmt::print_assembly unknown format\n");	
	}
}


/////////////////////////////////////
////// Compute_IC_Stmt //////////////
/////////////////////////////////////


Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}

Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->result = rhs.result;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}


Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return this->opd1;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return this->opd2;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	this->opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return this->result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io){
	this->result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	if (opd1 == NULL){
		printf("cs316: Error: in Compute_IC_Stmt::print_icode NULL encountered \n");
	}
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (idesc.get_ic_format() == i_r_o1_op_o2)
	{
		file_buffer << "\t";
		file_buffer << idesc.get_name();
		file_buffer << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " , ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";	
	}
	else if (idesc.get_ic_format() == i_r_op_o1){
		file_buffer << "\t";
		file_buffer << idesc.get_name();
		file_buffer << ":    \t";
		result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (idesc.get_ic_format() == i_o1_op_o2){
		file_buffer << "\t";
		file_buffer << idesc.get_name();
		file_buffer << ":    \t";
		opd1->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		printf("cs316: Error: in Compute_IC_Stmt::print_icode unknown format\n");
	}

}

//TO DO
void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (opd1 == NULL)
	{
		printf("cs316: Error: in Compute_IC_Stmt::print_assembly NULL found\n");
	}
	if (idesc.get_assembly_format() == a_op_r_o1_o2)
	{
		file_buffer << "\t" << idesc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (idesc.get_assembly_format() == a_op_r_o1){
		file_buffer << "\t" << idesc.get_mnemonic() << " ";
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else if (idesc.get_assembly_format() == a_op_o1_o2){
		file_buffer << "\t" << idesc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		printf("cs316: Error: in Compute_IC_Stmt::print_assembly unknown format\n");
	}
}

//////////////////////
/// Control_Flow_IC_Stmt
//////////////////////

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * opd1, Ics_Opd * opd2, string label, int size){
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->offset = label;
	this->actual_param_size = size;
	this->op_desc = *(machine_desc_object.spim_instruction_table[op]);	

}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->offset = rhs.offset;
	this->actual_param_size = rhs.actual_param_size;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
	return this->opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
	return this->opd2;
}

void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	this->opd2 = io;
}

string Control_Flow_IC_Stmt::get_Offset(){
	return this->offset;
}

void Control_Flow_IC_Stmt::set_Offset(string label){
	this->offset = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	
	if (idesc.get_ic_format() == i_op_o1_o2_st)
	{
		if (opd1 == NULL)
		{
			printf("cs316: Error: in Control_Flow_IC_Stmt::print_icode NULL found\n");
		}
		if (idesc.get_op() != beq && idesc.get_op() != bne)
		{
			printf("warning: other than beq/bne seen\n");	
		}
		file_buffer << "\t";
		file_buffer << "goto ";
		file_buffer << this->offset << "\n";
		// file_buffer << "\t";
		// file_buffer << idesc.get_name();
		// file_buffer << ":    \t";
		// opd1->print_ics_opd(file_buffer);
		// file_buffer << " , zero : goto ";
		// file_buffer << this->offset << "\n";
	}
	else if (idesc.get_ic_format() == i_op_st)
	{
		if (idesc.get_op() == jal){
			file_buffer << "\t";
			file_buffer << "call ";
			file_buffer << this->offset << "\n";
		}
		else{
			file_buffer << "\t";
			file_buffer << "goto ";
			file_buffer << this->offset << "\n";	
		}
		
			
		
	}
	else if (idesc.get_ic_format() == i_op){
		file_buffer << "\t";
		file_buffer << "return\n";
	}
	else{
		printf("cs316: Error: in Control_Flow_IC_Stmt::print_icode unknown format\n");	
	}
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	
	if (idesc.get_assembly_format() == a_op_o1_o2_st)
	{
		if (opd1 == NULL)
		{
			printf("cs316: Error: in Control_Flow_IC_Stmt::print_assembly NULL found\n");	
		}
		if (idesc.get_op() != beq && idesc.get_op() != bne)
		{
			printf("warning: other than beq/bne seen\n");	
		}
		
		file_buffer << "\t";
		file_buffer << idesc.get_mnemonic() << " ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", $zero, ";
		file_buffer << this->offset << "\n";
	}
	else if (idesc.get_assembly_format() == a_op_st)
	{
		if (idesc.get_op() == jal && actual_param_size > 0){
			file_buffer << "\tsub $sp, $sp, " << actual_param_size << "\n";
			file_buffer << "\tjal ";
			file_buffer << this->offset << "\n";
			file_buffer << "\tadd $sp, $sp, " << actual_param_size << "\n";
		}
		else {
			file_buffer << "\t";
			file_buffer << idesc.get_mnemonic() << " ";
			file_buffer << this->offset << "\n";	
		}
		
	}
	else if (idesc.get_assembly_format() == a_op){
		file_buffer << "\t";
		file_buffer << "j " << this->offset;
		file_buffer << "\n";
	}
	else{
		printf("cs316: Error: in Control_Flow_IC_Stmt::print_assembly unknown format\n");	
	}
}

/////////////////////////////////////
////// Label_IC_Stmt ////////////////
/////////////////////////////////////


Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){
	this->label = label;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}


Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){
	this->label = rhs.label;
	this->op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

string Label_IC_Stmt::get_label(){
	return this->label;
}

void Label_IC_Stmt::set_label(string label){
	this->label = label;
}


void Label_IC_Stmt::print_icode(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (idesc.get_ic_format() == i_op_st){
		file_buffer << "\n" << label << ":\n";
	}
	else{
		printf("cs316: Error: in Label_IC_Stmt::print_icode unknown format\n");
	}
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){
	Instruction_Descriptor & idesc = get_inst_op_of_ics();
	if (idesc.get_assembly_format() == a_op_st){
		file_buffer << "\n" << label << ":\n";
	}
	else{
		printf("cs316: Error: in Label_IC_Stmt::print_assembly unknown format\n");
	}
}

/////////////////////////////////////
////// Code_For_Ast /////////////////
/////////////////////////////////////

Code_For_Ast::Code_For_Ast(){}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	this->ics_list = ic_l;
	this->result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics){
	this->ics_list.push_back(&ics);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return this->ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return this->result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg){
	this->result_register = reg;
}

//TO DO
Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	this->result_register = rhs.result_register;
	this->ics_list = rhs.ics_list;
	return *this;
}
